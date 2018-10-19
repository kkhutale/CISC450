/* tcpserver.c */
/* Programmed by Adarsh Sethi */
/* Sept. 13, 2018 */    

#include <ctype.h>          /* for toupper */
#include <stdio.h>          /* for standard I/O functions */
#include <stdlib.h>         /* for exit */
#include <string.h>         /* for memset */
#include <sys/socket.h>     /* for socket, bind, listen, accept */
#include <netinet/in.h>     /* for sockaddr_in */
#include <unistd.h>         /* for close */


#define STRING_SIZE 1024   


/* SERV_TCP_PORT is the port number on which the server listens for
   incoming requests from cglients. You should change this to a different
   number to prevent conflicts with others in the class. */

#define SERV_TCP_PORT 56001

//function declaration(each function is explain later when defined)
void deposit(char accountType[], int amount, int sock_connection, int *checkingAcct, int *savingAcct);
void withdraw(char accountType[], int amount, int sock_connection, int *checkingAcct, int *savingAcct);
void transfer(char accountType[], int amount, int sock_connection, int *checkingAcct, int *savingAcct);
void checkBalance(char token[], int sock_connection, int *checkingAcct, int *savingAcct);
void processTransaction(char sentence[], int sock_connection, int *checkingAcct, int *savingAcct);



//this function helps the client to deposit a specified amount to an account 
void deposit(char accountType[], int amount, int sock_connection, int *checkingAcct, int *savingAcct){
   unsigned int msg_len;  /* length of message */
   int bytes_sent; /* number of bytes sent */
   int oldBalance;
   //response string declared 
   char response[STRING_SIZE] = "";
   printf("Type of account: %s\n",accountType);
   printf("Amount: $%d.00\n\n",amount);
   if(strcmp("Checking", accountType) == 0)/*compares the string*/
   {
      oldBalance = *checkingAcct; //Assigns oldbalance as checking account balance
      *checkingAcct = *checkingAcct + amount;//adds the old and newly deposited amount
      sprintf(response, "\nTransaction Type: Deposit\nAccount: Checking\nOld Balance = $%d.00\nNew Balance = $%d.00", oldBalance, *checkingAcct);
   }else if(strcmp("Saving", accountType) == 0){
      oldBalance = *savingAcct; //if saving account ,assigns old balance to savving account.
      *savingAcct = *savingAcct + amount; //adds the old and newly deposited amount.
      sprintf(response, "\nTransaction Type: Deposit\nAccount: Saving\nOld Balance = $%d.00\nNew Balance = $%d.00", oldBalance, *savingAcct); 
   }else{
      //displays error and asks the user for valid account name.
      char response[]="Error: Please enter a valid transaction.";
   }
   
   msg_len = strlen(response) + 1;
    //transmit message to the client
   bytes_sent = send(sock_connection, response, msg_len, 0);
   printf("Message sent to client:\n");
   printf("Message size: %d\n",msg_len);
   printf("Message: %s\n",response);
   
}

//this function helps the client to withdraw a specified amount from an account.
void withdraw(char accountType[], int amount, int sock_connection, int *checkingAcct, int *savingAcct){
   unsigned int msg_len;  /* length of message */
   int bytes_sent; /* number of bytes sent */
   int oldBalance;
   printf("Type of account: %s\n",accountType);
   printf("Amount: $%d.00\n\n",amount);
   if(amount % 20 == 0 ){
      char response[STRING_SIZE] = "";
      if(strcmp("Checking", accountType) == 0){
         if(amount <= *checkingAcct){ //withdraw if balance is greater than requested amount.
            oldBalance = *checkingAcct;
            *checkingAcct = *checkingAcct - amount;
            sprintf(response, "\nTransaction Type: Withdraw\nAccount: Checking\nOld Balance = $%d.00\nNew Balance = $%d.00", oldBalance, *checkingAcct);
            msg_len = strlen(response) + 1;
            bytes_sent = send(sock_connection, response, msg_len, 0);
            printf("Message sent to client:\n");
            printf("Message size: %d\n",msg_len);
            printf("Message: %s\n",response);
         }else{
            sprintf(response, "\nError: The withdraw amount must be less than the Checking account balance.");
            msg_len = strlen(response) + 1;
            //transmit message to the client
            bytes_sent = send(sock_connection, response, msg_len, 0);
            printf("Message sent to client:\n");
            printf("Message size: %d\n",msg_len);
            printf("Message: %s\n",response);
         }
         
      }
      else{
         // withdrawal is only allowed from the checking account
         char response[STRING_SIZE]="\nError: Amount can only be withdrawn from the checking account.";
         msg_len = strlen(response) + 1;
         //sent reaponce to client
         bytes_sent = send(sock_connection, response, msg_len, 0);
         printf("Message sent to client:\n");
         printf("Message size: %d\n",msg_len);
         printf("Message: %s\n",response);
      }
   }
   else{
      /*sent error response to the client asking to enter valid value.The amount to be withdrawn should only be in multiples of 20 dollars, and should not be
greater than the current balance in the account*/
      char response[STRING_SIZE]="Error: Please enter a valid amount(should be a multiple of 20 dollars).";
      msg_len = strlen(response) + 1;
      //send response to client
      bytes_sent = send(sock_connection, response, msg_len, 0);
      printf("Message sent to client:\n");
      printf("Message size: %d\n",msg_len);
      printf("Message: %s\n",response);
   }
   
   
}

