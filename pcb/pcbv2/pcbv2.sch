EESchema Schematic File Version 4
LIBS:pcbv2-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
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
L jak:XL6009_Up U2
U 1 1 5D84AA05
P 6500 2650
F 0 "U2" H 6525 2775 50  0000 C CNN
F 1 "XL6009_Up" H 6525 2684 50  0000 C CNN
F 2 "jak:XL6009" H 6500 2650 50  0001 C CNN
F 3 "" H 6500 2650 50  0001 C CNN
	1    6500 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 3650 5000 2750
$Comp
L jak:TP4056_breakout U1
U 1 1 5D84FEC6
P 3350 4450
F 0 "U1" H 3350 4400 50  0000 C CNN
F 1 "TP4056_breakout" H 3350 4500 50  0000 C CNN
F 2 "jak:tp4056_breakout" H 3350 4450 50  0001 C CNN
F 3 "" H 3350 4450 50  0001 C CNN
	1    3350 4450
	1    0    0    -1  
$EndComp
$Comp
L Device:Battery_Cell BT1
U 1 1 5D853CE0
P 3400 5800
F 0 "BT1" V 3655 5850 50  0000 C CNN
F 1 "Battery_Cell" V 3564 5850 50  0000 C CNN
F 2 "Connector_Wire:SolderWirePad_1x02_P7.62mm_Drill2.5mm" V 3400 5860 50  0001 C CNN
F 3 "~" V 3400 5860 50  0001 C CNN
	1    3400 5800
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3200 5800 3200 5450
Wire Wire Line
	3200 5250 3300 5250
Wire Wire Line
	3500 5800 3500 5450
Wire Wire Line
	3500 5250 3400 5250
$Comp
L jak:DSWY2596_Down U3
U 1 1 5D849321
P 5600 3550
F 0 "U3" H 5625 3675 50  0000 C CNN
F 1 "DSWY2596_Down" H 5625 3584 50  0000 C CNN
F 2 "jak:XL6009" H 5600 3550 50  0001 C CNN
F 3 "" H 5600 3550 50  0001 C CNN
	1    5600 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 3650 5400 3650
Wire Wire Line
	5400 3800 4900 3800
Wire Wire Line
	5000 2750 6300 2750
Wire Wire Line
	3550 4650 3850 4650
Wire Wire Line
	3050 4550 3150 4550
Wire Wire Line
	3000 4650 3150 4650
Wire Wire Line
	3300 5250 3300 5050
Wire Wire Line
	3400 5250 3400 5050
$Comp
L Transistor_FET:IRLZ44N Q2_LEDSwitch1
U 1 1 5D84B4E3
P 5600 3050
F 0 "Q2_LEDSwitch1" H 5806 3096 50  0000 L CNN
F 1 "IRLZ44N" H 5806 3005 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 5850 2975 50  0001 L CIN
F 3 "http://www.irf.com/product-info/datasheets/data/irlz44n.pdf" H 5600 3050 50  0001 L CNN
	1    5600 3050
	0    1    1    0   
$EndComp
$Comp
L Transistor_FET:IRLZ44N Q1_PwrReg1
U 1 1 5D84D099
P 4000 3450
F 0 "Q1_PwrReg1" V 4343 3450 50  0000 C CNN
F 1 "IRLZ44N" V 4252 3450 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 4250 3375 50  0001 L CIN
F 3 "http://www.irf.com/product-info/datasheets/data/irlz44n.pdf" H 4000 3450 50  0001 L CNN
	1    4000 3450
	0    1    1    0   
$EndComp
$Comp
L Switch:SW_SPST SW1
U 1 1 5D84DA38
P 3450 2700
F 0 "SW1" H 3450 2935 50  0000 C CNN
F 1 "SW_SPST" H 3450 2844 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 3450 2700 50  0001 C CNN
F 3 "~" H 3450 2700 50  0001 C CNN
	1    3450 2700
	-1   0    0    1   
$EndComp
$Comp
L Device:R R1
U 1 1 5D84E85F
P 3350 3050
F 0 "R1" V 3143 3050 50  0000 C CNN
F 1 "R" V 3234 3050 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 3280 3050 50  0001 C CNN
F 3 "~" H 3350 3050 50  0001 C CNN
	1    3350 3050
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R2
U 1 1 5D84F33A
P 5250 3000
F 0 "R2" H 5320 3046 50  0000 L CNN
F 1 "R" H 5320 2955 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 5180 3000 50  0001 C CNN
F 3 "~" H 5250 3000 50  0001 C CNN
	1    5250 3000
	0    -1   -1   0   
