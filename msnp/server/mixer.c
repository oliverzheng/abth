/*
** server.c -- a stream socket server demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define MYPORT 1863    // the port users will be connecting to

#define BACKLOG 10     // how many pending connections queue will hold

#define MAX_BUF_SIZE 5000

static void chat (int sock);

void sigchld_handler(int s)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

int main(void)
{
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct sockaddr_in my_addr;    // my address information
    struct sockaddr_in their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }
    
    my_addr.sin_family = AF_INET;         // host byte order
    my_addr.sin_port = htons(MYPORT);     // short, network byte order
    my_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
    memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof my_addr) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    while(1) {  // main accept() loop
        sin_size = sizeof their_addr;
		printf("waiting to accept connection\n");
        if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, \
                &sin_size)) == -1) {
            perror("accept");
            continue;
        }
        printf("server: got connection from %s\n", \
            inet_ntoa(their_addr.sin_addr));
        if (!fork()) { // this is the child process
            close(sockfd); // child doesn't need the listener
			
			chat(new_fd);
				
            close(new_fd);
            exit(0);
        }
        close(new_fd);  // parent doesn't need this
    }

    return 0;
}

static void chat (int sock)
{
	char buf[MAX_BUF_SIZE];
	int numbytes = 0;
	int trid = 0;
	
	// initialize
	// 1) receive ANS
    if ((numbytes=read(sock, buf, MAX_BUF_SIZE - 1)) <= 0) {
        perror("recv");
        exit(1);
    }
	buf[numbytes] = '\0';
	printf("Received: %s", buf);
	
	sscanf(buf, "ANS %d ", &trid);
	
	sprintf(buf, "IRO %d 1 1 eece412_alice@hotmail.com Alice 1985855532\r\n", trid);
	if (send(sock, buf, strlen(buf), 0) == -1) {
		perror("send");
	}
	printf("Sent:     %s", buf);
	
	sprintf(buf, "ANS %d OK\r\n", trid);
	if (send(sock, buf, strlen(buf), 0) == -1) {
		perror("send");
	}
	printf("Sent:     %s", buf);
	
	strcpy(buf, "");
	sprintf(buf, "MSG eece412_alice@hotmail.com Alice 125\r\n");
	sprintf(buf, "%sMIME-Version: 1.0\r\n", buf);
	sprintf(buf, "%sContent-Type: text/plain; charset=UTF-8\r\n", buf);
	sprintf(buf, "%sX-MMS-IM-Format: FN=MS%%20Shell%%20Dlg; EF=; CO=0; CS=0; PF=0\r\n\r\n", buf);
	sprintf(buf, "%s\r\n", buf);
	sprintf(buf, "%shi", buf);
	if (send(sock, buf, strlen(buf), 0) == -1) {
		perror("send");
	}
	printf("Sent:     %s", buf);
	
	strcpy(buf, "");
    while ((numbytes=read(sock, buf, MAX_BUF_SIZE - 1)) > 0) {
		printf("Received: %s", buf);
		strcpy(buf, "");
	}
}
