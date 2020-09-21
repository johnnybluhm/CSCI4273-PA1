/* 
 * udpclient.c - A simple UDP client
 * usage: udpclient <host> <port>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define BUFSIZE 1024
#define MAX_FILE_SIZE 10000

/* 
 * error - wrapper for perror
 */
void error(char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char **argv) {
    int sockfd, portno, n;
    int serverlen;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char *hostname;
    char buf[BUFSIZE];

    /* check command line arguments */
    if (argc != 3) {
       fprintf(stderr,"usage: %s <hostname> <port>\n", argv[0]);
       exit(0);
    }
    hostname = argv[1];
    portno = atoi(argv[2]);
    char user_cmd_unparsed[BUFSIZE];
    char user_cmd[BUFSIZE];
    char file_name[BUFSIZE];

    while(1){
    //Print user interface commands
    printf("Type any of the following commands:\n");
    printf("get [file_name]\n");
    printf("put [file_name]\n");
    printf("delete [file_name]\n");
    printf("ls\n");
    printf("exit\n");

    //get user command and filename
    //source https://www.includehelp.com/c/c-program-to-read-string-with-spaces-using-scanf-function.aspx
    scanf("%[^\n]", user_cmd_unparsed);


    //handle exit command
    if(strcmp("exit", user_cmd_unparsed)==0){
        printf("Goodbye!\n");
        return 0;
    }



   // printf("parsed cmd is %s\n", user_cmd);
    //printf("parsed file_name is %s\n",file_name);



    /* socket: create the socket */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    /* gethostbyname: get the server's DNS entry */
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", hostname);
        exit(0);
    }

    /* build the server's Internet address */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
	  (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(portno);

    /* get a message from the user to send to client */
    bzero(buf, BUFSIZE);
    strcpy(buf, user_cmd_unparsed);

    /* send the message to the server */
    serverlen = sizeof(serveraddr);
    n = sendto(sockfd, buf, strlen(buf), 0, &serveraddr, serverlen);
    if (n < 0) 
      error("ERROR in sendto");

    if(strcmp(user_cmd_unparsed, "ls")!=0){
    //parse string to get cmd and file_name seperate
    //source for strtok https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm
    char *token;
    token = strtok(user_cmd_unparsed," ");
    //get command of user string
    strcpy(user_cmd,token);

    if(token!=NULL){
    //get next token
    token = strtok(NULL, " ");
    //get file_name
    strcpy(file_name, token);
        }//if
    }//ls if

    /* print the server's reply */
   /* n = recvfrom(sockfd, buf, BUFSIZE, 0, &serveraddr, &serverlen);
    if (n < 0) 
      error("ERROR in recvfrom");
    printf("Echo from server: %s", buf);*/

    if(strcmp(user_cmd_unparsed,"ls")==0){
         /* print the server's reply */
    n = recvfrom(sockfd, buf, BUFSIZE, 0, &serveraddr, &serverlen);
    if (n < 0) 
      error("ERROR in recvfrom");
    printf("Ls from server: %s", buf);
        
      
    }

    
    
    else{
        //convert strings to ints so i can use switch
   int user_cmd_int;
    if(strcmp("get", user_cmd)== 0 ){
        user_cmd_int = 1;
    }
    else if(strcmp("put", user_cmd)==0){
        user_cmd_int=2;
    }
    else if(strcmp("delete", user_cmd)==0){
        user_cmd_int=3;
    }
    else if(strcmp("ls", user_cmd)==0){
        user_cmd_int=4;
    }
    printf("%d\n",user_cmd_int );
    switch(user_cmd_int){

        //get command
        case 1: 
        printf("handling get\n");
            n = recvfrom(sockfd, buf, BUFSIZE, 0, &serveraddr, &serverlen);
    if (n < 0) 
      error("ERROR in recvfrom");
    printf("Get from server: %s", buf);
        
        break;





        //put command
        case 2:

        //delete command
        case 3:


        //ls
        case 4:
        printf("ls\n");
            /* print the server's reply */
    n = recvfrom(sockfd, buf, BUFSIZE, 0, &serveraddr, &serverlen);
    if (n < 0) 
      error("ERROR in recvfrom");
    printf("Ls from server: %s", buf);
        break;
    }//switch
    
}//nested else
    return 0;
}//while
}//main
