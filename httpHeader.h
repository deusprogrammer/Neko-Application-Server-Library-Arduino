#include "Arduino.h"
#include "stdlib.h"
#include "httpQuery.h"
#include "strutils.h"

#ifndef HTTPHEADER_H
#define HTTPHEADER_H

#ifndef NULL
#define NULL 0
#endif

#define GET    0
#define PUT    1
#define POST   2 
#define DELETE 3

#ifdef __AVR_ATmega2560__
#define MAX_PAIRS 10
#elif __AVR_ATmega1280__
#define MAX_PAIRS 6
#else
#define MAX_PAIRS 4
#endif

class HTTPHeader {
private:
	int verb;
	char resource[64];
	int nPairs;
	HTTPQuery pairs[MAX_PAIRS];
	unsigned int contentLength;
public:
	HTTPHeader();
	void reset() {nPairs = 0;}
	void consumeLine(char* line);
	int getContentLength() {return contentLength;}
	int getVerb() {return verb;}
	//char* getHost() {return host;}
	char* getResource() {return resource;}
	//char* getVersion() {return "VERSION 1.0";}
	char* fetchQuery (char* key);
};
#endif