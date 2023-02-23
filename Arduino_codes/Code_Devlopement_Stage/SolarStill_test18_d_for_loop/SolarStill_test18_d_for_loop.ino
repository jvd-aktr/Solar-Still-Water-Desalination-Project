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

#define SERVOS 4
 
 int PTh=15; //present time in hours*
 int PTm=34; //present time in minutes*
 int day_of_year= 170; //day of the year of the experiment*
 
 
 double Hour=0; //initiation of variable "Hour" (hour corresponds to current time)
 double gamma=0; //initiation of variable "gamma" (fractional year (in radian))
 double eqtime=0; //initiation of variable "eqtime" (equation of time (in minute))
 double decl=0; //intiation of variable "decl" (solar declination angle (in radians))
 double time_offset=0; //initiation of variable "time_offset" (time offset (in minutes))
 double tst=0; //initiation of variable "tst" (true solar time (in minutes))
 double ha=0; //initiation of variable "ha" (solar hour angle (in radian))
 double phi=0; //initiation of variable "phi" (solar zenith angle angle (in radian))
 double alpha=0; //initiation of variable "alpha" (solar altitude angle angle (in radian))
 double beta=0; //initiation of variable "beta" (solar azimuth angle angle (in radian))
 double alpha_d=0; //initiation of variable "alpha_d" (solar altitude angle angle (in degree))
 double beta_d=0; //initiation of variable "beta_d" (solar azimuth angle angle (in degree))
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
 double latitude= 32.287989; //latitude position (in degree) of the experimental station (Monte Vista Ave)
 double longitude= -106.75385; //longitude position (in degree) of the experimental station (Monte Vista Ave)
 double lat= (d2r)*latitude; //latitude (in radian) for trigonometric functions
 unsigned long myTime;
 unsigned long last_seen; 
//-------------------------------------------------------------------------

//---------------UNIT#3-----------------
  //------Focus_Point--------
  double x[4]= {0, 0, 0, 0}; //x position (in inch) of unit#[]
  double y[4]= {0, 0, -18.50, 18.50}; //y position (in inch) of unit#[]
  double z[4]={0, 0, h, h}; //z position (in inch) of unit#[]

  double f_x[4]= {0, 0, 0, 0}; //initiation of x-componenet of target_vector
  double f_y[4]= {0, 0, 0, 0}; //initiation of y-componenet of target_vector
  double f_z[4]= {0, 0, 0, 0}; //initiation of z-componenet of target_vector

  
     
/*    //target_vector3: unit vector along reflected ray from flat mirror of unit#3
  double f3_x= -x3/(sqrt(x3*x3+y3*y3+z3*z3)); //x-componenet of target_vector3
  double f3_y= -y3/(sqrt(x3*x3+y3*y3+z3*z3)); //y-componenet of target_vector3
  double f3_z= z3/(sqrt(x3*x3+y3*y3+z3*z3)); //z-componenet of target_vector3
  */
  //-------------------------

  //-----Bisector_Vector3------
    //bisector_vector3: unit vector along bisector on fat mirror which will always be normal to the flat mirror of unit#3
  double m_x[4]={0, 0, 0, 0}; //initiation of variable "m_x[]" (x-component of bisector_vector[])
  double m_y[4]={0, 0, 0, 0}; //initiation of variable "m_y[]" (y-component of bisector_vector[])
  double m_z[4]={0, 0, 0, 0}; //initiation of variable "m_z[]" (z-component of bisector_vector[])
  //---------------------------

  double a[4]={0, 0, 0, 0}; //initiation of variable "a[]" (alt adjustment angle(in radian))
  double b[4]={0, 0, 0, 0}; //initiation of variable "b[]" (azi adjustment angle(in radian))
  int a_d[4]={0, 0, 0, 0}; //initiation of variable "a_d[]" (alt adjustment angle(in degree))
  int b_d[4]={0, 0, 0, 0}; //initiation of variable "b_d[]" (azi adjustment angle(in degree))
  int offset_alt[4]={0, 0, 3, 24}; //offset of altitude adjustment (in degree) for unit#[]
  int offset_azi[4]={0, 0, 3, 16}; //offset of azimuth adjustment (in degree) for unit#[]
  int a_d_offset[4]= {offset_alt[4] +a_d[4]}; //intial angle for altitude adjustment with offset for unit#[]
  int b_d_offset[4]= {offset_azi[4] +b_d[4]}; //intial angle for azimuth adjustment with offset for unit#[]

  double theta[4]={0, 0, 0, 0}; // for test only. it's is angle of projection of normal on xy plane

  Servo myservo_alt[SERVOS]; //create servo object to control a servo for altitude adjustment for unit#[]
  Servo myservo_azi[SERVOS]; //create servo object to control a servo for azimuthal adjustment for unit#[]
//-----------------------------------------
 
