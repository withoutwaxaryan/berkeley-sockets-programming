/* Client connects to the remote TCP Echo server.
   Then requests users to enter various strings, sends it to the echo server.
   THe Server echoes the message back and the client displays it to the user.
*/
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<error.h>
#include<strings.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>


#define ERROR     -1
#define BUFFER  1024


int main(int argc, char **argv)
{
    struct sockaddr_in remote_server;
    int sock; // socket descriptor to connect to the remote server
    char input[BUFFER]; // user input
    char output[BUFFER]; // output from remote server
    int len;

    if((sock = socket(AF_INET, SOCK_STREAM, 0))== -1) // error is present
    {
        perror("socket: ");
        exit(-1);
    }
    //  Remote Server Data Structure
    remote_server.sin_family = AF_INET;
    remote_server.sin_port = htons(atoi(argv[2])); // convert to Network Byte order
    remote_server.sin_addr.s_addr = inet_addr(argv[1]); // ip in ascii form which we convert to network byte order
    bzero(&remote_server.sin_zero, 8);

    if((connect(sock, (struct sockaddr *)&remote_server, sizeof(struct sockaddr_in))) == ERROR) // COnnect call to remote server
    {
        perror("connect");
        exit(-1);
    }
    while(1)
    {
        fgets(input, BUFFER, stdin); // input line 
        send(sock, input, strlen(input), 0);

        len = recv(sock, output, BUFFER, 0); // same message is sent back
        output[len] = '\0';
        printf("%s \n", output);
    }
    close(sock);
}
/*
Run the server first : ./EchoServer 10000
IN another terminal check tcp connections (10000 one) using : netstat -atn
Now run the client : ./EchoClient 127.0.0.1 10000 and start sending messages
check the server terminal as well to see the sent messages
press ctrl + c to disconnect client.
*/
