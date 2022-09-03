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


#define BUFFSIZE 80
#define PORT 9999
#define SA struct sockaddr

int input = 1;
pthread_t th_read, th_write, th_menu;
int sockfd, connfd, nulling;

int is_connected(int socket)
{
    int error_code;
    int error_code_size = sizeof(error_code);
    int err = getsockopt(socket, SOL_SOCKET, SO_ERROR, &error_code, &error_code_size);
    if (err == -1)
    {
        printf("ERROR: %d\n", error_code);
    }

}
int logMsg(char* time, char *message, char *sender)
{
    FILE *logFile;
    char *logMsg;
    // strcpy(logMsg, message);
    
    logFile  = fopen ("messages.log", "a");
    fprintf(logFile, "%s %s %s \n", time, sender , message); 
    fclose(logFile);
}

char* getTime()
{
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    return asctime (timeinfo);
}

void *writeFunc(void *sockfd)
{
    int *sfd = (int *) sockfd; 
    int socket = *sfd;
    char outBuff[BUFFSIZE];
    int n;
    size_t buffSize = sizeof(outBuff);
    printf("Server Write thread initialized.\n");


    // for (;;)
    //  {
        bzero(outBuff, buffSize);
        printf("Enter the string : \n -->");
        n = 0;
        // while ((outBuff[n++] = getchar()) != '\n');
        while ((outBuff[n++] = getchar()) != '\n');
        int success = write(socket, outBuff, buffSize);
        if (success != -1)
        {
            char *sender = "Server:";
            char *time = getTime();
            logMsg(time, outBuff, sender);
        }
        else
        {
            printf("Couldn't send message");
        }
        pthread_exit(0);
    // }
}

void *readFunc(void *sockfd)
{
    int *sfd = (int *) sockfd; 
    int socket = *sfd;
    char inpBuff[BUFFSIZE];
    size_t buffSize = sizeof(inpBuff);

    printf("Server Read thread initialized.\n");
    for (;;) {
        bzero(inpBuff, buffSize);
        int n = 0;
        do{
            usleep(100000);
            n = read(socket, inpBuff, buffSize);
        } while (n <= 0);

        
        printf("From Client : %s \n", inpBuff);

        char *sender = "Client:";
        char *time = getTime();
        logMsg(time, inpBuff, sender);


        if ((strncmp(inpBuff, "exit", 4)) == 0 || is_connected(socket) != 0) {
            printf("Client Exited.\n");
            pthread_exit(0);
            break;
        }
    }
    printf("Read func ending.\n");

    return(0);
}


// Driver function
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }

    int portNum = atoi(argv[1]);

    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;
   
    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(2);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));
   
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(portNum);
   
    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(3);
    }
    else
        printf("Socket successfully binded..\n");
    int err = (listen(sockfd, 5));
    // Now server is ready to listen and verification
    if (err != 0) {
        printf("Listen failed... Error Code On Listen: %d\n", err);
        exit(4);
    }
    else
        printf("Server listening.. on port: %d\n", sockfd);
    len = sizeof(cli);
   
    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("server accept failed...\n");
        exit(5);
    }
    else
        printf("server accept the client...\n");

// while (is_connected(connfd) == 0)
//     {
//         sleep(1);
        // timer ++;
        // if (timer == 20) break;
        // printf("Connection: connfd --> %d\n", is_connected(connfd));
    // }
    
    // Function for chatting between client and server
    // func(connfd);

    int retMenu;
    int retRead;

    retRead = pthread_create(&th_read, NULL, readFunc, (void *) &connfd);

    if (retRead == 0){
        //printf("Read thread succesfully created.\n");
    }
    else{
        printf("Read thread failed.\n");
    }
    // }
    // retMenu = pthread_create(&th_menu, NULL, menu, (void *) &nulling);

    int retWrite;
    while (true)
    {

        switch (input)
        {
            case 1:
                printf("2 - Send a message, 9 - Exit: \n -->");
                scanf("%d", &input);
                // printf("\n");
                usleep(1000);
                break;

            case 2:
                retWrite = pthread_create(&th_write, NULL, writeFunc, (void *) &connfd);
                input = 1;
                break;
            
            case 9:
            {
                printf("Closing with 9");
                close(sockfd);
                exit(9);
                break;
            }

        
        default:
            // printf("1 - Send a message, 9 - Exit: ");
            // scanf("%d", &input);
            // printf("..\n");
            break;

        }

    }


    printf("Closing with end of the code.\n");
   
    // After chatting close the socket
    close(sockfd);
    exit(0);
}
