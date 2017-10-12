#include <SoftwareSerial.h>
SoftwareSerial mySerial(9, 10);

void setup()
{
  mySerial.begin(9600);   // Setting the baud rate of GSM Module
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  delay(100);
}

void loop()
{
  char temp0;
  char temp1[10];
  char data[10];
  int j;
  byte discard;
  int a;
  if (Serial.available() > 0)
    switch (Serial.read())
    {
      case 's':
        SendMessage();
        break;
      case 'r':
        RecieveMessage();
        break;
    }
  if (mySerial.available() > 0)
  {
    for (j = 0; j < 46; j++)
    {
      discard = Serial.read();
    }
    for (j = 0; j < 11; j++)
    {
      temp0 = mySerial.read();
      temp1[j] = temp0;
      Serial.write(temp1);
    }
    data[10] = temp1[10];
   //Serial.write(temp1);
  }
}

void SendMessage()
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+918412089776\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("TEST MSG");// The SMS text you want to send
  delay(100);
  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}


void RecieveMessage()
{

  mySerial.println("AT+CNMI=2,2,0,0,0"); // AT Command to receive a live SMS
  delay(1000);

}




