%q2part2
f = 0:0.1:20;
m = [2,5,6,7,10];

for i =1:length(m)
a = f/(m(i));
plot(f,a);
grid on;
end
%gpt used