#include <Servo.h>

int SRh=6;  //sun rise time in hours
int SRm=15; //sun rise time in minutes
int SSh=18; //sun set time in hours
int SSm=24; //sun set time in minutes
int PTh=9;  //present time in hours
int PTm=45; //present time in minutes
int c= 544+35*((2394-544)/180);  //Azimuth angle
unsigned long t= ((SSh - SRh)*60 +(SSm - SRm))*60000/(2394-544); //time for altitude
unsigned long tt= ((SSh - SRh)*60 +(SSm - SRm))*60000/(2394-544-2*c); //time for azimuth
unsigned long T= ((23+SRh-SSh)*60 +(60+SRm-SSm))*60000;   //time for repositioning of mirrors
unsigned char a= 544+((PTh-SRh)*60 +(PTm-SRm))*60000/t; //initial angle of altitude when you start
unsigned char b= c + ((PTh-SRh)*60 +(PTm-SRm))*60000/tt; //initial angle of azimuth when you start

Servo myservo1;  // create servo object to control a servo
Servo myservo2;
// twelve servo objects can be created on most boards

int pos =0;    // variable to store the servo position

unsigned long last_seen1;
unsigned long last_seen2;

unsigned long myTime;



void setup() 
{
  Serial.begin(9600);
    Serial.print("myTime=");
    Serial.print(myTime);
    Serial.print("\t");
    Serial.print("last_seen1=");
    Serial.print(last_seen1);
    Serial.print("\t");
    Serial.print("last_seen2=");
    Serial.print(last_seen2);
    Serial.print("\t");

    Serial.print("a=");
    Serial.print(a);
    Serial.print("\t");
    Serial.print("b=");
    Serial.print(b);
    Serial.print("\t");
  myservo1.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo2.attach(8);


  
}

void loop() 
{
    myTime= millis();

  if( (myTime-last_seen1) > t)
   {
    a++;
    myservo1.write(a);
    last_seen1 = myTime;
   }

  if( (myTime-last_seen2) > tt)
  {
    b++;
    myservo2.write(b);
    last_seen2 = myTime;
  }
/*
Serial.println(myTime);
Serial.println(last_seen1);
Serial.println(last_seen2);

Serial.println(a);
Serial.println(a);*/

}