$EndComp
Connection ~ 5000 2750
Wire Wire Line
	5000 2600 5000 2750
Wire Wire Line
	3050 4550 3050 4250
Wire Wire Line
	3050 2700 3250 2700
Wire Wire Line
	3050 2700 3050 2600
Wire Wire Line
	3050 2600 5000 2600
Connection ~ 3050 2700
Wire Wire Line
	3650 2700 4000 2700
Wire Wire Line
	4000 2700 4000 2900
Wire Wire Line
	3200 3050 3000 3050
Wire Wire Line
	5400 3150 4900 3150
Wire Wire Line
	4900 3000 4900 3150
Connection ~ 4900 3150
Wire Wire Line
	4900 3000 5100 3000
Wire Wire Line
	5400 3000 5450 3000
Wire Wire Line
	5450 3000 5450 2850
Wire Wire Line
	5450 2850 5600 2850
Wire Wire Line
	5800 3150 6200 3150
Wire Wire Line
	6200 3150 6200 2900
Wire Wire Line
	6200 2900 6300 2900
Connection ~ 5600 2850
$Comp
L Connector:Conn_01x03_Female J1
U 1 1 5D857DF3
P 4250 4650
F 0 "J1" H 4278 4676 50  0000 L CNN
F 1 "Conn_01x03_Female" H 4278 4585 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Horizontal" H 4250 4650 50  0001 C CNN
F 3 "~" H 4250 4650 50  0001 C CNN
	1    4250 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4050 4550 3950 4650
Wire Wire Line
	4050 4650 3950 4550
Wire Wire Line
	3550 4550 3850 4550
$Comp
L jak:stm32_bluepill U4
U 1 1 5D862981
P 8400 1850
F 0 "U4" H 8400 2025 50  0000 C CNN
F 1 "stm32_bluepill" H 8400 1934 50  0000 C CNN
F 2 "bluepill:BluePill_STM32F103C" H 8400 1850 50  0001 C CNN
F 3 "" H 8400 1850 50  0001 C CNN
	1    8400 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 2000 5600 2350
Wire Wire Line
	8850 1900 8950 1900
Wire Wire Line
	9250 1900 9250 3950
Wire Wire Line
	9250 3950 6300 3950
Wire Wire Line
	6300 3950 6300 3650
Wire Wire Line
	6300 3650 5800 3650
Wire Wire Line
	7950 3700 7950 3800
Connection ~ 7950 3800
$Comp
L Connector:Conn_01x20_Female J2
U 1 1 5D86A421
P 7600 2800
F 0 "J2" H 7492 3885 50  0000 C CNN
F 1 "Conn_01x20_Female" H 7492 3794 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x20_P2.54mm_Vertical" H 7600 2800 50  0001 C CNN
F 3 "~" H 7600 2800 50  0001 C CNN
	1    7600 2800
	-1   0    0    -1  
$EndComp
Wire Wire Line
	5800 3800 7050 3800
Wire Wire Line
	5600 2000 7800 2000
Connection ~ 7800 2000
Wire Wire Line
	7800 2000 7950 2000
Connection ~ 7800 3800
Wire Wire Line
	7800 3800 7950 3800
Wire Wire Line
	7950 1900 7800 1900
Wire Wire Line
	7800 2100 7950 2100
Wire Wire Line
	7950 2200 7800 2200
Wire Wire Line
	7800 2300 7950 2300
Wire Wire Line
	7950 2400 7800 2400
Wire Wire Line
	7800 2500 7950 2500
Wire Wire Line
	7800 2700 7950 2700
Wire Wire Line
	7950 2800 7800 2800
Wire Wire Line
	7800 2900 7950 2900
Wire Wire Line
	7950 3000 7800 3000
Wire Wire Line
	7800 3100 7950 3100
Wire Wire Line
	7800 3300 7950 3300
Wire Wire Line
	7950 3400 7800 3400
Wire Wire Line
	7800 3500 7950 3500
Wire Wire Line
	7950 3600 7800 3600
Wire Wire Line
	7800 3700 7950 3700
Connection ~ 7950 3700
$Comp
L Connector:Conn_01x20_Female J3
U 1 1 5D887482
P 9150 2900
F 0 "J3" H 9042 1675 50  0000 C CNN
F 1 "Conn_01x20_Female" H 9042 1766 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x20_P2.54mm_Vertical" H 9150 2900 50  0001 C CNN
F 3 "~" H 9150 2900 50  0001 C CNN
	1    9150 2900
	1    0    0    1   
