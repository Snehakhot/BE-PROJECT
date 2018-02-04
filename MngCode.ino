

#include <SoftwareSerial.h>
SoftwareSerial mySerial(9,10);

int x,k,l,h,c=0,b;
char st[100],P[10];
char coordi[70];
char lati[22];

void SendMessage();
void RecieveMessage();
void SendMsgtoClient();
void extractLOC();
void SndmsgADA();
void SndMsgMFA();

void setup()
{
  
   Serial.write("-FLAG 1-");
   mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  delay(1000);
  x=0;
  mySerial.println("AT");
  delay(2000);
  mySerial.println("AT+CMGF=1");
  delay(2000);
  mySerial.println("AT+CNMI=2,2,0,0,0"); // AT Command to receive a live SMS
  delay(2000);

}

void loop()
{
  Serial.write("-FLAG 2-");
  delay(2000);
 if(mySerial.available()>0)
 {
    Serial.write(mySerial.read());
    l=millis();
    k=millis();    
    x=0;
    while((l-k)<=5000)
    {
      if(mySerial.available()>0)
      { 
        st[x]=mySerial.read();
        Serial.write(st[x]);
        x++;
      }
      l=millis();
    }
    Serial.write(st);
    
    if(st[50]!='*')
    {
     Serial.write("-FLAG 6-");
     extractLOC();
    }
    else
    {
       Serial.write("-FLAG 3-");
       Serial.println(st[50]);
       Serial.write(st[50]);
      int f=0,u=0,i=0,y;
    for(y=0;y<x-1;y++)
    {
      if(st[y]=='*'||f==1)
      {
        P[i]=st[y];
        Serial.write(st[y+1]);
        f=1;
        i++;
      }
    }
    P[4]='\0';
    Serial.println(P[i]);
    get();        
  }
 }
 if(mySerial.available()>0)
   {
    Serial.write(mySerial.read());
    }
   
}

void get()
{
  int a,b,c;
  a=strcmp(P,"*ada");
  b=strcmp(P,"*mfa");
  c=strcmp(P,"*loc");

  if(a==0)
  {  
    Serial.println("-FLAG ADA-");
    SendMessage();
    SndmsgADA();
  }
  else if(b==0)
  {  
    Serial.println("-FLAG MFA-");
    SendMessage();
    SndMsgMFA();
  }
  
  else if(c==0)
  {  
    Serial.println("-FLAG 4-");
    SendMessage();
  }
   
}

void SendMessage()
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+918412089776\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("*loc");// The SMS text you want to send
  delay(100);
  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  Serial.write("-FLAG 5-");
  
}

void extractLOC()
{
  Serial.write("---Special FLAG---");
  int x=0;
  while(x<71)
  {
  lati[x-50]=st[x];
  x++;
  }
  lati[21]='\0';
  Serial.write(lati);
  SendMsgtoClient();
 
}   

void SendMsgtoClient()
{
  Serial.write("-Client FLAG-");
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+919881459682\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println(lati);// The SMS text you want to send
  delay(100);
  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  if (mySerial.available()>0)
  Serial.write(mySerial.read());
  Serial.write("-FLAG 8-"); 
  
}
void SndmsgADA()
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+918412089776\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("Accident of the vehicle has occured at this location ");// The SMS text you want to send
  delay(100);
  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  Serial.write("-FLAG PoilceAlert-");
  
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+918412089776\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("Accident of the vehicle has occured at this location ");// The SMS text you want to send
  delay(100);
  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  Serial.write("-FLAG ClgAlert-");

  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+918412089776\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("Accident of the vehicle has occured at this location ");// The SMS text you want to send
  delay(100);
  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  Serial.write("-FLAG HospitalAlert-");
}

void SndMsgMFA()
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+918412089776\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("Mechanical Failure of the vehicle has occured at this location ");// The SMS text you want to send
  delay(100);
  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  Serial.write("-FLAG MechanicAlert-");
}




