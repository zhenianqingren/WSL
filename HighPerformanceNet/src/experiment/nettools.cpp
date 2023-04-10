#include "nettools.h"

int openf(const char *fn, int type)
{
    int fd;
    do
    {
        fd = open(fn, type);
        if (fd < 0 && errno != EINTR)
        {
            fprintf(stderr, "Fail Open: %s\n", strerror(errno));
            exit(1);
        }
    } while (fd < 0);
    return fd;
}

struct addrinfo *new_addr(const char *ip, const char *port)
{
    struct addrinfo *res;
    struct addrinfo hints;
    int err;
    struct sockaddr_in *tmp;

    bzero(&hints, sizeof(hints));
    hints.ai_socktype = SOCK_DGRAM;

    if ((err = getaddrinfo(ip, "openvpn", &hints, &res)) != 0)
    {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(err));
        exit(1);
    }

    tmp = (struct sockaddr_in *)res;
    tmp->sin_port = htons(atoi(port));

    return res;
}

void printaddr(struct addrinfo *addr)
{
    struct sockaddr_in *tmp = (struct sockaddr_in *)addr->ai_addr;
    char ip[IPBUF];
    int port;
    inet_ntop(tmp->sin_family, &tmp->sin_addr, ip, INET_ADDRSTRLEN);
    port = ntohs(tmp->sin_port);

    printf("current ip: %s   port: %d\n\n", ip, port);
}

int initrecv(int type, const struct sockaddr *addr, socklen_t alen, int qlen)
{
    int fd;
    int err = 0;

    if ((fd = socket(addr->sa_family, type, 0)) < 0)
        return -1;

    if (bind(fd, addr, alen) < 0)
        goto errout;

    if (type == SOCK_STREAM || type == SOCK_SEQPACKET)
        if (listen(fd, qlen) < 0)
            goto errout;

    return fd;

errout:
    err = errno;
    close(fd); // this place error may happen , so save error status about net
    errno = err;
    return -1;
}

int readf(int fd, char *msg, int size)
{
    int len = 0;
    while ((len = read(fd, msg, size)) > 0)
    {
        if (len < 0 && errno != EINTR)
        {
            fprintf(stderr, "read from file error\n");
            close(fd);
            exit(1);
        }
        if (len >= 0)
            break;
    }
    return len;
}

int writef(int fd, char *msg, int len)
{
    int ret;
    int pos = 0;
    while (len > 0)
    {
        if ((ret = write(fd, msg + pos, len)) < 0 && errno != EINTR)
        {
            fprintf(stderr, "Write Interrupt\n");
            close(fd);
            exit(1);
        }
        pos += ret;
        len -= ret;
    }

    return 0;
}

packet *createp(int fd, int seq)
{
    packet *ret = (packet *)malloc(sizeof(packet));
    ret->seq = seq;
    ret->syn = 0;

    int len;
    if ((len = readf(fd, ret->msg, PSIZE)) == 0)
        ret->seq = END;
    ret->msglen = len;
    return ret;
}

void confirm(int contype, int num, struct sockaddr_in *addr)
{
    char ip[IPBUF];
    int port;
    inet_ntop(addr->sin_family, &addr->sin_addr, ip, INET_ADDRSTRLEN);
    port = ntohs(addr->sin_port);

    if (contype == ACKTYPE)
        printf("receive ack: %d  from ip: %s   port: %d\n", num, ip, port);
    else if (contype == DATATYPE)
        printf("receive data size: %d  from ip: %s   port: %d\n", num, ip, port);
}

void sendpkt(int sockfd, struct sockaddr *dst, socklen_t alen, packet *pkt)
{
    if (sendto(sockfd, pkt, sizeof(packet), 0, dst, alen) < 0)
    {
        fprintf(stderr, "send packet error: %s\n", strerror(errno));
        exit(1);
    }
}
void recvpkt(int sockfd, struct sockaddr *src, socklen_t *alen, packet *pkt)
{
    if (recvfrom(sockfd, pkt, sizeof(packet), 0, src, alen) < 0)
    {
        fprintf(stderr, "recv packet error: %s\n", strerror(errno));
        exit(1);
    }
}

