/*
Server sends a message to the client.
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<arpa/inet.h>
int main()
{
    int sock; // variable for socket
    int cli; // socket descriptor for the client
    int sent; // bytes sent by send
    struct sockaddr_in server, client;
    unsigned int len; // can be negative as well
    char mesg[] = "Hello World";

    if((sock = socket(AF_INET, SOCK_STREAM, 0))== -1) // error is present
    {
        perror("socket: ");
        exit(-1);
    }
    server.sin_family = AF_INET;
    server.sin_port = htons(10000); // convert to Network order
    server.sin_addr.s_addr = INADDR_ANY; //Server should bind to all available interfaces on the machine
    bzero(&server.sin_zero, 8);


    // call Bind
    len = sizeof(struct sockaddr_in);

    if((bind(sock, (struct sockaddr *)&server, len)) == -1)
    {
        perror("bind");
        exit(-1);
    }

    // listen on the socket
    if((listen(sock, 5)) == -1) // 5 clients max
    {
        perror("listen");
        exit(-1);
    }

    while(1)
    {
        if((cli = accept(sock, (struct sockaddr *)&client, &len)) == -1)
        {
            perror("accept");
            exit(-1);
        }
        // client socket has returned and we can talk to the client
        sent = send(cli, mesg, strlen(mesg), 0);
        printf("Sent %d bytes to client : %s \n", sent, inet_ntoa(client.sin_addr));

        // close(cli);
        pclose(cli);

    }


}
// In one terminal run gcc HelloServer.c -o HelloServer
// and then ./HelloServer 
// This gets the server running
// in another terminal first check if the server is up by running : netstat -atp
// Then in the same terminal run : telnet localhost 10000
// As the client connects to the server, the server sends the message "Hello World" to the client, and then closes the connection.
// If you go to the previous terminal , you will see 11 bytes sent --> this is the length of the string "Hello World"
