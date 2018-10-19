/* tcp_ client.c */ 
/* Programmed by Adarsh Sethi */
/* Sept. 13, 2018 */     

#include <stdio.h>          /* for standard I/O functions */
#include <stdlib.h>         /* for exit */
#include <string.h>         /* for memset, memcpy, and strlen */
#include <netdb.h>          /* for struct hostent and gethostbyname */
#include <sys/socket.h>     /* for socket, connect, send, and recv */
#include <netinet/in.h>     /* for sockaddr_in */
#include <unistd.h>         /* for close */

#define STRING_SIZE 1024

void getRespose(int sock_client, char modifiedSentence[STRING_SIZE]);
void sendMsg(int sock_client, char sentence[STRING_SIZE]);
int connector();

int connector() 
{ 
    int sock_client;  /* Socket used by client */

    struct sockaddr_in server_addr;  /* Internet address structure that
                                        stores server address */
    struct hostent * server_hp;      /* Structure to store server's IP
                                        address */
    char server_hostname[STRING_SIZE]; /* Server's hostname */
    unsigned short server_port;  /* Port number used by server (remote port) */
   
    /* open a socket */

    if ((sock_client = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        perror("Client: can't open stream socket");
        exit(1);
    }

   /* Note: there is no need to initialize local client address information 
            unless you want to specify a specific local port
            (in which case, do it the same way as in udpclient.c).
            The local address initialization and binding is done automatically
            when the connect function is called later, if the socket has not
            already been bound. */

   /* initialize server address information */

   //printf("Enter hostname of server: ");
   //scanf("%s", server_hostname);
    if ((server_hp = gethostbyname("127.0.0.1")) == NULL) {
        perror("Client: invalid server hostname");
        close(sock_client);
        exit(1);
    }

   //printf("Enter port number for server: ");
   //scanf("%hu", &server_port);

    /* Clear server address structure and initialize with server address */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    memcpy((char *)&server_addr.sin_addr, server_hp->h_addr,
                                    server_hp->h_length);
    //server_addr.sin_port = htons(server_port);
    server_addr.sin_port = htons(56001);

    /* connect to the server */
 	if (connect(sock_client, (struct sockaddr *) &server_addr, 
                                    sizeof (server_addr)) < 0) {
        perror("Client: can't connect to server");
        close(sock_client);
        exit(1);
    }
    return sock_client;
} 

void getRespose(int sock_client, char modifiedSentence[STRING_SIZE]) {
   int bytes_recd = recv(sock_client, modifiedSentence, STRING_SIZE, 0);
   unsigned int msg_len; 
   msg_len = strlen(modifiedSentence) + 1;
   printf("\nThe response from server is: ");
   printf("%s\n", modifiedSentence);
   printf("Message size: %d\n", msg_len);
}

void sendMsg(int sock_client, char sentence[STRING_SIZE]){
   unsigned int msg_len; 
   msg_len = strlen(sentence) + 1;
   int bytes_sent = send(sock_client, sentence, msg_len, 0);
   printf("\nMessage sent to server: ");
   printf("%s\n", sentence);
   printf("Message size: %d\n", msg_len);
}

int main(void) {

    int sock_client;  /* Socket used by client */

    char *sentence[STRING_SIZE];  /* send message */
    char modifiedSentence[STRING_SIZE]; /* receive message */
    unsigned int msg_len;  /* length of message */                      
    int bytes_sent, bytes_recd; /* number of bytes sent or received */

   /* user interface */
   do{
      sock_client = connector();
      // user input answer
      int answer = 0;
      memset(sentence, 0, STRING_SIZE);
      // temp string to store message
      char tempStr[24] = "";
      printf("\nPlease choose type of transaction:\n\t1 - for Balance\n\t2 - for Deposit\n\t3 - for Withdraw\n\t4 - for Transfer\n\t5 - to Exit\n");
      //gets the input from the client
      scanf("%d", &answer);
      if(answer == 1){
         answer = 0;
         //ask client to enter the type of account to check the balance
         printf("\nPlease choose account:\n\t1 - for Checking\n\t2 - for Saving\n");
         //gets input from the client
         scanf("%d", &answer);
         if(answer == 1){
            //assigns string to sentence
            *sentence = "Balance_Checking";
            /* send message */
            sendMsg(sock_client, *sentence);
            /* receive message */
            getRespose(sock_client, modifiedSentence);
         }else if(answer == 2){
            //assigns string to sentence
            *sentence = "Balance_Saving";
            /* send message */
            sendMsg(sock_client, *sentence);
            /* receive message */
            getRespose(sock_client, modifiedSentence);
         }else{
            printf("%d is not a valid response!\n", answer);
         }
         //ask client to enter the type of account to deposit the amount
      }else if(answer == 2){
         answer = 0;
         printf("\nPlease choose account:\n\t1 - for Checking\n\t2 - for Saving\n");
         //gets input from the client
         scanf("%d", &answer);
         if(answer == 1){
            answer = 0;
            printf("\nPlease enter the amount to deposit:\n");
            scanf("%d", &answer);
            sprintf(tempStr, "Deposit_Checking_%d", answer);
            *sentence = tempStr;
            /* send message */
            sendMsg(sock_client, *sentence);
            /* receive message */
            getRespose(sock_client, modifiedSentence);
         }else if(answer == 2){
            answer = 0;
            printf("\nPlease enter the amount to deposit:\n");
            scanf("%d", &answer);
            sprintf(tempStr, "Deposit_Saving_%d", answer);
            *sentence = tempStr;
            /* send message */
            sendMsg(sock_client, *sentence);
            /* receive message */
            getRespose(sock_client, modifiedSentence);
         }else{
            printf("%d is not a valid response!\n", answer);
         }
      }else if(answer == 3){
         answer = 0;
         printf("\nPlease choose account:\n\t1 - for Checking\n\t2 - for Saving\n");
         //gets input from the client
         scanf("%d", &answer);
         if(answer == 1){
            answer = 0;
            printf("\nPlease enter the amount to withdraw:\n");
            scanf("%d", &answer);
            sprintf(tempStr, "Withdraw_Checking_%d", answer);
            *sentence = tempStr;
            /* send message */
            sendMsg(sock_client, *sentence);
            /* receive message */
            getRespose(sock_client, modifiedSentence);
         }else if(answer == 2){
            answer = 0;
            printf("\nPlease enter the amount to withdraw:\n");
            scanf("%d", &answer);
            sprintf(tempStr, "Withdraw_Saving_%d", answer);
            *sentence = tempStr;
            /* send message */
            sendMsg(sock_client, *sentence);
            /* receive message */
            getRespose(sock_client, modifiedSentence);
         }else{
            printf("%d is not a valid response!\n", answer);
         }
      }else if(answer == 4){
         answer = 0;
         printf("\nPlease enter account to transfer to:\n\t1 - for Checking\n\t2 - for Saving\n");
         scanf("%d", &answer);
         if(answer == 1){
            answer = 0;
            printf("\nPlease enter the amount to transfer:\n");
            scanf("%d", &answer);
            sprintf(tempStr, "Transfer_Checking_%d", answer);
            *sentence = tempStr;
            /* send message */
            sendMsg(sock_client, *sentence);
            /* receive message */
            getRespose(sock_client, modifiedSentence);
         }else if(answer == 2){
            answer = 0;
            printf("\nPlease enter the amount to transfer:\n");
            scanf("%d", &answer);
            sprintf(tempStr, "Transfer_Saving_%d", answer);
            *sentence = tempStr;
            /* send message */
            sendMsg(sock_client, *sentence);
            /* receive message */
            getRespose(sock_client, modifiedSentence);
         }else{
            printf("%d is not a valid response!\n", answer);
         }
      }else if(answer == 5){
         int nextTransaction;
         printf("\nDo you want to connect again?:\n\t1 - Yes\n\t2 - No\n");
         scanf("%d", &nextTransaction);
         if(nextTransaction == 1){
            close (sock_client);
            //sock_client = connector();
         }else if(nextTransaction == 2){
            printf("\nGoodbye\n");
            break;
         }else{
            printf("%d is not a valid response!\n", answer);
         }
      }else{
         printf("%d is not a valid response!\n", answer);
      }
      
   }while(1);
   
   close(sock_client);


   
}
