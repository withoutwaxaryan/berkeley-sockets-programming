/* A simple server in the internet domain using TCP
   The port number is passed as an argument.
*/
#include<stdio.h>
#include<stdlib.h>  // standard library - needed for atoi / htons etc
#include<sys/socket.h>  // contains structures needed for sockets - sockaddr
#include<sys/types.h>  // definitions of data types in system calls
#include<string.h>
#include<unistd.h>  // for read write and close
#include<netinet/in.h>  // structures and constants needed for Internet Domain Addresses


void error(const char *msg)
{
    perror(msg);  // integrates the error number and gives the error description
    exit(-1);
}

int main(int argc, char *argv[])  // argc, argv since we are using Terminals -- argc --> total number of parameters i.e. 2 (filename, port number) , argv is the array
{
    if (argc<2)
    {
        fprintf(stderr, "Port Number not provided. Program terminated. \n ");
        exit(-1);
    }

    int sock, newsock, portno, n;
    char buffer[255];  // to send our messages as a string
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;  // gives internet address etc

    sock = socket(AF_INET, SOCK_STREAM, 0);  // TCP
    if (sock < 0)
    {
        error("Error Opening Socket");
    }
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);  // Ascii to Int --> port number

    // Server Data Structure
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno); // convert to Network Byte order
    serv_addr.sin_addr.s_addr = INADDR_ANY; //Server should bind to all available interfaces on the machine --> 0.0.0.0

    // BIND 
    if((bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0)
    {
        error("binding failed ");
    }

    // LISTEN ON THE SOCKET
    if((listen(sock, 5)) == -1) // 5 clients max
    {
        perror("listen");
        exit(-1);
    }

    clilen = sizeof(cli_addr);

    if((newsock = accept(sock, (struct sockaddr *)&cli_addr, &clilen)) < 0)
    {
            perror("accept");
            exit(-1);
    }
    
    while(1)
    {
        // READ
        bzero(buffer, 256);
        n = read(newsock, buffer, 255);
        if (n < 0)
        {
            error("Error on Reading.");
        }
        printf("Client : %s\n", buffer);
        bzero(buffer, 256);
        fgets(buffer, 255, stdin);  //  stdio library --> reads bytes from strings

        // WRITE
        n = write(newsock, buffer, strlen(buffer));
        if (n < 0)
        {
            error("Error on Writing");
        }
        
        // Termination word is Bye
        int i = strncmp("Bye", buffer, 3);
        if (i == 0)
            break;
    }
    // CLOSE
    close(newsock);
    close(sock);
    return 0;
}
