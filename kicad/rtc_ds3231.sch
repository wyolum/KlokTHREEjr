EESchema Schematic File Version 4
LIBS:chromogram
LIBS:chromogram-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 11 12
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L chromogram:R R8
U 1 1 5A448924
P 2525 2750
F 0 "R8" H 2575 2550 40  0000 C CNN
F 1 "4K7" V 2532 2751 40  0000 C CNN
F 2 "chromogram_libs:r_0805" V 2455 2750 30  0001 C CNN
F 3 "~" H 2525 2750 30  0000 C CNN
	1    2525 2750
	1    0    0    -1  
$EndComp
$Comp
L chromogram:GND #PWR017
U 1 1 5A44892B
P 3675 4175
F 0 "#PWR017" H 3675 4175 30  0001 C CNN
F 1 "GND" H 3675 4105 30  0001 C CNN
F 2 "" H 3675 4175 60  0000 C CNN
F 3 "" H 3675 4175 60  0000 C CNN
	1    3675 4175
	1    0    0    -1  
$EndComp
Text Label 5500 4425 0    40   ~ 0
SDA
Text Label 5500 4550 0    40   ~ 0
SCL
$Comp
L chromogram:R R6
U 1 1 5A448933
P 4900 2550
F 0 "R6" H 4950 2350 40  0000 C CNN
F 1 "10K" V 4907 2551 40  0000 C CNN
F 2 "chromogram_libs:r_0805" V 4830 2550 30  0001 C CNN
F 3 "~" H 4900 2550 30  0000 C CNN
	1    4900 2550
	1    0    0    -1  
$EndComp
$Comp
L chromogram:R R7
U 1 1 5A44893A
P 5050 2550
F 0 "R7" H 5100 2350 40  0000 C CNN
F 1 "10K" V 5057 2551 40  0000 C CNN
F 2 "chromogram_libs:r_0805" V 4980 2550 30  0001 C CNN
F 3 "~" H 5050 2550 30  0000 C CNN
	1    5050 2550
	1    0    0    -1  
$EndComp
Text Label 4600 3000 0    40   ~ 0
RESET
$Comp
L chromogram:PWR_FLAG #FLG018
U 1 1 5A448942
P 2000 2325
F 0 "#FLG018" H 2000 2595 30  0001 C CNN
F 1 "PWR_FLAG" H 2000 2555 30  0000 C CNN
F 2 "" H 2000 2325 60  0000 C CNN
F 3 "" H 2000 2325 60  0000 C CNN
	1    2000 2325
	1    0    0    -1  
$EndComp
$Comp
L chromogram:DS3231N U130
U 1 1 5A448948
P 3675 3200
F 0 "U130" H 3075 3825 40  0000 L CNN
F 1 "DS3231N" H 3075 3750 40  0000 L CNN
F 2 "chromogram_libs:DS3231" H 3675 3200 60  0001 C CNN
F 3 "~" H 3675 3200 60  0000 C CNN
	1    3675 3200
	1    0    0    -1  
$EndComp
$Comp
L chromogram:R R9
U 1 1 5A44894F
P 2675 2750
F 0 "R9" H 2725 2550 40  0000 C CNN
F 1 "4K7" V 2682 2751 40  0000 C CNN
F 2 "chromogram_libs:r_0805" V 2605 2750 30  0001 C CNN
F 3 "~" H 2675 2750 30  0000 C CNN
	1    2675 2750
	1    0    0    -1  
$EndComp
$Comp
L chromogram:Batt_RTC BT1
U 1 1 5A448957
P 2000 3200
F 0 "BT1" V 1900 3125 40  0000 C CNN
F 1 "Batt_RTC" H 2150 3125 40  0000 C CNN
F 2 "chromogram_libs:CR2032_SMD" H 2000 3200 60  0001 C CNN
F 3 "" H 2000 3200 60  0000 C CNN
F 4 "BU2032SM-HD-GCT-ND " H 2100 3500 60  0001 C CNN "Digikey"
	1    2000 3200
	0    1    1    0   
$EndComp
Text Label 2075 2400 0    40   ~ 0
BAT
Text Label 2525 2225 0    40   ~ 0
5V
Text Label 2050 4100 0    40   ~ 0
GND
Wire Wire Line
	4575 3250 4625 3250
Wire Wire Line
	3675 2225 3675 2400
Connection ~ 4900 2225
Wire Wire Line
	4900 2225 4900 2400
Wire Wire Line
	2525 2225 2675 2225
Wire Wire Line
	5050 2225 5050 2400
Wire Wire Line
	2000 2325 2000 2400
