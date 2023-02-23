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
  int PTh=9;  //present time in hours*
  int PTm=45; //present time in minutes*
  int SNh=13; //solar noon in hours*
  int SNm=3; //solar noon in minutes*
  int SSh=19; //sun set time in hours*
  int SSm=49; //sun set time in minutes*
  
  int t1h=SNh-2; //2 hrs before solar noon
  int t1m=SNm;
  int t2h=SNh+2; //2 hrs after solar noon
  int t2m=SNm;
  
  double PT_alpha_d=42.38; //altitude angle at present time*
  double SNa_d=73.63; //solar noon altitude angle (in degree)*
  double t1_alpha_d=58.26; //altitude angle 2 hrs before solar noon*
  double t2_alpha_d=58.29; //altitude angle 2 hrs after solar noon*
  double alpha_L_d=-0.52; //altutude/elevation angle (in degree) at sun set*
  
  double PT_beta_d=97.96; //azimuthal angle @ present time*
  double SNb_d=180; //solar noon azimuthal angle (in degree)*
  double t1_beta_d=113.93; //azimuth angle 2 hrs before solar noon*
  double t2_beta_d=246.14; //azimuth angle 2 hrs after solar noon*
  double beta_L_d=289.61; //azimuth angle (in degree) at sun set*
  
 
  double h=23.625; //height(in inch) of convex mirror focal point from the origin(centre point of the base on which these mirror units are intalled)
//----------------------------------------------------

//--------------Calculation in degree-----------------

double t_alt_11_d= ((t1h-PTh)*60 +(t1m-PTm))*60000/(t1_alpha_d-PT_alpha_d); //time for altitude (in ms) between PT & 2hrs before SN
double t_alt_12_d= ((SNh-t1h)*60 +(SNm-t1m))*60000/(SNa_d-t1_alpha_d); //time for altitude (in ms) between 2hrs before SN & SN
double t_alt_21_d= ((t2h-SNh)*60 +(t2m-SNm))*60000/(SNa_d-t2_alpha_d); //time for altitude (in ms)between SN & 2hrs after SN
double t_alt_22_d= ((SSh-t2h)*60 +(SSm-t2m))*60000/(t2_alpha_d-alpha_L_d); //time for altitude (in ms)between SN & 2hrs after SN

double t_azi_11_d= ((t1h-PTh)*60 +(t1m-PTm))*60000/(t1_beta_d-PT_beta_d); //time for altitude (in ms) between PT & 2hrs before SN
double t_azi_12_d= ((SNh-t1h)*60 +(SNm-t1m))*60000/(SNb_d-t1_beta_d); //time for altitude (in ms) between 2hrs before SN & SN
double t_azi_21_d= ((t2h-SNh)*60 +(t2m-SNm))*60000/(t2_beta_d-SNb_d); //time for altitude (in ms)between SN & 2hrs after SN
double t_azi_22_d= ((SSh-t2h)*60 +(SSm-t2m))*60000/(beta_L_d-t2_beta_d); //time for altitude (in ms)between SN & 2hrs after SN

double t_1= ((t1h-PTh)*60 +(t1m-PTm))*60000; //time difference between 2hrs before solar noon and present time (in ms)
double t_sn= ((SNh-PTh)*60 +(SNm-PTm))*60000; //time difference between solar noon and present time (in ms)
double t_2= ((t2h-PTh)*60 +(t2m-PTm))*60000; //time difference between 2hrs after solar noon and present time (in ms)


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

  int a3_11_d= 90 -((theta3_d-PT_alpha_d)/2); //intial angle for altitude adjustment of unit#3 for t_alt_11_d
  int a3_12_d= 90 -((theta3_d-t1_alpha_d)/2); //intial angle for altitude adjustment of unit#3 for t_alt_12_d
  int a3_21_d= 90 -((theta3_d-SNa_d)/2); //intial angle for altitude adjustment of unit#3 for t_alt_21_d
  int a3_22_d= 90 -((theta3_d-t2_alpha_d)/2); //intial angle for altitude adjustment of unit#3 for t_alt_22_d

  int b3_11_d= 45 +(PT_beta_d)/2; //intial angle for azimuthal adjustment of unit#3 for t_azi_11_d
  int b3_12_d= 45 +(t1_beta_d)/2; //intial angle for azimuthal adjustment of unit#3 for t_azi_12_d
  int b3_21_d= 45 +(SNb_d)/2; //intial angle for azimuthal adjustment of unit#3 for t_azi_21_d
  int b3_22_d= 45 +(t2_beta_d)/2; //intial angle for azimuthal adjustment of unit#3 for t_azi_22_d

  
  Servo myservo3_alt; //create servo object to control a servo for altitude adjustment for unit#3
  Servo myservo3_azi; //create servo object to control a servo for azimuthal adjustment for unit#3
