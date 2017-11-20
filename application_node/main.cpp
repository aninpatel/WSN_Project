
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main()
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;

    struct test
    {
        char buffer[256]="hello";
        int a;
        bool tx=false;
    }t;


    //char buffer[256]="Hello";
    char a[256]="Hello";
    //string a=buffer;
    //cout <<strcmp(buffer,a)<<endl;
    char exit;
    bool flag=true;
    char cmp[256]="exit";
    bzero(t.buffer,256);
    do
    {
        cout<<"Enter the port number you want to connect to"<<endl;
        cin>>portno;
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
        {
            error("ERROR opening socket");
        }
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
        serv_addr.sin_port = htons(portno);
        //serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

        if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        {
            error("ERROR connecting");
        }
        cout<<"Please enter the message, enter exit if you want to exit: "<<endl;
        //fgets(buffer,255,stdin);
        cin>>t.buffer;

        cout<<"Enter the number: "<<endl;
        cin>>t.a;

        if(strcmp(t.buffer,cmp)==0)
        {
            flag=false;
            n = write(sockfd,(void *)&t,sizeof(t));
            if (n < 0)
            {
                error("ERROR writing to socket");
            }
            close(sockfd);
        }
        else
        {
            n = write(sockfd,(void *)&t,sizeof(t));
            if (n < 0)
            {
                error("ERROR writing to socket");
            }
            close(sockfd);
        }
        bzero(t.buffer,256);
    }while(flag);
    return 0;
}