Wire Wire Line
	4575 3000 5600 3000
Wire Wire Line
	4625 3250 4625 3350
Wire Wire Line
	4625 3650 4575 3650
Connection ~ 4625 3650
Wire Wire Line
	4625 3550 4575 3550
Connection ~ 4625 3550
Wire Wire Line
	4625 3450 4575 3450
Connection ~ 4625 3450
Wire Wire Line
	4625 3350 4575 3350
Connection ~ 4625 3350
Wire Wire Line
	2000 2400 3425 2400
Wire Wire Line
	2000 3400 2000 3475
Wire Wire Line
	2200 2850 2200 3025
Wire Wire Line
	2000 2850 2200 2850
Connection ~ 2000 2850
Wire Wire Line
	2200 3325 2200 3475
Wire Wire Line
	2200 3475 2000 3475
Connection ~ 2000 3475
Connection ~ 2000 2400
Wire Wire Line
	2000 4100 3675 4100
Wire Wire Line
	3675 4050 3675 4100
Wire Wire Line
	3875 4100 3875 4050
Connection ~ 3675 4100
Wire Wire Line
	4075 4100 4075 4050
Wire Wire Line
	3975 4100 3975 4050
Connection ~ 3875 4100
Connection ~ 3975 4100
Connection ~ 4075 4100
Wire Wire Line
	2525 2225 2525 2600
Wire Wire Line
	2675 2225 2675 2600
Wire Wire Line
	2675 2900 2675 3150
Wire Wire Line
	2675 3150 2775 3150
Wire Wire Line
	2525 2900 2525 3300
Wire Wire Line
	2525 3300 2775 3300
Wire Wire Line
	2675 4425 5825 4425
Connection ~ 2675 3150
Wire Wire Line
	2525 4550 5825 4550
Connection ~ 2525 3300
Wire Wire Line
	4575 2850 4900 2850
Wire Wire Line
	4900 2850 4900 2700
Wire Wire Line
	5050 2700 5050 3150
Wire Wire Line
	5050 3150 4575 3150
Connection ~ 2675 2225
Connection ~ 3675 2225
Connection ~ 5050 2225
Connection ~ 4625 4100
Text HLabel 5825 2225 2    40   Input ~ 0
5V
Text HLabel 5825 4100 2    40   Input ~ 0
GND
Text HLabel 5825 4425 2    40   Input ~ 0
SDA
Text HLabel 5825 4550 2    40   Input ~ 0
SCL
$Comp
L chromogram:C_NP C22
U 1 1 5A613CC0
P 2200 3175
F 0 "C22" H 2303 3213 40  0000 L CNN
F 1 "100nF" H 2303 3138 40  0000 L CNN
F 2 "chromogram_libs:c_0805" H 2200 3175 60  0001 C CNN
F 3 "" H 2200 3175 60  0000 C CNN
	1    2200 3175
	1    0    0    -1  
$EndComp
Wire Wire Line
	4900 2225 5050 2225
Wire Wire Line
	4625 3650 4625 4100
Wire Wire Line
	4625 3550 4625 3650
Wire Wire Line
	4625 3450 4625 3550
Wire Wire Line
	4625 3350 4625 3450
Wire Wire Line
	2000 2850 2000 2950
Wire Wire Line
	2000 3475 2000 4100
Wire Wire Line
	2000 2400 2000 2850
Wire Wire Line
	3675 4100 3875 4100
Wire Wire Line
	3675 4100 3675 4175
Wire Wire Line
	3875 4100 3975 4100
Wire Wire Line
	3975 4100 4075 4100
Wire Wire Line
	4075 4100 4625 4100
Wire Wire Line
	2675 3150 2675 4425
Wire Wire Line
	2525 3300 2525 4550
Wire Wire Line
	2675 2225 3675 2225
Wire Wire Line
	3675 2225 4900 2225
Wire Wire Line
	5050 2225 5825 2225
Wire Wire Line
	4625 4100 5825 4100
$Comp
L chromogram:Conn_01x01 TP1
U 1 1 5A48EA43
P 5800 3000
F 0 "TP1" H 5925 3100 50  0000 L CNN
F 1 "~RST" H 5925 2950 50  0000 L CNN
F 2 "chromogram_libs:BreakOutPad" H 5800 3000 50  0001 C CNN
F 3 "~" H 5800 3000 50  0001 C CNN
	1    5800 3000
	1    0    0    -1  
$EndComp
Text Label 4600 2850 0    40   ~ 0
SQR
Text Label 4600 3150 0    40   ~ 0
32kHz
$EndSCHEMATC
