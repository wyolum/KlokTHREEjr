EESchema Schematic File Version 4
LIBS:k3jr_controller-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 4
Title "KlokTHREEjr Controller"
Date "2018-02-07"
Rev "rev 1"
Comp "www.wyolum.com"
Comment1 "https://github.com/wyolum/KlokTHREEjr"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L KlokTHREEjr:SCREW HB45
U 1 1 5A4A80D0
P 1700 3750
F 0 "HB45" V 1700 3672 40  0000 R CNN
F 1 "mtg_hole_3mm_small" H 1700 3850 40  0001 C CNN
F 2 "k3jr_controller_libs:MountingHole_3.2mm_M3" H 1778 3713 60  0001 L CNN
F 3 "" H 1700 3750 60  0000 C CNN
F 4 "Value" H 1700 3750 50  0001 C CNN "manf#"
	1    1700 3750
	0    -1   -1   0   
$EndComp
$Comp
L KlokTHREEjr:SCREW HB46
U 1 1 5A4A8341
P 3200 3750
F 0 "HB46" V 3200 3672 40  0000 R CNN
F 1 "mtg_hole_3mm_small" H 3200 3850 40  0001 C CNN
F 2 "k3jr_controller_libs:MountingHole_3.2mm_M3" H 3278 3713 60  0001 L CNN
F 3 "" H 3200 3750 60  0000 C CNN
F 4 "Value" H 3200 3750 50  0001 C CNN "manf#"
	1    3200 3750
	0    1    1    0   
$EndComp
$Comp
L KlokTHREEjr:SCREW HB47
U 1 1 5A4A8811
P 1750 4250
F 0 "HB47" V 1750 4172 40  0000 R CNN
F 1 "mtg_hole_3mm" H 1750 4350 40  0001 C CNN
F 2 "k3jr_controller_libs:MountingHole_3.2mm_M3_Pad_Via" H 1828 4213 60  0001 L CNN
F 3 "" H 1750 4250 60  0000 C CNN
F 4 "Value" H 1750 4250 50  0001 C CNN "manf#"
	1    1750 4250
	0    -1   -1   0   
$EndComp
$Comp
L KlokTHREEjr:SCREW HB48
U 1 1 5A4A8818
P 3150 4250
F 0 "HB48" V 3150 4172 40  0000 R CNN
F 1 "mtg_hole_3mm" H 3150 4350 40  0001 C CNN
F 2 "k3jr_controller_libs:MountingHole_3.2mm_M3_Pad_Via" H 3228 4213 60  0001 L CNN
F 3 "" H 3150 4250 60  0000 C CNN
F 4 "Value" H 3150 4250 50  0001 C CNN "manf#"
	1    3150 4250
	0    1    1    0   
$EndComp
Text Notes 2300 3775 0    40   ~ 0
<- 6.1" ->
Text Notes 1700 4225 1    40   ~ 0
<- 1.35" ->
Wire Notes Line
	1600 3650 3300 3650
Wire Notes Line
	3300 3650 3300 5850
Wire Notes Line
	3300 5850 1600 5850
Wire Notes Line
	1600 5850 1600 3650
Text Notes 2100 5800 0    50   ~ 10
Mounting holes for\nC3jr compatibility\n3 mm diameter\nplated
$Comp
L KlokTHREEjr:Conn_01x01 J1
U 1 1 5A4866AD
P 6800 1825
F 0 "J1" H 6625 1875 40  0000 L CNN
F 1 "CLK_IN" H 6875 1850 40  0000 L CNN
F 2 "k3jr_controller_libs:1pin" H 6800 1825 50  0001 C CNN
F 3 "~" H 6800 1825 50  0001 C CNN
	1    6800 1825
	0    1    1    0   
$EndComp
$Comp
L KlokTHREEjr:Conn_01x01 J2
U 1 1 5A486E6A
P 7000 1825
F 0 "J2" H 6825 1875 40  0000 L CNN
F 1 "DAT_IN" H 7075 1850 40  0000 L CNN
F 2 "k3jr_controller_libs:1pin" H 7000 1825 50  0001 C CNN
F 3 "~" H 7000 1825 50  0001 C CNN
	1    7000 1825
	0    1    1    0   
$EndComp
$Comp
L KlokTHREEjr:Conn_01x01 J3
U 1 1 5A486F0A
P 7200 1825
F 0 "J3" H 7025 1875 40  0000 L CNN
F 1 "LDR1" H 7275 1850 40  0000 L CNN
F 2 "k3jr_controller_libs:1pin" H 7200 1825 50  0001 C CNN
F 3 "~" H 7200 1825 50  0001 C CNN
	1    7200 1825
	0    1    1    0   
$EndComp
$Comp
L KlokTHREEjr:Conn_01x01 J4
U 1 1 5A486F90
P 7400 1825
F 0 "J4" H 7225 1875 50  0000 L CNN
F 1 "LDR2" H 7475 1850 40  0000 L CNN
F 2 "k3jr_controller_libs:1pin" H 7400 1825 50  0001 C CNN
F 3 "~" H 7400 1825 50  0001 C CNN
	1    7400 1825
	0    1    1    0   
