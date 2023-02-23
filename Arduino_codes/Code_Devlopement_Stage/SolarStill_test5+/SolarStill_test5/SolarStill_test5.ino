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
 * FOR THIS CODE, THE EXPERIMENT SHOULD BE STARTED BEFORE SOLAR NOON
 
  */

            /*May 3 2021*/
//------you need to enter these from link (1)--------    
  int SRh=6;  //sun rise time in hours
  int SRm=18; //sun rise time in minutes
  int SSh=19; //sun set time in hours
  int SSm=49; //sun set time in minutes
  int PTh=13;  //present time in hours
  int PTm=2; //present time in minutes
  int SNh=13; //solar noon in hours
  int SNm=3; //solar noon in minutes
  double SNa_d=73.63; //solar noon angle (in degree)
  double alpha_0_d=-0.56; //altutude/elevation angle (in degree) at sun rise
  double alpha_L_d=5.24; //altutude/elevation angle (in degree) at sun set
  double beta_0_d=70.56; //azimuth angle (in degree) at sun rise
  double beta_L_d=289.61; //azimuth angle (in degree) at sun set
  double h=23.625; //height(in inch) of convex mirror focal point from the origin(centre point of the base on which these mirror units are intalled)
//----------------------------------------------------

//--------------Calculation in degree-----------------
double t_alt_1_d= ((SNh - SRh)*60 +(SNm - SRm))*60000/(SNa_d-alpha_0_d); //time for altitude (in ms) before solar noon for degree
double t_alt_2_d= ((SSh - SNh)*60 +(SSm - SNm))*60000/(SNa_d-alpha_L_d); //time for altitude (in ms) after solar noon for degree
double t_azi_d= ((SSh - SRh)*60 +(SSm - SRm))*60000/(beta_L_d-beta_0_d); //time for azimuth (in ms) for degree
double t_sn= ((SNh-PTh)*60 +(SNm-PTm))*60000; //time difference between solar noon and present time (in ms)
double alpha_1_d= alpha_0_d +((PTh-SRh)*60 +(PTm-SRm))*60000/t_alt_1_d; //initial angle of altitude when you start the exp.
double alpha_2_d= SNa_d -((PTh-SNh)*60 +(PTm-SNm))*60000/t_alt_2_d; //initial angle of altitude when you start the exp after solar noon
double beta_d= beta_0_d +((PTh-SRh)*60 +(PTm-SRm))*60000/t_azi_d; //initial angle of azimuth when you start the exp.

//----------------------------------------------------

//-----------Calculation for arduino------------------
       /*544 corespond to 0 degree angle/ 2394 correspond to 180 degree angle*/
  double C= (2394-544)/180; //conversion of degree in arduino angle parameter
  double t_alt_1= t_alt_1_d/C; //time for altitude (in ms) before solar noon for arduino
  double t_alt_2= t_alt_2_d/C; //time for altitude (in ms) after solar noon for arduino
  double t_azi= t_azi_d/C; //time for azimuth (in ms)for arduino
  int alpha_1= 544 +alpha_0_d*C +((PTh-SRh)*60 +(PTm-SRm))*60000/t_alt_1; //initial angle of altitude when you start the exp.
  int alpha_2= 544 +SNa_d*C -((PTh-SNh)*60 +(PTm-SNm))*60000/t_alt_2; //initial angle of altitude when you start the exp after solar noon
  int beta= 544 +beta_0_d*C +((PTh-SRh)*60 +(PTm-SRm))*60000/t_azi; //initial angle of azimuth when you start the exp.
  


//----------------------------------------------------

//-----------Calculation for arduino------------------
       /*544 corespond to 0 degree angle/ 2394 correspond to 180 degree angle*/
  /*int SNa= 544 +SNa_d*((2394-544)/180); //solar noon angle (for arduino)
  int alpha_0= 544 +alpha_0_d*((2394-544)/180); //altutude/elevation angle (for arduino) at sun rise
  int alpha_L= 544 +alpha_L_d*((2394-544)/180); //altutude/elevation angle (for arduino) at sun set
  int beta_0= 544 +beta_0_d*((2394-544)/180); //azimuth angle (for arduino) at sun rise
  int beta_L= 544 +beta_L_d*((2394-544)/180); //azimuth angle (for arduino) at sun set
  unsigned long t_alt_11= ((SNh - SRh)*60 +(SNm - SRm))*60000/(SNa-alpha_0); //time for altitude (in ms) before solar noon
  //unsigned long t_alt_1=10;
  unsigned long t_alt_2= ((SSh - SNh)*60 +(SSm - SNm))*60000/(SNa-alpha_L); //time for altitude (in ms) after solar noon
  //unsigned long t_alt_2=10;
  unsigned long t_azi= ((SSh - SRh)*60 +(SSm - SRm))*60000/(beta_L-beta_0); //time for azimuth (in ms)
  //unsigned long t_azi=10;
  unsigned long t_sn= ((SNh-PTh)*60 +(SNm-PTm))*60000; //time difference between solar noon and present time (in ms)
  //unsigned long t_sn= 10000;
  unsigned long alpha_1= alpha_0 +((PTh-SRh)*60 +(PTm-SRm))*60000/t_alt_1; //initial angle of altitude when you start the exp.
  //int alpha=544;
  unsigned long alpha_2= SNa -((PTh-SNh)*60 +(PTm-SNm))*60000/t_alt_2; //initial angle of altitude when you start the exp after solar noon
  unsigned long beta= beta_0 +((PTh-SRh)*60 +(PTm-SRm))*60000/t_azi; //initial angle of azimuth when you start the exp.
  //int beta=1024;*/
