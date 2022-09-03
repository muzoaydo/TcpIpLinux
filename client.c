#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>


#define BUFFSIZE 80
#define PORT 8080
#define SA struct sockaddr

int input = 1;
pthread_t th_read, th_write, th_menu;
int sockfd, nulling;



int is_connected(int socket)
{
    int error_code;
    int error_code_size = sizeof(error_code);
    int err = getsockopt(socket, SOL_SOCKET, SO_ERROR, &error_code, &error_code_size);
    // if (err == -1)
    // {
    //     printf("ERROR: %d\n", error_code);
    // }
    return err;

}



void *writeFunc(void *sockfd)
{
    int *sfd = (int *) sockfd; 
    int socket = *sfd;
    char outBuff[BUFFSIZE];
    int n;
    size_t buffSize = sizeof(outBuff);

    printf("Write thread initialized. Type exit for menu\n");

    while(outBuff !="exit")
    {
        bzero(outBuff, buffSize);
        printf("Enter the message : \n");
        n = 0;
        while ((outBuff[n++] = getchar()) != '\n')
            ;
        printf("\n");
        write(socket, outBuff, buffSize);        
        printf("End of write thread\n");
        pthread_exit(0);
    }
}

void *readFunc(void *sockfd)
{
    int *sfd = (int *) sockfd; 
    int socket = *sfd;
    char inpBuff[BUFFSIZE];
    size_t buffSize = sizeof(inpBuff);

    printf("Read thread initialized.\n");
    while (true) {
        bzero(inpBuff, buffSize);
        int n = 0;
        do{
            usleep(100000);
            n = read(socket, inpBuff, buffSize);
        } while (n <= 0);
        printf("From Server : %s \n", inpBuff);

        if ((strncmp(inpBuff, "exit", 4)) == 0 || is_connected(socket) != 0) {
            printf("Client Exited.\n");
            pthread_exit(0);
            break;
        }
    }
    return(0);
}

   
int main(int argc, char *argv[])
{

    int retRead;
    
    int portNum = atoi(argv[2]);
    struct sockaddr_in servaddr, cli;
    

    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    


   
    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");

    bzero(&servaddr, sizeof(servaddr));
   
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(portNum);

    // printf("servaddr port: %d\n", servaddr.sin_port );
    // while (input == 0)
    // {
    
    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else printf("connected to the server on port: %d..\n", sockfd);
   
    // function for chat
    // readFunc(sockfd);
    // writeFunc(sockfd);

    // int connection = is_connected(sockfd);
    // printf("Is Connected ? : %d", connection);
    int retMenu;

    retRead = pthread_create(&th_read, NULL, readFunc, (void *) &sockfd);

    int retWrite;
    while (true)
    {

        switch (input)
        {
            case 1:
                printf("2- Send a message, 9 - Exit\n -->");
                scanf("%d", &input);
                usleep(1000);
                break;

            case 2:
                retWrite = pthread_create(&th_write, NULL, writeFunc, (void *) &sockfd);
                input = 1;
                break;
            
            case 9:
            {
                close(sockfd);
                exit(0);
                break;
            }

        
        default:
            // printf("1 - Send a message, 9 - Exit\n");
            // scanf("%d", &input);
            break;

        }

    }
    // }
    // retMenu = pthread_create(&th_menu, NULL, menu, (void *) &nulling);
    // int timer = 0;
    // while (is_connected(sockfd) == 0)
    // {
        // sleep(1);
        // timer ++;
        // if (timer == 20) break;
        // printf("Connection: sockfd --> %d, connfd --> %d\n", is_connected(sockfd), is_connected(connfd));
    // }
    

    // close the socket
    printf("exiting the program.\n");
    close(sockfd);
    exit(0);
}