//this function helps client to transfer a specified amount from one account to the other
void transfer(char accountType[], int amount, int sock_connection, int *checkingAcct, int *savingAcct){
   unsigned int msg_len;  /* length of message */
   int bytes_sent; /* number of bytes sent */
   int oldBalance;
   printf("Type of account: %s\n",accountType);
   printf("Amount: $%d.00\n\n",amount);
   //checks type of account first and then transfer amount from the requested account type 
   if(strcmp("Checking", accountType) == 0){
      char response[STRING_SIZE] = "";
      //transfer amount only if balance in the checking account is greater than amount requested to transfer
      if(amount <= *savingAcct){
         oldBalance = *checkingAcct;
         *checkingAcct = *checkingAcct + amount;
         *savingAcct = *savingAcct - amount;
         sprintf(response, "\nTransaction Type: Transfer\nAccount: Checking\nOld Balance = $%d.00\nNew Balance = $%d.00", oldBalance, *checkingAcct);
         msg_len = strlen(response) + 1;
         bytes_sent = send(sock_connection, response, msg_len, 0);
         printf("Message sent to client:\n");
         printf("Message size: %d\n",msg_len);
         printf("Message: %s\n",response);
      }
      //Display error when amount requested to tranfer is greater than balance.
      else{
         char response[]="Error: Not enough balance to tranfer amount to the requested account.";
         msg_len = strlen(response) + 1;
         bytes_sent = send(sock_connection, response, msg_len, 0);
         printf("Message sent to client:\n");
         printf("Message size: %d\n",msg_len);
         printf("Message: %s\n",response);
         
      }
      
   }
   //transfer amount only if balance in the saving account is greater than amount requested to transfer
   else if(strcmp("Saving", accountType) == 0){
      char response[STRING_SIZE] = "";
      if(amount <= *checkingAcct){
         oldBalance = *savingAcct;
         *savingAcct = *savingAcct + amount;
         *checkingAcct = *checkingAcct - amount;
         sprintf(response, "\nTransaction Type: Transfer\nAccount: Saving\nOld Balance = $%d.00\nNew Balance = $%d.00", oldBalance, *savingAcct);
         msg_len = strlen(response) + 1;
         //send reponse to the client
         bytes_sent = send(sock_connection, response, msg_len, 0);
         printf("Message sent to client:\n");
         printf("Message size: %d\n",msg_len);
         printf("Message: %s\n",response);
      
      }
      //Display error when amount requested to tranfer is greater than balance.
      else{
         char response[]="Error: Not enough balance to tranfer amount to the requested account.";
         msg_len = strlen(response) + 1;
         bytes_sent = send(sock_connection, response, msg_len, 0);
         printf("Message sent to client:\n");
         printf("Message size: %d\n",msg_len);
         printf("Message: %s\n",response);
         
      }
   }
   
}

//this function helps client to check the balance in the specific account
void checkBalance(char token[], int sock_connection, int *checkingAcct, int *savingAcct){
   unsigned int msg_len;  /* length of message */
   int bytes_sent; /* number of bytes sent */
   
   char response[STRING_SIZE] = "";
   printf("Type of account: %s\n\n",token);
   //check balance for the checking account
   if(strcmp("Checking", token) == 0){
      sprintf(response, "\nTransaction Type: Check Balance\nAccount: Checking\nBalance = $%d.00", *checkingAcct);
      //check the balance for the saving account
   }else if(strcmp("Saving", token) == 0){
      sprintf(response, "\nTransaction Type: Check Balance\nAccount: Saving\nBalance = $%d.00", *savingAcct);  
   }else{
      //error when client request for an invalid account
      char response[]="Error: Please enter a valid transaction.";
   }
   
   msg_len = strlen(response) + 1;
   //send response to the client
   bytes_sent = send(sock_connection, response, msg_len, 0);
   printf("Message sent to client:\n");
   printf("Message size: %d\n",msg_len);
   printf("Message: %s\n",response);
}

