#ifndef datalink_h
#define datalink_h

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

#include "serial.h"

#define DATALINK_SYNC0 0xa3
#define DATALINK_SYNC1 0xb2
#define DATALINK_SYNC2 0xc1

#define DATALINK_MESSAGE_GCS2Arduino 101
#define DATALINK_MESSAGE_Arduino2GCS 102

struct datalink_ref
{
	struct datalinkWork_ref* work; /* working area */
	struct datalinkHeader_ref* header; /* working area */
	struct datalinkMessageGCS2Arduino_ref* GCS2Arduino; /* raw message */
	struct datalinkMessageArduino2GCS_ref* Arduino2GCS; /* raw message */
};

struct datalinkWork_ref
{
	unsigned int itime; /* number of messages received */
	int badChecksums; /* */
	int badHeaderChecksums; /*  */
};

struct datalinkHeader_ref
{
	unsigned char sync1; /*  */
	unsigned char sync2; /*  */
	unsigned char sync3; /*  */
	unsigned char spare; /*  */
	int messageID; /* id # */
	int messageSize; /* including header */
	unsigned int hcsum; /*  */
	unsigned int csum; /*  */
};

struct datalinkMessageGCS2Arduino_ref
{
	unsigned char sync1; /*  */
	unsigned char sync2; /*  */
	unsigned char sync3; /*  */
	unsigned char spare; /*  */
	int messageID; /* id # */
	int messageSize; /* including header */
	unsigned int hcsum; /*  */
	unsigned int csum; /*  */
	unsigned char fromGCS; /* */
	char align[3]; /* alignment */
};

struct datalinkMessageArduino2GCS_ref
{
	unsigned char sync1; /*  */
	unsigned char sync2; /*  */
	unsigned char sync3; /*  */
	unsigned char spare; /*  */
	int messageID; /* id # */
	int messageSize; /* including header */
	unsigned int hcsum; /*  */
	unsigned int csum; /*  */
	unsigned char fromArduino; /* */
	char align[3]; /* alignment */	
};

extern struct datalink_ref Datalink;
extern struct datalinkHeader_ref DatalinkMessageHeader;
extern struct datalinkMessageGCS2Arduino_ref DatalinkMessageGCS2Arduino;
extern struct datalinkMessageArduino2GCS_ref DatalinkMessageArduino2GCS;

void readDatalink ( serialPort_ref* port, datalink_ref* data );
void writeDatalink ( serialPort_ref* port, datalink_ref* data );

void datalinkCheckSumEncode(unsigned char* buf, int byteCount);

#endif