//-------------------------------------------------------------

//------to intiate the void loop-----------
  unsigned long last_seen_alt; //for altitude angle
  unsigned long last_seen_azi; //for azimuthal angle
  unsigned long myTime;
//-----------------------------------------

//---------UNIT#3-------------
  double x3= 18.75; //x position (in inch) of unit#3
  int y3=0; //y position (in inch) of unit#3
  double d3=sqrt(x3*x3+y3*y3); //distance (in inch) of unit#3 from origin(centre point of the base on which these mirror units are intalled)
  double theta3_d= (180/3.1415926)*atan(h/d3); //angle in degree
  int theta3= 544 +theta3_d*(C); //angle for arduino
  int a3_1= 544 +90*(C) -((theta3-alpha_1)/2); //intial angle for altitude adjustment (for arduino)
  int a3_2= 544 +90*(C) -((theta3-alpha_2)/2); //intial angle for altitude adjustment (for arduino)
  int b3= 544 +45*(C) +(beta-544)/2; //intial angle for azimuthal adjustment (for arduino)

  Servo myservo3_alt; //create servo object to control a servo for altitude adjustment
  Servo myservo3_azi; //create servo object to control a servo for azimuthal adjustment
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
    Serial.print("t_alt_2=");
    Serial.print(t_alt_2);
    Serial.print("\t");
    Serial.print("t_azi_d=");
    Serial.print(t_azi_d);
    Serial.print("\n");

    
    Serial.print("a3_1=");
    Serial.print(a3_1);
    Serial.print("\t");
    Serial.print("alpha_1=");
    Serial.print(alpha_1);
    Serial.print("\n");
    Serial.print("a3_2=");
    Serial.print(a3_2);
    Serial.print("\t");
    Serial.print("alpha_2=");
    Serial.print(alpha_2);
    Serial.print("\n");
    Serial.print("b3=");
    Serial.print(b3);
    Serial.print("\t");
    Serial.print("beta=");
    Serial.print(beta);
   
//---------------UNIT#3--------------------
  myservo3_alt.attach(9); //attaches the 'altitude adjustment servo' of unit#3 on pin 9 to the servo object
  myservo3_azi.attach(8); //attaches the 'azimuthal adjustment servo' of unit#3 on pin 9 to the servo object
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
        alpha_1++;
        a3_1= 544 +90*((2394-544)/180) -((theta3-alpha_1)/2); //intial angle for altitude adjustment (for arduino)
        myservo3_alt.write(a3_1);
        last_seen_alt = myTime;  
       }
      if (myTime > t_sn && (myTime-last_seen_alt) > t_alt_2)
         { alpha_2--;
           a3_2= 544 +90*((2394-544)/180) -((theta3-alpha_2)/2); //intial angle for altitude adjustment (for arduino)
           myservo3_alt.write(a3_2);
           last_seen_alt = myTime;
         }
      if( (myTime-last_seen_azi) > t_azi)
      {
        beta++;
        b3= 45*((2394-544)/180) +(beta/2); //intial angle for azimuthal adjustment (for arduino)
        myservo3_azi.write(b3);
        last_seen_azi = myTime;
      }
//-----------------------------------------

//---------------UNIT#4--------------------

//-----------------------------------------


    Serial.print(myTime);
    Serial.print("\t");
    Serial.print(t_alt_1);
    Serial.print("\t");
    Serial.print(t_alt_2);
    Serial.print("\t");
    Serial.println(t_azi_d);
    Serial.print("\t");

    Serial.print(a3_1);
    Serial.print("\t");
    Serial.println(alpha_1);
    Serial.print("\t");
    Serial.print(a3_2);
    Serial.print("\t");
    Serial.println(alpha_2);
    Serial.print("\t");
    Serial.print(b3);
    Serial.print("\t");
    Serial.println(beta);
    Serial.print("\t");
    
    delay(2000);
  }
