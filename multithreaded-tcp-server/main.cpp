#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>

int main(){

    int server_fd = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;      //IPv4 address family
    sockaddr.sin_port = htons(8080);
    sockaddr.sin_addr.s_addr = INADDR_ANY;      //Accept incoming requests from any IP

    int opt = 1;
    setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));     //So that the port doesn't go on a wait cycle of 60 seconds after the server is shut down

    int binding = bind(server_fd, (struct sockaddr*) &sockaddr,sizeof(sockaddr));
    if(binding == -1){
        std::cout<<"Couldn't make a working connection. Exiting the program\n";
        return -1;
    }

    int listening = listen(server_fd,3);
    if(listening==-1){
        std::cout<<"Couldn't make a working connection. Exiting the program\n";
        return -1;
    }

    while(true){
        struct sockaddr_in client_addr;
        socklen_t addrlen = sizeof(client_addr);
        int client_fd = accept(server_fd,(struct sockaddr*)&client_addr,&addrlen);      //The client_addr socket struct is used to store the details of the client such as IP Address and the port they are connected from
        if(client_fd==-1){
            std::cout<<"Couldn't accept the incoming connection. Please try again\n";
            continue;
        }
        
        char client_ip[INET_ADDRSTRLEN];    //array to store the IP address's readable version converted in the next step
        inet_ntop(AF_INET,&client_addr.sin_addr,client_ip,addrlen);
        std::cout<<"Connected by client from IP: "<<client_ip<<std::endl;
        close(client_fd);
    }
    close(server_fd);
    return 0;
}