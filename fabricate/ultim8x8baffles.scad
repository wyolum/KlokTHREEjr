inch=25.4;
//cube/led width
cw = 5.1;
//diagonal separation
//ds = sqrt(2*(cw*cw));
ds = 7.7;
//baffle height
bh=2;
echo (ds);
module chip_block(){
    rotate([0,0,45])translate([-cw/2,-cw/2,0])cube([cw,cw,cw]);
}
module light_cone(){
    cylinder(r1=3/2,r2 =cw/2,h=4,$fn=50);
}

difference(){
    translate([-.5*ds,-.5*ds,0]) cube([8*ds,8*ds,3.9]);
    for(xl = [0:ds:16*ds]){
        for (yl = [0:ds:8*ds]){
            //translate([xl,yl,0])chip_block();
            translate([xl,yl,0])light_cone();
        }
    }
}