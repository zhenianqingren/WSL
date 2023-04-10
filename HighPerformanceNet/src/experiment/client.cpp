#include "nettools.h"

static int expected = 0;
static int sendseq = 0;
static int sockfd;
static int fd;

static que_p queue;
static struct addrinfo *resendAddr;

static status _stat = first;

static char *fn;
static int request;

static void sigalarm(int signo)
{
    if (_stat == connected)
    {
        resend(sockfd, resendAddr->ai_addr, resendAddr->ai_addrlen, queue);
        alarm(TIMEOUT);
    }
}

static void sendRequest(struct sockaddr *dst, socklen_t alen)
{
    packet connp;
    connp.syn = 1;
    connp.request = request;
    connp.seq = X;
    strcpy(connp.msg, fn);
    if (sendto(sockfd, &connp, sizeof(packet), 0, dst, alen) < 0)
    {
        fprintf(stderr, "send packet error: %s\n", strerror(errno));
        exit(1);
    }
}

static void reconnect(int signo)
{
    if (_stat == first)
        sendRequest(resendAddr->ai_addr, resendAddr->ai_addrlen);
}

static void connecting(struct sockaddr *dst, socklen_t alen)
{
    int syn = 0;
    int seq = X;
    packet *pkt = NULL;
    packet recvp;
    for (;;)
    {
        switch (_stat)
        {
        case unconcious:
            break;
        case first:
            if (syn == 1 && seq == Y)
            {
                _stat = second;
                if (request == LOAD && pkt == NULL)
                {
                    pkt = (packet *)malloc(sizeof(packet));
                    pkt->syn = 1;
                    pkt->seq = X + 1;
                }
                else if (request == UPLOAD && pkt == NULL)
                {
                    _stat = connected;
                    return;
                }
                sendpkt(sockfd, dst, alen, pkt);
                break;
            }
            sendRequest(dst, alen);
            alarm(TIMEOUT);
            break;
        case second:
            if (syn == 0)
            {
                sendack(dst, alen, 0, sockfd);
                expected = 1;
                _stat = connected;
                writef(fd, recvp.msg, recvp.msglen);
                free(pkt);
                return;
            }
            else if (syn == 1 && seq == Y)
            {
                pkt->seq = X + 1;
                sendpkt(sockfd, dst, alen, pkt);
            }
            break;
        case connected:
            break;
        }
        recvpkt(sockfd, dst, &alen, &recvp);
        alarm(0);
        syn = recvp.syn;
        seq = recvp.seq;
    }
}

static void run(struct addrinfo *addr)
{
    connecting(addr->ai_addr, addr->ai_addrlen);
    if (request == LOAD)
        recverAction(addr->ai_addr, addr->ai_addrlen, &expected, fd, sockfd);
    else if (request == UPLOAD)
        senderAction(addr->ai_addr, addr->ai_addrlen, queue, &expected, &sendseq, fd, sockfd);
}

int main(int argc, char *argv[])
{
    if (argc < 5)
    {
        fprintf(stderr, "less argument\n");
        exit(1);
    }

    struct addrinfo *dst = new_addr(argv[1], argv[2]);
    printaddr(dst);
    resendAddr = dst;

    signal(SIGALRM, sigalarm);
    signal(SIGALRM, reconnect);

    request = atoi(argv[4]);
    fn = argv[3];

    if (request == LOAD)
        fd = openf("output.txt", O_WRONLY | O_CREAT | O_TRUNC);
    else if (request == UPLOAD)
        fd = openf(fn, O_RDONLY);
    else
    {
        fprintf(stderr, "illegal request\n");
        exit(1);
    }

    if ((sockfd = socket(dst->ai_family, SOCK_DGRAM, 0)) < 0)
    {
        fprintf(stderr, "socket create error: %s\n", strerror(errno));
        exit(1);
    }
    else
        run(dst);

    return 0;
}
