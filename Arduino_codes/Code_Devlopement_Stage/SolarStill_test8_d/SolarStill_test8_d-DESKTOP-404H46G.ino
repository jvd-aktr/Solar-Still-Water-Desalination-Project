#include <Servo.h>
#include <math.h>

/*
 * Link for solar information:(1) https://www.esrl.noaa.gov/gmd/grad/solcalc/ ****use this one
                              (2) https://www.esrl.noaa.gov/gmd/grad/solcalc/azel.html
                              (3) https://www.timeanddate.com/sun/usa/las-cruces
 * Las Cruces Coorinate: 32.280856° N(+), 106.75491° W (-)
 * Azimuth and Elevation - an angular coordinate system for locating positions in the sky. Azimuth is measured clockwise from true north 
   to the point on the horizon directly below the object. Elevation is measured vertically from that point on the horizon up to the object.
 * it requires 540000 ms for azimuth angle adjustment (for servo) of 1 degree. Not for everytime. 
 * offset_azi_testunit=8;  offset_alt_testunit=9;  (in degree)
  */

          /*june 2 2021 - > day_of_year= 153*/
 int PTh=10;  //present time in hours*
 int PTm=14 ; //present time in minutes*
 int day_of_year= 154; //day of the year of the experiment*
 
 
 double Hour=0; //initiation of variable "Hour"
 double gamma=0; //initiation of variable "gamma"
 double eqtime=0; //initiation of variable "eqtime"
 double decl=0; //intiation of variable "decl"
 double time_offset=0; //initiation of variable "time_offset"
 double tst=0; //initiation of variable "tst"
 double ha=0; //initiation of variable "ha"
 double phi=0; //initiation of variable "phi"
 double beta=0; //initiation of variable "beta"
 double alpha_d=0; //initiation of variable "alpha_d"
 double beta_d=0; //initiation of variable "beta_d"
 double snoon=0; //initiation of variable "snoon"
 double t_CT=0; //initiation of variable "t_CT"

 double pi= 3.1415926; //value of 'pi'
 double h=23.625; //height(in inch) of convex mirror focal point from the origin(centre point of the base on which these mirror units are intalled)

 int timezone=-6; //timezone in hour from UTC (Mountain Daylight Time)
 double latitude= 32.280856; //latitude position (in degree) of the experimental station (Jett Hall Annex)
 double longitude= -106.76; //longitude position (in degree) of the experimental station (Jett Hall Annex)
 double lat= (3.1415926/180)*latitude; //latitude (in radian) for trigonometric functions
 unsigned long myTime;
 unsigned long last_seen; 
//-------------------------------------------------------------------------

//---------UNIT#3-------------
  double x3= 18.75; //x position (in inch) of unit#3
  int y3=0; //y position (in inch) of unit#3
  double d3=sqrt(x3*x3+y3*y3); //distance (in inch) of unit#3 from origin(centre point of the base on which these mirror units are intalled)
  double theta3_d= (180/3.1415926)*atan(h/d3); //angle in degree

  int a3_d= 90 -((theta3_d-alpha_d)/2); //intial angle for altitude adjustment of unit#3
  int b3_d= 45 +(beta_d)/2; //intial angle for azimuthal adjustment of unit#3

  int offset_alt3=9; //ofset of altitude adjustment for unit#3
  int offset_azi3=8; //ofset of azimuth adjustment for unit#3
  int a3_d_offset= offset_alt3 +a3_d; //intial angle for altitude adjustment with offset for unit#3
  int b3_d_offset= offset_azi3 +b3_d; //intial angle for azimuth adjustment with offset for unit#3

  Servo myservo3_alt; //create servo object to control a servo for altitude adjustment for unit#3
  Servo myservo3_azi; //create servo object to control a servo for azimuthal adjustment for unit#3
//-------------------------------
 
