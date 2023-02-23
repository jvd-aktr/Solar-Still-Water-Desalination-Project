clc; close all;

% a_s=deg2rad(158.454753);  %Azimuth
% alpha=deg2rad(90-18.259991);  %Elevation=90-Zenith
% I_x=cos(alpha)*sin(a_s);
% I_y=-cos(alpha)*cos(a_s);
% I_z=sin(alpha);
LTH=11;   %Local Hour
LTM=15/60;
Lat=32.32;
Long=-106.74;
delT=-7;  %Time zone of Las Cruces
t=[datetime('today')];
N=day(t,'dayofyear');
%del_s=(deg2rad(23.45)*sin(deg2rad(360/365)*(284+N))) %Solar Declination
X=deg2rad((360*(N-1))/365.242);
EoT=229.2*(0.000075+(0.001868*cos(X))-(0.032077*sin(X))-(0.014615*cos(2*X))-(0.04089*sin(2*X)))
LSTM=15*delT;  %Conversion to deg
ST=LTH+(((4*(LSTM-Long))+EoT)/60)+LTM;
del=0.006918-0.399912*cos(X)+0.070257*sin(X)-0.006758*cos(2*X)+0.00907*sin(2*X)-0.002679*cos(3*X)+0.00148*sin(3*X)
hour_angle=deg2rad(15*(ST-12))
zenith_s=acos((cos(deg2rad(Lat))*cos(del)*cos(hour_angle))+(sin(deg2rad(Lat))*sin(del)))
decl=asin((0.39795*cos(0.98563*(deg2rad(N-173)))))
azimuth=sign(hour_angle)*abs(acos(((cos(zenith_s)*sin(deg2rad(Lat)))-sin(del))/(sin(zenith_s)*cos(deg2rad(Lat)))))
azimuth_deg=rad2deg(azimuth)
% phy_prime=(asin((cos(del)*sin(hour_angle))/sin(zenith_s)))
%Real_azimuth=deg2rad(mod(rad2deg(azimuth),180))   %Azimuth angle
Altitude=90-rad2deg(zenith_s)
% if cos(hour_angle)>=(tan(del)/tan(deg2rad(Lat))) 
%     gamma_s=180-rad2deg(phy_prime)
% else
%     gamma_s=180+rad2deg(phy_prime)
% end

Hh=10*0.0254;
Ht=72.5*0.0254;
R=72.5*0.0254;
d=sqrt((R^2)+(Ht-Hh)^2);
atr=atan((Ht-Hh)/R);
atr_deg=rad2deg(atr)
betahs=rad2deg((atr+deg2rad(Altitude))/2)
%Right Mirror X=72.*0.0254  Y=0
Y=0;
X=72.5*0.0254;
Z=0;
r_x=-X/sqrt((X^2)+(Y^2)+((Ht-Hh)^2))  %heliostat position w/r to receiver;
r_y=-Y/sqrt((X^2)+(Y^2)+((Ht-Hh)^2))  %heliostat position w/r to receiver;
r_z=(Ht)/sqrt((X^2)+(Y^2)+((Ht-Hh)^2))%heliostat position w/r to receiver;
r_vec=[r_x,r_y,r_z]
s_x=(-cos(hour_angle)*sin(deg2rad(Lat))*cos(decl))+(cos(deg2rad(Lat))*sin(decl));
s_y=sin(hour_angle)*cos(decl);
s_z=(cos(hour_angle)*sin(deg2rad(Lat))*cos(decl))+(cos(deg2rad(Lat))*sin(decl));
s_vec=[s_x,s_y,s_z]
m_x=((abs(s_z+r_z))/(s_z+r_z))*(s_x+r_x)*(1/(sqrt((s_x+r_x)^2)+((s_y+r_y)^2)+((s_z+r_z)^2)));
m_y=((abs(s_z+r_z))/(s_z+r_z))*(s_y+r_y)*(1/(sqrt((s_x+r_x)^2)+((s_y+r_y)^2)+((s_z+r_z)^2)));
m_z=(abs(s_z+r_z))*(1/(sqrt((s_x+r_x)^2)+((s_y+r_y)^2)+((s_z+r_z)^2)));
m_vec=[m_x,m_y,m_z]
origin=[-1,0,0]

ANGLE_X=90-(rad2deg(acos(dot(s_vec,origin))))
theta=acos((s_x*m_x)+(s_y*m_y)+(s_z*m_z))
quiver3(0,0,0,r_x,r_y,r_z)
hold on
quiver3(0,0,0,s_x,s_y,s_z)
hold on
quiver3(0,0,0,m_x,m_y,m_z)
hold on
quiver3(0,0,0,origin_x,origin_y,origin_z)
hold on
[x,y,z]=sphere;
axis equal
r_r=0.05;
X_r=x*r_r;
Y_r=y*r_r;
Z_r=z*r_r;
surf(X_r+r_x,Y_r+r_y,Z_r+r_z)