$EndComp
Connection ~ 8950 1900
Wire Wire Line
	8950 1900 9250 1900
Wire Wire Line
	8950 2000 8900 2000
Wire Wire Line
	8850 2100 8950 2100
Wire Wire Line
	8950 2200 8850 2200
Wire Wire Line
	8850 2300 8950 2300
Wire Wire Line
	8950 2800 8850 2800
Wire Wire Line
	8850 2900 8950 2900
Wire Wire Line
	8950 3000 8850 3000
Wire Wire Line
	8850 3100 8950 3100
Wire Wire Line
	8850 3300 8950 3300
Wire Wire Line
	8950 3400 8850 3400
Wire Wire Line
	8850 3500 8950 3500
Wire Wire Line
	8950 3600 8850 3600
Wire Wire Line
	8850 3700 8950 3700
Wire Wire Line
	8950 3800 8850 3800
$Comp
L Switch:SW_SPST SW2
U 1 1 5D8AD951
P 8400 4300
F 0 "SW2" H 8400 4535 50  0000 C CNN
F 1 "SW_SPST" H 8400 4444 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 8400 4300 50  0001 C CNN
F 3 "~" H 8400 4300 50  0001 C CNN
	1    8400 4300
	-1   0    0    1   
$EndComp
Wire Wire Line
	8600 4300 8650 4300
Wire Wire Line
	9600 4300 9600 3200
Wire Wire Line
	8850 3200 8950 3200
Connection ~ 8950 3200
Wire Wire Line
	8950 3200 9600 3200
Wire Wire Line
	8200 4300 8100 4300
Wire Wire Line
	7400 4300 7400 3200
Wire Wire Line
	7400 3200 7800 3200
Connection ~ 7800 3200
Wire Wire Line
	7800 3200 7950 3200
$Comp
L Device:CP C1
U 1 1 5D8B3A47
P 7000 2500
F 0 "C1" V 7255 2500 50  0000 C CNN
F 1 "CP" V 7164 2500 50  0000 C CNN
F 2 "Capacitor_THT:CP_Radial_D8.0mm_P5.00mm" H 7038 2350 50  0001 C CNN
F 3 "~" H 7000 2500 50  0001 C CNN
	1    7000 2500
	0    -1   -1   0   
$EndComp
$Comp
L Device:LED D1
U 1 1 5D8B5199
P 7000 1700
F 0 "D1" H 6993 1916 50  0000 C CNN
F 1 "LED" H 6993 1825 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7000 1700 50  0001 C CNN
F 3 "~" H 7000 1700 50  0001 C CNN
	1    7000 1700
	1    0    0    -1  
$EndComp
$Comp
L Jumper:SolderJumper_2_Open JP4
U 1 1 5D8B85FA
P 8400 4800
F 0 "JP4" H 8400 4575 50  0000 C CNN
F 1 "SolderJumper_2_Open" H 8400 4666 50  0000 C CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_RoundedPad1.0x1.5mm" H 8400 4800 50  0001 C CNN
F 3 "~" H 8400 4800 50  0001 C CNN
	1    8400 4800
	-1   0    0    1   
$EndComp
$Comp
L Jumper:SolderJumper_2_Open JP2
U 1 1 5D8D365D
P 4000 4100
F 0 "JP2" H 4000 3875 50  0000 C CNN
F 1 "SolderJumper_2_Open" H 4000 3966 50  0000 C CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_RoundedPad1.0x1.5mm" H 4000 4100 50  0001 C CNN
F 3 "~" H 4000 4100 50  0001 C CNN
	1    4000 4100
	-1   0    0    1   
$EndComp
$Comp
L Jumper:SolderJumper_2_Open JP3
U 1 1 5D8D42C0
P 4500 3550
F 0 "JP3" H 4500 3325 50  0000 C CNN
F 1 "SolderJumper_2_Open" H 4500 3416 50  0000 C CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_RoundedPad1.0x1.5mm" H 4500 3550 50  0001 C CNN
F 3 "~" H 4500 3550 50  0001 C CNN
	1    4500 3550
	-1   0    0    1   