//-------------------------------

//---------UNIT#4-------------
  double x4= -18.75; //x position (in inch) of unit#4
  int y4=0; //y position (in inch) of unit#4
  double d4=sqrt(x4*x4+y4*y4); //distance (in inch) of unit#4 from origin(centre point of the base on which these mirror units are intalled)
  double theta4_d= (180/3.1415926)*atan(h/d4); //angle in degree

  int a4_11_d= ((theta4_d+PT_alpha_d)/2); //intial angle for altitude adjustment of unit#4 for t_alt_11_d
  int a4_12_d= ((theta4_d+t1_alpha_d)/2); //intial angle for altitude adjustment of unit#4 for t_alt_12_d
  int a4_21_d= ((theta4_d+SNa_d)/2); //intial angle for altitude adjustment of unit#4 for t_alt_21_d
  int a4_22_d= ((theta4_d+t2_alpha_d)/2); //intial angle for altitude adjustment of unit#4 for t_alt_22_d

  int b4_11_d= -45 +(PT_beta_d)/2; //intial angle for azimuthal adjustment of unit#4 for t_azi_11_d
  int b4_12_d= -45 +(t1_beta_d)/2; //intial angle for azimuthal adjustment of unit#4 for t_azi_12_d
  int b4_21_d= -45 +(SNb_d)/2; //intial angle for azimuthal adjustment of unit#4 for t_azi_21_d
  int b4_22_d= -45 +(t2_beta_d)/2; //intial angle for azimuthal adjustment of unit#4 for t_azi_22_d

  
  Servo myservo4_alt; //create servo object to control a servo for altitude adjustment for unit#4
  Servo myservo4_azi; //create servo object to control a servo for azimuthal adjustment for unit#4
//-------------------------------


void setup() 
{
    
//---------------UNIT#3--------------------
  myservo3_alt.attach(5); //attaches the 'altitude adjustment servo' of unit#3 on pin 5 to the servo object
  myservo3_azi.attach(6); //attaches the 'azimuthal adjustment servo' of unit#3 on pin 6 to the servo object
//----------------------------------------- 

//---------------UNIT#3--------------------
  myservo4_alt.attach(7); //attaches the 'altitude adjustment servo' of unit#4 on pin 7 to the servo object
  myservo4_azi.attach(8); //attaches the 'azimuthal adjustment servo' of unit#4 on pin 8 to the servo object
//-----------------------------------------

}

