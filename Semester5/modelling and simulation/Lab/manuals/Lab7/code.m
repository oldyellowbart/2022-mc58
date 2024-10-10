time = [0 100];
theta0 = [0;6;-1;3;1;2];
[time,z]=ode45(@fun,time,theta0);
theta1=z(:,1);
angularv1=z(:,2);
theta2=z(:,3);
angularv2=z(:,4);
theta3=z(:,5);
angularv3=z(:,6);
subplot(3,2,1)
plot(time,theta1)
legend('Angular Displacement-1')
xlabel('time')
ylabel('Angular Displacement-1')
subplot(3,2,2)
plot(time,angularv1)
legend('Angular Velocity-1')%
xlabel('time')
ylabel('Angular Velocity-1')
subplot(3,2,3)
plot(time,theta2)
legend('Angular Displacment-2')%
xlabel('time')
ylabel('Angular Displacement-2')
subplot(3,2,4)
plot(time,angularv2)
legend('Angular Velocity-2')%
xlabel('time')
ylabel('Angular Velocity-2')
subplot(3,2,5)
plot(time,theta3)
legend('Angular Displacment-3')%
xlabel('time')
ylabel('Angular Displacement-3')
subplot(3,2,6)
plot(time,angularv3)
legend('Angular Velocity-3')%
xlabel('time')
ylabel('Angular Velocity-3')