$EndComp
$Comp
L KlokTHREEjr:Conn_01x01 J5
U 1 1 5A4872FB
P 7600 1825
F 0 "J5" H 7425 1875 40  0000 L CNN
F 1 "LDR3" H 7675 1850 40  0000 L CNN
F 2 "k3jr_controller_libs:1pin" H 7600 1825 50  0001 C CNN
F 3 "~" H 7600 1825 50  0001 C CNN
	1    7600 1825
	0    1    1    0   
$EndComp
$Comp
L KlokTHREEjr:Conn_01x01 J6
U 1 1 5A487301
P 7800 1825
F 0 "J6" H 7625 1875 40  0000 L CNN
F 1 "LDR4" H 7875 1850 40  0000 L CNN
F 2 "k3jr_controller_libs:1pin" H 7800 1825 50  0001 C CNN
F 3 "~" H 7800 1825 50  0001 C CNN
	1    7800 1825
	0    1    1    0   
$EndComp
$Comp
L KlokTHREEjr:Conn_01x01 J7
U 1 1 5A487307
P 8000 1825
F 0 "J7" H 7825 1875 40  0000 L CNN
F 1 "5V" H 8075 1850 40  0000 L CNN
F 2 "k3jr_controller_libs:1pin" H 8000 1825 50  0001 C CNN
F 3 "~" H 8000 1825 50  0001 C CNN
	1    8000 1825
	0    1    1    0   
$EndComp
$Comp
L KlokTHREEjr:Conn_01x01 J8
U 1 1 5A48730D
P 8200 1825
F 0 "J8" H 8025 1875 40  0000 L CNN
F 1 "GND" H 8275 1850 40  0000 L CNN
F 2 "k3jr_controller_libs:1pin" H 8200 1825 50  0001 C CNN
F 3 "~" H 8200 1825 50  0001 C CNN
	1    8200 1825
	0    1    1    0   
$EndComp
Wire Wire Line
	6800 1625 6800 1375
Wire Wire Line
	7000 1375 7000 1625
Wire Wire Line
	7200 1375 7200 1625
Wire Wire Line
	7400 1375 7400 1625
Wire Wire Line
	7600 1375 7600 1625
Wire Wire Line
	7800 1375 7800 1625
Wire Wire Line
	8000 1375 8000 1625
Wire Wire Line
	8200 1375 8200 1625
Text Label 6800 1375 3    40   ~ 0
CLK_IN
Text Label 7000 1375 3    40   ~ 0
DAT_IN
Text Label 7200 1375 3    40   ~ 0
LDR1
Text Label 7400 1375 3    40   ~ 0
LDR2
Text Label 7600 1375 3    40   ~ 0
LDR3
Text Label 7800 1375 3    40   ~ 0
LDR4
Text Label 8000 1375 3    40   ~ 0
5V
Text Label 8200 1375 3    40   ~ 0
GND
Text Notes 7000 3400 0    50   ~ 10
Mounting holes for\npiggy back attachment\nof "snap-off" control board.\nUse M3 HEX posts to connect\nLED board to control board
Text HLabel 6800 1375 1    40   Input ~ 0
CLK_IN
Text HLabel 7000 1375 1    40   Input ~ 0
DAT_IN
Text HLabel 7200 1375 1    40   Input ~ 0
LDR1
Text HLabel 7400 1375 1    40   Input ~ 0
LDR2
Text HLabel 7800 1375 1    40   Input ~ 0
LDR4
Text HLabel 7600 1375 1    40   Input ~ 0
LDR3
Text HLabel 8000 1375 1    40   Input ~ 0
5V
Text HLabel 8200 1375 1    40   Input ~ 0
GND
Wire Notes Line
	2350 4750 2550 4750
Wire Notes Line
	2450 4650 2450 4850
$Comp
L KlokTHREEjr:KiCad G5
U 1 1 5A50766D
P 9700 6850
F 0 "G5" H 9700 6582 60  0001 C CNN
F 1 "KiCad" H 9700 7118 60  0001 C CNN
F 2 "k3jr_controller_libs:KiCad-Logo2_6mm_SilkScreen" H 9700 6850 60  0001 C CNN
F 3 "" H 9700 6850 60  0001 C CNN
F 4 "Value" H 9700 6850 50  0001 C CNN "manf#"
	1    9700 6850
	1    0    0    -1  
$EndComp
$Comp
L KlokTHREEjr:OSHW G6
U 1 1 5A5077CE
P 10850 7000
F 0 "G6" H 10850 6950 40  0001 C CNN
F 1 "OSHW" H 10850 7100 40  0001 C CNN
F 2 "k3jr_controller_libs:OSHW_6mm" H 10850 7000 60  0001 C CNN
F 3 "" H 10850 7000 60  0000 C CNN
F 4 "Value" H 10850 7000 50  0001 C CNN "manf#"
	1    10850 7000
	1    0    0    -1  
$EndComp
$Comp
L KlokTHREEjr:LOGO_WL G4
U 1 1 5A5079CB
P 8450 6700
F 0 "G4" H 8450 6564 60  0001 C CNN
F 1 "LOGO_WL" H 8450 6836 60  0001 C CNN
F 2 "k3jr_controller_libs:Logo-WL3" H 8450 6700 60  0001 C CNN
F 3 "" H 8450 6700 60  0001 C CNN
F 4 "Value" H 8450 6700 50  0001 C CNN "manf#"
	1    8450 6700
	1    0    0    -1  
$EndComp
$EndSCHEMATC
