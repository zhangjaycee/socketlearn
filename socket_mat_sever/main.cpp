/**************************************************************
*******                                                 *******
***                                                         ***
**   Filename: img_server.c          Time: 15-07-19-16:14   **
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
#include<time.h>
#include <opencv2/opencv.hpp>


using namespace cv;

int main()//server
{
        int size;
        char rcvbuf[sizeof(Mat)];
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

              //  if((childpid=fork())==0){//child
                        close(listenfd);
                        //memset(rcvbuf, 0x00, sizeof(Mat));
                        int count=0;
                        char key;
                       // while(size =recv(connfd, rcvbuf , sizeof(rcvbuf),0)>0){
                           // memcpy(&frame_recv,rcvbuf,sizeof(Mat));
                           // printf("recved 1 f[%d],size=%d\n",count++,size);
                         while(count<30){

                             int  imgSize = 320*240*3;
                             uchar sockData[imgSize];

                             int bytes=0;
                             int full_flag;//if a frame compelted,it will be 1

                             for (int i = 0; i < imgSize; i += bytes) {
                                 full_flag=1;
                                 bytes = recv(connfd, sockData +i, imgSize  - i, 0);
                                 if (bytes== -1){
                                     printf("recv failed");
                                     exit(1);
                                 }else if(bytes== 0){
                                     full_flag=0;
                                 }
                             }
                            printf("[%d]recved,,full_flag=%d\n",++count,full_flag);
                             // change the last loop to below statement
                             if(full_flag==1){
                                 Mat frame_recv(Size(320, 240), CV_8UC3, sockData);
                                 printf("imshow\n");

                                 imshow("frame_recv",frame_recv);
                                // key=waitKey(33);
                                 if(waitKey(33)==27){
                                    close(connfd);
                                    printf("key break ok\n");
                                    break;
                                 }
                             }else{
                                 close(connfd);
                                 printf("unfull break ok\n");
                                 break;
                             }

                        }

               // }
        }
}
