#include <Servo.h>
#include <math.h>

/*
 * Instruction: enter the SR, SS, SN time and present time(2 min ahead of starting) then at exact present time click the reset button
 * Link for solar information:(1) https://www.esrl.noaa.gov/gmd/grad/solcalc/ ****use this one
                              (2) https://www.esrl.noaa.gov/gmd/grad/solcalc/azel.html
                              (3) https://www.timeanddate.com/sun/usa/las-cruces
 * Las Cruces Coorinate: 32.280856° N(+), 106.75491° W (-)
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
  int PTh=9;  //present time in hours
  int PTm=45; //present time in minutes
  int SNh=13; //solar noon in hours
  int SNm=3; //solar noon in minutes
  
  double SNa_d=73.63; //solar noon altitude angle (in degree)
  double SNb_d=180; //solar noon azimuthal angle (in degree)
  double alpha_0_d=-0.56; //altutude/elevation angle (in degree) at sun rise
  double alpha_L_d=-0.52; //altutude/elevation angle (in degree) at sun set
  double beta_0_d=70.56; //azimuth angle (in degree) at sun rise
  double beta_L_d=289.61; //azimuth angle (in degree) at sun set
  double h=23.625; //height(in inch) of convex mirror focal point from the origin(centre point of the base on which these mirror units are intalled)
//----------------------------------------------------

//--------------Calculation in degree-----------------
double t_alt_1_d= ((SNh - SRh)*60 +(SNm - SRm))*60000/(SNa_d-alpha_0_d); //time for altitude (in ms) before solar noon 
double t_alt_2_d= ((SSh - SNh)*60 +(SSm - SNm))*60000/(SNa_d-alpha_L_d); //time for altitude (in ms) after solar noon 
double t_azi_1_d= ((SSh - SRh)*60 +(SSm - SRm))*60000/(beta_L_d-beta_0_d); //time for azimuth (in ms) 
double t_sn= ((SNh-PTh)*60 +(SNm-PTm))*60000; //time difference between solar noon and present time (in ms)
double alpha_1_d= alpha_0_d +((PTh-SRh)*60 +(PTm-SRm))*60000/t_alt_1_d; //initial angle of altitude when you start the exp.
double alpha_2_d= SNa_d -((PTh-SNh)*60 +(PTm-SNm))*60000/t_alt_2_d; //initial angle of altitude when you start the exp after solar noon
double beta_d= beta_0_d +((PTh-SRh)*60 +(PTm-SRm))*60000/t_azi_1_d; //initial angle of azimuth when you start the exp.

//----------------------------------------------------

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
  int a3_1_d= 90 -((theta3_d-alpha_1_d)/2); //intial angle for altitude adjustment of unit#3 before solar noon
  int a3_2_d= 90 -((theta3_d-alpha_2_d)/2); //intial angle for altitude adjustment of unit#3 after solar noon
  int b3_d= 45 +(beta_d)/2; //intial angle for azimuthal adjustment

  Servo myservo3_alt; //create servo object to control a servo for altitude adjustment
  Servo myservo3_azi; //create servo object to control a servo for azimuthal adjustment
//-------------------------------

//---------UNIT#4-------------
  double x4= -18.75; //x position (in inch) of unit#3
  int y4=0; //y position (in inch) of unit#3
  double d4=sqrt(x4*x4+y4*y4); //distance (in inch) of unit#3 from origin(centre point of the base on which these mirror units are intalled)
  double theta4_d= (180/3.1415926)*atan(h/d4); //angle in degree
  int a4_1_d= ((alpha_1_d+theta4_d)/2); //intial angle for altitude adjustment of unit#4 before solar noon
  int a4_2_d= ((alpha_2_d+theta4_d)/2); //intial angle for altitude adjustment of unit#4 after solar noon
  int b4_d= (beta_d-90)/2; //intial angle for azimuthal adjustment of unit#4

  Servo myservo4_alt; //create servo object to control a servo for altitude adjustment
  Servo myservo4_azi; //create servo object to control a servo for azimuthal adjustment
//-------------------------------


void setup() 
{
    Serial.begin(9600);
    Serial.print("myTime=");
    Serial.print(myTime);
    Serial.print("\t");
    Serial.print("t_alt_1_d=");
    Serial.print(t_alt_1_d);
    Serial.print("\t");
    Serial.print("t_alt_2_d=");
    Serial.print(t_alt_2_d);
    Serial.print("\t");
    Serial.print("t_azi_1_d=");
    Serial.print(t_azi_1_d);
    Serial.print("\n");

    Serial.print("theta3_d=");
    Serial.print(theta3_d);
    Serial.print("\t");
    Serial.print("theta4_d=");
    Serial.print(theta4_d);
    Serial.print("\n");

    Serial.print("alpha_1_d=");
    Serial.print(alpha_1_d);
    Serial.print("\t");
    Serial.print("alpha_2_d=");
    Serial.print(alpha_2_d);
    Serial.print("\t");
    Serial.print("beta_d=");
    Serial.print(beta_d);
    Serial.print("\n");
    
    Serial.print("a3_1_d=");
    Serial.print(a3_1_d);
    Serial.print("\t");
    Serial.print("a4_1_d=");
    Serial.print(a4_1_d);
    Serial.print("\n");
    
    Serial.print("a3_2_d=");
    Serial.print(a3_2_d);
    Serial.print("\t");
    Serial.print("a4_2_d=");
    Serial.print(a4_2_d);
    Serial.print("\n");
    
    Serial.print("b3_d=");
    Serial.print(b3_d);
    Serial.print("\t");
    Serial.print("b4_d=");
    Serial.print(b4_d);
    Serial.print("\n");
    
   
//---------------UNIT#3--------------------
  myservo3_alt.attach(5); //attaches the 'altitude adjustment servo' of unit#3 on pin 5 to the servo object
  myservo3_azi.attach(6); //attaches the 'azimuthal adjustment servo' of unit#3 on pin 6 to the servo object
//-----------------------------------------    

//---------------UNIT#4--------------------
  myservo4_alt.attach(7); //attaches the 'altitude adjustment servo' of unit#3 on pin 5 to the servo object
  myservo4_azi.attach(8); //attaches the 'azimuthal adjustment servo' of unit#3 on pin 6 to the servo object
//-----------------------------------------
}

void loop() 
  {
    myTime= millis();

//---------------UNIT#3--------------------
      if(myTime < t_sn && (myTime-last_seen_alt) > t_alt_1_d)
       {alpha_1_d++;
        
          a3_1_d= 90 -((theta3_d-alpha_1_d)/2); //intial angle for altitude adjustment unit#3 before solar noon
          myservo3_alt.write(a3_1_d);
  
          a4_1_d= ((alpha_1_d+theta4_d)/2); //intial angle for altitude adjustment of unit#4 before solar noon
          myservo4_alt.write(a4_1_d);
        
        last_seen_alt = myTime;  
       }
      if(myTime > t_sn && (myTime-last_seen_alt) > t_alt_2_d) 
          {alpha_2_d--;
           
             a3_2_d= 90 -((theta3_d-alpha_2_d)/2); //intial angle for altitude adjustment unit#3 after solar noon
             myservo3_alt.write(a3_2_d);
  
             a4_2_d= ((alpha_2_d+theta4_d)/2); //intial angle for altitude adjustment of unit#4 after solar noon
             myservo4_alt.write(a4_2_d);
           
           last_seen_alt = myTime;
          }
      if( (myTime-last_seen_azi) > t_azi_1_d)
       {beta_d++;
        
          b3_d= 45 +(beta_d)/2; //intial angle for azimuthal adjustment unit#3
          myservo3_azi.write(b3_d);
  
          b4_d= (beta_d-90)/2; //intial angle for azimuthal adjustment of unit#4
          myservo4_azi.write(b4_d);
        
        last_seen_azi = myTime;
       }
//-----------------------------------------

    Serial.print(">>");
    Serial.print(myTime);
    Serial.print("\t");
    Serial.print(t_alt_1_d);
    Serial.print("\t");
    Serial.print(t_alt_2_d);
    Serial.print("\t");
    Serial.println(t_azi_1_d);

    Serial.print("\t");
    Serial.print(theta3_d);
    Serial.print("\t");
    Serial.print(theta4_d);
    Serial.print("\n");

    Serial.print("\t");
    Serial.print(alpha_1_d);
    Serial.print("\t");
    Serial.print(alpha_2_d);
    Serial.print("\t");
    Serial.println(beta_d);
    
    Serial.print("\t");
    Serial.print(a3_1_d);
    Serial.print("\t");
    Serial.println(a4_1_d);
    
    Serial.print("\t");
    Serial.print(a3_2_d);
    Serial.print("\t");
    Serial.println(a4_2_d);
    
    Serial.print("\t");
    Serial.print(b3_d);
    Serial.print("\t");
    Serial.println(b4_d);
    
    
    delay(10000);
  }
