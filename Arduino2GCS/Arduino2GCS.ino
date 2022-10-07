#include "datalink.h"

struct datalink_ref* data = &Datalink;
unsigned long count = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  data->Arduino2GCS->fromArduino = count++;
  writeDatalink();
  delay(100);
}

void writeDatalink()
{
	data->Arduino2GCS->messageID = DATALINK_MESSAGE_Arduino2GCS;
	datalinkCheckSumEncode ( ( unsigned char* ) data->Arduino2GCS, sizeof ( struct datalinkMessageArduino2GCS_ref) );
	Serial.write ( ( char* ) data->Arduino2GCS, sizeof ( struct datalinkMessageArduino2GCS_ref) );
}