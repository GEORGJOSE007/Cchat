#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <ctype.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, clilen;
     int shft_amt,i,e,d,j,n;
	
     char buffer[256];
     char org_msg[256],new_msg[256];
     char msg1 []="Please enter the message: ";	
     char msg2 []="Please enter the correct option : a) Encode b) Decode"; 	
     char msg3 []="Please enter the key: ";
     char msg4 []="-------Thanks---------";

     struct sockaddr_in serv_addr, cli_addr;
     
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }

     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");

     bzero((char *) &serv_addr, sizeof(serv_addr));

     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno); 

     printf("Initializing socket...\n");	
     printf("Waiting for client....\n");

     if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     
     listen(sockfd,5);
     clilen = sizeof(cli_addr);

     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
   	
   do
    {   
     n = write(newsockfd,msg1,strlen(msg1));
     if (n < 0) error("ERROR writing to socket"); 	
     	
     bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     if (n < 0) 
	error("ERROR reading from socket"); 
     
     strcpy(org_msg,buffer);
     printf("\nMessage : %s \n",org_msg);
  
     
     n = write(newsockfd,msg2,strlen(msg2));
     if (n < 0)
	 error("ERROR writing to socket");
     
     bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     if (n < 0)
         error("ERROR reading from socket");
	
     if (buffer[0]=='a')
	{
		e=1;
		d=0;
	}
     else if(buffer[0]=='b')
	{
		e=0;
		d=1;
	}
     
 
     n = write(newsockfd,msg3,strlen(msg3));
     if (n < 0) 
	error("ERROR writing to socket");

     bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     if (n < 0) 
	error("ERROR reading from socket"); 

     shft_amt=atoi(buffer);
     
     bzero(new_msg,256);
     

     if(e==1&&d==0)
    {
        printf("Encrypting Message...\n");
        for(i=0;i<strlen(org_msg)-1;i++)
     {
        org_msg[i]=tolower(org_msg[i]); 
	new_msg[i]=org_msg[i];
	for(j=1;j<=shft_amt;j++)	
	{	
	new_msg[i]++;
	if(new_msg[i]==toascii('z'+1))
	 new_msg[i]='a';
	}
      }
        printf("\nEncrypted Message : %s\n",new_msg);
     }
	
     if(e==0&&d==1)
     {
        printf("\nDecrypting Message...\n");
        for(i=0;i<strlen(org_msg)-1;i++)
      {
        org_msg[i]=tolower(org_msg[i]);	
	new_msg[i]=org_msg[i];
	for(j=1;j<=shft_amt;j++)	
	{	
	new_msg[i]--;
	if(new_msg[i]==toascii('a'-1))
	 new_msg[i]='z';
	}
       }
        printf("\nDecypted Message : %s\n",new_msg);
     }		
     n = write(newsockfd,new_msg,strlen(new_msg));	
     if (n < 0) error("ERROR writing to socket");

     bzero(buffer,256);
     n = read(newsockfd,buffer,255);
    }while(buffer[0]!='q');
     
     printf("\nClient Disconnected\n");
     n = write(newsockfd,msg4,strlen(msg4));
     if (n < 0)
	 error("ERROR writing to socket");	  
     close(sockfd);
	
     return 0;	 
}
