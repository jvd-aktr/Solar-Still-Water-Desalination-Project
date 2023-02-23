#include<math.h>
double x= -3.147326;
double y= 5.12;
double b1=0;
double b2=0;

double x_in[10]={-13.5235,13.5,0,0,13.5,-13.5,18.75,18.75,28.3125,28.3125};


void setup() {
  b1= (180/3.14)*atan2((y),(x));
  b2= (180/3.14)*atan((y)/(x));
    Serial.begin(9600);
    Serial.print("b1=");
    Serial.print(b1);
    Serial.print("\n");
    Serial.print("b2=");
    Serial.print(b2);
    Serial.print("\n");
    Serial.print("x_in=");
    Serial.print(x,6);
    Serial.print("\n");
}

void loop() {
  // put your main code here, to run repeatedly:

}
