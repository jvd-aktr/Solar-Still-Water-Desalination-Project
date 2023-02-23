/* Sweep
  by BARRAGAN <http://barraganstudio.com>
  This example code is in the public domain.

  modified 8 Nov 2013
  by Scott Fitzgerald
  http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;   // variable to store the servo position

void setup() {
  myservo.attach(10, 540, 2410); // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);
}

void loop()
{

  myservo.write(pos);              // tell servo to go to position in variable 'pos'
  delay(5000);
  //while(1);
  //    myservo.write(15);              // tell servo to go to position in variable 'pos'
  //    delay(5000);


//  myservo.write(1);              // tell servo to go to position in variable 'pos'
//  delay(5000);
  myservo.write(58);              // tell servo to go to position in variable 'pos'
  delay(4000);
  Serial.println(myservo.read());
  myservo.write(90);              // tell servo to go to position in variable 'pos'
  delay(4000);
  myservo.write(135);              // tell servo to go to position in variable 'pos'
  delay(4000);
  myservo.write(180);              // tell servo to go to position in variable 'pos'
  delay(4000);
  //    myservo.write(90);              // tell servo to go to position in variable 'pos'
  //    delay(8000);
}
