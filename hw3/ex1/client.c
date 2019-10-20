#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAX 1024

int isValidIpAddress(char *ipAddress)
{
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipAddress, &(sa.sin_addr));
    if (result != 0)
        return 1;
    else
        return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Error, too many or too few arguments\n");
        return 1;
    }
    //check if input id is valid
    if (isValidIpAddress(argv[1]) == 0)
    {
        printf("Not a valid ip address\n");
        return 1;
    }
    int sockfd;
    char buffer[MAX];
    struct sockaddr_in servaddr;
    // Creating socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2]));
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    printf("Client running on port %s\n", argv[2]);

    while (1)
    {
        printf("\nInsert string to send: ");
        memset(buffer, '\0', (strlen(buffer) + 1));
        fgets(buffer, MAX, stdin);

        if (strcmp(buffer, "\n") == 0)
        {
            printf("Closing now\n");
            close(sockfd);
            return 0;
            exit(1);
        }
        int n, len;
        len = sizeof(struct sockaddr_in);

        sendto(sockfd, buffer, strlen(buffer),
               MSG_CONFIRM, (const struct sockaddr *)&servaddr,
               sizeof(servaddr));

        printf("Request sent.\n");

        n = recvfrom(sockfd, (char *)buffer, MAX,
                     MSG_WAITALL, (struct sockaddr *)&servaddr,
                     &len);
        len = sizeof(struct sockaddr_in);
        buffer[n] = '\0';
        printf("Result: \n%s\n", buffer);
    }
    return 0;
}