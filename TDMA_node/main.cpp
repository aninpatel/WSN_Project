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
#include <pthread.h>
#include <ctime>
#include <cstdlib>
#include <time.h>
using namespace std;
class node
{
public:
    string ip_address;
    char *ip_addr;
    struct tcp_parameters
    {
        int port;
        int initial_socket,b_socket;
        char data_buffer[1024];
        struct sockaddr_in server_address, client_address;
    }tcp;
    struct location
    {
        int x;
        int y;
    }node_location;
public:
    node(char *ip="127.0.0.1",int p=70,int a=0,int b=0)
    {
        ip_addr=ip;
        tcp.port=p;
        node_location.x=a;
        node_location.y=b;
    }
    void set_ip(char *ip)
    {
        ip_addr=ip;
    }
    void set_port(int p)
    {
        tcp.port=p;
    }
    void set_location(int a,int b)
    {
        node_location.x=a;
        node_location.y=b;
    }
    void print()
    {
         cout<<"ip address is "<<ip_addr<<" "<<"port is "<<tcp.port<<" "<<"node location is "<<node_location.x<<","<<node_location.y<<endl;
    }
    void data()
    {
        cout<<"No data"<<endl;
    }
    static void *node_start(void *args)
    {
        struct tcp_parameters *t=(struct tcp_parameters *)args;
        t->initial_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (t->initial_socket < 0)
        {
            cout << "Error opening Socket"<<endl;
            pthread_exit(NULL);
        }
        bzero((char *) &(t->server_address), sizeof(t->server_address));
        t->server_address.sin_family = AF_INET;
        t->server_address.sin_addr.s_addr=htonl(INADDR_ANY);
        t->server_address.sin_port = htons(t->port);

        if (bind(t->initial_socket,(struct sockaddr *) &(t->server_address),sizeof(t->server_address)) < 0)
        {
            cout << "Error binding socket"<<endl;
            perror("error");
            pthread_exit(NULL);
        }
        listen(t->initial_socket,1);

        cout << "Port:" << (t->server_address.sin_port)<<endl;

        t->b_socket = accept(t->initial_socket,(struct sockaddr *) &(t->server_address),(socklen_t*)&(t->server_address));
        //t->b_socket = accept(t->initial_socket,(struct sockaddr *) &(t->server_address),sizeof(t->server_address));
        if (t->b_socket < 0)
        {
            //cout << "Error on accept" <<endl;
            perror("Error on accept");
            pthread_exit(NULL);
        }
        //bzero(t->data_buffer,1023);
        int n = read(t->b_socket,t->data_buffer,1023);
        if(n < 0)
        {
            cout<<"Error reading from socket"<<endl;
        }
        cout<<"Here is the message: "<<t->data_buffer<<" "<<endl;
        return ((void *)true);
    }
    static void *thread_control(void *tcp)
    {
        pthread_t tcp_listner;
        int rc;
        rc=pthread_create(&tcp_listner,NULL,node_start,(void *)&tcp);
        if(rc)
        {
            cout<<"Unable to create thread";
            exit(-1);
        }
        pthread_join(tcp_listner,NULL);
        return ((void *)true);
    }
};


class channel_manager
{

public:

    int superframe_period;
    int frame_period;
    double slot_period=1000;

public:

    void configure_parameters(int s_frame,int frame,int slot)
    {
        superframe_period=s_frame;
        frame_period=frame;
        slot_period=slot;
    }
    bool action()
    {
        pthread_t t;
        cout<<"Inside action"<<endl;
        int success=pthread_create(&t,NULL,timer,(void *)&slot_period);
        void *status=0;
        if(success)
        {
            cout<<"Error creaing thread"<<endl;
        }

        pthread_join(t,&status);
        //int *p=(int *)status;
        cout <<*(int *)status<<endl;
        /*if(*(int *)status == 1)
        {
            cout<<"Timer ticked and it's time to poll the nodes"<<endl;
        }*/
        return true;

    }

    static void *timer(void *time_frame)
    {
        double *t=(double *)time_frame;
        bool flag=true;
        int *f;
        //cout<<"Time for slot is "<<*t<<endl;
        double second_passed=0.0;
        clock_t start_time=clock();
        while(1)
        {
            while(flag)
            {
                second_passed=(1000*(clock() - start_time))/ (CLOCKS_PER_SEC);
                //cout<<second_passed<<endl;
                if(second_passed >= *t)
                {
                 //cout << "second passed :"<<second_passed<<endl;
                    flag=false;
                }
            }
            int a=1;
            f=&a;
            flag=true;
            second_passed=0.0;
            start_time=clock();
            printf("Timer ticks\n");
        }
        return ((void *)f);
    }

};

int main()
{
    int number_nodes;
    string ip;
    char *a=new char[100];
    string arr;
    int port;
    int x,y;
    cout<< "Enter the number of nodes you want to create :" <<endl;
    cin >> number_nodes;
    node n[number_nodes];
    for (int i=0;i<number_nodes;i++)
    {
        cout<<"Enter IP address for node "<<i+1<<endl;
        cin>>arr;
        strcpy(a, arr.c_str());
        n[i].set_ip(a);
        cout<<"Enter Port number for node "<<i+1<<endl;
        cin>>port;
        n[i].set_port(port);
        cout<<"Enter x and y coordinates for node "<<i+1<<endl;
        cin>>x;
        cin>>y;
        n[i].set_location(x,y);
    }
    for (int i=0;i<number_nodes;i++)
    {
        cout<<"Node "<<i+1<<"parameters :"<<endl;
        n[i].print();
    }
    pthread_t node_thread;

    /*for (int i=0;i<number_nodes;i++)
    {
        n[number_nodes].thread_control();
    }*/

    int q=pthread_create(&node_thread,NULL,n[0].thread_control,(void *)&(n[0].tcp));
    if(q)
    {
        cout<<"Error in node thread"<<endl;
    }
    pthread_t tcp_listner;
    channel_manager c;
    pthread_t t;
    int rc;
    int success;
    void *status=0;
    //rc=pthread_create(&tcp_listner,NULL,n[0].node_start,(void *)&(n[0].tcp));
    /*if(rc)
    {
        cout<<"Unable to create thread";
    }*/
    //printf("Hello");

    success=pthread_create(&t,NULL,c.timer,(void *)&(c.slot_period));
    if(success)
    {
        cout<<"Error creaing thread"<<endl;
    }
    pthread_join(t,&status);
    pthread_join(node_thread,NULL);
    /*pthread_t z;
    channel_manager c;
    bool flag=c.action();*/
    return 0;
}
