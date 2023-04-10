#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <libgen.h>
// splice 在两个文件描述符之间移动数据，零拷贝操作

int main(int argc, char *argv[])
{
    if (argc <= 2)
    {
        printf("usage: %s ip_address port_number filename\n", basename(argv[0]));
        return 1;
    }

    const char *ip = argv[1];
    int port = atoi(argv[2]);

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
        int pipefd[2];
        ret = pipe(pipefd);
        assert(ret != -1);
        // splice()  moves  data  between  two  file  descriptors without copying between kernel address space and user address space.  It
        // transfers up to len bytes of data from the file descriptor fd_in to the file descriptor fd_out, where one of the file  descrip‐
        // tors must refer to a pipe.

        // If  fd_in does not refer to a pipe and off_in is NULL, then bytes are read from fd_in starting from the file offset, and the
        // file offset is adjusted appropriately.

        // If fd_in does not refer to a pipe and off_in is not NULL, then off_in must point to a buffer which  specifies  the  starting
        // offset from which bytes will be read from fd_in; in this case, the file offset of fd_in is not changed.

        // If fd_in refers to a pipe, then off_in must be NULL.
        while (1)
        {
            ret = splice(connfd, NULL, pipefd[1], NULL, 32768, SPLICE_F_MOVE | SPLICE_F_MORE);

            assert(ret != -1);
            ret = splice(pipefd[0], NULL, connfd, NULL, 32768, SPLICE_F_MOVE | SPLICE_F_MORE);
        }
        close(sock);
    }
    return 0;
}
