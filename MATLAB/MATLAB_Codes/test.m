clc; clear all; close all; clear variables;

m_vec=[0,0]
        if m_vec(1)>0 && m_vec(2)>=0
            gamma_m=rad2deg(pi-atan(abs(m_vec(2))/abs(m_vec(1))));
        elseif m_vec(1)<=0 && m_vec(2)>0
            gamma_m=rad2deg(atan(abs(m_vec(2))/abs(m_vec(1))));
        elseif m_vec(1)<0 && m_vec(2)<=0
            gamma_m=rad2deg(-atan(abs(m_vec(2))/abs(m_vec(1))));
        elseif m_vec(1)>=0 && m_vec(2)<0
            gamma_m=rad2deg(-pi+atan(abs(m_vec(2))/abs(m_vec(1))));
        else
            gamma_m=pi;
        end