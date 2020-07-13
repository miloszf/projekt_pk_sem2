// Test 07 - zdane
// Wyszukiwanie indeksu pierwszego wyst¹pienia najmniejszej liczby w tablicy
		POB CST_1
		ODE CST_1
		£AD MIN_I
		POB PTR_0
		£AD POB_A
		POB ARRAY
		£AD MIN
		POB ARR_L
LOOP:	£AD VAR_I
		SOZ STOP
POB_A:	POB ARRAY
		ODE MIN
		SOM N_MIN
		SOB CD
N_MIN:	DOD MIN
		£AD MIN
		POB POB_A
		ODE PTR_0
		£AD MIN_I
CD:		POB POB_A
		DOD CST_1
		£AD POB_A
		POB VAR_I
		ODE CST_1
		SOB LOOP
		
STOP:	STP


CST_1:	RST 1
ARR_L:	RST 8
ARRAY:	RST 14
		RST 11
		RST 12
		RST 13
		RST 11
		RST 12
		RST 11
		RST 4
PTR_0:	POB ARRAY
VAR_I:	RPA
MIN:	RPA
MIN_I:	RPA
	