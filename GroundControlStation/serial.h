#ifndef serial_h
#define serial_h

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include "config.h"

#ifndef MIN
#define MIN(x1,x2) ((x1)<(x2)?(x1):(x2))
#endif
#ifndef MAX
#define MAX(x1,x2) ((x1)>(x2)?(x1):(x2))
#endif
#define LIMIT(x,xl,xu) ((x)>=(xu)?(xu):((x)<(xl)?(xl):(x)))
#define ABS(x) ((x)<0.0?(-(x)):(x))
#define SQ(x) ((x)*(x))
#define CUBE(x) ((x)*(x)*(x))
#define SIGN(x) ((x)<(0)?(-1):((x)>(0)?(1):(0)))
#define INRANGE(x,x1,x2) ((x)<=(x2)&&(x)>=(x1)?1:0)

#define BUFFERSIZE 1024*100

#define PORT_OFF   0
#define PORT_ON   1

#define SI_IPC 11

#define MODE_SERIAL 1
#define MODE_UDP    2
#define MODE_TCP    3

#define MODE_UDP    2
#define MODE_TCP    3

#define BSD_BASE_PORT 2000

#include "ether.h"

/** Must declare with C linkage because it is serial.c not .cpp */

#ifdef __cplusplus
extern "C" {
#endif

	struct serialPort_ref
	{
		int dataSource; /* 0=off,1=on,2=SITL,3=playback,4=worker */
		int init; /*  */
		int port; /* port number (1 lowest) */
		int si_port; /* SITL port */
		int baud; /*  */
		char termios[7]; /* 8|7, n|o|e, 1, default is 8n1  */
		char nameFmt[32]; /* port name format (posix) */
		char name[22]; /* port name (posix) */
		char connectTo[51]; /* host name of machine to connect to */
		int hridx; /* history recording index */
		int sridx; /* stream recorder index */
		int srmode; /* stream recorder mode */
		int portNum; /* port number */
		int portType; /* port type specifier for DSP, 0-fifo port, 1-direct uart */
		int isServer; /* is this a server port */
		int useSerial; /* Default is to use the hardware serial - used to change modes */
		int useSock; /* Use Berkeley Sockets - used to change modes */
		int useTcp; /* Use tcp instead of udp - used to change modes */
		int useI2C; /* Use I2C - For Jetson TX2 and other supported hardware */
		int connectionMode; /* The current connection mode (defaults to none (off)) (1=serial,2=UDP,3=TCP,4=SPI,5=I2C) */
		int newConnectionMode; /* Set to a mode to change modes (1=serial,2=UDP,3=TCP,5=I2C) */
		int connectState; /* state of tcp connection */
		int setHostnameManually; /* skip lookup of hostname and allow specification in myname */
		double connectRetry; /* connect retry frequency */
		double lastConnectRetry; /* last time connection was retried */
		int saveToFile; /* Save data to file */
		char saveFile[96]; /* file name to save */
		char readFile[96]; /* file to read from */
		int saveFileOpen; /* file is open for save */
		long saveFileFd; /* file descriptor */
		int readFileOpen; /* file is open for save */
		long readFileFd; /* file descriptor */
		int readChunk; /* bytes to read per call (read from file) */
		int portIsOpen; /* Is port open flag */
		int bytesread; /*  */
		int fd; /* File Descriptor */
		unsigned long dropwrite; /* bytes dropped while writing */
		unsigned long dropread; /* bytes dropped while reading */
		unsigned long sent; /* sent bytes */
		unsigned long received; /* received bytes */
		int blocking; /* make this a blocking port  */
		int blockingRead; /* make this a blocking read port */
		double blockTimeout; /* s, -1 = block for ever, timeout for select timeout */
		int blockDeadlineExpired; /* nondim, blocking read timed out */
		void* serialDevice; /* serial device pointer */
		void* sockDevice; /* bsd device pointer */
		char myname[47]; /* myhostname */
		int myport; /* myport */
		char remotename[44]; /* remotename */
		int remoteport; /* remoteport */
		int errorNumberRx; /*  */
		int errorNumberTx; /*  */
		char spiName[36]; /* port name (posix) */
		unsigned char spiSetSettings; /* Whether to set the settings below or use defaults */
		unsigned char spiMode; /* Bit packed mode for the SPI (using the SPI definitions above) (separate with | ) */
		unsigned short spiDelay; /* delay (usec) */
		int spiSpeed; /* max speed (Hz) */
		int spiBpw; /* bits per word */
		char I2CAddress; /* address of I2C slave device  */
		char I2CName[12]; /* I2C port name  */
		char seekState; /* state machine state when using readMsg to read port  */
		char align[3]; /* for alignment */
		char tmp[8]; /* array size is SERIALMSG_HEADER_SIZE  */
		char hdr[8]; /* buffer for filling in header array size is SERIALMSG_HEADER_SIZE */
		unsigned char bufferView[8]; /*  */
		unsigned char buffer[BUFFERSIZE]; /*  */
		unsigned char spiBuffer[1024]; /* Used for the read/write transfers */
	};

	extern struct serialPort_ref commPort;

	void readPort ( struct serialPort_ref* s );
	void writePort ( struct serialPort_ref* s, char* buffer, int size );
	void closePort ( struct serialPort_ref* s );
	void clearPort ( struct serialPort_ref* s, int shift );

#ifdef __cplusplus
}
#endif


#endif
