#ifndef NEKO_H
#define NEKO_H

#include "Ethernet.h"
#include "SPI.h"
#include "Arduino.h"
#include "httpHeader.h"
#include "stdlib.h"

#ifndef NULL
#define NULL 0
#endif

#ifdef __AVR_ATmega2560__
#define MAX_SERVICES 10
#elif __AVR_ATmega1280__
#define MAX_SERVICES 5
#else
#define MAX_SERVICES 3
#endif

struct WebService {
   char resourceName[32];
   void *(*callback)(EthernetClient*, HTTPHeader*, void*);
};

class ApplicationServer {
protected:
   HTTPHeader header;
   WebService getServices[MAX_SERVICES];
   WebService putServices[MAX_SERVICES];
   WebService postServices[MAX_SERVICES];
   WebService deleteServices[MAX_SERVICES];
   
   bool running;
   
   int octets[4];
   int port;
   
   int nGetServices;
   int nPutServices;
   int nPostServices;
   int nDeleteServices;
public:
   ApplicationServer();
   ApplicationServer(char* ipAddress, char* port = "80");
   
   void addService(int verb, char* resourceName, void *(*funcPtr)(EthernetClient*, HTTPHeader*, void*));
   WebService* fetchService(int verb, char* resourceName);
   
   void loop();
};

#endif