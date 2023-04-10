#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    if (argc <= 3)
    {
        printf("usage: %s ip_address port_number filename\n", basename(argv[0]));
        return 1;
    }

    const char *ip = argv[1];
    int port = atoi(argv[2]);

    const char *fn = argv[3];
    int fd = open(fn, O_RDONLY);
    assert(fd > 0);

    struct stat statbuf;
    fstat(fd, &statbuf);

    struct sockaddr_in address;
    memset(&address, 0, sizeof(struct sockaddr_in));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);

    int ret = bind(sock, (struct sockaddr *)&address, sizeof(address));
    assert(ret != 1);

    ret = listen(sock, 5);
    assert(ret != -1);

    struct sockaddr_in client;
    socklen_t client_addr_len = sizeof(client);

    int connfd = accept(sock, (struct sockaddr *)&client, &client_addr_len);
    if (connfd < 0)
        printf("error occur: %s\n", strerror(errno));
    else
    {
        sendfile(connfd, fd, NULL, statbuf.st_size);
        // sendfile()  copies  data  between one file descriptor and another.  Because this copying is done within the
        // kernel, sendfile() is more efficient than the combination of read(2)  and  write(2),  which  would  require
        // transferring data to and from user space.
        // in_fd should be a file descriptor opened for reading and out_fd should be a descriptor opened for writing.

        char buf[8] = {'t', 'e', 's', 't', '\0', '\0', '\0', '\0'};
        int nr = read(fd, buf, 8);
        printf("%d %s\n", nr, buf);
        close(sock);
    }
    return 0;
}