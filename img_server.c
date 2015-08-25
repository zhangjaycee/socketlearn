/**************************************************************
*******                                                 *******
***                                                         ***
**   Filename: chat_server.c          Time: 15-07-19-16:14   **
**   Email:    zhjcyx@gmail.com       By: Jaycee             **
***                                                         ***
*******                                                 *******
**************************************************************/

#define MAXLINE 50
#define SA struct sockaddr
#define SERV_PORT 9877
#define LISTENQ 5

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<termios.h>
#include<sys/ioctl.h>
#include<signal.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include"jc_wr.h"
#include"jc_err.h"
#include<time.h>

FILE * fp;

int main()//server
{
        int size;
        fp=fopen("recvtest.jpg","w+");
        char rcvbuf[30960];
        
        pid_t childpid;
        int listenfd,connfd;
        socklen_t clilen;
        struct sockaddr_in cliaddr,servaddr;
        listenfd=socket(AF_INET,SOCK_STREAM,0);//AF_INET:ipv4  SOCK_STREAM:字节流
        bzero(&servaddr,sizeof(servaddr));//将servaddr置零
        servaddr.sin_family = AF_INET;
        //INADDR_ANY本机地址0.0.0.0,可以代表任意所有网卡的地址
        //htonl函数把本机序转为网络序,用于long型；htons函数功能类似，用于short型
        servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
        servaddr.sin_port=htons(SERV_PORT);
        bind(listenfd,(SA *)&servaddr,sizeof(servaddr));
        listen(listenfd,LISTENQ);
        for(;;){
                clilen = sizeof(cliaddr);
        printf("ok\n");
                connfd=accept(listenfd,(SA *)&cliaddr,&clilen);
                if((childpid=fork())==0){//child
        printf("ok\n");
                        close(listenfd);

                        memset(rcvbuf, 0x00, sizeof(rcvbuf));
                        size = readn(connfd, rcvbuf , sizeof(rcvbuf));
                        fwrite(rcvbuf,1,size,fp);
                        printf("write finished,anykey to exit\n");
                        getchar();
                        close(connfd);
                        exit(0);
                }
        }
}
