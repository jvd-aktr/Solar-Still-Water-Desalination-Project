#include <Servo.h>
#include <math.h>

/*
 * Instruction: enter the SR, SS, SN time and present time(2 min ahead of starting) then at exact present time click the reset button
 * Link for solar information:(1) https://www.esrl.noaa.gov/gmd/grad/solcalc/ ****use this one
                              (2) https://www.esrl.noaa.gov/gmd/grad/solcalc/azel.html
                              (3) https://www.timeanddate.com/sun/usa/las-cruces
 * Las Cruces Coorinate: 32.3199° N, 106.7637° W
 * Azimuth and Elevation - an angular coordinate system for locating positions in the sky. Azimuth is measured clockwise from true north 
   to the point on the horizon directly below the object. Elevation is measured vertically from that point on the horizon up to the object.
 * for our setup and code, azimuthal angle (beta) should be greater than 90 degree when you start the exp.
 
  */

//made mistake converting from degree to arduino angle.. check it.
    /*May 3 2021*/
//------you need to enter these from link (1)--------    
  int SRh=6;  //sun rise time in hours
  int SRm=18; //sun rise time in minutes
  int SSh=19; //sun set time in hours
  int SSm=49; //sun set time in minutes
  int PTh=9;  //present time in hours
  int PTm=45; //present time in minutes
  int SNh=13; //solar noon in hours
  int SNm=3; //solar noon in minutes
  double SNa_d=73.63; //solar noon angle (in degree)
  double alpha_0_d=-0.56; //altutude/elevation angle (in degree) at sun rise
  double alpha_L_d=5.24; //altutude/elevation angle (in degree) at sun set
  double beta_0_d=70.56; //azimuth angle (in degree) at sun rise
  double beta_L_d=289.61; //azimuth angle (in degree) at sun set
  double h=23.625; //height(in inch) of convex mirror focal point from the origin(centre point of the base on which these mirror units are intalled)
//----------------------------------------------------

//-----------calculation for useful parameters----------------
       /*544 corespond to 0 degree angle/ 2394 correspond to 180 degree angle*/
  int SNa= 544 +SNa_d*((2394-544)/180); //solar noon angle (for arduino)
  int alpha_0= 544 +alpha_0_d*((2394-544)/180); //altutude/elevation angle (for arduino) at sun rise
  int alpha_L= 544 +alpha_L_d*((2394-544)/180); //altutude/elevation angle (for arduino) at sun set
  int beta_0= 544 +beta_0_d*((2394-544)/180); //azimuth angle (for arduino) at sun rise
  int beta_L= 544 +beta_L_d*((2394-544)/180); //azimuth angle (for arduino) at sun set
  unsigned long t_alt_1= ((SSh - SRh)*60 +(SSm - SRm))*60000/(SNa-alpha_0); //time for altitude (in ms) before solar noon
  //unsigned long t_alt_1=10;
  unsigned long t_alt_2= ((SSh - SRh)*60 +(SSm - SRm))*60000/(SNa-alpha_L); //time for altitude (in ms) after solar noon
  //unsigned long t_alt_2=10;
  unsigned long t_azi= ((SSh - SRh)*60 +(SSm - SRm))*60000/(beta_L-beta_0); //time for azimuth (in ms)
  //unsigned long t_azi=10;
  unsigned long t_sn= ((SNh-PTh)*60 +(SNm-PTm))*60000; //time difference between solar noon and present time (in ms)
  //unsigned long t_sn= 10000;
  unsigned long alpha= alpha_0 +((PTh-SRh)*60 +(PTm-SRm))*60000/t_alt_1; //initial angle of altitude when you start the exp.
  //int alpha=544;
  unsigned long beta= beta_0 +((PTh-SRh)*60 +(PTm-SRm))*60000/t_azi; //initial angle of azimuth when you start the exp.
  //int beta=1024;
//-------------------------------------------------------------

//------to intiate the void loop-----------
  unsigned long last_seen_alt; //for altitude angle
  unsigned long last_seen_azi; //for azimuthal angle
  unsigned long myTime;
//-----------------------------------------

//-----------UNIT#3------- ------
  double x3= 18.75; //x position (in inch) of unit#3
  int y3=0; //y position (in inch) of unit#3
  double d3=sqrt(x3*x3+y3*y3); //distance (in inch) of unit#3 from origin(centre point of the base on which these mirror units are intalled)
  double theta3_d= atan(h/d3); //angle in degree
  int theta3= 544 +theta3_d*((2394-544)/180); //angle for arduino
  int a3= 544 +(90*((2394-544)/180) -((theta3-alpha)/2)); //intial angle for altitude adjustment (for arduino)
  int b3= 544 +45*((2394-544)/180) +(beta/2); //intial angle for azimuthal adjustment (for arduino)

  Servo myservo3_alt; //create servo object to control a servo for altitude adjustment
  Servo myservo3_azi; //create servo object to control a servo for azimuthal adjustment
//-------------------------------