void setup() 
{
//---------------UNIT#[]--------------------
for(int i=0; i<4; i++)
    {
      double f_x[i]= {-x[i]/(sqrt(x[i]*x[i]+y[i]*y[i]+z[i]*z[i]))}; //x-componenet of target_vector
      double f_y[i]= {-y[i]/(sqrt(x[i]*x[i]+y[i]*y[i]+z[i]*z[i]))}; //y-componenet of target_vector
      double f_z[i]= {z[i]/(sqrt(x[i]*x[i]+y[i]*y[i]+z[i]*z[i]))}; //z-componenet of target_vector
    }
    
for(int j=0; j<SERVOS; j++)
{
  myservo_alt[j].attach(j+1); //attaches the 'altitude adjustment servo' of unit#[j] on pin (j+1) to the servo object
  myservo_azi[j].attach(j+5); //attaches the 'azimuthal adjustment servo' of unit#[j] on pin (j+5) to the servo object
}
 
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

    Serial.print("m_x[0]=");
    Serial.print(m_x[0]);
    Serial.print("\t");
    Serial.print("m_y[0]=");
    Serial.print(m_y[0]);
    Serial.print("\n");

    Serial.print("theta[0]=");
    Serial.print(theta[0]);
    Serial.print("\n");

    Serial.print("alpha_d=");
    Serial.print(alpha_d);
    Serial.print("\t");
    Serial.print("beta_d=");
    Serial.print(beta_d);
    Serial.print("\n");

    
    Serial.print("a_d[0]=");
    Serial.print(a_d[0]);
    Serial.print("\n");
    
    Serial.print("b_d[0]=");
    Serial.print(b_d[0]);
    Serial.print("\n");
    

//-----------------------------------------
}

void loop() 
{
  myTime= millis();
//----------------------------Solar_Angles---------------------------------
    Hour= (((PTh*60+PTm)*60000) +myTime)/3600000; //hour corresponds to current time

    gamma= (2*pi/365)*(day_of_year-1+((Hour-12)/24)); //fractional year (in radian) [for leap year, use 366 instead of 365 in the denominator]
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

    for(int u=0; u<4; u++)
    {
      m_x[u]=(((abs(s_z+f_z[u]))/(s_z+f_z[u]))*(s_x+f_x[u]))*(1/(sqrt((s_x+f_x[u])*(s_x+f_x[u])+(s_y+f_y[u])*(s_y+f_y[u])+(s_z+f_z[u])*(s_z+f_z[u]))));
      m_y[u]=(((abs(s_z+f_z[u]))/(s_z+f_z[u]))*(s_y+f_y[u]))*(1/(sqrt((s_x+f_x[u])*(s_x+f_x[u])+(s_y+f_y[u])*(s_y+f_y[u])+(s_z+f_z[u])*(s_z+f_z[u]))));
      m_z[u]=(abs(s_z+f_z[u]))*(1/(sqrt((s_x+f_x[u])*(s_x+f_x[u])+(s_y+f_y[u])*(s_y+f_y[u])+(s_z+f_z[u])*(s_z+f_z[u]))));

      //---test----
    theta[u]= (r2d)*atan2(m_y[u], m_x[u]);
    //-----------
    //a[u]= pi/2 -acos(m_z[u]); //elevation angle (in radian) of flat mirror of unit#[u]

      if(m_x[u]>0 && m_y[u]>=0)
      {
        a[u]= pi/2 +acos(m_z[u]);
        b[u]= pi/2 -atan(abs(m_y[u])/abs(m_x[u]));
      }
      else if(m_x[u]<=0 && m_y[u]>0)
      {
        a[u]= pi/2 -acos(m_z[u]);
        b[u]= pi/2 +atan(abs(m_y[u])/abs(m_x[u]));
      }
      else if(m_x[u]<0 && m_y[u]<=0)
      {
        a[u]= pi/2 -acos(m_z[u]);
        b[u]= pi/2 -atan(abs(m_y[u])/abs(m_x[u]));
      }
      else if(m_x[u]>=0 && m_y[u]<0)
      {
        a[u]= pi/2 +acos(m_z[u]);
        b[u]= pi/2 +atan(abs(m_y[u])/abs(m_x[u]));
      }
      else {b[u]=b[u];}

    a_d[u]= (r2d)*a[u]; //elevation angle (in degree) of flat mirror of unit#[u]
    b_d[u]= (r2d)*b[u]; //azimuth angle (in degree) of flat mirror of unit#[u]

    if(myTime - last_seen > 10000)
    { 
      a_d_offset[u]= offset_alt[u] +a_d[u]; //intial angle for altitude adjustment with offset for unit#3
      myservo_alt[u].write(a_d_offset[u]);
  
      
      b_d_offset[u]= offset_azi[u] +b_d[u]; //intial angle for azimuth adjustment with offset for unit#3
      myservo_azi[u].write(b_d_offset[u]);

      last_seen = myTime;
    }
                  
    }

    
    
   
//-----------------------------------------------


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
    Serial.print(m_x[0]);
    Serial.print("\t");
    Serial.print(m_y[0]);
    Serial.print("\n");

    
    Serial.print("\t");
    Serial.print(theta[0]);
    Serial.print("\n");

    Serial.print("\t");
    Serial.print(alpha_d);
    Serial.print("\t");
    Serial.print(beta_d);
    Serial.print("\n");

    Serial.print("\t");
    Serial.print(a_d[0]);
    Serial.print("\n");

    Serial.print("\t");
    Serial.print(b_d[0]);
    Serial.print("\n");

delay(5000);
    
}
