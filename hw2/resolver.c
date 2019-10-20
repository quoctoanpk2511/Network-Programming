#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <netdb.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int resolveToIP(char param[]);
int resolveToHostName(char param[]);

int main(int argc,char *argv[]){
	// struct in_addr ip;
	char input[30];
	if (argc != 2) {
		printf("Error, too many or too few arguments\n");
		return 0;
	}
	strcpy(input, argv[1]);
    if (isalpha(input[0])) {
    	resolveToIP(input);
  	} else {
  		resolveToHostName(input);
  	}
	return 0;
}

int resolveToIP(char param[]){
  	struct hostent *he;
  	// struct in_addr addr;
  	printf("Hostname: %s\n", param);
  	he = gethostbyname(param);
  	if(he == NULL) {
    	printf("Not found information\n");
   		return 0;
  	}
  	char **pAlias;
  	printf("Offcial IP: %s \n", inet_ntoa(*((struct in_addr *)he->h_addr)));
  	printf("Alias IP:\n");
    for (pAlias = he->h_aliases; *pAlias != NULL; pAlias++) {
    	printf("%s\n", inet_ntoa(*((struct in_addr *)pAlias)));
  	}
  	return 0;
}

int resolveToHostName(char param[]) {
    struct hostent *he;
  	struct in_addr addr;
  	printf("IP Address: %s\n", param);
  	addr.s_addr = inet_addr(param);
  	if (addr.s_addr == INADDR_NONE) {
    	printf("Not found information\n");
    	return 0;
  	} else
    	he = gethostbyaddr((char *) &addr, 4, AF_INET);
 	  if (he == NULL) {
    	printf("Not found information\n");
    	return 0;
    }
  	char **pAlias;
  	printf("Offcial name: %s \n", he->h_name);
  	printf("Alias name: \n");
  	for (pAlias = he->h_aliases; *pAlias != NULL; pAlias++) {
    	printf("%s\n", *pAlias);
  	}
    return 0;
}

// int resolveToHostName(char *param){
//   	struct hostent *hp;
//   	if((hp=gethostbyaddr((const void *)&ip,sizeof ip,AF_INET))==NULL){
//       	perror("Not found information\n");
//       	return 0;
//   	}
//   	printf("IP: %s",param);
//   	printf("offical Ip: %s\n",hp->h_name);
//   	printf("alias: %s\n",hp->h_aliases[0]);
//   	return 0;
// }

