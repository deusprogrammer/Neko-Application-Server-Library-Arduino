#ifndef NEKO_H
#define NEKO_H

#include "Arduino.h"
#include "httpHeader.h"
#include "stdlib.h"

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
   HTTPHeader header;
   WebService getServices[3];
   WebService putServices[3];
   WebService postServices[3];
   WebService deleteServices[3];
   
   bool running;
   
   int nGetServices;
   int nPutServices;
   int nPostServices;
   int nDeleteServices;
public:
   ApplicationServer();
   
   void addService(int verb, char* resourceName, void *(*funcPtr)(EthernetClient*, HTTPHeader*, void*));
   WebService* fetchService(int verb, char* resourceName);
   
   void loop();
};

ApplicationServer::ApplicationServer() {
   nGetServices = nPutServices = nPostServices = nDeleteServices = 0;
   running = true;
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

void ApplicationServer::loop() {
   WebService* service;
     
   // Enter a MAC address and IP address for your controller below.
   // The IP address will be dependent on your local network:
   byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
   IPAddress ip(10,0,0,2);

   // Initialize the Ethernet server library
   // with the IP address and port you want to use 
   // (port 80 is default for HTTP):
   EthernetServer server(80);
  
   // start the Ethernet connection and the server:
   Ethernet.begin(mac, ip);
   server.begin();
   Serial.println("Started Ethernet connection...");
  
  while (running) {
     // listen for incoming clients
     EthernetClient client = server.available();
     if (client) {
      Serial.println("Started Ethernet connection...");
     
      // an http request ends with a blank line
      boolean currentLineIsBlank = true;
      char line[256];
      int nBytes = 0;
       
      while (client.connected()) {
         if (client.available()) {
            char c = client.read();
            if (c == '\n' && currentLineIsBlank) {
               Serial.print("HEADER: ");
               Serial.println(header.getResource());
               if (header.getContentLength()) {
                  int contentLength = header.getContentLength();
                  Serial.println("DATA");
                  for (int i = 0; i < contentLength; i++) {
                     char c = client.read();
                     Serial.write(c);
                  }
               }

               service = fetchService(header.getVerb(), header.getResource());
               
               if (service)
                  service->callback(&client, &header, NULL);
               else {
                  client.println("Unable to find web service you requested");
                  delay(1);
                  client.stop();
               }
                  
               header.reset();
            }
            else if (c == '\n') {
               line[nBytes] = 0;
               header.consumeLine(line);
               nBytes = 0;
               currentLineIsBlank = true;
            } 
            else if (c != '\r') {
               if (nBytes < 256)
                  line[nBytes++] = c;
                  currentLineIsBlank = false;
               }
            }
         }
      }
   }
}

#endif