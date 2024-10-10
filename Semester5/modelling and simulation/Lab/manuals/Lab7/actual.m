%% In this code, we are gonna implement the ODE45 on rotational mechanical system 
function dz= actual(t,z)
%% Entering the Coefficients
T=4;j1=1;j2=50;k=3;D1=0.03;D2=0.02;D3=0.01;
%% State Variables for theoretically interpolating the dynamic system
%% For the first mass   
dz(1)=z(2);
dz(2)=1/j1*(T-D1*z(2)-k*z(1)+k*z(3));
%% For the second mass
dz(3)=z(4); 
dz(4)=1/j2*(-D2*z(4)-k*z(3)+k*z(1));  
dz=dz';
end