//------------UNIT#4-------------
  double x4= 18.75; //x position (in inch) of unit#3
  int y4=0; //y position (in inch) of unit#3
  double d4=sqrt(x4*x4+y4*y4); //distance (in inch) of unit#3 from origin(centre point of the base on which these mirror units are intalled)
  double theta4_d= atan(h/d4); //angle in degree
  int theta4= 544 +theta4_d*((2394-544)/180); //angle for arduino
  int a4= (alpha+theta4)/2; //intial angle for altitude adjustment (for arduino)
  int b4= 544 +(beta-(544+90*((2394-544)/180)))/2; //intial angle for azimuthal adjustment (for arduino)

  Servo myservo4_alt; //create servo object to control a servo for altitude adjustment
  Servo myservo4_azi; //create servo object to control a servo for azimuthal adjustment
//-------------------------------

// twelve servo objects can be created on most boards


void setup() 
{
    Serial.begin(9600);
    Serial.print("myTime=");
    Serial.print(myTime);
    Serial.print("\t");
    Serial.print("t_alt_1=");
    Serial.print(t_alt_1);
    Serial.print("\t");
    Serial.print("t_azi=");
    Serial.print(t_azi);
    Serial.print("\n");

    
    Serial.print("a3=");
    Serial.print(a3);
    Serial.print("\t");
    Serial.print("alpha=");
    Serial.print(alpha);
    Serial.print("\t");
    Serial.print("b3=");
    Serial.print(b3);
    Serial.print("\t");
    Serial.print("beta=");
    Serial.println(beta);

    Serial.print("a4=");
    Serial.print(a4);
    Serial.print("\t");
    Serial.print("alpha=");
    Serial.print(alpha);
    Serial.print("\t");
    Serial.print("b4=");
    Serial.print(b4);
    Serial.print("\t");
    Serial.print("beta=");
    Serial.println(beta);
   
//---------------UNIT#3--------------------
  myservo3_alt.attach(5); //attaches the 'altitude adjustment servo' of unit#3 on pin 5 to the servo object
  myservo3_azi.attach(6); //attaches the 'azimuthal adjustment servo' of unit#3 on pin 6 to the servo object
//-----------------------------------------    

//---------------UNIT#4--------------------
  myservo4_alt.attach(7); //attaches the 'altitude adjustment servo' of unit#3 on pin 7 to the servo object
  myservo4_azi.attach(8); //attaches the 'azimuthal adjustment servo' of unit#3 on pin 8 to the servo object
//-----------------------------------------    



  
}

void loop() 
  {
    myTime= millis();

//---------------UNIT#1--------------------


//-----------------------------------------

//---------------UNIT#2--------------------


//-----------------------------------------

//---------------UNIT#3--------------------
      if(myTime < t_sn && (myTime-last_seen_alt) > t_alt_1)
         {
          alpha++;
          a3= 90*((2394-544)/180) -((theta3-alpha)/2); //intial angle for altitude adjustment (for arduino)
          a4= (alpha+theta4)/2; //intial angle for altitude adjustment (for arduino)
          myservo3_alt.write(a3);
          myservo4_alt.write(a4);
          last_seen_alt = myTime;  
         }
      if (myTime > t_sn && (myTime-last_seen_alt) > t_alt_2)
         {
          alpha--;
          a3= 90*((2394-544)/180) -((theta3-alpha)/2); //intial angle for altitude adjustment (for arduino)
          a4= (alpha+theta4)/2; //intial angle for altitude adjustment (for arduino)
          myservo3_alt.write(a3);
          myservo4_alt.write(a4);
          last_seen_alt = myTime;
         }
      if( (myTime-last_seen_azi) > t_azi)
        {
         beta++;
         b3= 45*((2394-544)/180) +(beta/2); //intial angle for azimuthal adjustment (for arduino)
         b4= (beta-90*((2394-544)/180))/2; //intial angle for azimuthal adjustment (for arduino)
         myservo3_azi.write(b3);
         myservo4_azi.write(b4);
         last_seen_azi = myTime;
        }
//-----------------------------------------
/*
//---------------UNIT#4--------------------
if(myTime < t_sn && (myTime-last_seen_alt) > t_alt_1)
         {
          alpha++;
          a4= (alpha+theta4)/2; //intial angle for altitude adjustment (for arduino)
          myservo4_alt.write(a4);
          last_seen_alt = myTime;  
         }
      if (myTime > t_sn && (myTime-last_seen_alt) > t_alt_2)
         {
          alpha--;
          a4= (alpha+theta4)/2; //intial angle for altitude adjustment (for arduino)
          myservo4_alt.write(a4);
          last_seen_alt = myTime;
         }
      if( (myTime-last_seen_azi) > t_azi)
        {
         beta++;
         b4= (beta-90*((2394-544)/180)/2); //intial angle for azimuthal adjustment (for arduino)
         myservo4_azi.write(b4);
         last_seen_azi = myTime;
        }
//-----------------------------------------
*/

    Serial.print(myTime);
    Serial.print("\t");
    Serial.print(t_alt_1);
    Serial.print("\t");
    Serial.println(t_azi);
    

    Serial.print(a3);
    Serial.print("\t");
    Serial.print(alpha);
    Serial.print("\t");
    Serial.print(b3);
    Serial.print("\t");
    Serial.println(beta);

    Serial.print(a4);
    Serial.print("\t");
    Serial.print(alpha);
    Serial.print("\t");
    Serial.print(b4);
    Serial.print("\t");
    Serial.println(beta);
    Serial.print("\n");
    
    delay(2000);
  }
