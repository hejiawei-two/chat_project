#include<iostream>
#include<string>
#include<vector>
#include<mutex>
#include<sctring>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet/h>
#include<unistd.h>

std::vector<int>client_fds;
std::mutex client_mutex;

void broadcoast(const string& msg,int sender_fd){
    std::lock_guard<std::mutex>lock(client_mutex);
    for(int fd:client_fds){
        if(fd != sender_fd){
            send(fd,msg.c_str(),msg.size(),0);
        }
    }
}

handle_client(int client_fd){
    char buffer[1024];
   
    while(true){
    memset(buffer,0,sizeof(buffer));

    int bytes = recv(client_fd,buffer,sizeof(buffer)-1,0);
    if( bytes <= 0){
        std::lock_guard<std::mutex>lock(client_mutex);
        auto it = std::find_if(client_fds.begin(),client_fds.end(),[client_fd](int fd){return fd == client_fd})
        if(fd!=client_fds.end()){
            client_fds.eraser(it);
            break;
            }
        close(client_fd);
        std::cerr<<"Client disconnected.fd"<<std::endl;
        break;
        }
    std::string msg(buffer);
    std::cout<<"reveived From fd:"<<client_fd<<std::endl;
    broadcast(msg,client_fd);
    }
}


int main(){
    int server_fd = socket(AF_INET,SOCK_STREAM,0);

    if( server_fd == -1 ){
        std::cerr << "Socket Created Failed!\n";
        return -1;
    }

    int opt = 1;
    setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(server_fd,(sockaddr*)&addr,sizeof(addr))<0){
        std::cerr << "Bind Failed!\n";
        return -1;
    }

    if(listen(socket_fd,10)<0){
        std::cerr<<"Listen Failed!\n";
        return -1;
    }

    std::cout<<"Server listening on port 6666.\n";

    while(true){
        sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);

        int client_fd = accept(server_fd,(sockaddr*)client_addr,&len);
        if(client_fd < 0){
            std::cerr << "/Accepted Failed!\n";
            continue;
        }

        {
            std::lock_guard<std::mutex>lock(client_mutex);
            client_fds.push_back(client_fd);
        }

        std::cout<<"Client connected.fd:"<<client_fd<<std::endl;
        std::thread t(handle_client,client_fd);
        t.detach();
    }

    close(server_fd);
    return 0;
}