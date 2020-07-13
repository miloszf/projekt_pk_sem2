// Test 01 - zdane
// Obliczanie iloczynu 2 liczb naturalnych
		POB CST_0
		£AD VAR_C
		POB VAR_A
LOOP:		£AD VAR_I
	        SOZ STOP
		POB VAR_C
		DOD VAR_B
		£AD VAR_C
		POB VAR_I
		ODE CST_1
		SOB LOOP
STOP: 	STP

CST_1: RST 1
CST_0: RST 0
VAR_A: RST 4
VAR_B: RST 6
VAR_I: RPA
VAR_C: RPA