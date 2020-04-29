#include <String.h>

int gas_sensor1 = A0; //Sensor pin MQ4 int gas_sensor2 = A1; //Sensor pin MQ7 float m = -0.318; //Slope
float b = 1.133; //Y-Intercept

float R01 = 11.820; //Sensor Resistance in fresh air from previous code float R02 = 5.4; //Sensor Resistance in fresh air from previous code float ppm1;
float ppm2; void setup() {
	Serial.begin(9600); //Baud rate pinMode(gas_sensor1, INPUT);
	pinMode(gas_sensor2, INPUT);//Set gas sensor as input

}

void loop() {

	float sensor_volt1; //Define variable for sensor voltage float sensor_volt2;
	float RS_gas1; //Define variable for sensor resistance
	 
	float RS_gas2;

	float ratio1; //Define variable for ratio float ratio2;
	float sensorValue1 = analogRead(gas_sensor1); //Read analog values of sensor float sensorValue2 = analogRead(gas_sensor2);
	sensor_volt1 = sensorValue1 * (5.0 / 1023.0); //Convert analog values to voltage sensor_volt2 = sensorValue2 * (5.0 / 1023.0);
	RS_gas1 = ((5.0 * 10.0) / sensor_volt1) - 10.0; //Get value of RS in a gas RS_gas2 = ((5.0 * 10.0) / sensor_volt2) - 10.0;
	ratio1 = RS_gas1 / R01;

	ratio2 = RS_gas2 / R02;// Get ratio RS_gas/RS_air float ppm_log1 = (log10(ratio1) - b) / m;
	float ppm_log2 = (log10(ratio2) - b) / m;//Get ppm value in linear scale according to the the ratio value
	float ppm1 = pow(10, ppm_log1);

	float ppm2 = pow(10, ppm_log2);//Convert ppm value to log scale float percentage1 = ppm1 / 10000;
	float percentage2 = ppm2 / 10000;//Convert to percentage Serial.print("ppm1 = "); //Display "R0" Serial.println(percentage1); //Display value of R0
	 
	delay(1000);

	Serial.print("ppm2 = "); //Display "R0" Serial.println(percentage2); //Display value of R0 delay(1000);
	Send2Pachube();

	String	str1="GET

	http://api.thingspeak.com/update?api_key=I92PNT1AHLZPC4T3&field1="	+ String(percentage1);
	Serial.println(str1);//begin send data to remote server delay(4000);
	ShowSerialData();

	Serial.println((char)26);//sending

	delay(5000);//waitting for reply, important! the time is base on the condition of internet
	Serial.println(); ShowSerialData(); Send2Pachube();
	String	str2="GET

	http://api.thingspeak.com/update?api_key=I92PNT1AHLZPC4T3&field2="	+ String(percentage2);
	 
	Serial.println(str2);//begin send data to remote server delay(4000);
	if (float(percentage2)>0.2) //&& (int(percentage1)<2300))

	{

		Serial.println("GSM Modem Message :"); SendMessage();
		delay(5000);

	}

	ShowSerialData();

	Serial.println((char)26);//sending

	delay(5000);//waitting for reply, important! the time is base on the condition of internet
	Serial.println(); ShowSerialData();
	/* Serial.println("AT+CIPSHUT");//close the connection delay(100);
	ShowSerialData();*/ if (Serial.available())
	Serial.write(Serial.read());

}
 
void Send2Pachube()

{

	Serial.println("AT"); delay(1000); Serial.println("AT+CPIN?"); delay(1000); Serial.println("AT+CREG?"); delay(1000); Serial.println("AT+CGATT?"); delay(1000); Serial.println("AT+CIPSHUT"); delay(1000);
	Serial.println("AT+CIPSTATUS"); delay(2000); sSerial.println("AT+CIPMUX=0"); delay(2000);
	ShowSerialData();

	Serial.println("AT+CSTT=\"internet\"");//start task and setting the APN, delay(1000);
	ShowSerialData();
	 
	Serial.println("AT+CIICR");//bring up wireless connection delay(3000);
	ShowSerialData(); Serial.println("AT+CIFSR");//get local IP adress delay(2000);
	ShowSerialData(); Serial.println("AT+CIPSPRT=0"); delay(3000);
	ShowSerialData(); Serial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//start up the connection
	delay(6000); ShowSerialData();
	Serial.println("AT+CIPSEND");//begin send data to remote server delay(4000);
	ShowSerialData();

}

void ShowSerialData()

{

while(Serial.available()!=0)
 
	Serial.write(Serial.read());

}

void SendMessage()

{

	Serial.println("AT+CMGF=1");	//Sets the GSM Module in Text Mode delay(5000); // Delay of 1000 milli seconds or 1 second Serial.println("AT+CMGS=\"+918939708340\"\r"); // Replace x with mobile number
	delay(5000);

	Serial.println("Emergency Please Evacuate");// The SMS text you want to send delay(5000);
	Serial.println((char)26);// ASCII code of CTRL+Z delay(3000);
}
