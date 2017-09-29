
#include <SoftwareSerial.h>

SoftwareSerial mySerial(9, 10);

void setup() {
  // put your setup code here, to run once:
   mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  delay(100);
  
}

void loop() 
{
   // put your main code here, to run repeatedly:
   int i=0;
  if(i==0)
  {
      int sensorValue= analogRead(A0);
    float voltage = sensorValue * (5.0 / 1023.0);
    Serial.println(voltage);
    if (voltage > 3.00)
    {
        Serial.println("Beyond Threshold Value");
       //SendMessage();
        i++;
    }
      else
    {
      Serial.println("Normal");
    }
  
  }
  else{ }
 
}
 void SendMessage()
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+918412089776\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("Sensor value above threshold");// The SMS text you want to send
  delay(100);
   mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  
}

