

clc 
x=[]; 
fprintf("You have to enter an array of 5 random numbers between 1 and 10...\n\n"); 
for i=1:5 
        text=sprintf("Enter number # %d >> ",i); 
        x(end+1)=input(text); 
    while x(end)<1 || x(end)>10 
        fprintf('Number must be between 1 and 10!!\n'); 
        text=sprintf("Enter number # %d >> ",i); 
        x(end)=input(text); 
    end 
end 
fprintf("The entered array is:"); 
x 
y=length(x)*x; 
subplot(1,3,1); 
plot(x,y,'--r'); 
xlabel('x values'); 
ylabel('y values'); 
title('Plot of x versus y'); 
subplot(1,3,2); 
stem(y,x); 
xlabel('y values'); 
ylabel('x values'); 
title('Stem plot of y versus x'); 
if mean(x)<20 
    Z=3*x; 
elseif mean(x)>50 
    Z=x.^4; 
end 
subplot(1,3,3); 
bar(x,Z); 
xlabel('x values'); 
ylabel('z values'); 
title('Bar plot of x versus z'); 