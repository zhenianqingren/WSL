#include "nettools.h"

static int expected = 0;
static int sendseq = 0;
static int sockfd;
static int fd;

static que_p queue;

static socklen_t alen = MAXADDRLEN;

static char abuf[MAXADDRLEN];
static struct sockaddr *clientAddr = (sockaddr *)abuf;

static status _stat = unconcious;
const char *fn;
static int clientRequest;
const char *output = "output.txt";

static void sigalarm(int signo)
{
    if (_stat == connected)
    {
        resend(sockfd, clientAddr, alen, queue);
        alarm(TIMEOUT);
    }
}

static void connecting()
{
    packet pkt;
    for (;;)
    {
        recvpkt(sockfd, clientAddr, &alen, &pkt);
        switch (_stat)
        {
        case unconcious:
            if (pkt.syn == 1 && pkt.seq == X)
            {
                _stat = first;
                clientRequest = pkt.request;
                fn = pkt.request == UPLOAD ? output : pkt.msg;
                if (pkt.request == LOAD)
                    fd = openf(fn, O_RDONLY);
                else if (pkt.request == UPLOAD)
                    fd = openf(fn, O_WRONLY | O_TRUNC | O_CREAT);

                pkt.seq = Y;
                sendpkt(sockfd, clientAddr, alen, &pkt);
            }
            break;
        case first:
            if (pkt.syn == 1 && pkt.seq == X)
            {
                pkt.seq = Y;
                sendpkt(sockfd, clientAddr, alen, &pkt);
            }
            else if (pkt.syn == 1 && pkt.seq == X + 1)
            {
                _stat = connected;
                return;
            }
            else if (pkt.syn == 0 && pkt.seq == 0)
            {
                _stat = connected;
                sendack(clientAddr, alen, 0, sockfd);
                writef(fd, pkt.msg, pkt.msglen);
                expected = 1;
                return;
            }
        case second:
            break;
        case connected:
            break;
        }
    }
}

static void run(struct addrinfo *liste)
{
    if ((sockfd = initrecv(SOCK_DGRAM, liste->ai_addr, liste->ai_addrlen, 0)) < 0)
    {
        fprintf(stderr, "create socket error\n");
        exit(1);
    }
    for (;;)
    {
        connecting();
        if (clientRequest == LOAD)
            senderAction(clientAddr, alen, queue, &expected, &sendseq, fd, sockfd);
        else if (clientRequest == UPLOAD)
            recverAction(clientAddr, alen, &expected, fd, sockfd);
        else
            fprintf(stderr, "first shake hands fail in server\n");

        memset(abuf, 0, MAXADDRLEN);
        _stat = unconcious;
        expected = 0;
        sendseq = 0;
        alen = MAXADDRLEN;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "less argument\n");
        exit(1);
    }

    struct addrinfo *dst = new_addr(argv[1], argv[2]);
    printaddr(dst);

    signal(SIGALRM, sigalarm);
    run(dst);

    return 0;
}
