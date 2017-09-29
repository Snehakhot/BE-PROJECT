
#include <SoftwareSerial.h>
SoftwareSerial mySerial(9,10);

void get();
int x,k,l,h,b;
char a[5]="*loc";
char st[100],P[10];

void setup()
{
  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  delay(1000);                      
  x=0;
  mySerial.println("AT");
  delay(2000);
  mySerial.println("AT+CMGF=1");
  delay(2000);
  mySerial.println("AT+CNMI=2,2,0,0,0");
  delay(2000);
}
  
void loop()
{  
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
        x++;
      }
      l=millis();
    }
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
 
void get()
{
  int c;
  c=strcmp(P,"*loc");
  if (c==0)
  {
    Serial.println("ok");
    SendMessage();  
  }   
}

void SendMessage()
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+918412089776\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("Location Response");// The SMS text you want to send
  delay(100);
  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}
 

