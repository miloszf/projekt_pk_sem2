// Test 11 - zdane
// Program wypisuj¹cy trójk¹t o d³ugoœci boku podanej przez u¿ytkownika. Wykorzystuje podprogram do rysowania linii
		POB CST_0
		£AD VAR_A
		WPR 1
		ODE CHAR_0
		£AD SIZE
		ODE SIZE
		ODE SIZE
LOOP:	£AD ROW
		POB VAR_A
		DOD CST_1
		£AD VAR_A
		DNS
		POB CHAR_1
		DNS
		SDP DRAW
		POB SIZE
		ODE VAR_A
		DNS
		POB CHAR_2
		DNS
		SDP DRAW
		POB NEW_L
		WYP 2
		POB ROW
		DOD CST_1
		SOM LOOP
STOP: 	STP	

DRAW:	PZS
		£AD D_PTR
		PZS
		£AD D_CHAR
		PZS
		£AD D_VAR
		ODE D_VAR
		ODE D_VAR
D_LOOP:	£AD D_VAR
		SOZ D_STOP
		POB D_CHAR
		WYP 2
		POB D_VAR
		DOD CST_1
		SOB D_LOOP
D_STOP:	POB D_PTR
		DNS
		PWR
		
D_PTR:	RPA
D_VAR:	RPA
D_CHAR:	RPA

CST_0:	RST 0
CST_1:	RST 1
CHAR_1:	RST 'X'
CHAR_2:	RST '~'
NEW_L:	RST 13
CHAR_0: RST '0'

SIZE:	RPA
ROW:	RPA
VAR_A:	RPA
