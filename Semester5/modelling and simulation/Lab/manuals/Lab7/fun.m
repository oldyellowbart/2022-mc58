%% In this code, we are gonna implement the ODE45 on rotational mechanical system 
function dz= fun(~,z)
%% Entering the Coefficients
T=4;j1=1;j2=10;j3=6;k=3;D1=0.03;D2=0.02;D3=0.01;
%% State Variables for theoretically interpolating the dynamic system
%% For the first Inertial body
dz(1)=z(2);
dz(2)=1/j1*(T-D1*z(2)-k*z(1)+k*z(3));
%% For the second Inertial Body
dz(3)=z(4); 
dz(4)=1/j2*(-D2*z(4)+-D2*z(6)-k*z(3)+k*z(1));
%% For the third Inertial Body
dz(5)=z(6); 
dz(6)=1/j3*(-D2*z(5)+D2*z(4)-D3*z(6));
dz=dz';
end