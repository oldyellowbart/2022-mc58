clc 
a=input("Enter a positive integer >> "); 
b=[]; 
b(1)=a; 
i=2; 
while a~=1 
if rem(a,2) 
a=3*a+1; 
b(i)=a; 
i=i+1; 
else 
a=a/2; 
b(i)=a; 
i=i+1; 
 
end 
end 
b