
void sendCompressed(byte value)
{
	if (value < MIN_SEND_VALUE)
	{
		if (zero_counter < (COMPRESSED_ZERO_LIMIT - 1))
		{
			zero_counter++;
		}
		else
		{
#ifdef webSocket
			Serial2.write((byte)0);
			contBytes++;
			Serial2.write((byte)COMPRESSED_ZERO_LIMIT);
			contBytes++;
#endif // 		
#ifdef USB
			SerialUSB.write((byte)0);
			contBytes++;
			SerialUSB.write((byte)COMPRESSED_ZERO_LIMIT);
			contBytes++;
#endif // 	
			zero_counter = 0;
		}
	}
	else
	{
		if (zero_counter > 0)
		{
#ifdef webSocket
			Serial2.write((byte)0);
			contBytes++;
			Serial2.write((byte)zero_counter);
			contBytes++;
#endif 
#ifdef USB
			SerialUSB.write((byte)0);
			contBytes++;
			SerialUSB.write((byte)zero_counter);
			contBytes++;
#endif 
			zero_counter = 0;
		}
		if (value > MAX_SEND_VALUE)
		{
#ifdef webSocket
			Serial2.write((byte)MAX_SEND_VALUE);
			contBytes++;
#endif
#ifdef USB
			SerialUSB.write((byte)MAX_SEND_VALUE);
			contBytes++;
#endif
		}
		else
		{
			value = value - 1 + 2;
#ifdef webSocket
			Serial2.write((byte)value);
			contBytes++;
#endif 
#ifdef USB
			SerialUSB.write((byte)value);
			contBytes++;
#endif 
		}
	}
}
