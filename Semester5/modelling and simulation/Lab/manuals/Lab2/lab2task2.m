clc
A=xlsread('Datapoints.xlsx',1); 
x=A(:,1); 
y1=A(:,2); 
y2=A(:,3); 
y3=A(:,4); 
plot(x,y1,'r',x,y2,'b',x,y3,'g'); 
legend('B points','C points','D points'); 
xlabel('A points') 
ylabel('B, C and D points') 
title('Sheet 1 plots') 