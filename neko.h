#include "Arduino.h"
#include "Ethernet.h"
#include "stdlib.h"
#include "httpHeader.h"

#ifndef NEKO_H
#define NEKO_H

#ifndef NULL
#define NULL 0
#endif

#define MAX_SERVICES 10

struct WebService {
   char resourceName[32];
   void *(*callback)(EthernetClient*, HTTPHeader*, void*);
};

class ApplicationServer {
protected:
   WebService getServices[3];
   WebService putServices[3];
   WebService postServices[3];
   WebService deleteServices[3];
   int nGetServices;
   int nPutServices;
   int nPostServices;
   int nDeleteServices;
public:
   ApplicationServer();
   void addService(int verb, char* resourceName, void *(*funcPtr)(EthernetClient*, HTTPHeader*, void*));

   WebService* fetchService(int verb, char* resourceName);
};

ApplicationServer::ApplicationServer() {
   nGetServices = nPutServices = nPostServices = nDeleteServices = 0;
}

void ApplicationServer::addService(int verb, char* resourceName, void *(*funcPtr)(EthernetClient*, HTTPHeader*, void*)) {
   switch(verb) {
   case GET:
      getServices[nGetServices].callback = funcPtr;
      strcpy(getServices[nGetServices++].resourceName, resourceName);      
      break;
   case PUT:
      putServices[nPutServices].callback = funcPtr;
      strcpy(putServices[nPutServices++].resourceName, resourceName);      
      break;
   case POST:
      postServices[nPostServices].callback = funcPtr;
      strcpy(postServices[nPostServices++].resourceName, resourceName);      
      break;
   case DELETE:
      deleteServices[nDeleteServices].callback = funcPtr;
      strcpy(deleteServices[nDeleteServices++].resourceName, resourceName);      
      break;
   }
}

WebService* ApplicationServer::fetchService(int verb, char* resourceName) {
   int nServices;
   WebService* services;

   switch(verb) {
   case GET:
      nServices = nGetServices;
      services = getServices;
      break;
   case PUT:
      nServices = nPutServices;
      services = putServices;
      break;
   case POST:
      nServices = nPostServices;
      services = postServices;
      break;
   case DELETE:
      nServices = nDeleteServices;
      services = deleteServices;
      break;
   default:
      nServices = nGetServices;
      services = getServices;
      break;
   }

   for (int i = 0; i < nServices; i++) {
      Serial.print("COMPARING ");
      Serial.print(services[i].resourceName);
      Serial.print(" AND ");
      Serial.print(resourceName);
      if (strcmp(services[i].resourceName, resourceName) == 0) {
        return &services[i];
      }
   }

   return NULL;
}
#endif