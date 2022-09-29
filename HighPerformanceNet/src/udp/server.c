#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <errno.h>

#define size 128

int main(int argc, char const *argv[])
{
    if (argc <= 2)
    {
        fprintf(stderr, "useage:%s ip_address port_number\n", (char *)(basename(argv[0])));
        return 1;
    }

    char *ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in address;
    struct sockaddr_in client;
    bzero(&address, sizeof(address));
    bzero(&client, sizeof(client));
    address.sin_family = AF_INET;

    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    ip = argv[3];
    port = atoi(argv[4]);
    client.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &client.sin_addr);
    client.sin_port = htons(port);

    int sock = socket(PF_INET, SOCK_DGRAM, 0);
    assert(sock >= 0);

    int sockfd = bind(sock, (struct sockaddr *)&address, sizeof(address));
    assert(sockfd != -1);

    int recv;
    char buffer[size];
    memset(buffer, '\0', size);
    socklen_t addr_len = sizeof(client);
    while (1)
    {
        sleep(2);
        recv = recvfrom(sockfd, buffer, size, 0, (struct sockaddr *)&client, &addr_len);
        printf("%d\t%s\n", recv, buffer);
        memset(buffer, '\0', size);
    }
    // if (recv == -1)
    // {
    //     fprintf(stderr, "%s\n", strerror(errno));
    //     return 1;
    // }
    return 0;
}
