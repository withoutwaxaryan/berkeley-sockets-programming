/*
In the terminal you need to enter 
filename server_ipaddress portno


arg[0] --> filename
arg[1] --> server_ipaddress
arg[2] --> portno
*/

#include<stdio.h>
#include<stdlib.h>  // standard library - needed for atoi / htons etc
#include<unistd.h>  // for read write and close
#include<string.h>
#include<sys/types.h>  // definitions of data types in system calls
#include<sys/socket.h>  // contains structures needed for sockets - sockaddr
#include<netinet/in.h>  // structures and constants needed for Internet Domain Addresses
#include<netdb.h>

void error(const char *msg)
{
    perror(msg);  // integrates the error number and gives the error description
    exit(0);
}

int main(int argc, char *argv[])  // argc, argv since we are using Terminals -- argc --> total number of parameters i.e. 2 (filename, port number) , argv is the array
{
    int sock, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server; // defines the host structure --> netdb.h, stores info about host like IP address etc
    
    char buffer[256];
    if (argc < 3)
    {
        fprintf(stderr, "usage %s hostname port \n", argv[0]);
        exit(0);
    }
    portno = atoi(argv[2]); // Ascii to Int --> port number
    sock = socket(AF_INET, SOCK_STREAM, 0); // TCP
    if (sock < 0)
    {
        error(" Error opening Socket");
    }
    server = gethostbyname(argv[1]);  // gets the data --> ip address of the server
    if (server == NULL)  // if ip address is wrong
    {
        fprintf(stderr, " Error, no such host");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr,
          (char *) &serv_addr.sin_addr.s_addr, 
          server->h_length);  // copies n bytes from struct hostent *server to serv_addr -- > transferring information
    serv_addr.sin_port = htons(portno);

    // CONNECT
    if(connect(sock,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
       error("Connection Failed");
    }
    printf("Client : ");
    while(1)
    {
        bzero(buffer, 256);
        fgets(buffer, 255, stdin);
        // Write
        n = write(sock, buffer, strlen(buffer));
        if(n < 0)
        {
            error("Error on Writing");
        }
        bzero(buffer, 256);
        n = read(sock, buffer, 255);
        if(n < 0)
        {
            error("Error on Reading");
        }
        printf("Server : %s", buffer);

        // Termination word is Bye
        int i = strncmp("Bye", buffer, 3);
        if (i == 0)
            break;
    }
    // CLOSE
    close(sock);
    return 0;
    
}
