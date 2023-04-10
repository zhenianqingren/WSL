#ifndef RIO
#define RIO
#define ssize_t long
#define size_t unsigned long
#define RIO_BUFSIZE 8192

typedef struct
{
    int rio_fd;                // descriptor for this internal buf
    int rio_cnt;               // unread bytes in the internal buf
    char *rio_bufptr;          // next unread byte in internal buf
    char rio_buf[RIO_BUFSIZE]; // Internal buffer
} rio_t;

// no buffer
// io text data
ssize_t rio_readn(int fd, void *usrbuf, size_t n);
ssize_t rio_writen(int fd, void *usrbuf, size_t n);

// buffer
// binary & text data

void rio_readinitb(rio_t *rp, int fd);
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n);
#endif