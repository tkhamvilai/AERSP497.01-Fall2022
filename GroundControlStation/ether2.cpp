/*** BeginCopyright
 * Copyright 2002, Georgia Institute of Technology, All Rights Reserved.
 * Unauthorized use and/or redistribution is disallowed.
 * This library is distributed without any warranty; without even
 * the implied warranty of fitness for a particular purpose.
 *
 * UAV Laboratory
 * School of Aerospace Engineering
 * Georgia Institute of Technology
 * Atlanta, GA 30332
 * http://controls.ae.gatech.edu
 *
 * Contact Information:
 * Prof. Eric N. Johnson
 * http://www.ae.gatech.edu/~ejohnson
 * Tel : 404 385 2519
 * EndCopyright
 ***/
//#if defined(SUL_OS_LINUX)
/* This file is place holder of empty functions for OS specific funcs 
 * that do not work everywhere all of which is actually in ether.cpp */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <strings.h>
#include <netinet/in.h>		// hopefully this will work in qnx6.2
#include <arpa/inet.h>
	//#include "rmax/onboard_ref.h"
	//#include "rmax/ether.h"
	//#include "esim/util.h"
#include "ether.h"

#define PTR2INTEGER long

int openCom(struct serialPort_ref *s) {
	termios options;
	int dcbbaud;
//#ifdef LINUX
	if (s->name[0] != '/') {
		sprintf(s->name,"/dev/ttyS%d",s->port-1);
	}
//#endif
//#ifdef QNX
//	sprintf(s->name,"/dev/ser%d",s->port);
//#endif
	if (s->blocking) {
		s->fd = open(s->name, O_RDWR | O_NOCTTY);
	} else {
		s->fd = open(s->name, O_RDWR | O_NOCTTY | O_NONBLOCK);
	}


	/* get options */
	tcgetattr(s->fd,&options);
	memset(&options,0,sizeof(options));

	//options.c_cflag = CRTSCTS | CLOCAL | CREAD;
	options.c_cflag = CLOCAL | CREAD;
	/*options.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;*/
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;  /* set input mode (non-canonical, no echo,...) */

	// set character size (default is 8 bits per character )
	switch (s->termios[0]) {
	case '7':
		options.c_cflag |= CS7;
		break;
	default:
	case '8':
		options.c_cflag |= CS8;
		break;
	};

	// set parity (default is no parity )
	switch (s->termios[1]) {
	case 'e':
	case 'E':
		options.c_cflag |= PARENB;	// default is even
		break;
	case 'o':
	case 'O':
		options.c_cflag |= PARENB | PARODD;
		break;
	default:
	case 'n':
	case 'N':
		break;
	};

	// set stop bits 
	switch (s->termios[2]) {
	case '2':
		options.c_cflag |= CSTOPB;
		break;
	default:
	case '1':
		// this corresponds to 1 stop bit (default in posix)
		break;
	};

	// no flow control
	options.c_iflag &= ~(IXON | IXOFF | IXANY);


	/* Convert the Baud Rate to the Com library define for the DCB */
	switch ( s->baud ) {
	case 110:
		dcbbaud = B110;
		break;
	case 300:
		dcbbaud = B300;
		break;        
	case 600:
		dcbbaud = B600;
		break;
	case 1200:
		dcbbaud = B1200;
		break;
	case 2400:
		dcbbaud = B2400;
		break;
	case 4800:
		dcbbaud = B4800;
		break;
	case 9600:
		dcbbaud = B9600;
		break;            
	case 19200:
		dcbbaud = B19200;
		break;
	case 38400:
		dcbbaud = B38400;
		break;        
	case 57600:
		dcbbaud = B57600;
		break;
	case 115200:
		dcbbaud = B115200;
		break;
	case 230400:
		dcbbaud = B230400;
		break;
	default:
		return -1;
	}


	cfsetispeed(&options, dcbbaud);
	cfsetospeed(&options, dcbbaud);



	if (s->fd == 0 || s->fd == -1) {
		s->portIsOpen = 0;
		printf( " serial: Comport %s Open FAIL!\n", s->name );
	} else {
		s->portIsOpen = 1;
		printf( " serial: Comport %s Open SUCCESS baud %d %s (handle %d)\n", 
				s->name, s->baud, s->termios, s->fd );
	}

	/* now clean the modem line and activate the settings for the port*/
	tcflush(s->fd, TCIFLUSH);
	tcsetattr(s->fd,TCSANOW,&options);

	return 0;
}

