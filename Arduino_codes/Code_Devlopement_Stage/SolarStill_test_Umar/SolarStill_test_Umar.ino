#include <Servo.h>
#include <math.h>

/*
 * Link for solar information:(1) https://www.esrl.noaa.gov/gmd/grad/solcalc/ ****use this one
                              (2) https://www.esrl.noaa.gov/gmd/grad/solcalc/azel.html
                              (3) https://www.timeanddate.com/sun/usa/las-cruces
 * Las Cruces Coorinate: 32.287989° N(+), -106.75385° W (-)
 * Azimuth and Elevation - an angular coordinate system for locating positions in the sky. Azimuth is measured clockwise from true north 
   to the point on the horizon directly below the object. Elevation is measured vertically from that point on the horizon up to the object.
 * offset_azi_testunit=3;  offset_alt_testunit=3;  (in degree)
  */

          /*june 17 2021 - > day_of_year= 168*/
 int PTh=7;  //present time in hours*
 int PTm=8; //present time in minutes*
 int day_of_year= 170; //day of the year of the experiment*
 
 
 double Hour=0; //initiation of variable "Hour"
 double gamma=0; //initiation of variable "gamma"
 double eqtime=0; //initiation of variable "eqtime"
 double decl=0; //intiation of variable "decl"
 double time_offset=0; //initiation of variable "time_offset"
 double tst=0; //initiation of variable "tst" (true solar time (in minutes))
 double ha=0; //initiation of variable "ha" (solar hour angle (in radian))
 double phi=0; //initiation of variable "phi"
 double alpha=0; //initiation of variable "alpha"
 double beta=0; //initiation of variable "beta"
 double alpha_d=0; //initiation of variable "alpha_d"
 double beta_d=0; //initiation of variable "beta_d"
 double snoon=0; //initiation of variable "snoon" (solar noon (in minute))
 double t_CT=0; //initiation of variable "t_CT" (current time (in minutes))

 double s_x=0; //initiation of variable "s_x" (x-component of sun_vector)
 double s_y=0; //initiation of variable "s_y" (y-component of sun_vector)
 double s_z=0; //initiation of variable "s_z" (z-component of sun_vector)

 double pi= 3.1415926; //value of 'pi'
 double r2d= 180/pi; //convesion ratio of radian to degree conversion
 double d2r= pi/180; //convesion ratio of degree to radian conversion
 double h=23.625; //height(in inch) of convex mirror focal point from the origin(centre point of the base on which these mirror units are intalled)

 int timezone=-6; //timezone in hour from UTC (Mountain Daylight Time)
 double latitude= 32.287989; //latitude position (in degree) of the experimental station (Jett Hall Annex)
 double longitude= -106.75385; //longitude position (in degree) of the experimental station (Jett Hall Annex)
 double lat= (3.1415926/180)*latitude; //latitude (in radian) for trigonometric functions
 unsigned long myTime;
 unsigned long last_seen; 
//-------------------------------------------------------------------------

//---------------UNIT#3-----------------
  //------Focus_Point--------
  double x[11]= {0, 0,0,0,0,0,0,0,0,0,0,0}; //x position (in inch) of unit#3
  double y[11]= -18.75; //y position (in inch) of unit#3
  double z[11]=h; //z position (in inch) of unit#3
    //target_vector3: unit vector along reflected ray from flat mirror of unit#3

  //-------------------------

  //-----Bisector_Vector3------
    //bisector_vector3: unit vector along bisector on fat mirror which will always be normal to the flat mirror of unit#3
  double m3_x=0; //initiation of variable "m3_x" (x-component of bisector_vector3)
  double m3_y=0; //initiation of variable "m3_y" (y-component of bisector_vector3)
  double m3_z=0; //initiation of variable "m3_z" (z-component of bisector_vector3)
  //---------------------------

  double a3=0; //initiation of variable "a3" (alt adjustment angle(in radian))
  double b3=0; //initiation of variable "b3" (azi adjustment angle(in radian))
  int a3_d=0; //initiation of variable "a3_d" (alt adjustment angle(in degree))
  int b3_d=0; //initiation of variable "b3_d" (azi adjustment angle(in degree))
  int offset_alt3=3; //offset of altitude adjustment (in degree) for unit#3
  int offset_azi3=3; //offset of azimuth adjustment (in degree) for unit#3
  int a3_d_offset= offset_alt3 +a3_d; //intial angle for altitude adjustment with offset for unit#3
  int b3_d_offset= offset_azi3 +b3_d; //intial angle for azimuth adjustment with offset for unit#3

  double ang_elev[11] = 0;
  double ang_azim[11] = 0;

  double theta3=0; // for test only. it's is angle of projection of normal on xy plane

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

    Serial.print("m3_x=");
    Serial.print(m3_x);
    Serial.print("\t");
    Serial.print("m3_y=");
    Serial.print(m3_y);
    Serial.print("\n");

    Serial.print("theta3=");
    Serial.print(theta3);
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
  
    double Hour= (((PTh*60+PTm)*60000) +myTime)/3600000; //hour corresponds to current time

    double gamma= (2*pi/365)*(day_of_year-1+((Hour-12)/24)); //fractional year (in radian); [for leap year, use 366 instead of 365 in the denominator]
    eqtime= 229.18*(0.000075+0.001868*cos(gamma)-0.032077*sin(gamma)-0.014615*cos(2*gamma)-0.040849*sin(2*gamma)); //equation of time (in minutes)
    
    decl= 0.006918-0.399912*cos(gamma)+0.070257*sin(gamma)-0.006758*cos(2*gamma)+0.000907*sin(2*gamma)-0.002697*cos(3*gamma)+0.00148*sin(3*gamma);
                  //solar declination angle (in radians)
    time_offset= eqtime+4*longitude-60*timezone; //time offset (in minutes)

    tst= (PTh*60+PTm+time_offset) +(myTime/60000); //true solar time (in minutes)

    ha= (d2r)*((tst/4)-180); //solar hour angle(in radian)

    snoon= 360-4*longitude-eqtime; //solar noon for a given location is found from the longitude (in minutes)
    t_CT= (PTh*60+PTm) +(myTime/60000); //current time (in minutes)
    
    phi= acos(sin(lat)*sin(decl)+cos(lat)*cos(decl)*cos(ha)); //solar zenith angle angle (in radian)
    alpha= (pi/2 -phi); //solar altitude angle (in radian)
    beta= acos((sin(lat)*cos(phi)-sin(decl))/(cos(lat)*sin(phi))); //solar azimuth angle (in radian)

    if(t_CT <= snoon)
      {
        beta= pi-(beta); //solar azimuth angle (in radian) before solar noon
      }
    if(t_CT > snoon)
      {
        beta= pi+(beta); //solar azimuth angle (in radian) after solar noon
      }
    alpha_d= (r2d)*alpha; //solar altitude/elevation angle (in degree)
    beta_d= (r2d)*(beta); //solar azimuth angle (in degree)

    //------Sun_Vector------
    s_x= cos(alpha)*cos(beta); //x-component of sun_vector)
    s_y= -cos(alpha)*sin(beta); //y-component of sun_vector)
    s_z= sin(alpha); //z-component of sun_vector)
    //----------------------
    
