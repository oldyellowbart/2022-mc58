% %% Lab#05 T
%Translational and Rotational Mechanical Systems
% Example#01 (Simple Mass Spring Damper)
% X0=[0;1];%initial condiitons are zero
% TR=[0:1:10];%time response RANGE
% %t=0:0.1:50;
% [t,x]=ode45(@func1,TR,X0)
% Displacement=x(:,1);
% Velocity=x(:,2);
% plot(t, Displacement)
% hold on
% plot(t,Velocity)
% ac1=diff(Velocity)./diff(t)
% %acceleration = gradient(Velocity)
% plot(t,[2;ac1]) % Change the IC of Acceleration by manipulating 0,1,2
% %plot(t,[0;ac1])
% ylabel('x,v,a')
% xlabel('time')
% legend("displacement","velocity","acceleration");
% %state variables give the following function
% function dx = func1(t,x)
%     M=10;B=5;K=5;F=5;
%     dx(1)=x(2)%for x dot
%     dx(2)=(F-B*x(2)-K*x(1))/M    %for x dot dot
%     dx = dx';
% end
% Example#02 (Dual Mass System)
% clc;
% TR = [0 10];
% X0 = [0;6;-1;3];
% [t,y]=ode45(@fun,TR,X0);
% x1=y(:,1);%cloumn1
% v1=y(:,2);%cloumn2
% x2=y(:,3);%cloumn3
% v2=y(:,4);%cloumn4
% a1=gradient(v1);
% a2=gradient(v2);
% subplot(2,3,1)
% plot(t,x1)
% xlabel('time')
% ylabel('Displacement-1')
% subplot(2,3,2)
% plot(t,v1)
% xlabel('time')
% ylabel('Velocity-1')
% subplot(2,3,3)
% plot(t,a1)
% xlabel('time')
% ylabel('Acceleration-1')
% subplot(2,3,4)
% plot(t,x2)
% xlabel('time')
% ylabel('Displacement-2')
% subplot(2,3,5)
% plot(t,v2)
% xlabel('time')
% ylabel('Velocity-2')
% subplot(2,3,6)
% plot(t,a2)
% xlabel('time')
% ylabel('Acceleration-2')
% function dy= fun(t,y)
%    f=5;
%    m1=1;%mass1
%    m2=4;%mass2
%    k1=3;
%    k2=2;
%    k3=1;
%    fv1=0.03;
%    fv2=0.02;
%    fv3=0.01;
%    dy(1)=y(2);
%    dy(3)=y(4);
%    dy(2)=1/m1*(f-(k1+k2)*y(1)-(fv1+fv3)*y(2)+fv3*y(4)+k2*y(3));
%    dy(4)=1/m2*(-(fv2+fv3)*y(4)-(k2+k3)*y(3)+k2*y(1)+fv3*y(2));  
%    dy=dy';
% end
Simple Multivariable
TR = [0 100];
X0 = [0;6;-1;3];
for Fa=1:1:5
[t,x]=ode45(@(t,x) var(t,x,Fa),TR,X0)
disp1=x(:,1)
velocity1=x(:,2)
disp2=x(:,3)
velocity2=x(:,4)
subplot(1,4,1); hold on;
plot(t,disp1)
legend('Fa=1', 'Fa=2', 'Fa=3', 'Fa=4', 'Fa=5')
xlabel('time')
ylabel('Displacement-1')
subplot(1,4,2); hold on;
plot(t,velocity1)
legend('Fa=1', 'Fa=2', 'Fa=3', 'Fa=4', 'Fa=5')
xlabel('time')
ylabel('Velocity-1')
subplot(1,4,3); hold on;
plot(t,disp2)
legend('Fa=1', 'Fa=2', 'Fa=3', 'Fa=4', 'Fa=5')
xlabel('time')
ylabel('Displacement-2')
subplot(1,4,4); hold on;
plot(t,velocity2)
legend('Fa=1', 'Fa=2', 'Fa=3', 'Fa=4', 'Fa=5')
xlabel('time')
ylabel('Velocity-2')
end
function dx= var(t,x,Fa)
   m1=10;
   m2=40;
   k=4;
   b1=3;
   b2=5;
   dx(1)=x(2);
   dx(3)=x(4);
   dx(2)=(1/m1)*(-k*x(1)-b1*x(2)+k*x(3)+Fa);
   dx(4)=(1/m2)*(k*x(1)-2*k*x(3)-b2*x(4));  
   dx=dx';
end

Multivariable Code
clc;
query=input('Do you want to analyze the system at single Force (5N) or on a range of forces? (Single/Range) >> ','s');
TR = [0 10];
X0 = [0;0;0;0];
if query=="Single" || query=="single"
   range=[5,5];
   inc=1;
elseif query=="Range" || query=="range"
   range=input('Please enter a start and an end value for the force in the format [start,end] >> ');
   inc=input('Please enter an increment value >> ');
end
for F=range(1):inc:range(2)
    [t,y]=ode45(@(t,y) cart(t,y,F),TR,X0);
    x1=y(:,1);
    v1=y(:,2);
    x2=y(:,3);
    v2=y(:,4);
    subplot(1,4,1);
    plot(t,x1);
    hold on;
    xlabel('time');
    ylabel('Displacement-1');
    subplot(1,4,2);
    plot(t,v1);
    hold on;
    xlabel('time');
    ylabel('Velocity-1');
    subplot(1,4,3);
    plot(t,x2);
    hold on;
    xlabel('time');
    ylabel('Displacement-2');
    subplot(1,4,4);
    plot(t,v2);
    hold on;
    xlabel('time');
    ylabel('Velocity-2');
end
text="";
for i=range(1):inc:range(2)
    text(end+1)=sprintf("%dN force",i);
end
text=text(2:end);
for i=1:4
    subplot(1,4,i);legend(text);
end
function dy=cart(t,y,f)
   m1=1;
   m2=4;
   k1=3;
   k2=2;
   k3=1;
   c1=0.03;
   c2=0.02;
   c3=0.01;
   dy(1)=y(2);
   dy(3)=y(4);
   dy(2)=1/m1*(f-(k1+k2)*y(1)-(c1+c2)*y(2)+c2*y(4)+k2*y(3));
   dy(4)=1/m2*(-(c2+c3)*y(4)-(k2+k3)*y(3)+k2*y(1)+c2*y(2));  
   dy=dy';
end
