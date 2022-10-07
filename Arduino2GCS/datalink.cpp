#include "datalink.h"

struct datalinkWork_ref DatalinkWork = {
	0 , /* uint itime */
	0 , /* int badChecksums */
	0 , /* int badHeaderChecksums */
};

struct datalinkHeader_ref DatalinkMessageHeader = {
	0xa3 , /* uchar sync1 */
	0xb2 , /* uchar sync2 */
	0xc1 , /* uchar sync3 */
	0 , /* uchar spare */
	0 , /* int messageID */
	0 , /* int messageSize */
	0 , /* uint hcsum */
	0 , /* uint csum */
};

struct datalinkMessageGCS2Arduino_ref DatalinkMessageGCS2Arduino = {
	0xa3 , /* uchar sync1 */
	0xb2 , /* uchar sync2 */
	0xc1 , /* uchar sync3 */
	0 , /* uchar spare */
	0 , /* int messageID */
	0 , /* int messageSize */
	0 , /* uint hcsum */
	0 , /* uint csum */
	0 , /* */
	{0, 0, 0} /* alignment */
};

struct datalinkMessageArduino2GCS_ref DatalinkMessageArduino2GCS = {
	0xa3 , /* uchar sync1 */
	0xb2 , /* uchar sync2 */
	0xc1 , /* uchar sync3 */
	0 , /* uchar spare */
	0 , /* int messageID */
	0 , /* int messageSize */
	0 , /* uint hcsum */
	0 , /* uint csum */
	0 , /* */
	{0, 0, 0} /* alignment */
};

struct datalink_ref Datalink = {
	&DatalinkWork, /* dir work */
	&DatalinkMessageHeader, /* dir header */
	&DatalinkMessageGCS2Arduino, /* dir GCS2Arduino */
	&DatalinkMessageArduino2GCS, /* dir Arduino2GCS */
};

/**
 * @brief datalinkCheckSumCompute() calculates and returns the checksum of a
 * character buffer
 *
 * Calculates the checksum of a character buffer using a 32-bit Fletcher
 * checksum. Handles an odd number of bytes by calculating the checksum as if
 * there were an additional zero-byte appended to the end of the data.
 *
 * @param buf pointer to a character buffer array
 * @param byteCount the size in bytes of the character buffer
 */
uint32_t datalinkCheckSumCompute(unsigned char* buf, int32_t byteCount)
{

	uint32_t sum1 = 0xffff;
	uint32_t sum2 = 0xffff;
	uint32_t tlen = 0;
	uint32_t shortCount = byteCount / sizeof(short);
	uint32_t oddLength = byteCount % 2;


	/* this is Fletcher32 checksum modified to handle buffers with an odd number of bytes */

	while (shortCount)
	{
		/* 360 is the largest number of sums that can be performed without overflow */
		tlen = shortCount > 360 ? 360 : shortCount;
		shortCount -= tlen;
		do
		{
			sum1 += *buf++;
			sum1 += ((uint32_t)*buf++ << 8);
			sum2 += sum1;
		} while (--tlen);

		/* add last byte if there's an odd number of bytes (equivalent to appending a zero-byte) */
		if ((oddLength == 1) && (shortCount < 1))
		{
			sum1 += (uint32_t)*buf++;
			sum2 += sum1;
		}

		sum1 = (sum1 & (uint32_t)0xffff) + (sum1 >> 16);
		sum2 = (sum2 & (uint32_t)0xffff) + (sum2 >> 16);
	}

	/* Second reduction step to reduce sums to 16 bits */
	sum1 = (sum1 & (uint32_t)0xffff) + (sum1 >> 16);
	sum2 = (sum2 & (uint32_t)0xffff) + (sum2 >> 16);

	return(sum2 << 16 | sum1);
}

/**
 * @brief datalinkCheckSumEncode sets the header checksum and payload checksum of a
 * character buffer to be sent as a datalink message
 *
 * @param buf pointer to a character buffer
 * @param byteCount size of the character buffer in bytes
 */
void datalinkCheckSumEncode(unsigned char* buf, uint32_t byteCount)
{

	struct datalinkHeader_ref* h = (struct datalinkHeader_ref*)buf;

	h->sync1 = DATALINK_SYNC0;
	h->sync2 = DATALINK_SYNC1;
	h->sync3 = DATALINK_SYNC2;

	h->messageSize = byteCount;

	h->hcsum = datalinkCheckSumCompute(buf, sizeof(struct datalinkHeader_ref) - sizeof(int32_t) * 2);
	h->csum = datalinkCheckSumCompute(&(buf[sizeof(struct datalinkHeader_ref)]), byteCount - sizeof(struct datalinkHeader_ref));
}