int closeCom(struct serialPort_ref *s) {
	if (s->fd > 0) {
		close(s->fd);
		s->fd = 0;
		return 0;
	}
	return -1;
}

/*static int totalread = 0;*/
int readCom(struct serialPort_ref *s, void *buf, int nbytes) {
	/*
	int avail,bytesread;
	avail = 0;
	bytesread = 0;
	ioctl(s->fd,FIONREAD,(int)(&avail));
	bytesread = read(s->fd, (char*)buf, avail);
	totalread += bytesread; 
	printf("\r avai = %d, read = %d total = %d          ",avail,bytesread,totalread);
	return 0;
	*/

	int newBytes;
	newBytes = 0;
	ioctl( s->fd, FIONREAD, (PTR2INTEGER)(&newBytes) );
	if ( newBytes > 0 ) {
		newBytes = MIN( newBytes, nbytes);
		newBytes = read( s->fd, (char*)buf, newBytes );
		//printf("\r                     new = %d, nbytes  = %d",newBytes,nbytes);
		s->received += MAX(0,newBytes);
		//writeCom(s,s->buffer,newBytes); // for testing only
	}
	return newBytes;
}

/*static void printerrno(int kerrno) {
  switch(kerrno) {
  case EBADF:
	printf("\nEBADF");
	break;
  case EINVAL:
	printf("\nEINVAL");
	break;
  case EFAULT:
	printf("\nEFAULT");
	break;
  case EFBIG:
	printf("\nEFBIG");
	break;
  case EPIPE:
	printf("\nEPIPE");
	break;
  case EAGAIN:
	printf("\nEAGAIN");
	break;
  case EINTR:
	printf("\nEINTR");
	break;
  case ENOSPC:
	printf("\nENOSPC");
	break;
  case EIO:
	printf("\nEIO");
	break;
  default:
	printf("\ndunno errno");
	break;
  };

}*/

int writeCom(struct serialPort_ref *s, void *buf, int nbytes) {

	int written;
	written = write( s->fd, (char*)buf, nbytes );
	/*tcflush(s->fd, TCOFLUSH);*/
	if (  written != nbytes ) {
		//printerrno(errno);
		s->dropwrite += nbytes - written;
		printf( " serial: write error on %s\n", s->name );
	}
	s->sent += written;
	return written;
}


static int getaddr(struct sockaddr_in *ad, char *hostname, unsigned short port) {
	struct hostent *hp;
	memset(ad, 0, sizeof(struct sockaddr_in)); /* clear our address */
	hp = gethostbyname(hostname); 
	if ( hp == NULL) {
		printf(" getaddr: lookup of %s failed\n",hostname);
		return(-1);
	}
	memcpy((char*)(&(ad->sin_addr)), hp->h_addr,hp->h_length);
//printf( "ip found is = %u.%u.%u.%u\n", hp->h_addr_list[0][0], hp->h_addr_list[0][1], hp->h_addr_list[0][2], hp->h_addr_list[0][3] );
//printf( "%d\n", hp->h_length );
//if( hp->h_addr_list[0][hp->h_length] != NULL ) {
//printf( "ip found2 is %u.%u.%u.%u\n", hp->h_addr_list[1][0], hp->h_addr_list[1][1], hp->h_addr_list[1][2], hp->h_addr_list[1][3] );
//}
	ad->sin_family = AF_INET;
	ad->sin_port = htons(port);
	return 0;
	// bzero(&(ad->sin_zero),8);
}

class PosixUDP {
public:
	struct sockaddr_in ma;
	struct sockaddr_in ra;
	struct sockaddr_in bma;
	struct sockaddr_in bra;

