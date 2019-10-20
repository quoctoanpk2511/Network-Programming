#include "stdio.h"
#include "stdlib.h"
#include <string.h>

void menu() {
	printf("---------------------------------------------\n");
	printf("USER MANAGEMENT PROGRAM\n");
	printf("---------------------------------------------\n");
	printf("	1. Register\n");
	printf("	2. Sign in\n");
	printf("	3. Search\n");
	printf("	4. Sign out\n");
	printf("Your choice (1-4, other to quit): ");
}

struct User
{
	char userName[30];
	char password[30];
	int state;
};
typedef struct User user;

struct List {
    user account;
    struct List *next;
};
typedef struct List *node;

node createNode(user data){
    node temp; // declare a node
    temp = (node)malloc(sizeof(struct List)); // Cấp phát vùng nhớ dùng malloc()
    temp->next = NULL;// Cho next trỏ tới NULL
    temp->account = data; // Gán giá trị cho Node
    return temp;//Trả về node mới đã có giá trị
}
 
node addTail(node head, user data){
    node temp,p;// Khai báo 2 node tạm temp và p
    temp = createNode(data);//Gọi hàm createNode để khởi tạo node temp có next trỏ tới NULL và giá trị là value
    if(head == NULL){
        head = temp;     //Nếu linked list đang trống thì Node temp là head luôn
    }
    else{
        p  = head;// Khởi tạo p trỏ tới head
        while(p->next != NULL){
            p = p->next;//Duyệt danh sách liên kết đến cuối. Node cuối là node có next = NULL
        }
        p->next = temp;//Gán next của thằng cuối = temp. Khi đó temp sẽ là thằng cuối(temp->next = NULL mà)
    }
    return head;
}

void writeFile(char userName[30], char password[30]) {
	FILE *fp;
	char str[255]="\n";
	strcat(str,userName);
	strcat(str," ");
	strcat(str,password);
	strcat(str," 1");
	fp = fopen("account.txt", "a");
	if(fp == NULL){
      printf("Can't open file!");   
      exit(1);             
   	}
   	fprintf(fp,"%s",str);
   	fclose(fp);
}

void writeFileUpdate(char str[1000]) {
	FILE *fp;
	fp = fopen("account.txt", "w");
	if(fp == NULL){
      printf("Can't open file!");   
      exit(1);             
   	}
   	fprintf(fp,"%s",str);
   	fclose(fp);
}


node readFile(node head) {
	FILE *fp;
	char buff[255];
	char str[100][255];
	user data;
    fp = fopen("account.txt", "r");
    if(fp == NULL){
      perror("Cannot opent file!");  
      exit(1);             
   	}
   	int size = 0;
   	while (fscanf(fp, "%s", buff) != EOF) {
      strcpy(str[size], buff);       
      size++;
   	}
   	for (int i = 0; i < (size+1)/3; i++)
   	{
		strcpy(data.userName, str[3*i]);
		strcpy(data.password, str[3*i+1]);
		data.state = atoi(str[3*i+2]);
		head = addTail(head, data); 	
   	}
    fclose(fp);
    // node p = head;
    // while(p!=NULL) {
    // 	printf("%s\t%s\t%d\n", p->account.userName, p->account.password, p->account.state);
    // 	p = p->next;
    // }
    return head;
}

// int checkActive(node head,char userName[30]){
// 	int isActive = -1;
// 	node p = head;
// 	while(p != NULL){
// 		if(strcmp(userName, p->data.userName) == 0){
// 			isActive = p->data.state;
// 			break;
// 		}
//         p = p->next;
//     }
//     return isActive;
// }

int checkActive(node p) {
	if(p->account.state == 1) return 1;
	else return 0;
}

int checkUserName(node head, char userName[30]) {
	node p = head;
	while(p != NULL){
		if(strcmp(userName, p->account.userName) == 0){
			if(checkActive(p)) return 1; 
			else return 2;
		}
        p = p->next;
    }
    return 0;
}

