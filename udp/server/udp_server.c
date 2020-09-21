/* 
 * udpserver.c - A simple UDP echo server 
 * usage: udpserver <port>
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//needed for ls code
#include <dirent.h>

#define BUFSIZE 1024

long int findSize(char file_name[]);
int delete_file(char *file_name);
int ls(char *ls_contents);

/*
 * error - wrapper for perror
 */
void error(char *msg) {
  perror(msg);
  exit(1);
}

int main(int argc, char **argv) {
  int sockfd; /* socket */
  int portno; /* port to listen on */
  int clientlen; /* byte size of client's address */
  struct sockaddr_in serveraddr; /* server's addr */
  struct sockaddr_in clientaddr; /* client addr */
  struct hostent *hostp; /* client host info */
  char buf[BUFSIZE]; /* message buf */
  char *hostaddrp; /* dotted decimal host addr string */
  int optval; /* flag value for setsockopt */
  int n; /* message byte size */

  /* 
   * check command line arguments 
   */
  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }
  portno = atoi(argv[1]);

  /* 
   * socket: create the parent socket 
   */
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) 
    error("ERROR opening socket");

  /* setsockopt: Handy debugging trick that lets 
   * us rerun the server immediately after we kill it; 
   * otherwise we have to wait about 20 secs. 
   * Eliminates "ERROR on binding: Address already in use" error. 
   */
  optval = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, 
	     (const void *)&optval , sizeof(int));

  /*
   * build the server's Internet address
   */
  bzero((char *) &serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons((unsigned short)portno);

  /* 
   * bind: associate the parent socket with a port 
   */
  if (bind(sockfd, (struct sockaddr *) &serveraddr, 
	   sizeof(serveraddr)) < 0) 
    error("ERROR on binding");

  /* 
   * main loop: wait for a datagram, then echo it
   */
  clientlen = sizeof(clientaddr);
  printf("listening on port %d\n",portno);
while(1){
    /*
     * recvfrom: receive a UDP datagram from a client
     */
    bzero(buf, BUFSIZE);
    n = recvfrom(sockfd, buf, BUFSIZE, 0,
		 (struct sockaddr *) &clientaddr, &clientlen);
    if (n < 0)
      error("ERROR in recvfrom");


    char user_cmd_unparsed[BUFSIZE];
    char user_cmd[BUFSIZE];
    char file_name[BUFSIZE];
    strcpy(user_cmd_unparsed,buf);
   
    /* 
     * gethostbyaddr: determine who sent the datagram
     */
    hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, 
        sizeof(clientaddr.sin_addr.s_addr), AF_INET);
    if (hostp == NULL)
      error("ERROR on gethostbyaddr");
    hostaddrp = inet_ntoa(clientaddr.sin_addr);
    if (hostaddrp == NULL)
      error("ERROR on inet_ntoa\n");
    printf("server received datagram from %s (%s)\n", 
     hostp->h_name, hostaddrp);
    printf("server received %d/%d bytes: %s\n", strlen(buf), n, buf);

    if(strcmp(user_cmd_unparsed, "exit") == 0){
      
        n = sendto(sockfd, "Sever has exited gracefully", BUFSIZE, 0, 
         (struct sockaddr *) &clientaddr, clientlen);
       if (n < 0) 
      error("ERROR in sendto");
      printf("Goodbye\n");
      return 0;
    }//exit if

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


    //nested if to code ls edge case
    if(strcmp(user_cmd_unparsed, "ls") == 0){
      char ls_contents[BUFSIZE];
      ls(ls_contents);

      printf("%s\n", ls_contents);

      n = sendto(sockfd, ls_contents, strlen(ls_contents), 0, 
         (struct sockaddr *) &clientaddr, clientlen);
       if (n < 0) 
      error("ERROR in sendto");

    }//ls if
    else{

    
    int file_size;
    
    file_size = findSize(file_name);
    FILE *f_ptr;
    f_ptr = fopen(file_name, "r");
    char file_contents[file_size];

    switch(user_cmd_int){

        //get command
        case 1: 
        printf("In get\n");

      //fill string with file contents
   /*   int i;
      i=1;
      char c;
      c = fgetc(f_ptr);
      file_contents[0] =c;
      while( c!= EOF){
        c = fgetc(f_ptr);
        i++;
        file_contents[i] = c;
        printf("%s\n", file_contents);
        }*/
        for(int i = 0; i<=file_size+1; i++){
          file_contents[i]=fgetc(f_ptr);
          
        }
        printf("%s\n",file_contents );
            // Return if could not open file 
          printf("loop exits");
      
          

      //sending file 
      n = sendto(sockfd, file_contents, file_size, 0, 
         (struct sockaddr *) &clientaddr, clientlen);
       if (n < 0) 
      error("ERROR in sendto");
        break;




        //put command
        case 2:

        printf("%s\n",file_name );
    FILE *fp;

    //create file to write to
    fp =fopen(file_name, "w");


    bzero(buf, BUFSIZE);
    n = recvfrom(sockfd, buf, BUFSIZE, 0,
     (struct sockaddr *) &clientaddr, &clientlen);

    fputs(buf,fp);

    printf("%s\n", buf);
    if (n < 0)
      error("ERROR in recvfrom");


        printf("I Wll code put\n");
        break;

        //delete command
        case 3:
        printf("I Wll code delete\n");

        delete_file(file_name);
        n = sendto(sockfd, "File deleted successfully!", BUFSIZE, 0, 
         (struct sockaddr *) &clientaddr, clientlen);
       if (n < 0) 
      error("ERROR in sendto");
        break;
        break;

        //ls
        case 4:
        printf("CODING ls\n");
        //ls();
        break;
    }

    /* 
     * sendto: echo the input back to the client 
     */
   /* n = sendto(sockfd, buf, strlen(buf), 0, 
	       (struct sockaddr *) &clientaddr, clientlen);
    if (n < 0) 
      error("ERROR in sendto");*/
  //while(1)
  //
}//nested else
}//wheil
}//main






//https://www.geeksforgeeks.org/c-program-find-size-file/

long int findSize(char file_name[]) 
{ 
    // opening the file in read mode 
    FILE* fp = fopen(file_name, "r"); 
  
    // checking if the file exist or not 
    if (fp == NULL) { 
        printf("File Not Found!\n"); 
        return -1; 
    } 
  
    fseek(fp, 0L, SEEK_END); 
  
    // calculating the size of the file 
    long int res = ftell(fp); 
  
    // closing the file 
    fclose(fp); 

    return res;
}

//source: https://www.geeksforgeeks.org/c-program-delete-file/
int delete_file(char *file_name)
  { 
   if (remove(file_name) == 0) 
      printf("Deleted successfully"); 
   else
      printf("Unable to delete the file"); 
  
   return 0; 
} 


int ls(char *ls_contents)
{
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            strcat(ls_contents, dir->d_name);
            strcat(ls_contents, "\n");
        }
        closedir(d);
    }
    return(0);
}