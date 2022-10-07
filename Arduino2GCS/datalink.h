#ifndef datalink_h
#define datalink_h

#include <stdint.h>
#include <stdlib.h>

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
	uint32_t itime; /* number of messages received */
	int32_t badChecksums; /* */
	int32_t badHeaderChecksums; /*  */
};

struct datalinkHeader_ref
{
	unsigned char sync1; /*  */
	unsigned char sync2; /*  */
	unsigned char sync3; /*  */
	unsigned char spare; /*  */
	int32_t messageID; /* id # */
	int32_t messageSize; /* including header */
	uint32_t hcsum; /*  */
	uint32_t csum; /*  */
};

struct datalinkMessageGCS2Arduino_ref
{
	unsigned char sync1; /*  */
	unsigned char sync2; /*  */
	unsigned char sync3; /*  */
	unsigned char spare; /*  */
	int32_t messageID; /* id # */
	int32_t messageSize; /* including header */
	uint32_t hcsum; /*  */
	uint32_t csum; /*  */
	unsigned char fromGCS; /* */
	char align[3]; /* alignment */
};

struct datalinkMessageArduino2GCS_ref
{
	unsigned char sync1; /*  */
	unsigned char sync2; /*  */
	unsigned char sync3; /*  */
	unsigned char spare; /*  */
	int32_t messageID; /* id # */
	int32_t messageSize; /* including header */
	uint32_t hcsum; /*  */
	uint32_t csum; /*  */
	unsigned char fromArduino; /* */
	char align[3]; /* alignment */	
};

extern struct datalink_ref Datalink;
extern struct datalinkHeader_ref DatalinkMessageHeader;
extern struct datalinkMessageGCS2Arduino_ref DatalinkMessageGCS2Arduino;
extern struct datalinkMessageArduino2GCS_ref DatalinkMessageArduino2GCS;

uint32_t datalinkCheckSumCompute(unsigned char* buf, int32_t byteCount);
void datalinkCheckSumEncode(unsigned char* buf, uint32_t byteCount);

#endif