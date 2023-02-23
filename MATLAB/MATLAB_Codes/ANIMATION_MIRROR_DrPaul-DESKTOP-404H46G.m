clc; close all; clear all; clear variables;

% https://gml.noaa.gov/grad/solcalc/
%Local Hour
%%%%%%%%%%%%%%%%%%%%%%JAVED's
Lat=deg2rad(32.287989); %Latitude of Las Cruces (Javed's place)
Long=-106.75385; %Longitud of Las Cruces (Javed's place)
delT=-6;  %Time zone of Las Cruces
t=(datetime('today'));  %day of today
N=day(t,'dayofyear')-3;  %Convert today to # of day of the year
HOUR=linspace(6,20,15)';  %Vector from dawn to dusk
MINS=0/60;   %Add minutes to hours
MINS_TST=0;
SC=0/60;
SC_TST=0;

%X_in=[28.3125,28.3125,18.75,-18.75,-28.3125,-28.3125,-9.375,9.375,18.9357,-18.9357];
%Y_in=[-13.5,13.5,0,0,13.5,-13.5,18.75,18.75,28.3125,28.3125];
X_in=[-13.5,13.5,0,0,13.5,-13.5,18.75,18.75,28.3125,28.3125];
Y_in=-[28.3125,28.3125,18.75,-18.75,-28.3125,-28.3125,-9.375,9.375,18.9357,-18.9357];
Z_in=[0,0,0,0,0,0,0,0,0,0];
X=X_in*0.0254;
Y=Y_in*0.0254;
Z=Z_in*0.0254;

% myVideo = VideoWriter('myVideoFile'); %open video file
% myVideo.FrameRate = 10;
% open(myVideo)
for kk=linspace(1,15,15)
    for i=linspace(1,10,10)
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        TIME=HOUR(kk)+MINS+SC;  %Total time including hour+minutes
        GAMMA=((2*pi)/365)*(N-1+((TIME-12)/24));  %Fractional year
        EQN_OT=229.18*(0.000075+(0.001868*cos(GAMMA))-(0.032077*sin(GAMMA))-(0.014615*cos(2*GAMMA))-(0.040849*sin(2*GAMMA)));  %Equation of Time
        DECL=0.006918-(0.399912*cos(GAMMA))+(0.070257*sin(GAMMA))-(0.006758*cos(2*GAMMA))+(0.000907*sin(2*GAMMA))-(0.002697*cos(3*GAMMA))+(0.00148*sin(3*GAMMA)); %Solar Declination
        TIME_OFFSET=EQN_OT+(4*Long)-(60*delT);  %Time offset in minutes.  %Longitude should be in degrees
        TST=(HOUR(kk)*60)+MINS_TST+(SC_TST/60)+TIME_OFFSET;   %True Solar Time
        HA=deg2rad((TST/4)-180);   %solar hour angle in rad
        ZENITH_S=acos((sin(Lat)*sin(DECL))+(cos(Lat)*cos(DECL)*cos(HA)));
        S_NOON=deg2rad(360-(4*Long)-EQN_OT);
        ALPHA_S=(pi/2)-ZENITH_S; %Solar altitude angle (Rad)
        AZIMUTH_S=acos(((sin(Lat)*cos(ZENITH_S))-sin(DECL))/(cos(Lat)*sin(ZENITH_S)));
        if TIME<=S_NOON
            AZIMUTH_SS=pi-AZIMUTH_S;

        else
            AZIMUTH_SS=pi+AZIMUTH_S;

        end
        ALPHA_S_DEG(kk,:)=rad2deg(ALPHA_S);
        AZIMUTH_SS_DEG(kk,:)=rad2deg(AZIMUTH_SS);
        
        Hh=0*0.0254;...10*0.0254;
        Ht=(0+23.625)*0.0254;...72.5*0.0254;

        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        r_x=-X(i)/sqrt((X(i)^2)+(Y(i)^2)+((Ht-Hh)^2));  %heliostat position w/r to receiver;
        r_y=-Y(i)/sqrt((X(i)^2)+(Y(i)^2)+((Ht-Hh)^2));  %heliostat position w/r to receiver;
        r_z=(Ht-Hh)/sqrt((X(i)^2)+(Y(i)^2)+((Ht-Hh)^2));%heliostat position w/r to receiver;
        r_z0=[0 0 0 0 0 0 0 0 0 0]';
        r_vec(i,:)=[r_x;r_y;r_z]';
        r_vec_z0(i,:)=[r_x;r_y;r_z0(i)]';
%         s_x=(-cos(HA)*sin(Lat)*cos(DECL))+(cos(Lat)*sin(DECL));
%         s_y=sin(HA)*cos(DECL);
%         s_z=(cos(HA)*cos(Lat)*cos(DECL))+(sin(Lat)*sin(DECL));
        s_x=cos(ALPHA_S)*cos(AZIMUTH_SS);
        s_y=-cos(ALPHA_S)*sin(AZIMUTH_SS);
        s_z=sin(ALPHA_S);
        s_vec(kk,:)=[s_x;s_y;s_z]';
        m_x=(((abs(s_z+r_z))/(s_z+r_z))*(s_x+r_x))*(1/(sqrt(((s_x+r_x)^2)+((s_y+r_y)^2)+((s_z+r_z)^2))));
        m_y=(((abs(s_z+r_z))/(s_z+r_z))*(s_y+r_y))*(1/(sqrt(((s_x+r_x)^2)+((s_y+r_y)^2)+((s_z+r_z)^2))));
        m_z=(abs(s_z+r_z))*(1/(sqrt(((s_x+r_x)^2)+((s_y+r_y)^2)+((s_z+r_z)^2))));
        m_vec(kk,i,:)=[m_x;m_y;m_z]';
        ANGLE_X=(rad2deg(acos(dot(squeeze(m_vec(kk,i,:)),squeeze(r_vec_z0(i,:)))/(norm(squeeze(m_vec(kk,i,:)))*norm(squeeze(r_vec_z0(i,:)))))));
        Table_Elevation(kk,i,:)=ANGLE_X;
        Cos_dir_x(kk,i,:)=rad2deg(acos(m_vec(kk,i,1)/(norm(squeeze(m_vec(kk,i,:))))));
        Cos_dir_y(kk,i,:)=rad2deg(acos(m_vec(kk,i,2)/(norm(squeeze(m_vec(kk,i,:))))));
        Cos_dir_z(kk,i,:)=rad2deg(acos(m_vec(kk,i,3)/(norm(squeeze(m_vec(kk,i,:))))));
        aaa_x=cosd(Cos_dir_x(kk,i,:));
        aaa_y=cosd(Cos_dir_y(kk,i,:));
        aaa_z=cosd(Cos_dir_z(kk,i,:));
        aaa_vec(kk,i,:)=[aaa_x;aaa_y;aaa_z]';
        theta(kk,i,:)=acos((s_x*m_x)+(s_y*m_y)+(s_z*m_z));
%         THETA_1(kk,i,:)=(acos((r_vec(i,3)*sin(ALPHA_S))+(-r_vec(i,2)*cos(ALPHA_S)*sin(AZIMUTH_SS))+(r_vec(i,1)*cos(ALPHA_S)*cos(AZIMUTH_SS))))/2;
        
        A=[m_vec(kk,i,1) m_vec(kk,i,2) m_vec(kk,i,3)];
        null_A=null(A)';
        ELEV(kk,i,:)=rad2deg(acos(m_z));
        ALPHA_SURFACE(kk,i,:)=(asin((r_vec(i,3)+sin(ALPHA_S))/(2*cos(theta(kk,i,:)))));
        AZIMUTH_SURF(kk,i,:)=acos((r_vec(i,1)+(cos(ALPHA_S)*cos(AZIMUTH_SS)))/(2*cos(theta(kk,i,:))*cos(ALPHA_SURFACE(kk,i,:))));
        AZIMUTH_SURF_2(kk,i,:)=asin((-r_vec(i,2)+(cos(ALPHA_S)*sin(AZIMUTH_SS)))/(2*cos(theta(kk,i,:))*cos(ALPHA_SURFACE(kk,i,:))));
        ALPHA_SURFACE_DEG(kk,i,:)=rad2deg(ALPHA_SURFACE(kk,i,:));
        AZIMUTH_SURF_DEG(kk,i,:)=rad2deg(AZIMUTH_SURF(kk,i,:));
        AZIMUTH_SURF_DEG_2(kk,i,:)=rad2deg(AZIMUTH_SURF_2(kk,i,:));
        %mirror_w_r_h(kk,i,:)=ELEV(kk,i,:)-theta(kk,i,:);
%         mirror_w_r_v(kk,i,:)=90-ELEV(kk,i,:);
%         gamma_m(kk,i,:)=atan2(m_vec(kk,i,1),-m_vec(kk,i,2));
%         gamma_m_deg(kk,i,:)=rad2deg(gamma_m(kk,i,:));
        if m_vec(kk,i,1)>0 && m_vec(kk,i,2)>=0
            gamma_m(kk,i,:)=rad2deg(pi-atan(abs(m_vec(kk,i,2))/abs(m_vec(kk,i,1))));
        elseif m_vec(kk,i,1)<=0 && m_vec(kk,i,2)>0
            gamma_m(kk,i,:)=rad2deg(atan(abs(m_vec(kk,i,2))/abs(m_vec(kk,i,1))));
        elseif m_vec(kk,i,1)<0 && m_vec(kk,i,2)<=0
            gamma_m(kk,i,:)=rad2deg(-atan(abs(m_vec(kk,i,2))/abs(m_vec(kk,i,1))));
        else
            gamma_m(kk,i,:)=rad2deg(-pi+atan(abs(m_vec(kk,i,2))/abs(m_vec(kk,i,1))));
        end
%         
        
        %%%Orthonormal vectors
%                 quiver3(X(i),Y(i),Z(i),null_A(1,1),null_A(1,2),null_A(1,3),'k')
%                 axis equal
%                 hold on
%                 quiver3(X(i),Y(i),Z(i),null_A(2,1),null_A(2,2),null_A(2,3),'k')
%                 axis equal
%                 hold on
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        %%%%%%%%%%%Basin and Floor
        % axis off
        % axis equal
        % patch([1 -1 -1 1], [1 1 -1 -1], [0 0 0 0], [1 1 -1 -1],'FaceColor','w')
        % hold on
        % my_vertices = [0.1 0.1 0; 0.1 -0.1 0; -0.1 -0.1 0; -.1 0.1 0; 0.1 0.1 -0.1; 0.1 -0.1 -0.1; -0.1 -0.1 -0.1; -.1 0.1 -0.1];
        % my_faces = [1 2 3 4; 2 6 7 3; 4 3 7 8; 1 5 8 4; 1 2 6 5; 5 6 7 8];
        % axis off
        % axis equal
        % patch('Vertices', my_vertices, 'Faces', my_faces, 'FaceColor', 'c');
        % hold on
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        
        
%         %%%%%%%%%%%%%%%%%%%%%%%%%
%                 axis off
%                 axis equal
%                 [x,y,z]=sphere;
%                 axis equal
%                 r_r=0.05;
%                 X_r=x*r_r;
%                 Y_r=y*r_r;
%                 Z_r=z*r_r;
%                 surf(X_r,Y_r,Z_r+Ht)
%                 colormap jet
%                 hold on
%                 axis off
%                 axis equal
%         % %         quiver3(X(i),Y(i),0,-X(i),-Y(i),Z(i))
%         % %         axis equal
%         % %         hold on
%                 fxx=@(s,t) ((null_A(1,1)*s + null_A(2,1)*t)*0.03)+X(i);
%                 fyy=@(s,t) ((null_A(1,2)*s + null_A(2,2)*t)*0.03)+Y(i);
%                 fzz=@(s,t) ((null_A(1,3)*s + null_A(2,3)*t)*0.03)+Z(i);
%                 fmesh(fxx,fyy,fzz,'MeshDensity',4,'EdgeColor','c','LineWidth',2)
%                 camlight
%                 axis equal
%                 hold on
%                 qq=quiver3(X(i),Y(i),Z(i),r_vec(i,1),r_vec(i,2),r_vec(i,3),'b','LineWidth',3)
%                 axis equal
%                 hold on
%                 qqq=quiver3(X(i),Y(i),Z(i),s_vec(kk,1),s_vec(kk,2),s_vec(kk,3),'r','LineWidth',3)
%                 qqq.ShowArrowHead = 'off';
%                 axis equal
%                 hold on
%         quiver3(X(i),Y(i),Z(i),m_vec(kk,i,1),m_vec(kk,i,2),m_vec(kk,i,3),'k')
%         % axis equal
%         hold on
%                 an=animatedline((s_vec(kk,1)*3),(s_vec(kk,2)*3),(s_vec(kk,3)*3),'Marker','p','Color','r','MarkerSize',20,'LineStyle','-','LineWidth',10)
%                 axis equal
%                 pause(0.05)
%                 hold on
%                 view(270,90)
%                 drawnow
        %         %%%%%%%%%%%%%%%%%%%%%%%
%         %
%         %
%         % %         [x,y,z]=sphere;
%         % %         axis equal
%         % %         r_r=0.25;
%         % %         X_sun=x*r_r;
%         % %         Y_sun=y*r_r;
%         % %         Z_sun=z*r_r;
%         % %         surf(X_sun+(s_vec(kk,1)*3),Y_sun+(s_vec(kk,2)*3),Z_sun+(s_vec(kk,3)*3))
%         % %         colormap hot
%         % %         drawnow
%         
%                 frame = getframe(gcf); %get frame
%                 writeVideo(myVideo, frame);
%         
    end
%         clf
end
% close(myVideo)
