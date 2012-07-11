#include "neko.h"

ApplicationServer::ApplicationServer() {
   nGetServices = nPutServices = nPostServices = nDeleteServices = 0;
   running = true;
   
   for (int i = 0; i < 4; i++)
      this->octets[i] = 0;
   
   this->port = 80;
}

ApplicationServer::ApplicationServer(char* ipAddress, char* port) {
   nGetServices = nPutServices = nPostServices = nDeleteServices = 0;
   running = true;
   
   int nTokens;
   char** cOctets = stringSplit(ipAddress, ".", &nTokens);
   
   for (int i=0; i < nTokens; i++) {
      this->octets[i] = atoi(cOctets[i]);
   }
   
   this->port = atoi(port);
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
   IPAddress ip(this->octets[0], this->octets[1], this->octets[2], this->octets[3]);

   // Initialize the Ethernet server library
   // with the IP address and port you want to use 
   // (port 80 is default for HTTP):
   EthernetServer server(this->port);
  
   // start the Ethernet connection and the server:
   Ethernet.begin(mac, ip);
   server.begin();
   Serial.println("Started Ethernet connection...");
  
  while (running) {
     // listen for incoming clients
     EthernetClient client = server.available();
     if (client) {
      Serial.println("Client connected...");
     
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