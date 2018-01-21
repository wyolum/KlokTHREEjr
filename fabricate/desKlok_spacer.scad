
difference(){
  cylinder(h=(37.5 - 20), d=4 + 1.4, $fn=30);
  translate([0, 0, -1])cylinder(h=37.5 + 2 - 20, d=4, $fn=30);
}