	int openSock(struct serialPort_ref *s);
	int closeSock(struct serialPort_ref *s);
	int readSock(struct serialPort_ref *s, void *buf, int nbytes);
	int writeSock(struct serialPort_ref *s, void *buf, int nbytes);
	void copyaddr() {
		memcpy(&ma,&bma,sizeof(bma));
		memcpy(&ra,&bra,sizeof(bra));
	}
	bool tryConnect(struct serialPort_ref *s);
	bool isAlreadyConnectedError();
	bool isBlockingError();
	bool isConnReset();	

};

int PosixUDP::openSock(struct serialPort_ref *s) {
    unsigned long one = 1;
	/*int tmp;*/

	// formulate addresses of my port and remote ports
	if (s->isServer) {
		s->myport     = BSD_BASE_PORT + s->portNum + 1000;
		s->remoteport = BSD_BASE_PORT + s->portNum;
	} else {
		s->myport     = BSD_BASE_PORT + s->portNum;
		s->remoteport = BSD_BASE_PORT + s->portNum + 1000;
	}

      // is isServer is actually set to 99 then you can manually set port numbers
	//this was added for Mavlink gcs setup
	if( s->isServer == 99 ) {
		s->myport     =  s->portNum;
		s->remoteport =  s->port;  
	}

	if (s->connectionMode == MODE_TCP) {
		s->remoteport = s->portNum;
	}

	if( !s->setHostnameManually )
    	gethostname(s->myname,sizeof(s->myname));

	strcpy(s->remotename,s->connectTo);

	if ( getaddr(&bma,s->myname,s->myport) != 0 ) return -1;
	if ( getaddr(&bra,s->remotename,s->remoteport) != 0 ) return -1;
	copyaddr();


	// create the socket
	if (s->connectionMode == MODE_TCP) {
		s->fd = socket(AF_INET, SOCK_STREAM, 0);
	} else {
		s->fd = socket(AF_INET, SOCK_DGRAM, 0);
	}

	if (s->fd == -1 ) {
		printf(" serial: openSock Cannot make socket\n");
		return -1;
	} else {
		ioctl(s->fd, FIONBIO, &one);
	}

	if (s->connectionMode == MODE_TCP) {
		if(!this->tryConnect(s)) {
			printf( " serial: Comport still waiting (TCP attempting to connect to %s:%d)\n",s->connectTo,s->portNum);
		}

	} else {

		if (bind(s->fd, (struct sockaddr *)(&ma), sizeof(ma)) == -1) {
			printf(" serial: open Cannot bind socket to %s:%d\n",s->myname,s->myport);
			s->portIsOpen = 0;
			return(-1);
		} else {
			printf( " serial: Comport %d Open SUCCESS (UDP Sockets listening at %s:%d to %s:%d)\n", 
					s->port,s->myname,s->myport,s->remotename,s->remoteport);
		}
	}
	s->portIsOpen = 1;
	return 0;
}




int PosixUDP::closeSock(struct serialPort_ref *s) {
	if (s->fd > 0) {
		close(s->fd);
		s->fd = 0;
	}
	s->portIsOpen = 0;
	s->connectState = 0;
	return 0;
}

int PosixUDP::readSock(struct serialPort_ref *s, void *buf, int nbytes) {
    int toread, status, totalNew = 0;
	socklen_t sizeofsockaddr;
	toread = 0;
	sizeofsockaddr = sizeof(struct sockaddr);

	copyaddr();
	if(!this->tryConnect(s)) return 0;

	
	// if socket is connected
    if (s->portIsOpen ) {
        do {
            status = ioctl(s->fd,FIONREAD,(PTR2INTEGER)&toread);
            if (status == -1) {
                printf(" serial: problem receiving at %s:%d from %s:%d\n",
                       s->myname,s->myport,s->remotename,s->remoteport);
                return totalNew;
            }
		
            //ret = recvfrom(sd, (char*)buf, MIN(toread,(unsigned long)nbytes),  0, (struct sockaddr*)ra, &sizeofsockaddr );
            if(s->connectionMode == MODE_TCP) {
                status = recv(s->fd,(char*)buf, MIN(toread,nbytes),  0);
            } else {
                status = recvfrom(s->fd, (char*)buf, MIN(toread,nbytes),  0, (struct sockaddr*)(&ra), &sizeofsockaddr );
            }

            //printf("\n status = %d", status);
            if (status == -1) {
                return totalNew;
            } else {
                //buf += status;
                buf = (void*)( (char*)buf + status );
                totalNew += status;
                s->received += status;
            }
        } while( status > 0 );

	}
    return totalNew;
}