int sendq(struct sockaddr *dst, socklen_t alen, que_p &queue, int *seqAddr, int fd, int sockfd)
{
    while (queue.size() < WINDOW)
    {
        *seqAddr = *seqAddr % SEQCOUNT;
        packet *newp = createp(fd, *seqAddr);
        if (count != LOST)
            sendpkt(sockfd, dst, alen, newp);
        ++count;
        queue.push(newp);
        ++(*seqAddr);
    }

    return 0;
}

int recvq(struct sockaddr *src, socklen_t *lAddr, int *expected, int fd, int sockfd)
{
    packet recvp;
    recvpkt(sockfd, src, lAddr, &recvp);
    int ret;
    if (recvp.seq == *expected || recvp.seq == END)
    {
        confirm(DATATYPE, recvp.msglen, (sockaddr_in *)src);
        *expected = recvp.seq == END ? END : (*expected + 1) % SEQCOUNT;
        ret = recvp.seq == END ? END : 0;
        writef(fd, recvp.msg, recvp.msglen);
    }
    else if LEFT (recvp.seq, *expected)
        ret = recvp.seq;

    return ret;
}

int sendack(struct sockaddr *dst, socklen_t alen, int ack, int sockfd)
{
    if (sendto(sockfd, &ack, sizeof(int), 0, dst, alen) < 0)
    {
        fprintf(stderr, "send ack error: %s\n", strerror(errno));
        return 1;
    }
    return 0;
}

void clearq(que_p &queue)
{
    packet *rele;
    while (queue.size())
    {
        rele = queue.front();
        queue.pop();
        free(rele);
    }
    printf("file transmission end\n");
}

void slideWindow(que_p &queue, int *expected, int ack)
{
    packet *rele;
    while (*expected != (ack + 1) % SEQCOUNT)
    {
        rele = queue.front();
        queue.pop();
        free(rele);
        *expected = (*expected + 1) % SEQCOUNT;
    }
}

int recvack(struct sockaddr *src, socklen_t *lAddr, que_p &queue, int *expected, int sockfd)
{
    int ack;
    int left;

    alarm(TIMEOUT);
    if (recvfrom(sockfd, &ack, sizeof(int), 0, src, lAddr) < 0)
    {
        fprintf(stderr, "recv ack error: %s\n", strerror(errno));
        exit(1);
    }
    left = alarm(0);

    if (left != 0 && (!LEFT(ack, *expected) || ack == -1))
    {
        confirm(ACKTYPE, ack, (sockaddr_in *)src);
        if (ack == END)
        {
            clearq(queue);
            return END;
        }
        slideWindow(queue, expected, ack);
    }
    return left;
}

void resend(int sockfd, struct sockaddr *addr, socklen_t alen, que_p &queue)
{
    int qlen = queue.size();
    printf("time exceed ! ! ! begin resend\n");

    packet *tmp;
    while (qlen > 0)
    {
        tmp = queue.front();
        queue.pop();
        queue.push(tmp);
        if (sendto(sockfd, tmp, sizeof(packet), 0, addr, alen) < 0)
        {
            fprintf(stderr, "send packet error: %s\n", strerror(errno));
            exit(1);
        }
        --qlen;
        printf("seq: %d has been resended\n", tmp->seq);
    }
}

void senderAction(struct sockaddr *dst, socklen_t alen, que_p &queue, int *expected, int *sendseq, int fd, int sockfd)
{
    int ack;
    for (;;)
    {
        sendq(dst, alen, queue, sendseq, fd, sockfd);
        if ((ack = recvack(dst, &alen, queue, expected, sockfd)) == END)
        {
            close(fd);
            return;
        }
    }
}

void recverAction(struct sockaddr *src, socklen_t alen, int *expected, int fd, int sockfd)
{
    int ack, res;
    for (;;)
    {
        res = recvq(src, &alen, expected, fd, sockfd);
        ack = res == END ? END : (*expected == 0 ? SEQCOUNT - 1 : *expected - 1);
        sendack(src, alen, ack, sockfd);
        if (res == END)
        {
            close(fd);
            return;
        }
    }
}
