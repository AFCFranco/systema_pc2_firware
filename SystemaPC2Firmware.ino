/**********************************************************************************************************
Project: Flexible sensor matrix
By: GIBIC GROUP. Andres Castaño and Yovany Ocampo 
LastRev: 6/12/2018
Description: this sketch use arduino Due, multiplexors and shiftRegisters. 
Uncomment #define prueba to test the hardware and let other options commented. 
To communicate using websocket uncomment #define webSocket
To communicate using USB uncomment #define USB
webSocket and USB can work together

MACROS / PIN DEFS
**********************************************************************************************************/



#include <WebSocketsServer.h>
#include <WebSocketsClient.h>
#include <WebSockets.h>
#include <Ethernet.h>

#include <SPI.h>





//************************************************************
//#define webSocket
//#define USBWS
//#define USB
#define prueba
//**************************************************************
#define BAUD_RATE                 115200
#define BAUD_RATE1                115200
//bits to select Rows into the multiplexers
#define FA 2
#define FB 3
#define FC 4
#define FD 5
//Pines para manejo de shift register
#define shiftData 9
#define shiftClock 8
//bits to enable multplexers
#define CHIP_COLM_0 6//5
#define CHIP_COLM_1 7//6
#define CHIP_COLM_2 10//7
#define CHIP_COLM_3 45
#define CHIP_COLM_4 21
#define CHIP_COLM_5 20
#define LED 12


#define PACKET_END_BYTE           0xFF
#define MAX_SEND_VALUE            254  //reserve 255 (0xFF) to mark end of packet
#define COMPRESSED_ZERO_LIMIT     254


#define MIN_SEND_VALUE			15//values below this threshold will be treated and sent as zeros

//the real amount of data send is 94*44. 
#define ROW_COUNT                 96//94//48
#define COLUMN_COUNT              88//88//48//sensor 1
#define PIN_ADC_INPUT             A1



/**********************************************************************************************************
GLOBALS
**********************************************************************************************************/
//String IP_SERVIDOR = "AT+CIPSTART=\"TCP\",\"192.168.0.124\",10000";//24
int zero_counter = 0;
int cont_Pos_Fila = 0;
unsigned long tiempo = 0;
unsigned int contBytes = 0;
byte chipSelectPin = 29;
byte angulo = 0;
unsigned int offset[ROW_COUNT][COLUMN_COUNT];
bool setOffset = true;
unsigned long tiempoOffset = 0;
unsigned long tiempoLed = 0;
unsigned long tiempoLed2 = 0;


/**********************************************************************************************************
setup()
**********************************************************************************************************/
void setup()
{
	//imu initial configurations
	SPI.begin();
	SPI.beginTransaction(SPISettings(10000, MSBFIRST, SPI_MODE3));
	pinMode(27, OUTPUT);
	digitalWrite(27, HIGH);
	pinMode(29, OUTPUT);
	digitalWrite(29, HIGH);
	//Serial config
	SerialUSB.begin(BAUD_RATE1); //pc communication 
	Serial2.begin(BAUD_RATE);// esp8266 - wifi communication



	//needed to make Serial USB work properly
	tiempo = millis() + 5000;
	while (!SerialUSB && tiempo > millis());
	SerialUSB.println("Starting...");

	//initial pin config
	for (int i = 2; i <= 13; i++)pinMode(i, OUTPUT);
	for (int i = 2; i <= 13; i++)digitalWrite(i, 0);
	pinMode(CHIP_COLM_3, OUTPUT);
	pinMode(CHIP_COLM_4, OUTPUT);
	pinMode(CHIP_COLM_5, OUTPUT);
	pinMode(LED, OUTPUT);
	digitalWrite(CHIP_COLM_3, 0);
	digitalWrite(CHIP_COLM_4, 0);
	digitalWrite(CHIP_COLM_5, 0);
	digitalWrite(LED, HIGH);


	pinMode(33, INPUT);
	digitalWrite(33, HIGH);
	//reset shift registers in order to make all pins start with cero volts
	for (int i = 2; i <= 50; i++) {
		digitalWrite(shiftData, LOW);
		digitalWrite(shiftClock, HIGH);
		delay(1);
		digitalWrite(shiftClock, LOW);
	}
	//while (true) {
	//	
	//	/*angulo=calcularAngulo(27);
	//	SerialUSB.print("angulo 2: ");
	//	SerialUSB.println(angulo);
	//	delay(500);*/
	//	angulo = calcularAngulo(29);
	//	SerialUSB.print("angulo 1: ");
	//	SerialUSB.println(angulo);
	//	delay(500);
	//}


	tiempoOffset = millis();
	tiempoLed = millis();
}