int PosixUDP::writeSock(struct serialPort_ref *s, void *buf, int nbytes) {
	int ret;
	copyaddr();
	if(!this->tryConnect(s)) return 0;
	// if socket is connected
	if (s->portIsOpen) {
		if(s->connectionMode == MODE_TCP) {
			ret = send(s->fd,(char*)buf, nbytes, 0);
		} else {
			ret = sendto(s->fd, (char*)buf, nbytes, 0, (struct sockaddr*)(&ra), sizeof(struct sockaddr) );
		}

		/*
		printf("\r nbytesd = %d ret = %d errno = %d  sinport = %d ptr = 0x%x    ",
		   nbytes,ret,errno,((struct sockaddr_in*)(&ra))->sin_port,&(ra));
		*/

		if (ret == -1) {
			return 0;
		} else {
			s->sent += ret;
			return ret;
		}
	}
	return 0;
}




bool PosixUDP::isAlreadyConnectedError() {
    if(errno == EISCONN) {
        return true;
    } else {
        return false;
    }
}
bool PosixUDP::isBlockingError() {
    switch (errno) {
    case EWOULDBLOCK: // always == NET_EAGAIN?
    case EALREADY:
    case EINPROGRESS:
        return true;
    }
    return false;
}

bool PosixUDP::isConnReset() {
    switch (errno) {
    case ECONNRESET: // always == NET_EAGAIN?
        return true;
    }
    return false;
}


bool PosixUDP::tryConnect(struct serialPort_ref *s) {
	if(s->connectionMode == MODE_TCP) {
		if(s->connectState == 1) {
			return true;
		} else {
			int ret = ::connect(s->fd,(const sockaddr*)(&ra),sizeof(struct sockaddr_in));
			if(ret == -1) {
				if(this->isBlockingError()) {
					return false; // still connecting
				} else if(this->isAlreadyConnectedError()) {
					printf( " serial: Comport Open Success (TCP Sockets connected to %s:%d)\n",s->connectTo,s->portNum);
					s->connectState = 1;
					return true;
				} else if(this->isConnReset()) {
					printf(" serial: TCP Connection to  %s:%d was reset, so closing port\n",s->connectTo,s->portNum);
					this->closeSock(s);
					return false;
				}
			} else {
				printf( " serial: Comport Open Success (TCP Sockets connected to %s:%d)\n",s->connectTo,s->portNum);
				s->connectState = 1;
				return true;
			}
			return false;
		}
	} else {
		if( s->portIsOpen) {
			return true;
		} else {
			return false;
		}
	}
}
int openSock(struct serialPort_ref *s) {
	PosixUDP *sdev;
	sdev = new PosixUDP();
	s->sockDevice = (void*)sdev;
	return sdev->openSock(s);
}

int closeSock(struct serialPort_ref *s) {
	int ret;
	PosixUDP *sdev;
	sdev = (PosixUDP*)(s->sockDevice);
	ret =  sdev->closeSock(s);
	delete sdev;
	s->sockDevice = NULL;
	return 0;
}

int readSock(struct serialPort_ref *s, void *buf, int nbytes) {
	PosixUDP *sdev;
	sdev = (PosixUDP*)(s->sockDevice);
	return sdev->readSock(s,buf,nbytes);
}

int writeSock(struct serialPort_ref *s, void *buf, int nbytes) {
	PosixUDP *sdev;
	sdev = (PosixUDP*)(s->sockDevice);
	return sdev->writeSock(s,buf,nbytes);
}


//#endif
















