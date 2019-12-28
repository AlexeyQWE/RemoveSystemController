#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define N 1024
#define default_port 3425

int sock, listener;

void settings()
{
    struct sockaddr_in addr;
    listener = socket(AF_INET, SOCK_STREAM, 0);

    if(listener < 0)
    {
        perror("socket");
        exit(1);
    }
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(default_port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }

    listen(listener, 1);
}

void read_sys_info(char *sys_info)
{
   int i = 0; 

   FILE *fl = fopen("res/sys_info.txt", "r");

   while(fscanf(fl, "%c", &sys_info[i]) != EOF)
        ++i;
    
    fclose(fl);
}

void sys_controller(void)
{
	char sys_info[N];
    char command[N];
	int bytes_read;
   
    settings();

    while(1)
    {
        sock = accept(listener, NULL, NULL);
       
        if(sock < 0)
        {
            perror("accept");
            exit(3);
        }

        switch(fork())
        {
        	case -1:
           	 perror("fork");
            	break;

            case 0:
            	close(listener);

		        while(1)
		        {
		            bytes_read = recv(sock, command, sizeof(command), 0);
			   
					if(bytes_read <= 0) 
		                break;			

					system("mkdir res");
					system("echo -n > res/sys_info.txt");
					system(command);

					read_sys_info(sys_info);
					send(sock, sys_info, sizeof(sys_info), 0);	
		           
					system("rm -rf res");
		        }
		        close(sock);
	            _exit(0);
         
         	default:
            	close(sock);
        }	
    }
    
    close(listener);

}

int main()
{
	int pid = fork();
	
	switch(pid) 
	{
		case 0:
			setsid();
			//chdir("/");
			close(0);
			close(1);
			close(2);
			sys_controller();
			exit(0);
		case -1:
			printf("Fail: unable to fork\n");
			break;
		default:
		   printf("OK: demon with pid %d is created\n", pid);
		   break;
	}

    return 0;
}
