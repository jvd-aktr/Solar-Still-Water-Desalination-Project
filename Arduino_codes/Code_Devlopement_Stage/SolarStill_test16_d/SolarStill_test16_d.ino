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

#define SERVOS 11 
 int PTh=7;  //present time in hours*
 int PTm=8; //present time in minutes*
 int day_of_year= 170; //day of the year of the experiment*

 //---------------Solar_Angle---------------
 double Hour=0; //initiation of variable "Hour" (hour corresponds to current time)
 double gamma=0; //initiation of variable "gamma" (hour corresponds to current time)
 double eqtime=0; //initiation of variable "eqtime" (equation of time)
 double decl=0; //intiation of variable "decl" (solar declination angle (in radians))
 double time_offset=0; //initiation of variable "time_offset" (time offset (in minutes))
 double tst=0; //initiation of variable "tst" (true solar time (in minutes))
 double ha=0; //initiation of variable "ha" (solar hour angle (in radian))
 double phi=0; //initiation of variable "phi" (solar zenith angle (in radian))
 double alpha=0; //initiation of variable "alpha" (solar altitude angle (in radian))
 double beta=0; //initiation of variable "beta" (solar azimuth angle (in radian)
 double alpha_d=0; //initiation of variable "alpha_d" (solar altitude angle (in degree))
 double beta_d=0; //initiation of variable "beta_d" (solar altitude angle (in degree))
 double snoon=0; //initiation of variable "snoon" (solar noon (in minute))
 double t_CT=0; //initiation of variable "t_CT" (current time (in minutes))

 double s_x=0; //initiation of variable "s_x" (x-component of sun_vector)
 double s_y=0; //initiation of variable "s_y" (y-component of sun_vector)
 double s_z=0; //initiation of variable "s_z" (z-component of sun_vector)
 //-----------------------------------------
 
 double pi= 3.1415926; //value of 'pi'
 double r2d= 180/pi; //convesion ratio of radian to degree conversion
 double d2r= pi/180; //convesion ratio of degree to radian conversion
 double h=23.625; //height(in inch) of convex mirror focal point from the origin 
                  //(centre point of the base on which these mirror units are intalled)

 int timezone=-6; //timezone in hour from UTC (Mountain Daylight Time)
 double latitude= 32.287989; //latitude position (in degree) of the experimental station (Jett Hall Annex)
 double longitude= -106.75385; //longitude position (in degree) of the experimental station (Jett Hall Annex)
 double lat= (d2r)*latitude; //latitude (in radian) for trigonometric functions
 unsigned long myTime;
 unsigned long last_seen; 
//-------------------------------------------------------------------------

//---------------Mirror_UNITS-----------------
  //------Focus_Point--------
  double x[11]= {0,0,0,0,0,0,0,0,0,0,0}; //x position (in inch) of mirror unit
  double y[11]= {0,0,-18.75,0,0,0,0,0,0,0,0}; //y position (in inch) of unit#3
  double z[11]={h,h,h,h,h,h,h,h,h,h,h};; //z position (in inch) of unit#3   
  //-------------------------

  double m_alt_d[11] = {0,0,0,0,0,0,0,0,0,0,0}; //inititiin of output of the function (mirror altitude angle)
  double m_azi_d[11] = {0,0,0,0,0,0,0,0,0,0,0}; //inititiin of output of the function (mirror azimuth angle)
  
  int offset_alt_d[11]={0,0,3,0,0,0,0,0,0,0,0}; //offset of altitude adjustment (in degree) for unit#3
  int offset_azi_d[11]={0,0,3,0,0,0,0,0,0,0,0}; //offset of azimuth adjustment (in degree) for unit#3
  int m_alt_d_offset[11]= {0,0,0,0,0,0,0,0,0,0,0}; //intial angle for altitude adjustment with offset for unit#3
  int m_azi_d_offset[11]= {0,0,0,0,0,0,0,0,0,0,0}; //intial angle for azimuth adjustment with offset for unit#3

  Servo myservo_alt[SERVOS]; //create servo object to control a servo for altitude adjustment for unit#3
  Servo myservo_azi[SERVOS]; //create servo object to control a servo for azimuthal adjustment for unit#3
//-------------------------------

void setup() 
{
  //---------------UNIT#3--------------------
  
  for(int i=0; i<SERVOS; i++)
  {
    myservo_alt[i].attach(i+1); //attaches the 'altitude adjustment servo' of unit#3 on pin 5 to the servo object
    myservo_azi[i].attach(i+12); //attaches the 'azimuthal adjustment servo' of unit#3 on pin 6 to the servo object

        
        Serial.print("m_alt_d[i]=");
        Serial.print(m_alt_d[i]);
        Serial.print("\t");
        Serial.print("m_azi_d[i]=");
        Serial.print(m_azi_d[i]);
        Serial.print("\n");
  }
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

    Serial.print("alpha_d=");
    Serial.print(alpha_d);
    Serial.print("\t");
    Serial.print("beta_d=");
    Serial.print(beta_d);
    Serial.print("\n");
}

