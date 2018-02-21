

#include <SoftwareSerial.h>
#include <TinyGPS++.h>
SoftwareSerial mySerial(9,10);/* (Rx, Tx) */
SoftwareSerial GPS_SoftSerial(4, 3);
TinyGPSPlus gps;

int x,k,l,h,c=0,b,f=0;
static char st[150],P[10];
static char lati[22 ];
volatile float minutes, seconds;
volatile int degree, secs, mins;

void SendMessage();
void RecieveMessage();
void SendMsgtoClient();
void extractLOC();
void Serial_flush();
void Get_Loc_GPS();

void setup()
{
    Serial.println("-FLAG 1-");
    mySerial.begin(9600);   // Setting the baud rate of GSM Module  
    GPS_SoftSerial.begin(9600); /* Define baud rate for software serial communication */
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

void Get_Loc_GPS()
{

        smartDelay(1000);  /* Generate precise delay of 1ms */
        unsigned long start;
        double lat_val, lng_val, alt_m_val;
        uint8_t hr_val, min_val, sec_val;
        bool loc_valid, alt_valid, time_valid;
        lat_val = gps.location.lat(); /* Get latitude data */
        loc_valid = gps.location.isValid(); /* Check if valid location data is available */
        lng_val = gps.location.lng(); /* Get longtitude data */
        alt_m_val = gps.altitude.meters();  /* Get altitude data in meters */
        alt_valid = gps.altitude.isValid(); /* Check if valid altitude data is available */
        hr_val = gps.time.hour(); /* Get hour */
        min_val = gps.time.minute();  /* Get minutes */
        sec_val = gps.time.second();  /* Get seconds */
        time_valid = gps.time.isValid();  /* Check if valid time data is available */
        if (!loc_valid)
        {          
          Serial.print("Latitude : ");
          Serial.println("*****");
          Serial.print("Longitude : ");
          Serial.println("*****");
        }
        else
        {
          DegMinSec(lat_val);
          Serial.print("Latitude in Decimal Degrees : ");
          Serial.println(lat_val, 6);
          Serial.print("Latitude in Degrees Minutes Seconds : ");
          Serial.print(degree);
          Serial.print("\t");
          Serial.print(mins);
          Serial.print("\t");
          Serial.println(secs);
          DegMinSec(lng_val); /* Convert the decimal degree value into degrees minutes seconds form */
          Serial.print("Longitude in Decimal Degrees : ");
          Serial.println(lng_val, 6);
          Serial.print("Longitude in Degrees Minutes Seconds : ");
          Serial.print(degree);
          Serial.print("\t");
          Serial.print(mins);
          Serial.print("\t");
          Serial.println(secs);
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
        if (!time_valid)
        {
          Serial.print("Time : ");
          Serial.println("*****");
        }
        else
        {
          char time_string[32];
          sprintf(time_string, "Time : %02d/%02d/%02d \n", hr_val, min_val, sec_val);
          Serial.print(time_string);    
        }
  
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (GPS_SoftSerial.available())  /* Encode data read from GPS while data is available on serial port */
      gps.encode(GPS_SoftSerial.read());
/* Encode basically is used to parse the string received by the GPS and to store it in a buffer so that information can be extracted from it */
  } while (millis() - start < ms);
}

void DegMinSec( double tot_val)   /* Convert data in decimal degrees into degrees minutes seconds form */
{  
  degree = (int)tot_val;
  minutes = tot_val - degree;
  seconds = 60 * minutes;
  minutes = (int)seconds;
  mins = (int)minutes;
  seconds = seconds - minutes;
  seconds = 60 * seconds;
  secs = (int)seconds;
}