//this function calls all the above function when necessary and process the transaction accordingly.
void processTransaction(char sentence[], int sock_connection, int *checkingAcct, int *savingAcct){
   unsigned int msg_len;  /* length of message */
   int bytes_sent; /* number of bytes sent */
   
   // Returns first token  
   char *token = strtok(sentence, "_");
   //printf("Passing to function: %s\n", token);
    printf("Client requested for: %s\n",token);
   if (strcmp("Balance", token) == 0){
      token = strtok(NULL, "_");
      
      //calls check balance function to check balance the the account
      checkBalance(token, sock_connection, checkingAcct, savingAcct);
   }else if(strcmp("Deposit", token) == 0){
      token = strtok(NULL, "_");
      char accountType[STRING_SIZE];
      strcpy (accountType, token);
      token = strtok(NULL, "_");
      int amount = atoi(token);
      //calls deposit function to deposit ammount to a requested account
      deposit(accountType, amount, sock_connection, checkingAcct, savingAcct);
   }else if (strcmp("Withdraw", token) == 0){
      token = strtok(NULL, "_");
      char accountType[STRING_SIZE];
      strcpy (accountType, token);
      token = strtok(NULL, "_");
      int amount = atoi(token); 
      //calls withdraw function to witdraw amount from the checking account
      withdraw(accountType, amount, sock_connection, checkingAcct, savingAcct);
   }else if (strcmp("Transfer", token) == 0){
      token = strtok(NULL, "_");
      char accountType[STRING_SIZE];
      strcpy (accountType, token);
      token = strtok(NULL, "_");
      int amount = atoi(token); 
      //calls tranfer function to tranfer amount to another account
      transfer(accountType, amount, sock_connection, checkingAcct, savingAcct);
   }else{
      //displays error message for invalid entry
      char response[]="Error: Please enter a valid transaction.";
      msg_len = strlen(response) + 1;
      //send response to client
      bytes_sent = send(sock_connection, response, msg_len, 0);
      printf("Message sent to client:\n");
      printf("Message size: %d\n",msg_len);
      printf("Message: %s\n",response);
      
   }
}
//this function prints the message recived from the client and the the size of the message

int main(void) {

   
   int checkingAcct = 0; /* Checking account balance initialize  */
   int savingAcct = 0;   /* Saving account balance initialize*/
   
   int *pCheckingAcct;
   int *pSavingAcct;

   pCheckingAcct = &checkingAcct;
   pSavingAcct = &savingAcct;

   int sock_server;  /* Socket on which server listens to clients */
   int sock_connection;  /* Socket on which server exchanges data with client */

   struct sockaddr_in server_addr;  /* Internet address structure that
                                        stores server address */
   unsigned int server_addr_len;  /* Length of server address structure */
   unsigned short server_port;  /* Port number used by server (local port) */

   struct sockaddr_in client_addr;  /* Internet address structure that
                                        stores client address */
   unsigned int client_addr_len;  /* Length of client address structure */

   char sentence[STRING_SIZE];  /* receive message */
   char modifiedSentence[STRING_SIZE]; /* send message */
   unsigned int msg_len;  /* length of message */
   int bytes_sent, bytes_recd; /* number of bytes sent or received */
   unsigned int i;  /* temporary loop variable */

   /* open a socket */
   if ((sock_server = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
      perror("Server: can't open stream socket");
      exit(1);                                                
   }

   /* initialize server address information */
   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   server_addr.sin_addr.s_addr = htonl (INADDR_ANY);  /* This allows choice of
                                        any host interface, if more than one
                                        are present */ 
   server_port = SERV_TCP_PORT; /* Server will listen on this port */
   server_addr.sin_port = htons(server_port);

   /* bind the socket to the local server port */
   if (bind(sock_server, (struct sockaddr *) &server_addr,
                                    sizeof (server_addr)) < 0) {
      perror("Server: can't bind to local address");
      close(sock_server);
      exit(1);
   }                     

   /* listen for incoming requests from clients */
   if (listen(sock_server, 50) < 0) {    /* 50 is the max number of pending */
      perror("Server: error on listen"); /* requests that will be queued */
      close(sock_server);
      exit(1);
   }
   printf("I am here to listen ... on port %hu\n\n", server_port);
  
   client_addr_len = sizeof (client_addr);

   /* wait for incoming connection requests in an indefinite loop */
   for (;;) {

      sock_connection = accept(sock_server, (struct sockaddr *) &client_addr, 
                                         &client_addr_len);
                     /* The accept function blocks the server until a
                        connection request comes from a client */
      if (sock_connection < 0) {
         perror("Server: accept() error\n"); 
         close(sock_server);
         exit(1);
      }
 
      /* receive the message */
      bytes_recd = recv(sock_connection, sentence, STRING_SIZE, 0); 
      
      if (bytes_recd > 0){
         msg_len=strlen(sentence) + 1;
         printf("\nMessage from client:\n");
         printf("Message size: %d\n",msg_len);
         
         processTransaction(sentence, sock_connection, pCheckingAcct, pSavingAcct);
      }

      
   } 
}