void loop() 
{
  myTime= millis();
  //-----------------------Solar_Angle----------------------------
    double Hour= (((PTh*60+PTm)*60000) +myTime)/3600000; //hour corresponds to current time

    double gamma= (2*pi/365)*(day_of_year-1+((Hour-12)/24)); 
                   //fractional year (in radian); [for leap year, use 366 instead of 365 in the denominator]
    eqtime= 229.18*(0.000075+0.001868*cos(gamma)-0.032077*sin(gamma)-0.014615*cos(2*gamma)-0.040849*sin(2*gamma)); 
                            //equation of time (in minutes)
    
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

for (int i = 0; i < 11; i++)
    {
      Mirror_angle(s_x, s_y, s_z, x[i], y[i], z[i], i);
                  
       if(myTime - last_seen > 10000)
        { 
          m_alt_d_offset[i]= offset_alt_d[i] +m_alt_d[i]; //intial angle for altitude adjustment with offset for unit#3
          myservo_alt[i].write(m_alt_d_offset[i]);
      
          
          m_azi_d_offset[i]= offset_azi_d[i] +m_azi_d[i]; //intial angle for azimuth adjustment with offset for unit#3
          myservo_azi[i].write(m_azi_d_offset[i]);
    
          last_seen = myTime;
    
        Serial.print("\t");
        Serial.print(m_alt_d[i]);
        Serial.print("\t");
        Serial.print(m_azi_d[i]);
        Serial.print("\n");
        }
    }
//-----------------------------------------------------------------------------


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
    Serial.print(alpha_d);
    Serial.print("\t");
    Serial.print(beta_d);
    Serial.print("\n");

    delay(10000);
}

void Mirror_angle(double _sx, double _sy, double _sz, double _px, double _py, double _pz, int _mindex)
{
  double _fx= -_px/(sqrt(_px*_px+_py*_py+_pz*_pz)); //x-componenet of target_vector number (_mindex + 1)
  double _fy= -_py/(sqrt(_px*_px+_py*_py+_pz*_pz)); //y-componenet of target_vector number (_mindex + 1)
  double _fz= _pz/(sqrt(_px*_px+_py*_py+_pz*_pz)); //z-componenet of target_vector number (_mindex + 1)
  
  double _mx=(((abs(_sz+_fz))/(_sz+_fz))*(_sx+_fx))*(1/(sqrt((_sx+_fx)*(_sx+_fx)+(_sy+_fy)*(_sy+_fy)+(_sz+_fz)*(_sz+_fz))));
  double _my=(((abs(_sz+_fz))/(_sz+_fz))*(_sy+_fy))*(1/(sqrt((_sx+_fx)*(_sx+_fx)+(_sy+_fy)*(_sy+_fy)+(_sz+_fz)*(_sz+_fz))));
  double _mz=(abs(_sz+_fz))*(1/(sqrt((_sx+_fx)*(_sx+_fx)+(_sy+_fy)*(_sy+_fy)+(_sz+_fz)*(_sz+_fz))));

  double a=0; //elevation angle (in radian) of flat mirror of mirror unit
  double b=0; //azimutha angle (in radian) of flat mirror of mirror unit

    //---test----
    //theta= (r2d)*atan2(_my, _mx);

    //a= pi/2 -acos(_mz); //elevation angle (in radian) of flat mirror of unit#3

      if(_mx>0 && _my>=0)
      {
        a= pi/2 +acos(_mz);
        b= pi/2 -atan(abs(_my)/abs(_mx));
      }
      else if(_mx<=0 && _my>0)
      {
        a= pi/2 -acos(_mz);
        b= pi/2 +atan(abs(_my)/abs(_mx));
      }
      else if(_mx<0 && _my<=0)
      {
        a= pi/2 -acos(_mz);
        b= pi/2 -atan(abs(_my)/abs(_mx));
      }
      else if(_mx>=0 && _my<0)
      {
        a= pi/2 +acos(_mz);
        b= pi/2 +atan(abs(_my)/abs(_mx));
      }
      else {b=b;}

    m_alt_d[_mindex]= (r2d)*a; //elevation angle (in degree) of flat mirror of unit#3
    m_azi_d[_mindex]= (r2d)*b; //azimuth angle (in degree) of flat mirror of unit#3
}
