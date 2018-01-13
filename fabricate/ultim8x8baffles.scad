inch=25.4;
//cube/led width
cw = 7;
led_h = 1.4;
//diagonal separation
//ds = sqrt(2*(cw*cw));
ds = 7.7;
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

difference(){
    translate([-.5*ds,-.5*ds,0]) cube([L, W,led_h]);
    for(xl = [0:ds:8*ds]){
        for (yl = [0:ds:16*ds]){
            translate([xl,yl,0])chip_block();
            //translate([xl,yl,0])light_cone();
        }
    }

    translate([ds * .5,ds * .5, 0])chip_block();
    translate([ds * 2.5,ds * 2.5, 0])chip_block();
    translate([ds * 4.5,ds * 4.5, 0])chip_block();
    translate([ds * 6.5,ds * 6.5, 0])chip_block();
    translate([6.5 * ds,ds * .5, 0])chip_block();
    translate([4.5 * ds,ds * 2.5, 0])chip_block();
    translate([2.5 * ds,ds * 4.5, 0])chip_block();
    translate([0.5 * ds,ds * 6.5, 0])chip_block();

    translate([ds * .5,ds * 8.5, 0])chip_block();
    translate([ds * 2.5,ds * 10.5, 0])chip_block();
    translate([ds * 4.5,ds * 12.5, 0])chip_block();
    translate([ds * 6.5,ds * 14.5, 0])chip_block();
    translate([6.5 * ds,ds * 8.5, 0])chip_block();
    translate([4.5 * ds,ds * 10.5, 0])chip_block();
    translate([2.5 * ds,ds * 12.5, 0])chip_block();
    translate([0.5 * ds,ds * 14.5, 0])chip_block();

    translate([-10,-10,-1])cube([10,200, 10]);
    translate([L - ds, -10, -1])cube([10,200, 10]);
    translate([-2 - ds, W-8, -1])cube([100,10, 10]);
    translate([-2 - ds, -10, -1])cube([100,10, 10]);
}

for(yl = [ds:ds:16*ds]){
    translate([-cw/2, -ds/2 + yl-.5, led_h])cube([L, 1, 2]);
} 
for(xl = [ds:ds:8*ds]){
    translate([-cw/2 + xl-.8, -ds/2, led_h])cube([1, W, 2]);
} 