$EndComp
$Comp
L Jumper:SolderJumper_2_Open JP1
U 1 1 5D8D4C85
P 3500 3550
F 0 "JP1" H 3500 3325 50  0000 C CNN
F 1 "SolderJumper_2_Open" H 3500 3416 50  0000 C CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_RoundedPad1.0x1.5mm" H 3500 3550 50  0001 C CNN
F 3 "~" H 3500 3550 50  0001 C CNN
	1    3500 3550
	-1   0    0    1   
$EndComp
Wire Wire Line
	3000 3050 3000 3550
Wire Wire Line
	4900 3150 4900 3550
Wire Wire Line
	3500 3050 4000 3050
Connection ~ 4000 3050
Wire Wire Line
	4000 3050 4000 3250
Wire Wire Line
	3350 3550 3000 3550
Connection ~ 3000 3550
Wire Wire Line
	3000 3550 3000 4100
Wire Wire Line
	3650 3550 3800 3550
Wire Wire Line
	4200 3550 4350 3550
Wire Wire Line
	4650 3550 4900 3550
Connection ~ 4900 3550
Wire Wire Line
	4900 3550 4900 3800
Wire Wire Line
	3850 4100 3000 4100
Connection ~ 3000 4100
Wire Wire Line
	3000 4100 3000 4350
Wire Wire Line
	4150 4100 4900 4100
Wire Wire Line
	4900 4100 4900 3800
Connection ~ 4900 3800
Wire Wire Line
	8250 4800 7400 4800
Wire Wire Line
	7400 4800 7400 4300
Connection ~ 7400 4300
Wire Wire Line
	8550 4800 9600 4800
Wire Wire Line
	9600 4800 9600 4300
Connection ~ 9600 4300
Wire Wire Line
	6850 2750 6850 2500
Wire Wire Line
	6850 2500 6850 1700
Connection ~ 6850 2500
Wire Wire Line
	7150 2900 7150 2500
Wire Wire Line
	7150 2500 7150 1700
Connection ~ 7150 2500
$Comp
L Connector:TestPoint TP10
U 1 1 5D91BBFF
P 6850 1300
F 0 "TP10" H 6908 1418 50  0000 L CNN
F 1 "TestPoint" H 6908 1327 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 7050 1300 50  0001 C CNN
F 3 "~" H 7050 1300 50  0001 C CNN
	1    6850 1300
	1    0    0    -1  
$EndComp
$Comp
L Connector:TestPoint TP12
U 1 1 5D91FB50
P 7150 1300
F 0 "TP12" H 7208 1418 50  0000 L CNN
F 1 "TestPoint" H 7208 1327 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 7350 1300 50  0001 C CNN
F 3 "~" H 7350 1300 50  0001 C CNN
	1    7150 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	6850 1300 6850 1700
Connection ~ 6850 1700
Wire Wire Line
	7150 1300 7150 1700
Connection ~ 7150 1700
$Comp
L Connector:TestPoint TP7
U 1 1 5D927595
P 5600 2350
F 0 "TP7" V 5554 2538 50  0000 L CNN
F 1 "TestPoint" V 5645 2538 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 5800 2350 50  0001 C CNN
F 3 "~" H 5800 2350 50  0001 C CNN
	1    5600 2350
	0    1    1    0   
$EndComp
Connection ~ 5600 2350
Wire Wire Line
	5600 2350 5600 2850
$Comp
L Connector:TestPoint TP8
U 1 1 5D92852B
P 6200 3150
F 0 "TP8" V 6154 3338 50  0000 L CNN
F 1 "TestPoint" V 6245 3338 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 6400 3150 50  0001 C CNN
F 3 "~" H 6400 3150 50  0001 C CNN
	1    6200 3150
	0    1    1    0   
$EndComp
Connection ~ 6200 3150
$Comp
L Connector:TestPoint TP9
U 1 1 5D928864
P 6300 3650
F 0 "TP9" V 6254 3838 50  0000 L CNN
F 1 "TestPoint" V 6345 3838 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 6500 3650 50  0001 C CNN
F 3 "~" H 6500 3650 50  0001 C CNN
	1    6300 3650
	0    1    1    0   
$EndComp
Connection ~ 6300 3650
$Comp
L Connector:TestPoint TP11
U 1 1 5D928C37
P 7050 3800
F 0 "TP11" H 7108 3918 50  0000 L CNN
F 1 "TestPoint" H 7108 3827 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 7250 3800 50  0001 C CNN
F 3 "~" H 7250 3800 50  0001 C CNN
	1    7050 3800
	1    0    0    -1  
