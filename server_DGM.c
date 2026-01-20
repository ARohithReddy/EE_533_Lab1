#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>


void error (char* msg) {
  perror(msg);
  exit(1);
}

void dostuff(int sockfd) {
	printf("entering dostuff function. \n ");
  	char buffer[256];
	int n;
	bzero(buffer, 256);
	n = read(sockfd, buffer, 255);
	if (n<0) error("ERROR reading from socket");
	printf("Here is the message : %s", buffer);
	n = write(sockfd, "I got yout message", 18);
	if (n <0) error("ERROR writing to socket");
}

int main(int argc, char* argv []) {
  int sockfd, newsockfd, portno, clilen, n;
  char buffer[256];
  
  struct sockaddr_in serv_addr, cli_addr;
  
  if (argc <2) {
    fprintf(stderr,"ERROR, no port provided");
    exit(1);
  }
  
  signal(SIGCHLD, SIG_IGN);

  sockfd = socket(AF_INET, SOCK_DGRAM,0);
  if (sockfd <0) 
    error("ERROR opening socket");
  bzero((char *) &serv_addr, sizeof(serv_addr));
  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(portno);
  serv_addr.sin_addr.s_addr = INADDR_ANY;


  
  if (bind(sockfd, (const struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
     error("ERROR on binding");

  //listen(sockfd, 5);

  clilen = sizeof(cli_addr);
  while (1) {
  newsockfd = recvfrom(sockfd, buffer, 256, 0, (struct sockaddr *) &cli_addr, &clilen);
  if (newsockfd <0)
   	error("ERROR on accpet");

  bzero(buffer, 256);
  n = read(newsockfd, buffer, 255);
  if (n<0) error("ERROR reading from socket");
  printf("Here is the message : %s", buffer);

  n = sendto(newsockfd, "I got yout message", 18, 0, (struct sockaddr *) & cli_addr, clilen);
  if (n <0) error("ERROR writing to socket");

  }

  return 0;
  
}
