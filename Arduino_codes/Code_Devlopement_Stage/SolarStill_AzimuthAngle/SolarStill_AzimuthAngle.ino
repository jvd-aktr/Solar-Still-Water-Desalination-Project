/* Code for Azimuth Angle */
#include<Servo.h>

int SRh=6;  //sun rise time in hours
int SRm=15; //sun rise time in minutes
int SSh=18; //sun set time in hours
int SSm=24; //sun set time in minutes
int PTh=9;  //present time in hours
int PTm=45; //present time in minutes
int c= 35;  //Azimuth angle

unsigned long tt= ((SSh - SRh)*60 +(SSm - SRm))*60000/(180-2*c); //time for azimuth
unsigned long T= ((23+SRh-SSh)*60 +(60+SRm-SSm))*60000;   //time for repositioning of mirrors

unsigned char b= c + ((PTh-SRh)*60 +(PTm-SRm))*60000/tt; //initial angle of azimuth when you start
Servo myservo1;  // create servo object to control a servo for azimuth

// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() 
{
    Serial.begin(9600);
    Serial.print("tt=");
    Serial.print(tt);
    Serial.print("\t");
    Serial.print("T=");
    Serial.print(T);
    Serial.print("\t"); 
  
  myservo1.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop() 
{
  for (pos = b; pos <= (180 - c); pos += 1) // goes from 0 degrees to 180 degrees
    {                                      // in steps of 1 degree
      myservo1.write(pos);              // tell servo to go to position in variable 'pos'
      Serial.print("b=");
      Serial.print(pos);
      Serial.print("\n");
      delay(tt);                       // waits 'tt' ms for the servo to reach the position=
    }
    for (pos = 180; pos >= b; pos -= 1)  // goes from 180 degrees to b degrees
     { 
      myservo1.write(pos);              // tell servo to go to position in variable 'pos'
      delay(100);
     }
    
    delay(T);                       // waits 15ms for the servo to reach the position
}
