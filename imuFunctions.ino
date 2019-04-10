
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
#ifdef IPS4
		LSB = readIMU(60);
		HSB = readIMU(59);
#endif
#ifdef IPS2
		LSB = readIMU(62);
		HSB = readIMU(61);
#endif
#ifdef IPS3
		LSB = readIMU(62);
		HSB = readIMU(61);
#endif	
		accel_yOut = (((HSB << 8) & 0xff00) + (LSB & 0x00ff)); //+(HSB&0xff00);
	}
	double x = (double)(accel_yOut) / (double)(16900); //convert from adc to gravity
#ifdef IPS4
	double tetha1 = -(asin(x) * 180 / M_PI - OFFSET_TETHA_IPS4);
#endif
#ifdef IPS2
	double tetha1 = -(asin(x) * 180 / M_PI - OFFSET_TETHA_IPS2);
#endif
#ifdef IPS3
	double tetha1 = -(asin(x) * 180 / M_PI - OFFSET_TETHA_IPS3);
#endif	
	if(CS==CS1)return tetha1;
	else if (CS == CS2) return tetha1;
}