void setup() 
{
//---------------UNIT#3--------------------
  myservo3_alt.attach(5); //attaches the 'altitude adjustment servo' of unit#3 on pin 5 to the servo object
  myservo3_azi.attach(6); //attaches the 'azimuthal adjustment servo' of unit#3 on pin 6 to the servo object
//----------------------------------------- 

//-----------------------------------------
    Serial.begin(9600);
    Serial.print("myTime=");
    Serial.print(myTime);
    Serial.print("\t");
    Serial.print("tst=");
    Serial.print(tst);
    Serial.print("\t");
    Serial.print("time_offset=");
    Serial.print(time_offset);
    Serial.print("\t");
    Serial.print("eqtime=");
    Serial.print(eqtime);
    Serial.print("\t");
    Serial.print("decl=");
    Serial.print(decl);
    Serial.print("\n");

    Serial.print("snoon=");
    Serial.print(snoon);
    Serial.print("\t");
    Serial.print("t_CT=");
    Serial.print(t_CT);
    Serial.print("\n");

    Serial.print("theta3_d=");
    Serial.print(theta3_d);
    Serial.print("\n");

    Serial.print("alpha_d=");
    Serial.print(alpha_d);
    Serial.print("\t");
    Serial.print("beta_d=");
    Serial.print(beta_d);
    Serial.print("\n");
    
    Serial.print("a3_d=");
    Serial.print(a3_d);
    Serial.print("\n");
    
    Serial.print("b3_d=");
    Serial.print(b3_d);
    Serial.print("\n");
    

//-----------------------------------------
}

void loop() 
{
  myTime= millis();
  
    Hour= (((PTh*60+PTm)*60000) +myTime)/3600000; //hour corresponds to current time

    gamma= (2*pi/365)*(day_of_year-1+((Hour-12)/24)); //fractional year (in radian); [for leap year, use 366 instead of 365 in the denominator]
    eqtime= 229.18*(0.000075+0.001868*cos(gamma)-0.032077*sin(gamma)-0.014615*cos(2*gamma)-0.040849*sin(2*gamma)); //equation of time (in minutes)
    
    decl= 0.006918-0.399912*cos(gamma)+0.070257*sin(gamma)-0.006758*cos(2*gamma)+0.000907*sin(2*gamma)-0.002697*cos(3*gamma)+0.00148*sin(3*gamma);
                  //solar declination angle (in radians)
    time_offset= eqtime+4*longitude-60*timezone; //time offset (in minutes)

    tst= (PTh*60+PTm+time_offset) +(myTime/60000); //true solar time (in minutes)

    ha= (pi/180)*((tst/4)-180); //solar hour angle(in radian)

    snoon= 360-4*longitude-eqtime; //solar noon for a given location is found from the longitude (in minutes)
    t_CT= (PTh*60+PTm) +(myTime/60000); //current time (in minutes)
    
    phi= acos(sin(lat)*sin(decl)+cos(lat)*cos(decl)*cos(ha)); //solar zenith angle angle (in radian)
    beta= acos((sin(lat)*cos(phi)-sin(decl))/(cos(lat)*sin(phi))); //solar azimuth angle (in radian)

    alpha_d= (180/pi)*(pi/2-phi); //solar altitude/elevation angle (in degree)
    if(t_CT <= snoon)
      {
        beta_d= 180-(180/pi)*(beta); //solar azimuth angle (in degree) before solar noon
      }
    if(t_CT > snoon)
      {
        beta_d= 180+(180/pi)*(beta); //solar azimuth angle (in degree) after solar noon
      }

//-----------------------------------------------------------------------------

//--------------------(3)------------------------
   if(myTime - last_seen > 100000)
    {
      a3_d= 90 -((theta3_d-alpha_d)/2); //intial angle for altitude adjustment of unit#3
      a3_d_offset= offset_alt3 +a3_d; //intial angle for altitude adjustment with offset for unit#3
      myservo3_alt.write(a3_d_offset);
  
      b3_d= 45 +(beta_d)/2; //intial angle for azimuthal adjustment of unit#3
      b3_d_offset= offset_azi3 +b3_d; //intial angle for azimuth adjustment with offset for unit#3
      myservo3_azi.write(b3_d_offset);

      last_seen = myTime;

    }
//--------------------(4)------------------------


    Serial.print(">>");
    Serial.print(myTime);
    Serial.print("\t");
    Serial.print(tst);
    Serial.print("\t");
    Serial.print(time_offset);
    Serial.print("\t");
    Serial.print(eqtime);
    Serial.print("\t");
    Serial.print(decl);
    Serial.print("\n");

    Serial.print("\t");
    Serial.print(snoon);
    Serial.print("\t");
    Serial.print(t_CT);
    Serial.print("\n");
    
    Serial.print("\t");
    Serial.print(theta3_d);
    Serial.print("\n");

    Serial.print("\t");
    Serial.print(alpha_d);
    Serial.print("\t");
    Serial.print(beta_d);
    Serial.print("\n");

    Serial.print("\t");
    Serial.print(a3_d);
    Serial.print("\n");

    Serial.print("\t");
    Serial.print(b3_d);
    Serial.print("\n");

delay(5000);
    
}
