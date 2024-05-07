#include <SoftwareSerial.h>
SoftwareSerial gprsSerial(7,8);
 
SoftwareSerial PlantowerSerial(4, 5);  // RX, TX

// Variables to store PM1, PM2.5, and PM10 values
float pm1 = 0.0;
float pm2_5 = 0.0;
float pm10 = 0.0;


void setup()
{
  gprsSerial.begin(9600);                       // the GPRS baud rate   
  Serial.begin(9600);                          // the GPRS baud rate 
   PlantowerSerial.begin(9600);               // Initialize the Arduino's serial monitor.


 
  delay(1000);
}
 
void loop()
{

  // Check if there's data available to read from the Plantower sensor.
  if (PlantowerSerial.available() >= 32) {  // Check for a complete data packet (32 bytes).
    // Read the data from the sensor into a buffer.
    byte buffer[32];
    PlantowerSerial.readBytes(buffer, 32);

    // Check if the data packet starts with the correct header bytes.
    if (buffer[0] == 0x42 && buffer[1] == 0x4D) {
      // Extract PM1, PM2.5, and PM10 values from the data packet.
      pm1 = ((float)((unsigned int)buffer[10] << 8 | buffer[11]));
      pm2_5 = ((float)((unsigned int)buffer[12] << 8 | buffer[13]));
      pm10 = ((float)((unsigned int)buffer[14] << 8 | buffer[15]));

      // Print the extracted values to the serial monitor in three columns.
      Serial.print("   PM1: ");
      Serial.print(    pm1 );
      Serial.print(" ug/m3\t");

      Serial.print("   PM2.5:  ");
      Serial.print(    pm2_5   );
      Serial.print(" ug/m3\t");

      Serial.print("   PM10:   ");
      Serial.print(   pm10   );
      Serial.println(" ug/m3");
    }
  }

   
  if (gprsSerial.available())
    Serial.write(gprsSerial.read());
 
  gprsSerial.println("AT");
  delay(1000);
 
  gprsSerial.println("AT+CPIN?");
  delay(1000);
 
  gprsSerial.println("AT+CREG?");
  delay(1000);
 
  gprsSerial.println("AT+CGATT?");
  delay(1000);
 
  gprsSerial.println("AT+CIPSHUT");
  delay(1000);
 
  gprsSerial.println("AT+CIPSTATUS");
  delay(2000);
 
  gprsSerial.println("AT+CIPMUX=0");
  delay(2000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CSTT=\"mtngprs.com\"");//start task and setting the APN,
  delay(1000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIICR");//bring up wireless connection
  delay(3000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIFSR");//get local IP adress
  delay(2000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIPSPRT=0");
  delay(3000);
 
  ShowSerialData();
  
  gprsSerial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//start up the connection
  delay(6000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIPSEND");//begin send data to remote server
  delay(4000);
  ShowSerialData();
  
String str="GET https://api.thingspeak.com/update?api_key=WGN9QVX5HDTYFYXY&field1=" + String(pm1) +"&field2="+String(pm2_5)+"&field3="+String(pm10);
  
  Serial.println(str);
  gprsSerial.println(str);//begin send data to remote server
  
  delay(4000);
  ShowSerialData();
 
  gprsSerial.println((char)26);//sending
  delay(5000);//waitting for reply, important! the time is base on the condition of internet 
  gprsSerial.println();
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIPSHUT");//close the connection
  delay(100);
  ShowSerialData();
} 
void ShowSerialData()
{
  while(gprsSerial.available()!=0)
  Serial.write(gprsSerial.read());
  delay(5000); 
  
}
