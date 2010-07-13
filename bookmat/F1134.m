f=zeros(101);
for x2=1:101
  if x2<60 & x2>40
  f(x2)=x2-50;
  end
  if x2>=60 f(x2)=10;
  end
  if x2<=40 f(x2)=-10; 
  end 
end

for x3=0:.1:10
x1=1+10.*x3;
G(x1)=0.5.*f(x1);
F(x1)=-G(x1);
end




U=zeros(21,101);
x=0:.1:10;
t=0:.5:10;
[X,T]=meshgrid(x,t);

for t1=1:21;
for x1=1:101;
c1=x1-t1+1;
c2=x1+t1-1;
if c1<=0 
   if c2>100
    U(t1,x1)=10;
   end
   if c2<=100
    U(t1,x1)=5+G(c2);
   end
end
if c1>0
   if c2>100
    U(t1,x1)=5+F(c1);
   end
   if c2<=100
    U(t1,x1)=F(c1)+G(c2);
   end
end

end
end
mesh(x,t,U/10)
xlabel('x','fontsize',20)
ylabel('t','fontsize',20)
zlabel('u','fontsize',20)
axis([0 10 0 9.9 0 1])


