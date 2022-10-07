#include "serial.h"

struct serialPort_ref commPort = {
	PORT_ON, /* int dataSource */
	1 , /* int init */
	ARDUINO_PORT_NUM , /* int port */
	SI_IPC , /* int si_port */
	115200 , /* int baud */
	"8n1\0\0", /* string termios */
	"/dev/ttyS%d\0                 \0", /* string nameFmt */
	"\0                  \0", /* string name */
	"127.0.0.1", /* string connectTo */
	-1 , /* int hridx */
	-1 , /* int sridx */
	-1 , /* int srmode */
	SI_IPC, /* int portNum */
	1 , /* int portType */
	0 , /* int isServer */
	1 , /* int useSerial */
	0 , /* int useSock */
	0 , /* int useTcp */
	0 , /* int useI2C */
	0 , /* int connectionMode */
	0 , /* int newConnectionMode */
	0 , /* int connectState */
	1 , /* int setHostnameManually */
	1.0 , /* double connectRetry */
	0.0 , /* double lastConnectRetry */
	0 , /* int saveToFile */
	"log_file%s.bin\0                                                                              \0", /* string saveFile */
	"read%d.bin\0                                                                                  \0", /* string readFile */
	0 , /* int saveFileOpen */
	0 , /* long saveFileFd */
	0 , /* int readFileOpen */
	0 , /* long readFileFd */
	1 , /* int readChunk */
	0 , /* int portIsOpen */
	0 , /* int bytesread */
	0, /* int fd */
	0 , /* ulong dropwrite */
	0 , /* ulong dropread */
	0      , /* ulong sent */
	0 , /* ulong received */
	0 , /* int blocking */
	0 , /* int blockingRead */
	0.1 , /* double blockTimeout */
	0   , /* int blockDeadlineExpired */
	0, /* voidptr serialDevice */
	0, /* voidptr sockDevice */
	"127.0.0.1", /* string myname */
	0 , /* int myport */
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\0", /* string remotename */
	0 , /* int remoteport */
	-1 , /* int errorNumberRx */
	-1 , /* int errorNumberTx */
	"/dev/spidev1.1\0                  \0", /* string spiName */
	0 , /* uchar spiSetSettings */
	( 0 | 0 ) | 0x04 , /* uchar spiMode */
	0 , /* ushort spiDelay */
	245000 , /* int spiSpeed */
	8 , /* int spiBpw */
	0 , /* char I2CAddress */
	"/dev/i2c-0\0", /* string I2CName */
	0 , /* char seekState */
	{0}  , /* char align[3] */
	{0,0,0,0,0,0,0,0}  , /* char tmp[8] */
	{0,0,0,0,0,0,0,0} , /* char hdr[8] */
	{0,0,0,0,0,0,0,0} , /* uchar bufferView[8] */
	{0} , /* unsigned char buffer[BUFFERSIZE] */
	{0} , /* unsigned char spiBuffer[1024] */
};

static void initPort( struct serialPort_ref *s ) {

	/* safety check to make sure port is not negative */
	if( s->port <= 0 ) {
		s->port = 1;
	}

	// Determine whether to change the connection mode
	if(s->useSerial == 1)
	{
		s->newConnectionMode = MODE_SERIAL;
	}
	else if(s->useSock == 1)
	{
		s->newConnectionMode = (s->useTcp == 1) ? MODE_TCP : MODE_UDP;
	}
	else if(s->newConnectionMode == 0)
	{
		// Don't change the connection mode; just re-initialize
		s->newConnectionMode = s->connectionMode;
	}
	// else: do nothing - the new connection mode can be set directly if all other flags are zero

	closePort(s);		/*this statement closes the port first if initialized */

	// Set to now open the new mode
	s->connectionMode = s->newConnectionMode;
	s->newConnectionMode = 0;

	switch( s->dataSource ) {

	case PORT_ON:
		// take care of opening serial
		if(s->connectionMode == MODE_SERIAL) {
			openCom(s);
		}
		// take care of opening sock
		else if((s->connectionMode == MODE_UDP) || (s->connectionMode == MODE_TCP)) {
			openSock(s);
		}
		break;

    default:
		break;
	}

	s->bytesread = 0;
	s->init = 0;

}

