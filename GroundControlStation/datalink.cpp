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
unsigned int datalinkCheckSumCompute(unsigned char* buf, int byteCount)
{

	unsigned int sum1 = 0xffff;
	unsigned int sum2 = 0xffff;
	unsigned int tlen = 0;
	unsigned int shortCount = byteCount / sizeof(short);
	unsigned int oddLength = byteCount % 2;

	/* this is Fletcher32 checksum modified to handle buffers with an odd number of bytes */

	while (shortCount)
	{
		/* 360 is the largest number of sums that can be performed without overflow */
		tlen = shortCount > 360 ? 360 : shortCount;
		shortCount -= tlen;
		do
		{
			sum1 += *buf++;
			sum1 += (*buf++ << 8);
			sum2 += sum1;
		} while (--tlen);

		/* add last byte if there's an odd number of bytes (equivalent to appending a zero-byte) */
		if ((oddLength == 1) && (shortCount < 1))
		{
			sum1 += *buf++;
			sum2 += sum1;
		}

		sum1 = (sum1 & 0xffff) + (sum1 >> 16);
		sum2 = (sum2 & 0xffff) + (sum2 >> 16);
	}

	/* Second reduction step to reduce sums to 16 bits */
	sum1 = (sum1 & 0xffff) + (sum1 >> 16);
	sum2 = (sum2 & 0xffff) + (sum2 >> 16);

	return(sum2 << 16 | sum1);
}

/**
 * @brief datalinkCheckSumEncode sets the header checksum and payload checksum of a
 * character buffer to be sent as a datalink message
 *
 * @param buf pointer to a character buffer
 * @param byteCount size of the character buffer in bytes
 */
void datalinkCheckSumEncode(unsigned char* buf, int byteCount)
{

	struct datalinkHeader_ref* h = (struct datalinkHeader_ref*)buf;

	h->sync1 = DATALINK_SYNC0;
	h->sync2 = DATALINK_SYNC1;
	h->sync3 = DATALINK_SYNC2;

	h->messageSize = byteCount;

	h->hcsum = datalinkCheckSumCompute(buf, sizeof(struct datalinkHeader_ref) - sizeof(int) * 2);
	h->csum = datalinkCheckSumCompute(&(buf[sizeof(struct datalinkHeader_ref)]),
		byteCount - sizeof(struct datalinkHeader_ref));
}

void readDatalink(serialPort_ref* port, datalink_ref* data)
{
	struct datalinkHeader_ref* header = &DatalinkMessageHeader;
	int index, j, done, ni, nj, oldUniqueID;
	unsigned char* bf;
	void* dataPtr;
	int size;

	readPort(port);

	if (port->dataSource == PORT_OFF) return;

	done = 0;
	index = 0;

	while ((index <= port->bytesread - (int)sizeof(struct datalinkHeader_ref)) && !done)
	{
		if ((port->buffer[index] == DATALINK_SYNC0) &&
			(port->buffer[index + 1] == DATALINK_SYNC1) &&
			(port->buffer[index + 2] == DATALINK_SYNC2))
		{
			bf = &(port->buffer[index]);

			memcpy(header, bf, sizeof(struct datalinkHeader_ref));

			if (datalinkCheckSumCompute(bf, sizeof(struct datalinkHeader_ref) - sizeof(int) * 2) == data->header->hcsum &&
				data->header->messageSize >= sizeof(struct datalinkHeader_ref) &&
				data->header->messageSize < BUFFERSIZE)
			{
				if (data->header->messageSize + index <= port->bytesread)
				{
					/* have read in the entire message */
					if (datalinkCheckSumCompute(&bf[sizeof(struct datalinkHeader_ref)], data->header->messageSize - sizeof(struct datalinkHeader_ref)) == data->header->csum)
					{
						switch (data->header->messageID)
						{
						case DATALINK_MESSAGE_Arduino2GCS:
							if (data->header->messageSize == sizeof(struct datalinkMessageArduino2GCS_ref))
							{
								memcpy(data->Arduino2GCS, bf, data->header->messageSize);
							}
							break;

						default:
							/* unrecognized type */
							break;
						}

						data->work->itime++;
					}
					else
					{ /* checksum bad */
						data->work->badChecksums++;
					}
					index += data->header->messageSize - 1;

				}
				else
				{ /* end of buffer includes a partial message - come back later... */
					index--;
					done = 1;
				}
			}
			else
			{ /* header checksum is bad */
				index += sizeof(struct datalinkHeader_ref) - 1;
				data->work->badHeaderChecksums++;
			}
		}
		index++; /* start seq not found, go to next byte */

		if (index < 0) index = BUFFERSIZE - 1;
	}
	clearPort(port, index);
}

void writeDatalink(serialPort_ref* port, datalink_ref* data)
{
	data->GCS2Arduino->messageID = DATALINK_MESSAGE_GCS2Arduino;
	datalinkCheckSumEncode ( ( unsigned char* ) data->GCS2Arduino, sizeof ( struct datalinkMessageGCS2Arduino_ref) );
	writePort ( port, ( char* ) data->GCS2Arduino, sizeof ( struct datalinkMessageGCS2Arduino_ref) );
}