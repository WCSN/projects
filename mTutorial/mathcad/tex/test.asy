unitsize(1cm);
pair O=(0,0);
real [] a={0,10,20,30,40,50,60,70,80,90};
real [] g={1,.95,.9,.85,.8,.75,.7,.65,.6,.55};
path rec=shift(3,-.5)*slant(.6)*yscale(.5)*unitsquare;
for(int i=0; i<=9;++i)
{
pen[] p; p[i]=gray(g[i]);
transform [] t;
t[i]=yscale(.5+.1*i)*slant(1-.2*i)*rotate(.5*a[i],O);
filldraw(shift(i/5,0)*t[i]*rec,p[i]);
}
shipout(bbox(3mm));