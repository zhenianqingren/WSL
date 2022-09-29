#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

#define SIZE 128

int main(int argc, char const *argv[])
{
    if (argc <= 2)
    {
        fprintf(stderr, "useage:%s ip_address port_number\n", (char *)(basename(argv[0])));
        return 1;
    }

    char *ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in host;
    bzero(&host, sizeof(host));

    host.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &host.sin_addr);
    host.sin_port = htons(port);

    int sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    assert(sockfd >= 0);

    int count = 10;
    char buffer[SIZE] = "123456";
    int send;
    while (count-- > 0)
        if ((send = sendto(sockfd, buffer, SIZE, 0, (struct sockaddr *)&host, sizeof(host))) == -1)
        {
            fprintf(stderr, "%s\n", strerror(errno));
            return 1;
        }
    return 0;
}
