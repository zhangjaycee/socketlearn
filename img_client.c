/**************************************************************
*******                                                 *******
***                                                         ***
**   Filename: chat_client.c          Time: 15-08-19-16:14   **
**   Email:    zhjcyx@gmail.com       By: Jaycee             **
***                                                         ***
*******                                                 *******
**************************************************************/

#define MAXLINE 50 
#define SA struct sockaddr
#define SERV_PORT 9877

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<signal.h>
#include<termios.h>
#include<sys/ioctl.h>
#include"jc_wr.h"
#include"jc_err.h"
#include<time.h>

 FILE * fp;


void img_send(int sockfd)
{
        if (fp == NULL){
                fprintf(stderr, "cannot open image file\n");    
                exit(1);
        }
        fseek(fp, 0, SEEK_END);
        if (ferror(fp)) {
                fprintf(stderr, "fseek() failed\n");
                int r = fclose(fp);
                if (r == EOF) {
                fprintf(stderr, "cannot close file handler\n");          
                }    
                exit(1);
        }  
  
        int flen = ftell(fp);
  
        if (flen == -1) {
                perror("error occurred");
                int r = fclose(fp);
                if (r == EOF) {
                        fprintf(stderr, "cannot close file handler\n");
                }
      
        exit(1);      
        }
  
        fseek(fp, 0, SEEK_SET);
  
        if (ferror(fp)) {
                fprintf(stderr, "fseek() failed\n");
        int r = fclose(fp);

        if (r == EOF) {
                fprintf(stderr, "cannot close file handler\n");
        }    
      
        exit(1);
        }

        char data[flen+1];

        int size = fread(data, 1, flen, fp);
  
        if (ferror(fp)) {
                fprintf(stderr, "fread() failed\n");
                int r = fclose(fp);
                if (r == EOF) {
                        fprintf(stderr, "cannot close file handler\n");
                }
        exit(1);      
        }
  
        int r = fclose(fp);

        if (r == EOF) {
                fprintf(stderr, "cannot close file handler\n");
        } 
        //printf("%d\n [%#x]\n", size, data);

        writen(sockfd,data,size);
}

int main(int  argc, char **argv )
{
        fp=fopen("sendtest.jpg","r");
        char ip_addr;
        int connfd;
        pid_t childpid;
        struct sockaddr_in servaddr;
        if(argc!=2){
                printf("usage:tcpcli <IPaddress>\ndefault:127.0.0.1 start\n");
        }
        connfd = socket(AF_INET,SOCK_STREAM,0);
        bzero(&servaddr,sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(SERV_PORT);
        if(argc!=2){
                inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr);
        }else{
                inet_pton(AF_INET,argv[1],&servaddr.sin_addr);
        }
        for(;;){
                int conn_st=connect(connfd,(SA *)&servaddr,sizeof(servaddr));
                if(conn_st==-1){
                         printf("connect error\n");
                         close(connfd);
                         exit(0);
                 }
                if((childpid=fork())==0){//child
                        img_send(connfd);
                        printf("send finished,anykey to exit()\n");
                        getchar();
                        exit(0);
                }
        }
}


