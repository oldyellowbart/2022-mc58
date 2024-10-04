
% Example#02 (Dual Mass System)

clc;
Simple Multivariable
TR = [0 10];
X0 = [0;6;-1;3];
for f=1:1:5
[t,y]=ode45(@(t,y) fun(t,y,f),TR,X0);
x1=y(:,1);%cloumn1
v1=y(:,2);%cloumn2
x2=y(:,3);%cloumn3
v2=y(:,4);%cloumn4
subplot(1,4,1); hold on;
plot(t,x1)
legend('Fa=1', 'Fa=2', 'Fa=3', 'Fa=4', 'Fa=5')
xlabel('time')
ylabel('Displacement-1')
subplot(1,4,2); hold on;
plot(t,v1)
legend('Fa=1', 'Fa=2', 'Fa=3', 'Fa=4', 'Fa=5')
xlabel('time')
ylabel('Velocity-1')
subplot(1,4,3); hold on;
plot(t,x2)
legend('Fa=1', 'Fa=2', 'Fa=3', 'Fa=4', 'Fa=5')
xlabel('time')
ylabel('Displacement-2')
subplot(1,4,4); hold on;
plot(t,v2)
legend('Fa=1', 'Fa=2', 'Fa=3', 'Fa=4', 'Fa=5')
xlabel('time')
ylabel('Velocity-2')
end
function dy= fun(t,y,f)
   m1=1;%mass1
   m2=4;%mass2
   k1=3;
   k2=2;%same as k1 
   b1=0.03;
   b2=0.02;
   dy(1)=y(2);
   dy(3)=y(4);
   dy(2)=1/m1*(f-(b1)*y(2)-(k1)*y(1)+(k1)*y(3));
   dy(4)=1/m2*(-(b2)*y(4)-2*(k1)*y(3)+k1*y(1));  
   dy=dy';
end
