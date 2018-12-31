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
	int16_t accel_zOut; 
	int16_t accel_yOut;
	if (CS == 29) {
		LSB = readIMU(62);
		HSB = readIMU(61);
		 accel_yOut = (((HSB << 8) & 0xff00) + (LSB & 0x00ff)); //+(HSB&0xff00);
		LSB = readIMU(62);
		HSB = readIMU(61);
		 accel_yOut = (((HSB << 8) & 0xff00) + (LSB & 0x00ff)); //+(HSB&0xff00);
	}
	else if(CS == 27) {
		LSB = readIMU(62);
		HSB = readIMU(61);
		 accel_yOut = (((HSB << 8) & 0xff00) + (LSB & 0x00ff)); //+(HSB&0xff00);
		LSB = readIMU(62);
		HSB = readIMU(61);
		 accel_yOut = (((HSB << 8) & 0xff00) + (LSB & 0x00ff)); //+(HSB&0xff00);
	}
	double x = (double)(accel_zOut) / (double)(16900);
	double tetha1 = acos(x) * 180 / M_PI;
	x = (double)(accel_yOut) / (double)(16900);
	double tetha2 = asin(x) * 180 / M_PI;
	//SerialUSB.println(accel_zOut);
	if (!isnan(tetha1) && tetha1 > 45) {
		return tetha1;
	}
	else if (!isnan(tetha2))
		if (tetha2 > 100)return 0;
	return  tetha2 - 1;
}