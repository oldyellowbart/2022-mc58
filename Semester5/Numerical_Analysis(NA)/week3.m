%solving system of linear equation
a = [magic(3) eye(3)];
b= rref(a)%reduced row echelon form guaze jordan method
clc
c = [4 5 7;6 7 5 ; 5 6 7 ];
inv(c)

clc
x=linspace(0,2*pi,50); % a lin vect between 0 to 2pi with built-in 100 points  
y=sin(x); 
plot(x,y,'--p') 
xlabel('x') 
ylabel('sin(x)') 
title('plot of sine function') 