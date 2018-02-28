
#include <SoftwareSerial.h>
SoftwareSerial mySerial(9,10);/* (Rx, Tx) */

int x,k,l,h,c=0,b,f=0;
static char st[150],P[10];
static char lati[22 ];

void SendMessage();
void RecieveMessage();
void SendMsgtoClient();
void extractLOC();
void Serial_flush();

void setup()
{
    Serial.println("-FLAG 1-");
    mySerial.begin(9600);   // Setting the baud rate of GSM Module  
    Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
    delay(1000);
   // x=0;
    //mySerial.println("AT");
    //delay(2000);
    //mySerial.println("AT+CMGF=1");
    //delay(2000);
    mySerial.println("AT+CNMI=2,2,0,0,0"); // AT Command to receive a live SMS
    //delay(2000);
    //Serial.println("-FLAG 1END-");
    
}

void loop()
{
   
    Serial.println("-FLAG 2-");
    delay(2000);
    if(mySerial.available()>0)
    {
        //Serial.write(mySerial.read());
        //l=millis();
        //k=millis();    
        x=0;
        l=0;
        while(l<=15000)
        {
            if(mySerial.available()>0)
            { 
                st[x]=mySerial.read();
   
                x++;
                   
            }
           // l=millis();
            l++;
            

           
            
        }

        Serial_flush();
       //if(f==1||f==2) 
       Serial.print(st); 
       
       Serial.println("-FLAG 2END-");
       if(st[51]=='*')
        {
            Serial.println("-FLAG 3-");
            int f=0,i=0,y;
            for(y=0;y<x-1;y++)
            {
                 if(st[y]=='*'||f==1)
                 {
                    P[i]=st[y];
                    //Serial.write(st[y+1]);
                    f=1;
                    i++;
                 }
            }
            P[4]='\0';
            get(); 
            Serial.println("-FLAG 3END-");       
         }
        else if(st[14]=='1' && st[51]!='*')
        {
            Serial.println("-FLAG 6-");
            extractLOC();
            SendMsgtoClient();   
            
            Serial.println("--ALERT--");
            Serial.println(lati);
            Serial.println("--COORDIATES OBTAINED--");
            Serial.println("-FLAG 6END-");
            f=2;
        }
    }
   // if(mySerial.available()>0) 
       //Serial.write(mySerial.read());
    
}

void get()
{
    int c,temp;
    c=strcmp(P,"*loc");
    if(c==0)
    {  
        Serial.println("-FLAG 4-");
        //for(temp=0;temp<100;temp++)
       Serial_flush();
        SendMessage();
        Serial.println("-FLAG 4END-");
    }     
}

void SendMessage()
{
    Serial.println("-FLAG 5-");
    mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
    delay(1000);  // Delay of 1000 milli seconds or 1 second
    mySerial.println("AT+CMGS=\"+918412089776\"\r"); // Replace x with mobile number
    delay(1000);
    mySerial.println("*loc");// The SMS text you want to send
    delay(100);
    mySerial.println((char)26);// ASCII code of CTRL+Z
    delay(1000);
    Serial.println("-FLAG 5END-");
}

void extractLOC()
{
    Serial.println("-FLAG 7-");
    //Serial.print(st);
    int x=51;
    while(x<72)
    {
        lati[x-51]=st[x];
        x++;
    }
   
    lati[22]='\0';
    //Serial.println(lati); 
    
    Serial.println("-FLAG 7END-");
 
}   

void SendMsgtoClient()
{
      Serial.println("-FLAG 8-"); 
      mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
      delay(1000);  // Delay of 1000 milli seconds or 1 second
      mySerial.println("AT+CMGS=\"+919881459682\"\r"); // Replace x with mobile number
      delay(1000);
      mySerial.print(lati);// The SMS text you want to send
      delay(100);
      mySerial.println((char)26);// ASCII code of CTRL+Z
      delay(1000);
      Serial.println("-FLAG 8END-"); 
      
}

void Serial_flush()
{
    while(mySerial.available())
      mySerial.read();
}

