#include <Servo.h>
#include <math.h>

/*
   Link for solar information:(1) https://www.esrl.noaa.gov/gmd/grad/solcalc/ ****use this one
                              (2) https://www.esrl.noaa.gov/gmd/grad/solcalc/azel.html
                              (3) https://www.timeanddate.com/sun/usa/las-cruces
   Las Cruces Coorinate: 32.287989° N(+), -106.75385° W (-)
   Azimuth and Elevation - an angular coordinate system for locating positions in the sky. Azimuth is measured clockwise from true north
   to the point on the horizon directly below the object. Elevation is measured vertically from that point on the horizon up to the object.
   offset_azi_testunit=3;  offset_alt_testunit=3;  (in degree)
*/

#define latitude  32.287989 //latitude position (in degree) of the experimental station (Jett Hall Annex)
#define longitude -106.75385; //longitude position (in degree) of the experimental station (Jett Hall Annex)
#define lat  ((3.1415926 / 180) * latitude) //latitude (in radian) for trigonometric functions



/*june 17 2021 - > day_of_year= 168*/
const unsigned char PTh = 7; //present time in hours*
const unsigned char PTm = 8; //present time in minutes*
const unsigned int day_of_year = 170; //day of the year of the experiment*

const unsigned char mirror_alt_offset[NumOfMirrorUnits] = {0};
const unsigned char mirror_azi_offset[NumOfMirrorUnits] = {0};

const unsigned char NumOfMirrorUnits = 11;

double Hour = 0; //initiation of variable solar hour
double gamma = 0; //initiation of variable gamma used in governing equation
double eqtime = 0; //initiation of variable "eqtime"
double decl = 0; //intiation of variable "decl"
double time_offset = 0; //initiation of variable "time_offset"
double tst = 0; //initiation of variable "tst" (true solar time (in minutes))
double ha = 0; //initiation of variable "ha" (solar hour angle (in radian))
double phi = 0; //initiation of variable "phi"
double alpha = 0; //initiation of variable "alpha"
double beta = 0; //initiation of variable "beta"
double alpha_d = 0; //initiation of variable "alpha_d"
double beta_d = 0; //initiation of variable "beta_d"
double snoon = 0; //initiation of variable "snoon" (solar noon (in minute))
double t_CT = 0; //initiation of variable "t_CT" (current time (in minutes))

double sun_vec_x = 0; //initiation of  (x-component of sun_vector)
double sun_vec_y = 0; //initiation of  (y-component of sun_vector)
double sun_vec_z = 0; //initiation of  (z-component of sun_vector)

double pi = 3.1415926; //value of 'pi'
double r2d = 180 / pi; //convesion ratio of radian to degree conversion
double d2r = pi / 180; //convesion ratio of degree to radian conversion
double h = 23.625; //height(in inch) of convex mirror focal point from the origin(centre point of the base on which these mirror units are intalled)

int timezone = -6; //timezone in hour from UTC (Mountain Daylight Time)


unsigned long CurrTime;
unsigned long LastTime;
//-------------------------------------------------------------------------

//---------------UNIT#3-----------------
//------Focus_Point--------
double mirror_pos_x[NumOfMirrorUnits] = {0}; //x position (in inch) of unit#3
double mirror_pos_y[NumOfMirrorUnits] = { -18.75}; //y position (in inch) of unit#3
double mirror_pos_z[NumOfMirrorUnits] = {h}; //z position (in inch) of unit#3
//target_vector3: unit vector along reflected ray from flat mirror of unit#3

//-------------------------

//-----Bisector_Vector3------
//bisector_vector3: unit vector along bisector on fat mirror which will always be normal to the flat mirror of unit#3
double mirror_vect_x[NumOfMirrorUnits] = {0}; //initiation of variable "_mx" (x-component of bisector_vector3)
double mirror_vect_y[NumOfMirrorUnits] = {0}; //initiation of variable "_my" (y-component of bisector_vector3)
double mirror_vect_z[NumOfMirrorUnits] = {0}; //initiation of variable "_mz" (z-component of bisector_vector3)
//---------------------------