/** This function can be nilpotently. It always closes a valid file descriptor
 *  If the file descriptor is invalid we do not care anyway -
 */
void closePort( struct serialPort_ref *s )  {

	if(((s->connectionMode == MODE_UDP) || (s->connectionMode == MODE_TCP)) && (s->portIsOpen == 1))
	{
		closeSock(s);
		s->portIsOpen = 0;
		s->bytesread = 0;
	}
	else if((s->connectionMode == MODE_SERIAL) && (s->portIsOpen == 1))
	{
		closeCom(s);
		s->portIsOpen = 0;
		s->bytesread = 0;
	}
}


void readPort( struct serialPort_ref *s ) {

	int newBytes = 0;

	if( 1 == s->init )
		initPort( s );

	/* Read from device only if port is on */
	switch( s->dataSource ) {

	case PORT_ON:
		if( s->bytesread >= BUFFERSIZE || s->bytesread < 0 ) s->bytesread = 0; /* something very wrong... */
		if( (s->connectionMode == MODE_SERIAL) && s->portIsOpen) {
			newBytes = readCom( s, s->buffer + s->bytesread, BUFFERSIZE - s->bytesread );
			newBytes = LIMIT( newBytes, 0, BUFFERSIZE - s->bytesread ); /* extra protection in case return arguments are wierd in error cases */
			if( newBytes > 0 ) {
				s->bytesread += newBytes;
				memcpy( s->bufferView, s->buffer, 8 );
			}
		}
		else if( ((s->connectionMode == MODE_UDP) || (s->connectionMode == MODE_TCP)) && s->portIsOpen) {
#ifdef WIN32
			WinSerial *ws = (WinSerial*)s->serialDevice;
			if(ws) WaitForSingleObject( ws->readLock, INFINITE);
#endif
			newBytes = readSock(s, s->buffer + s->bytesread, BUFFERSIZE - s->bytesread);
#ifdef WIN32
			if(ws) ReleaseMutex(ws->readLock);
#endif
			newBytes = LIMIT( newBytes, 0, BUFFERSIZE - s->bytesread ); /* extra protection in case return arguments are weird in error cases */
			if( newBytes > 0 ) {
				s->bytesread += newBytes;
				memcpy( s->bufferView, s->buffer, 8 );
			}
		}
		break;

	default:
		break;
	}

}

void writePort( struct serialPort_ref *s, char *buffer, int size ) {

       
	int written = 0;

	/* should probably add functionality to check if send buffer is empty... */

	if( 1 == s->init )
		initPort( s );
	


	switch( s->dataSource ) {

	case PORT_ON:
		if( (s->connectionMode == MODE_SERIAL) && s->portIsOpen ) {
			written = writeCom( s, buffer, size );
		}
		else if( ((s->connectionMode == MODE_UDP) || (s->connectionMode == MODE_TCP)) && s->portIsOpen ) {
#ifdef WIN32
			WinSerial *ws = (WinSerial*)s->serialDevice;
			if(ws) WaitForSingleObject( ws->writeLock, INFINITE);
#endif
			written = writeSock( s, buffer, size );
#ifdef WIN32
			if(ws) ReleaseMutex(ws->writeLock);
#endif
		}
		break;

	default:
		break;
	}
}

void clearPort( struct serialPort_ref *s, int shift ) {

	if( shift != 0 ) {
		if( s->dataSource == PORT_ON ) {
			if( shift > 0 ) {
				if( shift >= s->bytesread ) {
					s->bytesread = 0;
				} else {
					memmove( s->buffer, &(s->buffer[shift]), s->bytesread - shift );
					/*memcpy( s->buffer, &(s->buffer[shift]), s->bytesread - shift );*/
					s->bytesread -= shift;
				}
				memcpy( s->bufferView, s->buffer, 8 );
			} else {
				s->bytesread = 0;
			}
		}
	}

}
