#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


#define BUFF_SIZE 1024

char *resultMessage(char buffer[], int len)
{
    int pos_1 = 0, pos_2 = 0, isError = 0;
    char number[BUFF_SIZE];
    char alpha[BUFF_SIZE];
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
	int listenfd, connfd;
	char buff[BUFF_SIZE+1];
	struct sockaddr_in servaddr, cliaddr;
	
	//Step 1: Construct socket
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	
	//Step 2: Bind address to socket
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));
	if(bind(listenfd, (struct sockaddr *) &servaddr,
		sizeof(servaddr))){
		perror("Error: ");
		return 0;
	}
	
	//Step 3: Listen request from client
	if(listen(listenfd, 10)){
		perror("Error! Cannot listen.");
		return 0;
		}
	printf("Server started!\n");
	
	
	while(1){

		//Step 4: Communicate with client
		int rcvBytes, sendBytes, cliaddrLen = sizeof(cliaddr);

		//accept request
		connfd = accept(listenfd, (struct sockaddr *) & cliaddr, &cliaddrLen);

		while(1){
			//receive message from client
			rcvBytes = recv(connfd, buff, BUFF_SIZE, 0);
			if(rcvBytes < 0){
				perror("Error :");
				close(connfd);
				return 0;
			} else{
				buff[rcvBytes] = '\0';
				printf("Receive from client[%s:%d] %s\n",
				inet_ntoa(cliaddr.sin_addr),
				ntohs(cliaddr.sin_port), buff);
				
				//Echo to client
				sendBytes = send(connfd, resultMessage(buff, strlen(buff)), strlen(buff), 0);
				if(sendBytes < 0){
					perror("Error: ");
					close(connfd);
					return 0;
				}
				printf("Result sent.\n");
			}
		}
		
	} //end while
	printf("end\n");
	return 0;
}