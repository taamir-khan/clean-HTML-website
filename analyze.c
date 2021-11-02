#include "analyze.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>

void analyzeURL(char* url,char* host,int* port,char* doc)
{
   /* TODO: Implement the URL decomposition.
    * input: URL the given url
      output: host : where you place the host part of the URL
              port : where you place the port number (if any)
              doc  : where you place the document name
   */
   if (strstr(url, "http://") != NULL) {
       url += 7;
   }
   if (sscanf(url, "%[^:]:%d%s", host, port, doc) != 3) {
       assert(sscanf(url, "%[^/]%s", host, doc) == 2);
   }
}
