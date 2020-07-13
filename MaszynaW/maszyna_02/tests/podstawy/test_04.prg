// Test 04 - zdane
//Obliczanie d³ugoœci tablicy zakoñczonej bajtem o ustalonej wartoœci (np. 0)
		POB CST_1
		ODE CST_1
		£AD ARR_L
		POB PTR_0
		£AD LOOP
LOOP:	POB ARRAY
		ODE CHAR
		SOZ STOP
		POB ARR_L
		DOD CST_1
		£AD ARR_L
		POB LOOP
		DOD CST_1
		£AD LOOP
		SOB LOOP
		
STOP:	STP


CST_1:	RST 1
CHAR:	RST 0
ARRAY:	RST 10
		RST 11
		RST 12
		RST 13
		RST 12
		RST 11
		RST 0
PTR_0:	POB ARRAY
ARR_L:	RPA
	