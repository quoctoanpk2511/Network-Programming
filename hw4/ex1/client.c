#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


#define BUFF_SIZE 1024

int main(int argc, char const *argv[])
{
	int clientfd;
	char buff[BUFF_SIZE+1];
	struct sockaddr_in servaddr;

	//Step 1: Construct socket
	clientfd = socket(AF_INET, SOCK_STREAM, 0);

	//Step 2: Specify server’s address
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	servaddr.sin_port = htons(atoi(argv[2]));
	
	//Step 4: Connect server
	if(connect(clientfd, (struct sockaddr *) &servaddr,sizeof(servaddr))){
		perror("Error: ");
		return 0;
	}
	
	//Step 5: Communicate with server
	// char buff[BUFF_SIZE];
	while (1){
		int ret;
		
		//Send message
		printf("Send to server: ");
		memset(buff, '\0', (strlen(buff) + 1));
		fgets(buff, BUFF_SIZE, stdin);
		if (strcmp(buff, "\n") == 0){
            printf("Closing now\n");
            close(clientfd);
            return 0;
            exit(1);
        }
		ret = send(clientfd, buff, strlen(buff), 0);
		if(ret < 0){
			perror("Error: ");
			return 0;
		}

		//Receive echo message
		ret = recv(clientfd, buff, BUFF_SIZE, 0);
		if(ret < 0){
			perror("Error: ");
			return 0;
		}
		printf("Receive from server: \n%s\n", buff);
	}
	return 0;
}