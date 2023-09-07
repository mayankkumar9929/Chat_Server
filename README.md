
# Chat Server 

This Chat Server uses socket programming and multithreading and works on Linux based OS. The code is written in C++ language. 


## Deployment

To deploy this project first install g++. 
Then run the following commands.

```bash
 g++ server.cpp -lpthread -o server 

 g++ client.cpp -lpthread -o client 
```
To run the server,type
```bash
./server 
```
To run the client, type
```bash
./client 
```
To operate for multiple clients, Open new terminal and type 
./client command .

## Internal Working
The application consists of server and clients which are
 connected to each other by sockets. 

### Server 
The server is supposed to show the activity of various
clients in the chat room. On executing the server code,
the socket is created. Then the bind socket and listen socket
functions are executed. After this the server is open for 
various clients to join. After receiving the request to join
om the client, a new thread is created. And the msg_broadcast 
function is used to send the messages of one client to all 
other clients. 

### Client 
The client side first request to join the server by 
entering a unique username. After this, a socket is created,
and the client is ready to interact with all the other clients.
The entry of each new client is displayed on the server.
The client can exit the chat room by typing “#exit”. 


## Learnings
During the course of this project, I became familiar with the 
concepts of socket programming and multi-threading and their 
use in the implementation of chat server. I also learnt about 
the concept and implementation of mutex in this project. 
## Additional Tasks
 
Chat room server is implemented by using the concepts of socket 		   programming and multi-threading. 

The concept of mutex has been used while adding and 
removing clients from the chat room for proper synchronization. 

The client can exit the chat room by typing “#exit”. 

Entry and Exit of clients is color coded.
## Video Demo


https://user-images.githubusercontent.com/107770107/174855327-a3e68c72-6616-4c26-bfa9-71a2de97a5ad.mp4


## References
https://www.geeksforgeeks.org/socket-programming-cc/ 

https://www.geeksforgeeks.org/multithreading-c-2/ 

https://beej.us/guide/bgnet/pdf/bgnet_a4_c_1.pdf 
