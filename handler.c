#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define N 1024
#define default_port 3425

int sock;

void settings(char *host_id)
{
    struct sockaddr_in addr;
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
  
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(default_port); 
    addr.sin_addr.s_addr = inet_addr(host_id);

    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(2);
    }
}

int main(int argc, char **argv)
{ 
    char command[N];
    char sys_info[N];
    
    system("clear"); 
    printf("\n----------\n   log\n----------\n\n");
    
    while (1) 
    {
       settings(argv[1]);

        gets(command, N, stdin);
	 
		if ((strcmp(command, "clear")) == 0) {
	 		system("clear"); 
   	 		printf("\n----------\n   log\n----------\n\n");
		}
		else {
			strcat(command, " >> res/sys_info.txt");

			send(sock, command, sizeof(command), 0);
			recv(sock, sys_info, sizeof(sys_info), 0);
		
			printf("----------\n%s\n\n", sys_info);
			close(sock);
		}
    }

    return 0;
}