$EndComp
Connection ~ 7050 3800
Wire Wire Line
	7050 3800 7800 3800
$Comp
L Connector:TestPoint TP3
U 1 1 5D929188
P 3200 5450
F 0 "TP3" V 3395 5522 50  0000 C CNN
F 1 "TestPoint" V 3304 5522 50  0000 C CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 3400 5450 50  0001 C CNN
F 3 "~" H 3400 5450 50  0001 C CNN
	1    3200 5450
	0    -1   -1   0   
$EndComp
Connection ~ 3200 5450
Wire Wire Line
	3200 5450 3200 5250
$Comp
L Connector:TestPoint TP4
U 1 1 5D929D8B
P 3500 5450
F 0 "TP4" V 3454 5638 50  0000 L CNN
F 1 "TestPoint" V 3545 5638 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 3700 5450 50  0001 C CNN
F 3 "~" H 3700 5450 50  0001 C CNN
	1    3500 5450
	0    1    1    0   
$EndComp
Connection ~ 3500 5450
Wire Wire Line
	3500 5450 3500 5250
$Comp
L Connector:TestPoint TP5
U 1 1 5D92A4F4
P 3850 4550
F 0 "TP5" H 3908 4668 50  0000 L CNN
F 1 "TestPoint" H 3908 4577 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 4050 4550 50  0001 C CNN
F 3 "~" H 4050 4550 50  0001 C CNN
	1    3850 4550
	1    0    0    -1  
$EndComp
Connection ~ 3850 4550
Wire Wire Line
	3850 4550 3950 4550
$Comp
L Connector:TestPoint TP6
U 1 1 5D92AA34
P 4000 2900
F 0 "TP6" V 3954 3088 50  0000 L CNN
F 1 "TestPoint" V 4045 3088 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 4200 2900 50  0001 C CNN
F 3 "~" H 4200 2900 50  0001 C CNN
	1    4000 2900
	0    1    1    0   
$EndComp
Connection ~ 4000 2900
Wire Wire Line
	4000 2900 4000 3050
$Comp
L Connector:TestPoint TP1
U 1 1 5D92B351
P 3000 4350
F 0 "TP1" V 3195 4422 50  0000 C CNN
F 1 "TestPoint" V 3104 4422 50  0000 C CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 3200 4350 50  0001 C CNN
F 3 "~" H 3200 4350 50  0001 C CNN
	1    3000 4350
	0    -1   -1   0   
$EndComp
Connection ~ 3000 4350
Wire Wire Line
	3000 4350 3000 4650
$Comp
L Connector:TestPoint TP2
U 1 1 5D92E480
P 3050 4250
F 0 "TP2" V 3004 4438 50  0000 L CNN
F 1 "TestPoint" V 3095 4438 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 3250 4250 50  0001 C CNN
F 3 "~" H 3250 4250 50  0001 C CNN
	1    3050 4250
	0    1    1    0   
$EndComp
Connection ~ 3050 4250
Wire Wire Line
	3050 4250 3050 2700
$Comp
L Connector:TestPoint TP14
U 1 1 5D92F419
P 8650 4300
F 0 "TP14" H 8708 4418 50  0000 L CNN
F 1 "TestPoint" H 8708 4327 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 8850 4300 50  0001 C CNN
F 3 "~" H 8850 4300 50  0001 C CNN
	1    8650 4300
	1    0    0    -1  
$EndComp
Connection ~ 8650 4300
Wire Wire Line
	8650 4300 9600 4300
$Comp
L Connector:TestPoint TP13
U 1 1 5D92F909
P 8100 4300
F 0 "TP13" H 8158 4418 50  0000 L CNN
F 1 "TestPoint" H 8158 4327 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 8300 4300 50  0001 C CNN
F 3 "~" H 8300 4300 50  0001 C CNN
	1    8100 4300
	1    0    0    -1  
$EndComp
Connection ~ 8100 4300
Wire Wire Line
	8100 4300 7400 4300
Wire Wire Line
	6700 2750 6850 2750
Wire Wire Line
	6700 2900 7150 2900
$Comp
L Connector:TestPoint TP15
U 1 1 5DA27B8B
P 3850 4650
F 0 "TP15" H 3792 4676 50  0000 R CNN
F 1 "TestPoint" H 3792 4767 50  0000 R CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 4050 4650 50  0001 C CNN
F 3 "~" H 4050 4650 50  0001 C CNN
	1    3850 4650
	-1   0    0    1   
