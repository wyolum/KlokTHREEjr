H = 10;
// H = 37.5 - 20 // with 10mm spacer
difference(){
  cylinder(h=H, d=4 + 1.4, $fn=30);
  translate([0, 0, -1])cylinder(h=H + 2, d=4, $fn=30);
}
