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

    //parse string to get cmd and file_name seperate
    //source for strtok https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm
    char *token;
    token = strtok(user_cmd_unparsed," ");
    //get command of user string
    strcpy(user_cmd,token);

    //get next token
    token = strtok(NULL, " ");
    //get file_name
    strcpy(file_name, token);

    printf("parsed cmd is %s\n", user_cmd);
    printf("parsed file_name is %s\n",file_name);

    switch(user_cmd){



    }
    


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

    /* get a message from the user */
    bzero(buf, BUFSIZE);
    printf("Please enter msg: ");
    fgets(buf, BUFSIZE, stdin);

    /* send the message to the server */
    serverlen = sizeof(serveraddr);
    n = sendto(sockfd, buf, strlen(buf), 0, &serveraddr, serverlen);
    if (n < 0) 
      error("ERROR in sendto");
    
    /* print the server's reply */
    n = recvfrom(sockfd, buf, strlen(buf), 0, &serveraddr, &serverlen);
    if (n < 0) 
      error("ERROR in recvfrom");
    printf("Echo from server: %s", buf);
    return 0;
}
