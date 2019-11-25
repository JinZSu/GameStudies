// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <bits/stdc++.h> 
#include <string.h>
#include <net/if.h> 
#include <pthread.h>
#define PORT 8080
#define NUM_THREADS 8
template <typename variable>
void *Connection(void *threadid,listenfd,void(variable::* nameoffunctionpointer)(vector<unsigned char>),void* charlesclass) {
	long tid = (long)thread;
	unsigned char buffer[1024];
	cout << "Thread with id : " << tid << "  ...starting " << endl;

	cout << "Thread with id : " << tid << "  ...exiting " << endl;
}

class Server{
private:
    int server_fd, new_socket, valread;

    struct sockaddr_in myaddress; //STRUCT TO HOLD PACKET INFO THIS SERVER
    struct sockaddr_in address; //STRUCT TO HOLD PACKET INFO THIS ROBOT

    int opt = 1;
    int PORT_NUM = 42069; // Dummy PORT holder
    int addrlen = sizeof(address);

    char str[INET_ADDRSTRLEN];
    char *ip = "127.0.0.1";  // Dummy IP holder
    char buffer[1024];
    char *hello = "Hello from server";

    pthread_t threads[NUM_THREADS];
	pthread_attr_t attr;
public:
    Socket(){
        // Creating socket file descriptor
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }
        
        // Forcefully attaching socket to the port 8080
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,&opt, sizeof(opt)))
        {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        myaddress.sin_family = AF_INET; //TYPE OF PROTCOL
	if(ip==NULL){
        	myaddress.sin_addr.s_addr = INADDR_ANY; // GET ANY FREE IP
	}
	else{
		inet_pton(AF_INET,ip, &myaddress.sin_addr.s_addr); //SET INOUT IP
	}
        myaddress.sin_port = htons( PORT_NUM ); //SET PORT
        
        // Forcefully attaching socket to the port 8080
        if (bind(server_fd, (struct sockaddr *)&myaddress,
                                    sizeof(myaddress))<0)
        {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
	// Listen to the port
        if (listen(server_fd, 8) < 0)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }
	// If correctly connected
        int i = 0;
        while(TRUE){
	        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
	                        (socklen_t*)&addrlen))<0)
	        {
	            perror("accept");
	            exit(EXIT_FAILURE);
	        }
	        else{
					cout << "Socket() : creating thread, " << i << endl;
					rc = pthread_create(&threads[i], &attr, Connection, (void*) i++,new_socket );
					if (rc) {
					 cout << "Error:unable to create thread," << rc << endl;
					 exit(-1);
					}
					printf("The Server is on IP:%s Port:%d \n",ip,ntohs(myaddress.sin_port));
					printf("The Client is on IP:%s Port:%d \n",inet_ntoa(address.sin_addr),ntohs(address.sin_port));
	        	}	
	        }
    }

    threads_end(){
	int rc;
	int i = 0;
	void *status;

	// free attribute and wait for the other threads
	pthread_attr_destroy(&attr);
	for( i = 0; i < NUM_THREADS; i++ ) {
	rc = pthread_join(threads[i], &status);
		if (rc) {
		 cout << "Error:unable to join," << rc << endl;
		 exit(-1);
		}
	cout << "Main: completed thread id :" << i ;
	cout << "  exiting with status :" << status << endl;
	}

        valread = read( new_socket , buffer, 1024);
        printf("%s\n",buffer );
        send(new_socket , hello , strlen(hello) , 0 );
        //recv();
        printf("Hello message sent\n");
	cout << "Main: program exiting." << endl;
	pthread_exit(NULL);
    }

    Server(int argc, char const *argv[]){
	
	if (argc > 3) {
	fprintf(stderr, "Too much Input\n");
	exit(1);
	}
	else if (argc == 3) {
	PORT_NUM = atoi(argv[1]);
	ip = (char*) argv[2];
	}
	else if (argc == 2) {
	PORT_NUM = atoi(argv[1]);
	}
	// Initialize and set thread joinable
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        
        socket();
        threads_end();
    }
    
};

int main(int argc, char const *argv[])
{

    rdt::Server(argc,argv);
    
}

