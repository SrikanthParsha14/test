N=128;
T=1;
t=linspace(0,T,N);
x=12*sin(2*pi*10*t+pi/4)+5*cos(2*pi*40*t);
dt=t(2)-t(1);
f=1/dt;
X=fft(x);
F=X(1:N/2+1);
f=f*(0:N/2)/N;
plot(f,abs(F),'-*');
xlabel('Frequency')
ylabel('|F(k)|')