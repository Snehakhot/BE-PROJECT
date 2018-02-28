
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(9,10);/* (Rx, Tx) */
SoftwareSerial GPS_SoftSerial(4, 3);/* (Rx, Tx) */
// Create an object named gps of the class TinyGPSPlus
TinyGPSPlus gps;      

volatile float minutes, seconds;
volatile int degree, secs, mins;


int x,k,l,h,c=0,b,f=0;
static char st[150],P[10];
static char lati[22 ];

void SendMessage();
void RecieveMessage();
void SendMsgtoClient();
void extractLOC();
void Get_LOC();
static void smartDelay(unsigned long ms);



void setup()
{
    Serial.println("-DEMO 1-");
    Serial.println("-FLAG 1-");
    mySerial.begin(9600);   // Setting the baud rate of GSM Module  
    
     Serial.println("-DEMO 2-");
    Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
    delay(1000);
    mySerial.println("AT+CNMI=2,2,0,0,0"); // AT Command to receive a live SMS
     Serial.println("-DEMO 3-");
    Serial.println("-FLAG 1END-");
    
}

void loop()
{
   
    Serial.println("-FLAG 2-");
    delay(2000);
    if(mySerial.available()>0)
    {
      
        x=0;
        l=0;
        while(l<=15000)
        {
            if(mySerial.available()>0)
            { 
                st[x]=mySerial.read();
   
                x++;
                   
            }
          
            l++;
            

           
            
        }
      
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
            //SendMsgtoClient();   
          
        }
    }
   
    
}

void get()
{
    int c,temp;
    c=strcmp(P,"*loc");
    if(c==0)
    {  
        Serial.println("-FLAG 4-");
        //SendMessage();
        mySerial.end();
        GPS_SoftSerial.begin(9600); // Define baud rate for software serial communication
        Get_LOC();
        GPS_SoftSerial.end();
        mySerial.begin(9600);
        
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
    int x=51;
    while(x<72)
    {
        lati[x-51]=st[x];
        x++;
    }
   
    lati[22]='\0';
   
    
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

void Get_LOC()
{
        Serial.println("-FLAG 9-");
        smartDelay(1000); /* Generate precise delay of 1ms */
        unsigned long start;
        double lat_val, lng_val, alt_m_val;
        bool loc_valid, alt_valid, time_valid;
        lat_val = gps.location.lat(); /* Get latitude data */
        loc_valid = gps.location.isValid(); /* Check if valid location data is available */
        lng_val = gps.location.lng(); /* Get longtitude data */
        alt_m_val = gps.altitude.meters();  /* Get altitude data in meters */
        alt_valid = gps.altitude.isValid(); /* Check if valid altitude data is available */
        
        if (!loc_valid)
        {          
          Serial.print("Latitude : ");
          Serial.println("*****");
          Serial.print("Longitude : ");
          Serial.println("*****");
        }
        else if(loc_valid)
        {
          Serial.print("Latitude in Decimal Degrees : ");
          Serial.println(lat_val, 6);
          Serial.print("Longitude in Decimal Degrees : ");
          Serial.println(lng_val, 6);
        }
        
        if (!alt_valid)
        {
          Serial.print("Altitude : ");
          Serial.println("*****");
        }
        else
        {
          Serial.print("Altitude : ");
          Serial.println(alt_m_val, 6);    
        }
        
        Serial.println("-FLAG 9END-"); 
        
        

}
static void smartDelay(unsigned long ms)
{
  Serial.println("-FLAG 10-"); 
  unsigned long start = millis();
  do 
  {
    while (GPS_SoftSerial.available())  /* Encode data read from GPS while data is available on serial port */
      gps.encode(GPS_SoftSerial.read());
/* Encode basically is used to parse the string received by the GPS and to store it in a buffer so that information can be extracted from it */
  } while (millis() - start < ms);
  Serial.println("-FLAG 10END-"); 
}


