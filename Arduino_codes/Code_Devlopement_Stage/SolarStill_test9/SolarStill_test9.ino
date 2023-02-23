#include <Servo.h>
#include <math.h>

          /*May 21 2021*/
 int PTh=10;  //present time in hours*
 int PTm=05; //present time in minutes*
 int day_of_year= 141; //day of the year of the experiment*
 
 
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
 double a3=0; //initiation of variable "a3"
 double b3=0; //initiation of variable "b3"

       /*544 corespond to 0 degree angle/ 2394 correspond to 180 degree angle*/
 double C= (2394-544)/180; //conversion of degree in arduino angle parameter
 double pi= 3.1415926; //value of 'pi'
 double h=23.625; //height(in inch) of convex mirror focal point from the origin(centre point of the base on which these mirror units are intalled)

 int timezone=-6; //timezone in hour from UTC (Mountain Daylight Time)
 double latitude= 32.280856; //latitude position (in degree) of the experimental station (Jett Hall Annex)
 double longitude= -106.76; //longitude position (in degree) of the experimental station (Jett Hall Annex)
 double lat= (3.1415926/180)*latitude; //latitude (in radian) for trigonometric functions
 unsigned long myTime;
//-------------------------------------------------------------------------

//---------UNIT#3-------------
  double x3= 18.75; //x position (in inch) of unit#3
  int y3=0; //y position (in inch) of unit#3
  double d3=sqrt(x3*x3+y3*y3); //distance (in inch) of unit#3 from origin(centre point of the base on which these mirror units are intalled)
  double theta3_d= (180/3.1415926)*atan(h/d3); //angle in degree

  double a3_d= 90 -((theta3_d-alpha_d)/2); //intial angle (in degree) for altitude adjustment of unit#3
  double b3_d= 45 +(beta_d)/2; //intial angle (in degree) for azimuthal adjustment of unit#3

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
    Serial.print("tst");
    Serial.print(tst);
    Serial.print("\t");
    Serial.print("time_offset");
    Serial.print(time_offset);
    Serial.print("\t");
    Serial.print("eqtime");
    Serial.print(eqtime);
    Serial.print("\n");
    Serial.print("longitude");
    Serial.print(longitude);
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
    Serial.print("\t");
    Serial.print("a3=");
    Serial.print(a3);
    Serial.print("\n");
    
    Serial.print("b3_d=");
    Serial.print(b3_d);
    Serial.print("\t");
    Serial.print("b3=");
    Serial.print(b3);
    Serial.print("\n");
    

//-----------------------------------------
}

void loop() 
{
  myTime= millis();
  
    Hour= (((PTh*60+PTm)*60000) +myTime)/3600000; //hour corresponds to current time
    day_of_year=  141;

    gamma= (2*pi/365)*(day_of_year-1+((Hour-12)/24)); //fractional year (in radian); [for leap year, use 366 instead of 365 in the denominator]
    eqtime= 229.18*(0.000075+0.001868*cos(gamma)-0.032077*sin(gamma)-0.014615*cos(2*gamma)-0.040849*sin(2*gamma)); //equation of time (in minutes)
    
    decl= 0.006918-0.399912*cos(gamma)+0.070257*sin(gamma)-0.006758*cos(2*gamma)+0.000907*sin(2*gamma)-0.002697*cos(3*gamma)+0.00148*sin(3*gamma);
                  //solar declination angle (in radians)
    time_offset= eqtime+4*longitude-60*timezone; //time offset (in minutes)

    tst= (PTh*60+PTm+time_offset) +(myTime/60000); //true solar time (in minutes)

    ha= (pi/180)*((tst/4)-180); //solar hour angle(in radian)

    phi= acos(sin(lat)*sin(decl)+cos(lat)*cos(decl)*cos(ha)); //solar zenith angle angle (in radian)
    beta= acos((sin(lat)*cos(phi)-sin(decl))/(cos(lat)*sin(phi))); //solar azimuth angle (in radian)

    alpha_d= (180/pi)*(pi/2-phi); //solar altitude/elevation angle (in degree)
    beta_d= (180/pi)*beta; //solar azimuth angle (in degree)

//-----------------------------------------------------------------------------

//--------------------(3)------------------------
    a3_d= 90 -((theta3_d-alpha_d)/2); //intial angle (in degree) for altitude adjustment of unit#3
    a3= 544 +a3_d*C; //initial angle (for arduino) for altitude adjustment of unit#3
    myservo3_alt.write(a3);

    b3_d= 45 +(beta_d)/2; //intial angle (in degree) for azimuthal adjustment of unit#3
    b3= 544 +b3_d*C; //initial angle (for arduino) for azimuthal adjustment of unit#3
    myservo3_azi.write(b3);
//--------------------(4)------------------------


    Serial.print(">>");
    Serial.print(myTime);
    Serial.print("\t");
    Serial.print(tst);
    Serial.print("\t");
    Serial.print(time_offset);
    Serial.print("\t");
    Serial.print(eqtime);
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
    Serial.print("\t");
    Serial.print(a3);
    Serial.print("\n");

    Serial.print("\t");
    Serial.print(b3_d);
    Serial.print("\t");
    Serial.print(b3);
    Serial.print("\n");

delay(2000);
    
}