void loop() 
  {
    myTime= millis();

//---------------Alt_Adj for UNITS--------------------
if(myTime < t_1 && (myTime-last_seen_alt) > t_alt_11_d)
       {PT_alpha_d++;
                        //...........(3)............//
          a3_11_d= 90 -((theta3_d-PT_alpha_d)/2); //mirror angle for altitude adjustment of unit#3 for Alt_t11_d
          myservo3_alt.write(a3_11_d);
                        //...........(4)............//
          a4_11_d= ((theta4_d+PT_alpha_d)/2); //intial angle for altitude adjustment of unit#4 for t_alt_11_d
          myservo4_alt.write(a4_11_d);
                        //...........(5)............//
         
     
        last_seen_alt = myTime;  
       }
      if(t_1 < myTime < t_sn && (myTime-last_seen_alt) > t_alt_12_d) 
          {t1_alpha_d++;
                        //...........(3)............//
             a3_12_d= 90 -((theta3_d-t1_alpha_d)/2); //mirror angle for altitude adjustment of unit#3 for Alt_t12_d
             myservo3_alt.write(a3_12_d);
                        //...........(4)............//
             a4_12_d= ((theta4_d+t1_alpha_d)/2); //intial angle for altitude adjustment of unit#4 for t_alt_12_d
             myservo4_alt.write(a4_12_d);
                        //...........(5)............//
  
           
           last_seen_alt = myTime;
          }
      if(t_sn < myTime < t_2 && (myTime-last_seen_alt) > t_alt_21_d)
          {SNa_d--;
                        //...........(3)............//
             a3_21_d= 90 -((theta3_d-SNa_d)/2); //mirror angle for altitude adjustment of unit#3 for Alt_t21_d 
             myservo3_alt.write(a3_21_d);
                        //...........(4)............//
             a4_21_d= ((theta4_d+SNa_d)/2); //intial angle for altitude adjustment of unit#4 for t_alt_21_d
             myservo4_alt.write(a4_21_d);
                        //...........(5)............//
             

           last_seen_alt = myTime;
          }
      if(myTime > t_2 && (myTime-last_seen_alt) > t_alt_22_d)
          {t2_alpha_d--;
                        //...........(3)............//
             a3_22_d= 90 -((theta3_d-t2_alpha_d)/2); //mirror angle for altitude adjustment of unit#3 for Alt_t22_d
             myservo3_alt.write(a3_22_d);
                        //...........(4)............//
             a4_22_d= ((theta4_d+t2_alpha_d)/2); //intial angle for altitude adjustment of unit#4 for t_alt_22_d
             myservo4_alt.write(a4_22_d);
                        //...........(5)............//

             

           last_seen_alt = myTime;
          }
          
//-----------------Azi_Adj for UNITS-------------------

       if(myTime < t_1 && (myTime-last_seen_azi) > t_azi_11_d)
         {PT_beta_d++;
                        //...........(3)............//
            b3_11_d= 45 +(PT_beta_d)/2; //arm angle for azimuthal adjustment of unit#3 for t_azi_11_d
            myservo3_azi.write(b3_11_d);
                        //...........(4)............//
            b4_11_d= -45 +(PT_beta_d)/2; //intial angle for azimuthal adjustment of unit#4 for t_azi_11_d
            myservo4_azi.write(b4_11_d);
                        //...........(5)............//

            
        
          last_seen_azi = myTime;
         }
       if(t_1 < myTime < t_sn && (myTime-last_seen_azi) > t_azi_12_d)
         {t1_beta_d++;
                        //...........(3)............//
            b3_12_d= 45 +(t1_beta_d)/2; //arm angle for azimuthal adjustment of unit#3 for t_azi_12_d
            myservo3_azi.write(b3_12_d);
                        //...........(4)............//
            b4_12_d= -45 +(t1_beta_d)/2; //intial angle for azimuthal adjustment of unit#4 for t_azi_12_d
            myservo4_azi.write(b4_12_d);
                        //...........(5)............//
            
        
          last_seen_azi = myTime;
         }
       if(t_sn < myTime < t_2 && (myTime-last_seen_azi) > t_azi_21_d)
         {SNb_d++;
                        //...........(3)............//
            b3_21_d= 45 +(SNb_d)/2; //arm angle for azimuthal adjustment of unit#3 for t_azi_21_d
            myservo3_azi.write(b3_21_d);
                        //...........(4)............//
            b4_21_d= -45 +(SNb_d)/2; //intial angle for azimuthal adjustment of unit#4 for t_azi_21_d
            myservo4_azi.write(b4_21_d);
                        //...........(5)............//

            
        
          last_seen_azi = myTime;
         }
       if(myTime > t_2 && (myTime-last_seen_azi) > t_azi_22_d)
         {t2_beta_d++;
                        //...........(3)............//
            b3_22_d= 45 +(t2_beta_d)/2; //arm angle for azimuthal adjustment of unit#3 for t_azi_22_d
            myservo3_azi.write(b3_22_d);
                        //...........(4)............//
            b4_22_d= -45 +(t2_beta_d)/2; //intial angle for azimuthal adjustment of unit#4 for t_azi_22_d
            myservo4_azi.write(b4_22_d);
                        //...........(5)............//


            
          last_seen_azi = myTime;
         }
       
//-----------------------------------------
    
    delay(10000);
  }
