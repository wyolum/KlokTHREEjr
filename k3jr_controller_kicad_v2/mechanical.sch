EESchema Schematic File Version 4
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 4
Title "KlokTHREEjr"
Date "2017-12-26"
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
	8000 1375 8000 1625
Wire Wire Line
	8200 1375 8200 1625
Text Label 6800 1375 3    40   ~ 0
CLK_IN
Text Label 7000 1375 3    40   ~ 0
DAT_IN
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
Text HLabel 8000 1375 1    40   Input ~ 0
5V
Text HLabel 8200 1375 1    40   Input ~ 0
GND
Wire Notes Line
	2350 4750 2550 4750
Wire Notes Line
	2450 4650 2450 4850
$EndSCHEMATC