//-----------------------------------------------------------------------------

//--------------------(3)------------------------

    //to put function there
   Servo allservos_elev[11];
   allservos_elev[0].attach(5);
   allservos_elev[1].attach(7);
   
    for (int i = 0; i < 11; i++)
    {
      Mirror_angle(s_x, s_y, s_z, x[i], y[i], z[i], i);
                  
   if(myTime - last_seen > 10000)
    { 
      a3_d_offset=9 offset_alt3 +a3_d; //intial angle for altitude adjustment with offset for unit#3   //in place of a3_d write ang_elev[2]
      myservo3_alt.write(a3_d_offset);
  
      
      b3_d_offset= offset_azi3 +b3_d; //intial angle for azimuth adjustment with offset for unit#3
      myservo3_azi.write(b3_d_offset);

      last_seen = myTime;
    }
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
    Serial.print(m3_x);
    Serial.print("\t");
    Serial.print(m3_y);
    Serial.print("\n");

    
    Serial.print("\t");
    Serial.print(theta3);
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





//--------------------------------

void Mirror_angle(double _sx, double _sy, double _sz, double _px, double _py, double _pz, int _mindex)
{
  double _fx= -_px/(sqrt(_px*_px+_py*_py+_pz*_pz)); //x-componenet of target_vector number (_mindex + 1)
  double _fy= -_py/(sqrt(_px*_px+_py*_py+_pz*_pz)); //y-componenet of target_vector number (_mindex + 1)
  double _fz= _pz/(sqrt(_px*_px+_py*_py+_pz*_pz)); //z-componenet of target_vector number (_mindex + 1)
  
  double _mx=(((abs(_sz+_fz))/(_sz+_fz))*(_sx+_fx))*(1/(sqrt((_sx+_fx)*(_sx+_fx)+(_sy+_fy)*(_sy+_fy)+(_sz+_fz)*(_sz+_fz))));
  double _my=(((abs(_sz+_fz))/(_sz+_fz))*(_sy+_fy))*(1/(sqrt((_sx+_fx)*(_sx+_fx)+(_sy+_fy)*(_sy+_fy)+(_sz+_fz)*(_sz+_fz))));
  double _mz=(abs(_sz+_fz))*(1/(sqrt((_sx+_fx)*(_sx+_fx)+(_sy+_fy)*(_sy+_fy)+(_sz+_fz)*(_sz+_fz))));

    //---test----
    theta3= (r2d)*atan2(m3_y, m3_x);

    //a3= pi/2 -acos(m3_z); //elevation angle (in radian) of flat mirror of unit#3

      if(m3_x>0 && m3_y>=0)
      {
        a3= pi/2 +acos(m3_z);
        b3= pi/2 -atan(abs(m3_y)/abs(m3_x));
      }
      else if(m3_x<=0 && m3_y>0)
      {
        a3= pi/2 -acos(m3_z);
        b3= pi/2 +atan(abs(m3_y)/abs(m3_x));
      }
      else if(m3_x<0 && m3_y<=0)
      {
        a3= pi/2 -acos(m3_z);
        b3= pi/2 -atan(abs(m3_y)/abs(m3_x));
      }
      else if(m3_x>=0 && m3_y<0)
      {
        a3= pi/2 +acos(m3_z);
        b3= pi/2 +atan(abs(m3_y)/abs(m3_x));
      }
      else {b3=b3;}

    ang_elev[_mindex]= (r2d)*a3; //elevation angle (in degree) of flat mirror of unit#3
    ang_azim[_mindex]= (r2d)*b3; //azimuth angle (in degree) of flat mirror of unit#3
}
