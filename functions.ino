
void selectColumna(int colmn)
{
	int multColm = 0;
	bitSet(multColm, colmn >> 4);//to enable the mux chip
	digitalWrite(CHIP_COLM_0, bitRead(multColm, 0));
	digitalWrite(CHIP_COLM_1, bitRead(multColm, 1));
	digitalWrite(CHIP_COLM_2, bitRead(multColm, 2));
	digitalWrite(CHIP_COLM_3, bitRead(multColm, 3));
	digitalWrite(CHIP_COLM_4, bitRead(multColm, 4));
	digitalWrite(CHIP_COLM_5, bitRead(multColm, 5));
	//to select the channel
	digitalWrite(FD, bitRead(colmn, 3));
	digitalWrite(FC, bitRead(colmn, 2));
	digitalWrite(FB, bitRead(colmn, 1));
	digitalWrite(FA, bitRead(colmn, 0));
}

int selectFila()
{
	if (cont_Pos_Fila == 0)
	{
		digitalWrite(shiftData, HIGH);
		digitalWrite(shiftClock, HIGH);
		delay(10);
		digitalWrite(shiftClock, LOW);
		digitalWrite(shiftData, LOW);
		cont_Pos_Fila++;
	}
	digitalWrite(shiftClock, HIGH);
	digitalWrite(shiftClock, LOW);
	digitalWrite(shiftData, LOW);
	cont_Pos_Fila++;
	if (cont_Pos_Fila == ROW_COUNT + 1)
	{
		cont_Pos_Fila = 0;
	}
	return cont_Pos_Fila;
}

void leerSensores() {
	float tempC1;
	float tempC2;
	float tempC3;
	byte angulo1;
	byte angulo2;
	byte angulo3;
	sensors.requestTemperatures();
	tempC1 = sensors.getTempC(Thermometer_1);
	tempC2 = sensors.getTempC(Thermometer_2);
	tempC3 = sensors.getTempC(Thermometer_3);
	angulo1 = calcularAngulo(CS1);
	angulo2 = calcularAngulo(CS2);
	sensores[0] = tempC1;
	sensores[1] = tempC2;
	sensores[2] = tempC3;
	sensores[3] = angulo1;
	sensores[4] = angulo2;
	sensores[5] = 0;
	//SerialUSB.println(sensores[0]);
	//SerialUSB.println(sensores[1]);
	//SerialUSB.println(sensores[2]);
	//SerialUSB.print("angulo 2: ");
	//SerialUSB.println(sensores[3]);
	//SerialUSB.print("angulo 1: ");
	//SerialUSB.println(sensores[4]);
}