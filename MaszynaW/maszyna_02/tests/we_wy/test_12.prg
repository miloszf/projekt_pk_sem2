// Test 12 - zdane
// Program testuj�cy rozszerzony zestaw operacji JAL
		WPR 1
		ODE ZERO
		�AD NUMBER
MNOZ:	POB NUMBER
		MNO CST_3
		DNS
		SDP PRINT
DZIEL:	POB NUMBER
		DZI CST_3
		DNS
		SDP PRINT
INCR:	POB NUMBER
		IAK
		DNS
		SDP PRINT
DECR:	POB NUMBER
		DAK
		DNS
		SDP PRINT
  POB CST_3
SHFT_R:	SHR NUMBER VAR
		POB VAR
		DNS
		SDP PRINT
L_LUB:	LUB NUMBER CST_3 VAR
		POB VAR
		DNS
		SDP PRINT
L_I:	I NUMBER CST_3 VAR
		POB VAR
		DNS
		SDP PRINT
NEG_:	POB NUMBER
		NEG
		DNS
		SDP PRINT
STOP:	STP

PRINT:	PZS
		�AD P_PTR
		PZS
		�AD P_NUM
		POB CST_10
		ODE CST_10
		DNS
		ODE P_NUM
		SOM P_LOOP
		POB MINUS
		WYP 2
		POB P_NUM
P_LOOP:	�AD P_NUM
		DZI CST_10
		�AD P_VAR
		MNO CST_10
		ODE P_NUM
		DOD ZERO
		DNS
		POB P_VAR
		SOM P_LOOP
P_PRNT:	PZS
		SOZ P_STOP
		WYP 2
		SOB P_PRNT
P_STOP:	POB NEW_L
		WYP 2
		POB P_PTR
		DNS
		PWR

CST_10:	RST 10
P_PTR:	RPA
P_NUM:	RPA	
P_VAR:	RPA
		
ZERO:	RST '0'
MINUS:	RST '-'
CST_3:	RST 3
NEW_L:	RST 13

NUMBER:	RPA
VAR:	RPA