int checkPassword(node head, char password[30]) {
	node p = head;
	while(p != NULL){
		if(strcmp(password, p->account.password) == 0){
			return 1;
		}
        p = p->next;
    }
    return 0;
}

void registerFunc() {
	node head = NULL;
	head = readFile(head);
	char userName[30];
	printf("User Name: ");
	scanf("%s", userName);
	if(checkUserName(head, userName)) {
		while(1) {
			printf("Account is exited\n");
			printf("User Name: ");
			scanf("%s", userName);
			if (!checkUserName(head, userName))
				break;
		}
	}
	char password[30];
	printf("Password: ");
	scanf("%s", password);
	writeFile(userName, password);
	printf("Your account is created!\n");
}

void blockAccount(node head, char userName[30]) {
	char str[1000] = "";
	char state[10];
	node p = head;
	while(p != NULL){
		if(strcmp(userName, p->account.userName) == 0){
			p->account.state = 0;
		}
		strcat(str, p->account.userName);
		strcat(str, " ");
		strcat(str, p->account.password);
		strcat(str, " ");
		sprintf(state, "%d", p->account.state);
		strcat(str, state);
		if (p->next==NULL) {
			break;
		}
		strcat(str, "\n");
		p = p->next;
	}
	writeFileUpdate(str);
}

void signInFunc(char userName[30]) {
	node head = NULL;
	head = readFile(head);
	printf("User Name: ");
	scanf("%s", userName);
	if(checkUserName(head, userName) == 0) {
		while(1) {
			printf("Cannot find account!\n");
			printf("User Name: ");
			scanf("%s", userName);
			if (checkUserName(head, userName) == 1)
				break;
			if (checkUserName(head,userName) == 2)
			{
				printf("Your account is not active!\n");
				return;
			}
		}
	} else if(checkUserName(head,userName) == 2) {
		printf("Your account is not active!\n");
		return;
	}
	char password[30];
	printf("Password: ");
	scanf("%s", password);
	int i = 3;
	if(!checkPassword(head, password)) {
		while(1) {
			i--;
			printf("Password is incorrect!\n");
			printf("Account will be blocked after %d times.\n", i);
			printf("Password: ");
			scanf("%s", password);
			if (checkPassword(head, password)) {
				break;
			}
			if (i == 1)
			{
				blockAccount(head, userName);
				printf("Your account is blocked!\n");
				return;
			}
		}
	}
	printf("You are login!\n");
	printf("Hello %s\n", userName);
}

void search(){
	node head = NULL;
	head =readFile(head);
	char userName[30];
	printf("User Name: ");
	scanf("%s", userName);
	int check = checkUserName(head,userName);
	switch(check){
		case 0:
			printf("%s\n", "Can't find account!");
			break;
		case 1:
			printf("%s\n", "Account is active!");
			break; 
		case 2:
			printf("%s\n", "Account is blocked!");
			break;
	}

}

void signOut(char currentUserName[30]){
	node head = NULL;
	head =readFile(head);
	char userName[30];
	printf("Current User Name: ");
	scanf("%s", userName);
	int check = checkUserName(head, userName);
	if (check == 1) {
		if(strcmp(currentUserName, userName) != 0){
			printf("Invalid User Name!\n");
		} else printf("Goodbye %s!\n", userName);
	}
	else printf("Can't find account!\n");
}

int main(int argc, char const *argv[])
{
	char currentUserName[30];
	int isLogin = 0;
	while(1) {
		menu();
		int ch;
		scanf("%d", &ch);
		switch(ch) {
			case 1:
				registerFunc();		
				break;
			case 2:
				signInFunc(currentUserName);
				isLogin = 1;
				break;
			case 3:
				if (isLogin == 0) {
					printf("Account is not sign in!\n");
					break;
				} else {
					search();
					break;
				}
			case 4:
				if (isLogin == 0) {
					printf("Account is not sign in!\n");
					break;
				} else {
					signOut(currentUserName);
					break;
				}
				// break;
			default:
				return 0;
		}
	}
	return 0;
}