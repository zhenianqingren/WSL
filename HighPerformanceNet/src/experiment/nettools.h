#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <queue>

#define PSIZE 64
#define ACK 1
#define SEQCOUNT 8
#define MAXADDRLEN 256
#define IPBUF 32
#define END -1
#define WINDOW 4
#define TIMEOUT 4

#define ACKTYPE 0
#define DATATYPE 1

#define LOAD 0
#define UPLOAD 1

#define X 64
#define Y 128
#define abs(x) ((x) < 0 ? (-x) : (x))
#define LEFT(r, e) ((r) < (e) || (e) + SEQCOUNT - (r) < WINDOW)

#define LOST 4
static int count = 0;

typedef struct packet
{
    int syn;
    int seq;
    int request;
    char msg[PSIZE];
    int msglen;
} packet;

typedef enum status
{
    unconcious,
    first,
    second,
    connected
} status;

using que_p = std::queue<packet *>;

int openf(const char *, int);
struct addrinfo *new_addr(const char *, const char *);
void printaddr(struct addrinfo *);
int initrecv(int, const struct sockaddr *, socklen_t, int);

int readf(int, char *, int);
int writef(int, char *, int);
packet *createp(int, int);

void confirm(int, int, struct sockaddr_in *);

void sendpkt(int, struct sockaddr *, socklen_t, packet *);         //发送单个报文
void recvpkt(int, struct sockaddr *, socklen_t *, packet *);       //接收单个报文
int sendq(struct sockaddr *, socklen_t, que_p &, int *, int, int); //将窗口内可发送的报文发送
int recvq(struct sockaddr *, socklen_t *, int *, int, int);        //接收报文以及对于不是expect的seq的处理
int sendack(struct sockaddr *, socklen_t, int, int);               //发送确认
int recvack(struct sockaddr *, socklen_t *, que_p &, int *, int);  //接收确认

void clearq(que_p &);
void slideWindow(que_p &, int *, int);
void resend(int, struct sockaddr *, socklen_t, que_p &);

void senderAction(struct sockaddr *, socklen_t, que_p &, int *, int *, int, int);
void recverAction(struct sockaddr *, socklen_t, int *, int, int);
void sAndr(struct sockaddr *, socklen_t *, int, packet *, int, const char *);
void rAnds(struct sockaddr *, socklen_t *, int, int, int, const char *, int *);