double mirror_alt[NumOfMirrorUnits] = {0}; //initiation of  (mirror alt angle(in radian) calculated based on position and sun vector)
double mirror_azi[NumOfMirrorUnits] = {0}; //initiation of  (mirror azi angle(in radian))


double mirror_alt_mod[NumOfMirrorUnits] = {0}; //mirror altitude angle corrected for motor's inhereted offset
double mirror_azi_mod[NumOfMirrorUnits] = {0}; //mirror azimuth angle corrected for motor's inhereted offset

Servo myservo3_alt; //create servo object to control a servo for altitude adjustment for unit#3
Servo myservo3_azi; //create servo object to control a servo for azimuthal adjustment for unit#3


const unsigned char AltServoPins[NumOfMirrorUnits] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}; //initiallzing servo pins for alt motors
const unsigned char AziServoPins[NumOfMirrorUnits] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}; //initiallzing servo pins for azimuth motors

Servo AltServo[NumOfMirrorUnits];
Servo AziServo[NumOfMirrorUnits];


//-------------------------------

void setup()
{
  for (int i = 0; i < NumOfMirrorUnits; i++)
  {
    AltServo[i].attach(AltServoPins[i]);
    AziServo[i].attach(AziServoPins[i]);
  }
}

void loop()
{
  CurrTime = millis();

  Hour = (((PTh * 60 + PTm) * 60000) + CurrTime) / 3600000; //hour corresponds to current time

  gamma = (2 * pi / 365) * (day_of_year - 1 + ((Hour - 12) / 24)); //fractional year (in radian); [for leap year, use 366 instead of 365 in the denominator]
  eqtime = 229.18 * (0.000075 + 0.001868 * cos(gamma) - 0.032077 * sin(gamma) - 0.014615 * cos(2 * gamma) - 0.040849 * sin(2 * gamma)); //equation of time (in minutes)

  decl = 0.006918 - 0.399912 * cos(gamma) + 0.070257 * sin(gamma) - 0.006758 * cos(2 * gamma) + 0.000907 * sin(2 * gamma) - 0.002697 * cos(3 * gamma) + 0.00148 * sin(3 * gamma);
  //solar declination angle (in radians)
  time_offset = eqtime + 4 * longitude - 60 * timezone; //time offset (in minutes)// because of different time zones

  tst = (PTh * 60 + PTm + time_offset) + (CurrTime / 60000); //true solar time (in minutes) //incorporate time zone offset in solar time

  ha = (d2r) * ((tst / 4) - 180); //solar hour angle(in radian)

  snoon = 360 - 4 * longitude - eqtime; //solar noon for a given location is found from the longitude (in minutes)
  t_CT = (PTh * 60 + PTm) + (CurrTime / 60000); //current time (in minutes)

  phi = acos(sin(lat) * sin(decl) + cos(lat) * cos(decl) * cos(ha)); //solar zenith angle angle (in radian)
  alpha = (pi / 2 - phi); //solar altitude angle (in radian)
  beta = acos((sin(lat) * cos(phi) - sin(decl)) / (cos(lat) * sin(phi))); //solar azimuth angle (in radian)

  if (t_CT <= snoon)
  {
    beta = pi - (beta); //solar azimuth angle (in radian) before solar noon
  }
  if (t_CT > snoon)
  {
    beta = pi + (beta); //solar azimuth angle (in radian) after solar noon
  }
  alpha_d = (r2d) * alpha; //solar altitude/elevation angle (in degree)
  beta_d = (r2d) * (beta); //solar azimuth angle (in degree)

  //------Sun_Vector------
  sun_vec_x = cos(alpha) * cos(beta); //x-component of sun_vector)
  sun_vec_y = -cos(alpha) * sin(beta); //y-component of sun_vector)
  sun_vec_z = sin(alpha); //z-component of sun_vector)
  //----------------------


  for (int i = 0; i < NumOfMirrorUnits; i++)
  {
    Mirror_angles(sun_vec_x, sun_vec_y, sun_vec_z, i);


    if (CurrTime - LastTime > 10000)
    {

      AltServo[i].write(r2d * mirror_alt[i] + mirror_alt_offset[i]);
      AziServo[i].write(r2d * mirror_alt[i] + mirror_alt_offset[i]);

      LastTime = CurrTime;
    }
  }

  //--------------------(4)------------------------



  delay(5000);

}

