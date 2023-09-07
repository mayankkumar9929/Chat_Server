#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#define MAX_LEN 200
#define NUM_COLORS 6
using namespace std;

struct terminal{
	int id;
	string name;
	int socket;
	thread th;
};

vector<terminal> clients;
string def_col="\033[0m";
string colors[]={"\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m","\033[36m"};
int sd=0;
mutex cout_mutex,client_mutex;

string color(int code);
void set_name(int id, char name[]);
void shared_print(string str, bool endLine);
void handle_client(int client_socket, int id);
int msg_broadcast(string message, int sender_id);
int msg_broadcast(int num, int sender_id);
void end_connection(int id);


int main(){
	int server_socket;
	
	if((server_socket=socket(AF_INET,SOCK_STREAM,0))==-1){
	perror("socket: ");
	exit(-1);
	}
	
	struct sockaddr_in server;
	server.sin_family=AF_INET;
	server.sin_port=htons(10000);
	server.sin_addr.s_addr=INADDR_ANY;
	bzero(&server.sin_zero,0);
	
	
	if((bind(server_socket,(struct sockaddr *)&server,sizeof(struct sockaddr_in)))==-1){
	perror("bind error: ");
	exit(-1);
	}
	
	
	if((listen(server_socket,8))==-1){
	perror("listen error: ");
	exit(-1);
	}
	
	
	struct sockaddr_in client;
	int client_socket;
	unsigned int len=sizeof(sockaddr_in);
	
	
	cout << colors[NUM_COLORS-1]<<endl ; 
	cout<<"*********CHAT SERVER***********"<<endl ;
	cout<<def_col;
	
	while(true){
		if((client_socket=accept(server_socket,(struct sockaddr *)&client,&len))==-1){
		perror("accept error: ");
		exit(-1);
		}
		
		sd++;
		thread t(handle_client,client_socket,sd);
		lock_guard<mutex> guard(client_mutex);
		clients.push_back({sd, string("Anonymous"),client_socket,(move(t))});
	}
	
	
	for(int i=0; i<clients.size(); i++){
		if(clients[i].th.joinable()) clients[i].th.join();
	}
	
	
	close(server_socket);
	
	return 0;
}


// Naming the client
void set_name(int id, char name[]){
	for(int i=0; i<clients.size(); i++){
			if(clients[i].id==id) 
			clients[i].name=string(name);		
	}	
}



	string color(int code){
			return colors[code%NUM_COLORS];
		}


// Synchronising the cout statements
void shared_print(string str, bool endLine=true){	
	lock_guard<mutex> guard(cout_mutex);
	cout<<str;
	if(endLine) cout<<endl;
}



void handle_client(int client_socket, int id){
	char name[MAX_LEN],str[MAX_LEN];
	recv(client_socket,name,sizeof(name),0);
	set_name(id,name);	

	// Display welcome message
	string welcome_message=string(name)+string(" has joined the Chat Room");
	msg_broadcast("#NULL",id);	
	msg_broadcast(id,id);								
	msg_broadcast(welcome_message,id);	
	shared_print(color(id)+welcome_message+def_col);
	
	while(true){
		int bytes_received=recv(client_socket,str,sizeof(str),0);
		if(bytes_received<=0)
			return;
		if(strcmp(str,"#exit")==0){
			// Display leaving message
			string message=string(name)+string(" has left the Chat Room");		
			msg_broadcast("#NULL",id);			
			msg_broadcast(id,id);						
			msg_broadcast(message,id);
			shared_print(color(id)+message+def_col);
			end_connection(id);							
			return;
		}
		msg_broadcast(string(name),id);					
		msg_broadcast(id,id);		
		msg_broadcast(string(str),id);
		shared_print(color(id)+name+" : "+def_col+str);		
	}	
}


// Broadcasting the message to all clients except the sender
int msg_broadcast(string message, int sender_id){
	char temp[MAX_LEN];
	strcpy(temp,message.c_str());
	for(int i=0; i<clients.size(); i++){
		if(clients[i].id!=sender_id) 
		send(clients[i].socket,temp,sizeof(temp),0);
	}		
}

// Broadcasting a number to all clients except the sender
int msg_broadcast(int num, int sender_id){
	for(int i=0; i<clients.size(); i++){
		if(clients[i].id!=sender_id) 
		send(clients[i].socket,&num,sizeof(num),0);
	}		
}


void end_connection(int id){
	for(int i=0; i<clients.size(); i++){
		if(clients[i].id==id){
			lock_guard<mutex> guard(client_mutex);
			clients[i].th.detach();
			clients.erase(clients.begin()+i);
			close(clients[i].socket);
			break;
		}
	}				
}