$EndComp
Connection ~ 3850 4650
Wire Wire Line
	3850 4650 3950 4650
Wire Wire Line
	8950 2400 8850 2400
Wire Wire Line
	8950 2500 8850 2500
Wire Wire Line
	8950 2600 8850 2600
Wire Wire Line
	8950 2700 8850 2700
$Comp
L Connector:Conn_01x03_Female J4
U 1 1 5DA5CB77
P 8800 1350
F 0 "J4" H 8828 1376 50  0000 L CNN
F 1 "Conn_01x03_Female" H 8828 1285 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 8800 1350 50  0001 C CNN
F 3 "~" H 8800 1350 50  0001 C CNN
	1    8800 1350
	1    0    0    -1  
$EndComp
$Comp
L Jumper:SolderJumper_3_Open JP5
U 1 1 5DA5FF45
P 8050 1350
F 0 "JP5" V 8096 1417 50  0000 L CNN
F 1 "SolderJumper_3_Open" V 8005 1417 50  0000 L CNN
F 2 "Jumper:SolderJumper-3_P1.3mm_Open_RoundedPad1.0x1.5mm" H 8050 1350 50  0001 C CNN
F 3 "~" H 8050 1350 50  0001 C CNN
	1    8050 1350
	0    -1   -1   0   
$EndComp
$Comp
L Jumper:SolderJumper_3_Open JP6
U 1 1 5DA6276F
P 8300 1450
F 0 "JP6" V 8346 1517 50  0000 L CNN
F 1 "SolderJumper_3_Open" V 8255 1517 50  0000 L CNN
F 2 "Jumper:SolderJumper-3_P1.3mm_Open_RoundedPad1.0x1.5mm" H 8300 1450 50  0001 C CNN
F 3 "~" H 8300 1450 50  0001 C CNN
	1    8300 1450
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8050 1150 8300 1150
Wire Wire Line
	8300 1150 8300 1250
Wire Wire Line
	8050 1550 8050 1650
Wire Wire Line
	8050 1650 8300 1650
Wire Wire Line
	8450 1450 8600 1450
Wire Wire Line
	8600 1350 8500 1350
Wire Wire Line
	8500 1350 8500 1200
Wire Wire Line
	8500 1200 8450 1200
Wire Wire Line
	8200 1200 8200 1350
Wire Wire Line
	8600 1250 8900 1250
Wire Wire Line
	8900 1250 8900 2000
Connection ~ 8900 2000
Wire Wire Line
	8900 2000 8850 2000
$Comp
L Connector:TestPoint TP17
U 1 1 5DA808A3
P 8600 1450
F 0 "TP17" H 8542 1476 50  0000 R CNN
F 1 "TestPoint" H 8542 1567 50  0000 R CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 8800 1450 50  0001 C CNN
F 3 "~" H 8800 1450 50  0001 C CNN
	1    8600 1450
	-1   0    0    1   
$EndComp
Connection ~ 8600 1450
$Comp
L Connector:TestPoint TP16
U 1 1 5DA80DF7
P 8450 1200
F 0 "TP16" H 8508 1318 50  0000 L CNN
F 1 "TestPoint" H 8508 1227 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 8650 1200 50  0001 C CNN
F 3 "~" H 8650 1200 50  0001 C CNN
	1    8450 1200
	1    0    0    -1  
$EndComp
Connection ~ 8450 1200
Wire Wire Line
	8450 1200 8200 1200
Wire Wire Line
	8050 2500 7950 2500
Connection ~ 7950 2500
Wire Wire Line
	8050 1650 7350 1650
Wire Wire Line
	7350 1650 7350 2500
Wire Wire Line
	7350 2500 7800 2500
Connection ~ 8050 1650
Connection ~ 7800 2500
Wire Wire Line
	8050 1150 7400 1150
Wire Wire Line
	7400 1150 7400 2600
Wire Wire Line
	7400 2600 7800 2600
Connection ~ 8050 1150
Connection ~ 7800 2600
Wire Wire Line
	7800 2600 7950 2600
$Comp
L power:GND #PWR0101
U 1 1 5DAA9E0E
P 3000 4800
F 0 "#PWR0101" H 3000 4550 50  0001 C CNN
F 1 "GND" H 3005 4627 50  0000 C CNN
F 2 "" H 3000 4800 50  0001 C CNN
F 3 "" H 3000 4800 50  0001 C CNN
	1    3000 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	3000 4800 3000 4650
Connection ~ 3000 4650
$EndSCHEMATC