void loop()
{
	zero_counter = 0;
	contBytes = 0;
	char a = 0;
#ifdef USB
		//while (SerialUSB.read() != '*') {
		//}
	//while (SerialUSB.available())SerialUSB.read();
#endif // USB

#ifdef webSocket
	while (a != '*') {
		if ((millis() - tiempoLed) > 5000) {
			if ((millis() - tiempoLed2) < 500) {
				digitalWrite(LED, LOW);
			}
			else if ((millis() - tiempoLed2) < 1000) {
				digitalWrite(LED, HIGH);
			}
			else {
				tiempoLed2 = millis();
			}
		}
		
		a = Serial2.read();
		//if (a != 255)SerialUSB.print(a);
	}
	tiempoLed = millis();
	digitalWrite(LED, HIGH);

#endif
#ifdef webSocket
	while (Serial2.available()) {
		char a = Serial2.read();
		//Serial.print(a);
	}


	Serial2.write(byte(2 + 48));
	Serial2.write(byte(2 + 48));
	Serial2.write(byte(1 + 48));

	Serial2.write(byte(2 + 48));
	Serial2.write(byte(3 + 48));
	Serial2.write(byte(1 + 48));

	Serial2.write(byte(2 + 48));
	Serial2.write(byte(4 + 48));
	Serial2.write(byte(1 + 48));

	Serial2.write(byte(0 + 48));
	Serial2.write(byte(0 + 48));

	Serial2.write(byte(0 + 48));
	Serial2.write(byte(0 + 48));

	Serial2.write(byte(0 + 48));
	Serial2.write(byte(0 + 48));
#endif

#ifdef USBWS
	SerialUSB.write(byte(2 + 48));
	SerialUSB.write(byte(2 + 48));
	SerialUSB.write(byte(1 + 48));

	SerialUSB.write(byte(2 + 48));
	SerialUSB.write(byte(3 + 48));
	SerialUSB.write(byte(1 + 48));

	SerialUSB.write(byte(2 + 48));
	SerialUSB.write(byte(4 + 48));
	SerialUSB.write(byte(1 + 48));

	SerialUSB.write(byte(4 + 48));
	SerialUSB.write(byte(2 + 48));

	SerialUSB.write(byte(4 + 48));
	SerialUSB.write(byte(1 + 48));

	SerialUSB.write(byte(4 + 48));
	SerialUSB.write(byte(1 + 48));
#endif
	for (int i = 0; i < ROW_COUNT; i++)
	{
		selectFila();
		if (i <=2 || i>=93 )continue;
		for (int j = 0; j < COLUMN_COUNT/2 ; j++)
		{
			if (i < 48) {
				selectColumna(j);
			}
			else {
				selectColumna(j + 48);
			}
#ifdef prueba
			SerialUSB.print("i ");
			SerialUSB.print(i);
			SerialUSB.print(",j ");
			SerialUSB.println(j);
			char a = 0;
			while (a != 'c'&a != 'f') {
				a = SerialUSB.read();
				SerialUSB.print("Fila: ");
				SerialUSB.print(i);
				SerialUSB.print(" ,Columna: ");
				SerialUSB.print(j);
				SerialUSB.print("= ");
				SerialUSB.println(((analogRead(PIN_ADC_INPUT))));
				/*	int dato = analogRead(PIN_ADC_INPUT);
					SerialUSB.write(highByte(dato));
					SerialUSB.write(lowByte(dato));*/

				delay(50);
				if (a == 'f')
					j = COLUMN_COUNT;
			}
#endif // prueba
#ifndef prueba
			unsigned long raw_reading = 0;
			delayMicroseconds(10);
			raw_reading = analogRead(PIN_ADC_INPUT);
			raw_reading >> 4;
			


			if (setOffset == true && (millis() - tiempoOffset) < 6000 && (millis() - tiempoOffset) > 2000) {
				offset[i][j] = max(offset[i][j], raw_reading);
			}
			if ((millis() - tiempoOffset) > 6000) {
				setOffset = false;
			}
			long temporal = (long)(raw_reading - offset[i][j]);
			if (temporal < 0)temporal = 0;
			raw_reading = temporal;
			

			/*if (setOffset == true && (millis() - tiempoOffset) < 6000 && (millis() - tiempoOffset) > 2000) {
				offset[i][j] = max(offset[i][j], raw_reading);
			}
			raw_reading = raw_reading - offset[i][j];*/
			if (raw_reading > 253)raw_reading = 253;
			if (raw_reading < 0)raw_reading = 0;
			byte send_reading = (byte)(lowByte(raw_reading));
			//SerialUSB.print(i);
			//SerialUSB.print("       ");
			//SerialUSB.println(j);
			sendCompressed(send_reading);			
#endif // !prueba
		}
	}

	//*******************************************
	if (zero_counter > 0)
	{
#ifdef webSocket
		Serial2.write((byte)0);
		contBytes++;
		Serial2.write((byte)zero_counter);
		contBytes++;
#endif
#ifdef USBWS
		SerialUSB.write((byte)0);
		contBytes++;
		SerialUSB.write((byte)zero_counter);
		contBytes++;
#endif

#ifdef USB
		SerialUSB.write((byte)0);
		contBytes++;
		SerialUSB.write((byte)zero_counter);
		contBytes++;
#endif 
	}
#ifdef webSocket
	Serial2.write((byte)PACKET_END_BYTE);
	SerialUSB.println("Trama enviada ");
	contBytes += 15;
	SerialUSB.println(contBytes);
	contBytes = 0;
#endif // webSocket
#ifdef USB
	SerialUSB.write((byte)PACKET_END_BYTE);
	contBytes = 0;
#endif // USB
}
/**********************************************************************************************************
sendCompressed() - If value is nonzero, send it via serial terminal as a single byte. If value is zero,
increment zero count. The current zero count is sent and cleared before the next nonzero value
**********************************************************************************************************/
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