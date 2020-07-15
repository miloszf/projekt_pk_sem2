// Test 10 - zdane
// Program wypisuj¹cy odwrócony trójk¹t o d³ugoœci boku podanej przez u¿ytkownika
		WPR 1
		ODE CHAR_0
		£AD SIZE
		ODE SIZE
		ODE SIZE
LOOP_R:	£AD VAR_R
LOOP_C:	£AD VAR_C
		POB CHAR
		WYP 2
		POB VAR_C
		DOD CST_1
		SOM LOOP_C
		POB NEW_L
		WYP 2
		POB VAR_R
		DOD CST_1
		SOM LOOP_R
STOP: 	STP	

CST_1:	RST 1
CHAR:	RST '#'
NEW_L:	RST 13
CHAR_0: RST '0'

SIZE:	RPA
VAR_R:	RPA
VAR_C:	RPA