//--------------------------------

void Mirror_angles(double _sx, double _sy, double _sz, int _mindex)
{
  double _fx = -mirror_pos_x[_mindex] / (sqrt(mirror_pos_x[_mindex] * mirror_pos_x[_mindex] + mirror_pos_y[_mindex] * mirror_pos_y[_mindex] + mirror_pos_z[_mindex] * mirror_pos_z[_mindex])); //x-componenet of target_vector number (_mindex + 1)
  double _fy = -mirror_pos_y[_mindex] / (sqrt(mirror_pos_x[_mindex] * mirror_pos_x[_mindex] + mirror_pos_y[_mindex] * mirror_pos_y[_mindex] + mirror_pos_z[_mindex] * mirror_pos_z[_mindex])); //y-componenet of target_vector number (_mindex + 1)
  double _fz = mirror_pos_z[_mindex] / (sqrt(mirror_pos_x[_mindex] * mirror_pos_x[_mindex] + mirror_pos_y[_mindex] * mirror_pos_y[_mindex] + mirror_pos_z[_mindex] * mirror_pos_z[_mindex])); //z-componenet of target_vector number (_mindex + 1)

  double _mx = (((abs(_sz + _fz)) / (_sz + _fz)) * (_sx + _fx)) * (1 / (sqrt((_sx + _fx) * (_sx + _fx) + (_sy + _fy) * (_sy + _fy) + (_sz + _fz) * (_sz + _fz))));
  double _my = (((abs(_sz + _fz)) / (_sz + _fz)) * (_sy + _fy)) * (1 / (sqrt((_sx + _fx) * (_sx + _fx) + (_sy + _fy) * (_sy + _fy) + (_sz + _fz) * (_sz + _fz))));
  double _mz = (abs(_sz + _fz)) * (1 / (sqrt((_sx + _fx) * (_sx + _fx) + (_sy + _fy) * (_sy + _fy) + (_sz + _fz) * (_sz + _fz))));

  //calculating mirror angles

  if (_mx > 0 && _my >= 0)
  {
    mirror_alt[_mindex] = pi / 2 + acos(_mz);
    mirror_azi[_mindex] = pi / 2 - atan(abs(_my) / abs(_mx));
  }
  else if (_mx <= 0 && _my > 0)
  {
    mirror_alt[_mindex] = pi / 2 - acos(_mz);
    mirror_azi[_mindex] = pi / 2 + atan(abs(_my) / abs(_mx));
  }
  else if (_mx < 0 && _my <= 0)
  {
    mirror_alt[_mindex] = pi / 2 - acos(_mz);
    mirror_azi[_mindex] = pi / 2 - atan(abs(_my) / abs(_mx));
  }
  else if (_mx >= 0 && _my < 0)
  {
    mirror_alt[_mindex] = pi / 2 + acos(_mz);
    mirror_azi[_mindex] = pi / 2 + atan(abs(_my) / abs(_mx));
  }
  else {
    mirror_azi[_mindex] = mirror_azi[_mindex];
  }

  mirror_alt[_mindex] = (r2d) * mirror_alt[_mindex]; //elevation angle (in degree) of flat mirror of unit#3
  mirror_azi[_mindex] = (r2d) * mirror_azi[_mindex]; //azimuth angle (in degree) of flat mirror of unit#3
}
