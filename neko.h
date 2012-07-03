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

#endif