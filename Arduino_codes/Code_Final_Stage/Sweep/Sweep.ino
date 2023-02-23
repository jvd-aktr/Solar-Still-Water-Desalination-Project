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

int pos = 0;    // variable to store the servo position

void setup() {
  myservo.attach(4,558,2475);  // attaches the servo on pin 9 to the servo object
}

void loop() {

  myservo.write(0);              // tell servo to go to position in variable 'pos'
  delay(5000);  
 // while(1);
 myservo.write(50/1.5);              // tell servo to go to position in variable 'pos'
  delay(5000); 
  
  myservo.write(90/1.5);              // tell servo to go to position in variable 'pos'
  delay(5000);  

  myservo.write(120/1.5);              // tell servo to go to position in variable 'pos'
  delay(5000); 
  
  myservo.write(135/1.5);              // tell servo to go to position in variable 'pos'
  delay(5000); 
//    
  
  myservo.write(180/1.5);              // tell servo to go to position in variable 'pos'
  delay(5000);  // waits 15ms for the servo to reach the position
  // waits 15ms for the servo to reach the position

}
