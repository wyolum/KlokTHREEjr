inch=25.4;
//cube/led width
cw = 7;
led_h = 1.4;
//diagonal separation
//ds = sqrt(2*(cw*cw));
ds = .3*inch;
// ds = 7.7;
L = 8 * ds;
W = 16 * ds;

//baffle height
bh=2;
echo (ds);
module chip_block(){
    rotate([0,0,45])translate([-cw/2,-cw/2,-1])cube([cw,cw,led_h+2]);
}
module light_cone(){
    cylinder(r1=3/2,r2 =cw/2,h=4,$fn=50);
}


for(yl = [0:ds:17*ds]){
    translate([-cw/2, -ds/2 + yl-.5, led_h])cube([L, 1, 2]);
} 
for(xl = [0:ds:8*ds]){
    translate([-cw/2 + xl-.8, -ds/2-.5, led_h])cube([1, W+1, 2]);
} 

w = .2*inch;
h = led_h;
translate([(0 + .5) * ds, (1 + .5) * ds, 0])rotate(a=45, v=[0, 0, 1])translate([-w/2, -w/2, 0]) cube([w, w, h]);
translate([(0 + .5) * ds, (13 + .5) * ds, 0])rotate(a=45, v=[0, 0, 1])translate([-w/2, -w/2, 0]) cube([w, w, h]);
translate([(6 + .5) * ds, (1 + .5) * ds, 0])rotate(a=45, v=[0, 0, 1])translate([-w/2, -w/2, 0]) cube([w, w, h]);
translate([(6 + .5) * ds, (13 + .5) * ds, 0])rotate(a=45, v=[0, 0, 1])translate([-w/2, -w/2, 0]) cube([w, w, h]);

translate([(0 + .5) * ds, (1 + .5) * ds, led_h])cylinder($fn=4, r1=w/2 * sqrt(2), r2=1, h=bh);
translate([(0 + .5) * ds, (13 + .5) * ds, led_h])cylinder($fn=4, r1=w/2 * sqrt(2), r2=1, h=bh);
translate([(6 + .5) * ds, (1 + .5) * ds, led_h])cylinder($fn=4, r1=w/2 * sqrt(2), r2=1, h=bh);
translate([(6 + .5) * ds, (13 + .5) * ds, led_h])cylinder($fn=4, r1=w/2 * sqrt(2), r2=1, h=bh);

//translate([(1 + .5) * ds, (1 + .5) * ds, 0])rotate(a=45, v=[0, 0, 1])translate([-w/2, -w/2, 0]) cube([w, w, h]);
//translate([(-2 + .5) * ds, (1 + .5) * ds, led_h])cylinder($fn=4, r1=w/2 * sqrt(2), r2=0, h=bh);
