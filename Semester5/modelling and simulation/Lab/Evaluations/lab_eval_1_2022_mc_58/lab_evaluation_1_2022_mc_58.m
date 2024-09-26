clc
d = [8 5 1];
n = [1 2];
G = tf(n, d);%part a just select and evaluate the line
pole(G);%part(b)
dcgain(G);%part(b)
zero(G);%part(b)
A = ss(G);%part(c)-state-space
disp(A);%part(c)-state space
%chat gpt start part(d)
t = 0:0.01:5; %time from o to 5 secnd stepsize .01
ramp = (t >= 0 & t < 2) .* t; %first half
step = (t >= 2) * 2;%second half
ramped_step = ramp + step;%combine
plot(t, ramped_step);
xlabel('Time (seconds)');
ylabel('Amplitude');
title('Ramped-Step Input Signal');
grid on;
%chat gpt end
lsim(G,ramped_step, t);%part(f)applying the signal