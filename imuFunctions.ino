	//
//byte readIMU(int dataToRead) {
//	digitalWrite(chipSelectPin, LOW);
//	dataToRead += 128;
//	SPI.transfer(dataToRead);
//	int dato = SPI.transfer(0);
//	digitalWrite(chipSelectPin, HIGH);
//	return dato;
//}
//
//void writeIMU(byte address, byte value) {
//	digitalWrite(chipSelectPin, LOW);
//	//address;
//	SPI.transfer(address);
//	SPI.transfer(value);
//	digitalWrite(chipSelectPin, HIGH);
//}
//
//byte calcularAngulo(byte CS) {
//	chipSelectPin = CS;
//	int8_t LSB = readIMU(62);
//	int8_t HSB = readIMU(61);
//	int16_t accel_zOut = (((HSB << 8) & 0xff00) + (LSB & 0x00ff)); //+(HSB&0xff00);
//	double x = (double)(accel_zOut) / (double)(16900);
//	double tetha = acos(x) * 180 / M_PI;
//	//SerialUSB.println(accel_zOut);
//	if (!isnan(tetha))
//		return (byte)tetha;
//	else
//		return (byte)0;
//	delay(200);
//}
//


byte readIMU(int dataToRead) {
	digitalWrite(chipSelectPin, LOW);
	dataToRead += 128;
	SPI.transfer(dataToRead);
	int dato = SPI.transfer(0);
	digitalWrite(chipSelectPin, HIGH);
	return dato;
}

void writeIMU(byte address, byte value) {
	digitalWrite(chipSelectPin, LOW);
	//address;
	SPI.transfer(address);
	SPI.transfer(value);
	digitalWrite(chipSelectPin, HIGH);
}

double calcularAngulo(byte CS) {
	chipSelectPin = CS;
	int8_t LSB;
	int8_t HSB;
	int8_t R;
	int16_t accel_zOut; 
	int16_t accel_yOut;
	if (chipSelectPin == 14 || chipSelectPin == 15 || chipSelectPin == 27 || chipSelectPin == 29) {
		//acel x 60 - 59 sabana 1
		//acel y 62 - 61 sabana 3
#ifdef IPS_4
		LSB = readIMU(59);
		HSB = readIMU(60);
#endif // IPS_4		
		accel_yOut = (((HSB << 8) & 0xff00) + (LSB & 0x00ff)); //+(HSB&0xff00);
	}
	//SerialUSB.println(accel_yOut);
#ifdef DEBUG

#endif // DEBUG
	double x = (double)(accel_yOut) / (double)(16900); //convert from adc to gravity
#ifdef IPS_4
	double tetha1 = -(asin(x) * 180 / M_PI-OFFSET_TETHA_IPS4 );
#endif // IPS_4

	
	if(CS==CS1)return tetha1;
	else if (CS == CS2) return tetha1;
	//x = (double)(accel_yOut) / (double)(16900);
	//double tetha2 = asin(x) * 180 / M_PI;
	////SerialUSB.println(accel_zOut);
	//if (!isnan(tetha1) && tetha1 > 45) {
	//	return tetha1;
	//}
	//else if (!isnan(tetha2))
	//	if (tetha2 > 100)return 0;
	//return  tetha2 - 1;
}