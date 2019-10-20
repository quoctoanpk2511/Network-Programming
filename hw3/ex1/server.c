#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAX 1024

char *resultMessage(char buffer[], int len)
{
    int pos_1 = 0, pos_2 = 0, isError = 0;
    char number[MAX];
    char alpha[MAX];
    for (int i = 0; i < len - 1; i++)
    {
        char ch = buffer[i];
        //check if character is number
        if (ch >= '0' && ch <= '9')
        {
            number[pos_1] = ch;
            pos_1++;
        }
        //check if character is alphabet;
        else if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
        {
            alpha[pos_2] = ch;
            pos_2++;
        }
        //character is special symbol
        else
        {
            isError = 1;
        }
    }
    number[pos_1] = '\n';
    number[pos_1 + 1] = '\0';
    alpha[pos_2] = '\0';
    //if requested string have any special symbol
    if (isError == 1)
    {
        return "Error";
    }
    //sent result back
    else
    {
        return strcat(number, alpha);
    }
}

int main(int argc, char const *argv[])
{
    int i;

    if (argc != 2)
    {
        printf("Error, too many or too few arguments\n");
        return 1;
    }
    int sockfd;
    char buffer[MAX];
    struct sockaddr_in servaddr, cliaddr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(atoi(argv[1]));
    //servaddr.sin_port = htons(PORT);
    bzero(&(servaddr.sin_zero), 8); /* zero the rest of the structure */

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr,
             sizeof(struct sockaddr)) == -1)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Server running on port %s\n", argv[1]);
    while (1)
    {
        int len, n, result_len;
        len = sizeof(struct sockaddr_in);

        n = recvfrom(sockfd, (char *)buffer, MAX,
                     MSG_WAITALL, (struct sockaddr *)&cliaddr,
                     &len);
        if (n < 0)
        {
            perror("Error: ");
            close(sockfd);
            return 0;
        }
        buffer[n] = '\0';
        printf("Recieve form Client : %s\n", buffer);
        result_len = strlen(resultMessage(buffer, strlen(buffer)));
        n = sendto(sockfd, (const char *)resultMessage(buffer, strlen(buffer)),
                   result_len,
                   MSG_CONFIRM, (const struct sockaddr *)&cliaddr,
                   len);
        if (n < 0)
        {
            perror("Error: ");
            close(sockfd);
            return 0;
        }
        printf("Result sent.\n");
    }
    return 0;
}