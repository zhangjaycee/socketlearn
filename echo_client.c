/**************************************************************
*******                                                 *******
***                                                         ***
**   Filename: echo_client.c          Time: 15-05-28-23:15   **
**   Email:    zhjcyx@gmail.com       By: Jaycee             **
***                                                         ***
*******                                                 *******
**************************************************************/

#define MAXLINE 25
#define SA struct sockaddr
#define SERV_PORT 9877

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include"jc_wr.h"
#include"jc_err.h"


void str_cli(FILE *fp,int sockfd)
{
    //printf("str_li()start.\n");
    char sendline[MAXLINE],recvline[MAXLINE];
    while(fgets(sendline,MAXLINE,fp)!=NULL){
        writen(sockfd,sendline,strlen(sendline));
    //printf("str_li()write to server ok.\n");
        if(read(sockfd,recvline,MAXLINE)==0)
            err_quit("str_cli:server terminated prematurely");
        fputs(recvline,stdout);
    }
}

int main(int  argc, char **argv )
{
    int sockfd;
    struct sockaddr_in servaddr;
    if(argc!=2)
        err_quit("usage:tcpcli <IPaddress>");
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET,argv[1],&servaddr.sin_addr);
    
    connect(sockfd,(SA *)&servaddr,sizeof(servaddr));

    str_cli(stdin,sockfd);

    exit(0);

}


