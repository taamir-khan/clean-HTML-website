#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/select.h>
#include <assert.h>
#include "analyze.h"

void checkError(int status,int line)
{
   if (status < 0) {
      printf("socket error(%d)-%d: [%s]\n",getpid(),line,strerror(errno));
      exit(-1);
   }
}

int main(int argc,char* argv[])
{
   // Set up server address
   if (argc < 2) {
      printf("Usage is: wgettext <url>\n");
      return 1;
   }
   char host[512];
   int  port = 80;
   char url[512];
   analyzeURL(argv[1],host,&port,url);
   printf("[%s] [%d] [%s]\n",host,port,url);
   
   int sid = socket(PF_INET, SOCK_STREAM, 0);
   struct sockaddr_in addr;
   struct hostent *server = gethostbyname(host);
   addr.sin_family = AF_INET;
   addr.sin_port = htons(port);
   memcpy(&addr.sin_addr.s_addr, server->h_addr_list[0], server->h_length);
   
   //Server side 
   int status = connect(sid, (struct sockaddr*) &addr, sizeof(addr));
   checkError(status, __LINE__);
   
   char clientMessage[1024];
   snprintf(clientMessage, sizeof(clientMessage), "GET /%s\n", url);
   int rem = strlen(clientMessage)+1;
   int sent = 0;
   while (rem != 0) {
       status = send(sid, clientMessage + sent, rem, 0);
       checkError(status, __LINE__);
       sent += status;
       rem -= status;
   }
   
   dup2(sid, 0);
   execlp("html2text", "html2text", NULL);

   return 0;
}


