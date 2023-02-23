#include <Servo.h>

int ta=1000;
int tsn=20000;
int alpha1=0;
int alpha2=20;

unsigned long myTime;
unsigned long lastseen;

Servo myservo;

void setup() 
{
    Serial.begin(9600);
    Serial.print("myTime=");
    Serial.print(myTime);
    Serial.print("\t");
    Serial.print("ta=");
    Serial.print(ta);
    Serial.print("\t");
    Serial.print("tsn");
    Serial.print(tsn);
    Serial.print("\n");

    Serial.print("alpha1=");
    Serial.print(alpha1);
    Serial.print("\t");
    Serial.print("alpha2=");
    Serial.print(alpha2);
    Serial.print("\t");

    myservo.write(6);
    

}

void loop() 
{
  myTime= millis();
    if(myTime < tsn && (myTime-lastseen) > ta)
       {
        alpha1++;
        myservo.write(alpha1);
        lastseen = myTime;
       }
     if(myTime > tsn && (myTime-lastseen) > ta)
       {
        alpha2--;
        myservo.write(alpha2);
        lastseen = myTime;
       }

    Serial.print(">>");
    Serial.print(myTime);
    Serial.print("\t");
    Serial.print(ta);
    Serial.print("\t");
    Serial.println(tsn);

    Serial.print("\t");
    Serial.print(alpha1);
    Serial.print("\t");
    Serial.println(alpha2);

delay(1000);
}
