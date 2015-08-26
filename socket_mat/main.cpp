#define SA struct sockaddr
#define SERV_PORT 9877
#define LISTENQ 5


#include<stdio.h>
#include <opencv2/opencv.hpp>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<termios.h>
#include<sys/ioctl.h>
#include<signal.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include <netinet/in.h>
#include<arpa/inet.h>
#include"jc_wr.h"

#include<time.h>


using namespace  cv;

int main(int argc, char *argv[])
{   
    //init cam
    int camNumber=0;
    if(argc>1){
        camNumber=atoi(argv[1]);
    }
    VideoCapture webcam;
    webcam.open(camNumber);

    if(!webcam.isOpened()){
        printf("cam open error!\n");
        exit(1);
    }
    webcam.set(CV_CAP_PROP_FRAME_WIDTH,320);
    webcam.set(CV_CAP_PROP_FRAME_HEIGHT,240);
    printf("width=%f  height=%f\n",webcam.get(CV_CAP_PROP_FRAME_WIDTH),webcam.get(CV_CAP_PROP_FRAME_HEIGHT));


    //init socket
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

            int conn_st=connect(connfd,(SA *)&servaddr,sizeof(servaddr));
            if(conn_st==-1){
                     printf("connect error\n");
                     close(connfd);
                     exit(0);
             }

                //grav start
                printf("forked\n");
                Mat frame;
                int count=0;
                char sendbuf[sizeof(frame)+1];
                char key;
                while(webcam.read(frame)){
                    printf("frab a frame [%d]\n",++count);
                    imshow("frame",frame);
                    frame = (frame.reshape(0,1));
                    int num=frame.total();                      /* num = 320*240 */
                    int num2=frame.elemSize();                  /* mun2 =3 channel */
                    int  imgSize = frame.total()*frame.elemSize();

                   // memcpy(sendbuf,&frame,sizeof(Mat));
                    //send(connfd,sendbuf,sizeof(Mat),0);
                  send(connfd, frame.data, imgSize, 0);
                  printf("send a frame\n");

                    key=waitKey(20);
                    if(key==27){
                        break;
                    }

                    if(count==500){

                        break;
                    }
                }

            close(connfd);

}
