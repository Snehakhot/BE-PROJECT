

#include <SoftwareSerial.h>
SoftwareSerial mySerial(9,10);/* (Rx, Tx) */

int x,k,l,h,c=0,b;
static char st[100],P[10];
char coordi[70];
char lati[22];

void SendMessage();
void RecieveMessage();
void SendMsgtoClient();
void extractLOC();

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
   
    Serial.println("-FLAG 2-");
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
                Serial.print(st[x]);
                x++;
            }
            l=millis();
        }

        Serial.println(st[46]);
        Serial.println(st[47]);
        Serial.println(st[48]); 
        Serial.println(st[49]);
        Serial.println(st[50]);
        Serial.println(st[51]);
        Serial.println(st[52]);
        Serial.println(st[53]);
        
    
       
       if(st[50]=='*')
        {
            Serial.write("-FLAG 3-");
            int f=0,i=0,y;
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
            Serial.println("");
            Serial.println(P[i]);
            Serial.println("");
            get();        
         }
        // else
        {
            Serial.write("-FLAG 6-");
            //extractLOC();
            //SendMsgtoClient();
        }
    }
    if(mySerial.available()>0) 
       Serial.write(mySerial.read());
}

void get()
{
    int c;
    c=strcmp(P,"*loc");
    if(c==0)
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
    Serial.write("-Ext_FLAG-");
    int x=0;
    while(x<71)
    {
        lati[x-50]=st[x];
        x++;
    }
    lati[21]='\0';
    Serial.write(lati);
    Serial.write("-FLAG 7-");
 
}   

void SendMsgtoClient()
{
      Serial.write("-Cli_FLAG-");
      mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
      delay(1000);  // Delay of 1000 milli seconds or 1 second
      mySerial.println("AT+CMGS=\"+917058391851\"\r"); // Replace x with mobile number
      delay(1000);
      mySerial.println(lati);// The SMS text you want to send
      delay(100);
      mySerial.println((char)26);// ASCII code of CTRL+Z
      delay(1000);
      if (mySerial.available()>0)
        Serial.write(mySerial.read());
      Serial.write("-FLAG 8-"); 
      
}



