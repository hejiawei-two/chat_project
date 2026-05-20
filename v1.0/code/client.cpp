#include<iostream>
#include<string>
#include<thread>
#include<cstring>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

void handler(int client_fd){
    char buffer[1024];
    
    while(true){
        memset(buffer,0,sizeof(buffer));
        int bytes = recv(client_fd,buffer,sizeof(buffer)-1,0);
        if(bytes <= 0){
            std::cerr<<"Received from Server Failed\n";
            close(client_fd);
            exit(0);
        }
        std::cout<<"\n[Message:]"<<buffer<<"\nYOU\n"<<std::flush;
    }
}

int main(){
    int client_fd = socket(AF_INET,SOCK_STREAM,0);
    if(client_fd == -1){
        std::cerr << "Socket Creation Failed\n";
        return -1;
    }
    
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr);

    int c_fd = connect(client_fd,(sockaddr*)&addr,sizeof(addr));

    if( c_fd == -1){
        std::cerr <<"Connection Failed!\n";
        return -1;
    }
    std::thread t (handler,client_fd);
    t.detach();

    std::string input;
    while(true){
        std::cout << "YOU:" << std::flush;
        std::getline(std::cin,input);
        if(input == "/quit") break;
        send(client_fd,input.c_str(),input.size(),0);
    }
    close(client_fd);
    return 0;
}
