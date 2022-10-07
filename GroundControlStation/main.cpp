#include <iostream>
#include <iomanip>

#include "datalink.h"
#include "serial.h"

int main ()
{
	struct serialPort_ref* port = &commPort;
	struct datalink_ref* data = &Datalink;
	unsigned long count = 0;

	while ( true )
	{
		data->GCS2Arduino->fromGCS = count++;
		readDatalink ( port, data );
		std::cout << "MessageSize: " << data->Arduino2GCS->messageSize << ", ";
		std::cout << "Checksum: " << std::setw(10) << std::setfill(' ') << data->Arduino2GCS->csum << ", ";
		std::cout << "Data From Arduino: " << std::setw(3) << std::setfill(' ') << (int)data->Arduino2GCS->fromArduino;
		std::cout << '\r' << std::flush;
	}
	return 0;
}
