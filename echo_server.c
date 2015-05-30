/**************************************************************
*******                                                 *******
***                                                         ***
**   Filename: echo_server.c          Time: 15-05-28-21:17   **
**   Email:    zhjcyx@gmail.com       By: Jaycee             **
***                                                         ***
*******                                                 *******
**************************************************************/

#include<stdio.h>
#include<sys/socket.h>

void str_echo(int sockfd)
{
    ssize_t n;
    char buf[MAXLINE];
again:
    while((n=read(sockfd,buf,MAXLINE))>0)
        writen(sockfd,buf,n);
    if(n<0&&errno==EINTR)
        goto again;
    else if(n<0)
        err_sys("str_echo : read error");
}

int main()
{
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
        connfd=accept(listenfd,(SA *)&cliaddr,&clilen);
        if((childpid=fork())==0){//child
            close(listenfd);
            str_echo(connfd);
            exit(0);
        }
        close(connfd);
    }
}
