# set terminal png transparent nocrop enhanced font arial 8 size 420,320 
# set output 'vector.3.png'
set key bmargin center horizontal Left reverse enhanced autotitles nobox
set isosamples 31, 31
set rrange [ 0.00000 : 10.0000 ] noreverse nowriteback
set trange [ -5.00000 : 5.00000 ] noreverse nowriteback
set urange [ -5.00000 : 5.00000 ] noreverse nowriteback
set vrange [ -5.00000 : 5.00000 ] noreverse nowriteback
set xrange [ -10.0000 : 10.0000 ] noreverse nowriteback
set x2range [ -10.0000 : 10.0000 ] noreverse nowriteback
set yrange [ -10.0000 : 10.0000 ] noreverse nowriteback
set y2range [ -10.0000 : 10.0000 ] noreverse nowriteback
set zrange [ -10.0000 : 10.0000 ] noreverse nowriteback
set cbrange [ -10.0000 : 10.0000 ] noreverse nowriteback
r(x,y)=sqrt(x*x+y*y)
v1(x,y)=  q1/(r((x-x0),y))
v2(x,y)=  q2/(r((x+x0),y))
vtot(x,y)=v1(x,y)+v2(x,y)
e1x(x,y)= q1*(x-x0)/r(x-x0,y)**3
e1y(x,y)= q1*(y)/r(x-x0,y)**3
e2x(x,y)= q2*(x+x0)/r(x+x0,y)**3
e2y(x,y)= q2*(y)/r(x+x0,y)**3
etotx(x,y)=e1x(x,y)+e2x(x,y)
etoty(x,y)=e1y(x,y)+e2y(x,y)
enorm(x,y)=sqrt(etotx(x,y)*etotx(x,y)+etoty(x,y)*etoty(x,y))
dx1(x,y)=coef*etotx(x,y)/enorm(x,y)
dy1(x,y)=coef*etoty(x,y)/enorm(x,y)
dx2(x,y)=coef*etotx(x,y)
dy2(x,y)=coef*etoty(x,y)
q1 = 1
x0 = 1.0
q2 = -1
coef = 0.7
xmin = -10.0
xmax = 10.0
ymin = -10.0
ymax = 10.0
plot "field2xy.tmp" u 1:2:(coef*dx1($1,$2)):(coef*dy1($1,$2)) w vec,      "equipo2.tmp" w l
