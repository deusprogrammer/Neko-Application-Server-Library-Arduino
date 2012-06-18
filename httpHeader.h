#include "Arduino.h"
#include "stdlib.h"
#include "httpQuery.h"

#ifndef HTTPHEADER_H
#define HTTPHEADER_H

#ifndef NULL
#define NULL 0
#endif

#define GET    0
#define PUT    1
#define POST   2 
#define DELETE 3

#define MAX_PAIRS 3

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
	//char* getHost() {return host;}
	char* getResource() {return resource;}
	//char* getVersion() {return "VERSION 1.0";}
	char* fetchQuery (char* key);
};
#endif