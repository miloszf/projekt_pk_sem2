		SOB MAIN
		SOB	INT1
		SOB INT2
		SOB INT3
		SOB INT4
		
MAIN:	MAS 15
		POB CST_1
		ODE CST_1
		�AD	CNT_I1
		�AD	CNT_I2
		�AD	CNT_I3
		�AD	CNT_I4
		MAS 0
		WPR 1
		�AD CHAR
		WPR 1
		�AD N
		ODE ZERO
		SOM STOP
		SOZ STOP
		�AD N
		POB CST_9
		ODE N
		SOM STOP
		POB N
		ODE N
		ODE N
		�AD N
L_COL:	�AD I
		POB N
L_LINE:	�AD J
		POB CHAR
		WYP 2
		POB J
		DOD CST_1
		SOM L_LINE
		POB NL
		WYP 2
		POB I
		DOD CST_1
		SOM L_COL
STOP:	STP

CST_1:	RST 1
CST_9:	RST 9
ZERO:	RST '0'
NL:		RST 13
CHAR:	RPA
N:		RPA
I:		RPA
J:		RPA

CNT_I1:	RPA
CNT_I2: RPA
CNT_I3: RPA
CNT_I4: RPA

INT1:	CZM MASK_1
		MAS 15
		DNS
		POB INTN_1
		DOD ZERO
		WYP 2
		POB CNT_I1
		DOD CST_1
		�AD CNT_I1
		POB INTN_1
		DOD INTN_1
		ODE CNT_I1
		SOZ STOP
		PZS
		MSK MASK_1
		PWR
		
MASK_1:	RPA
INTN_1:	RST 1

INT2:	CZM MASK_2
		MAS 15
		DNS
		POB INTN_2
		DOD ZERO
		WYP 2
		POB CNT_I2
		DOD CST_1
		�AD CNT_I2
		POB INTN_2
		DOD INTN_2
		ODE CNT_I2
		SOZ STOP 
		PZS      
		MSK MASK_2
		PWR
		
MASK_2:	RPA
INTN_2:	RST 2

INT3:	CZM MASK_3
		MAS 15
		DNS
		POB INTN_3
		DOD ZERO
		WYP 2
		POB CNT_I3
		DOD CST_1
		�AD CNT_I3
		POB INTN_3
		DOD INTN_3
		ODE CNT_I3
		SOZ STOP 
		PZS      
		MSK MASK_3
		PWR
		
MASK_3:	RPA
INTN_3:	RST 3

INT4:	CZM MASK_4
		MAS 15
		DNS
		POB INTN_4
		DOD ZERO
		WYP 2
		POB CNT_I4
		DOD CST_1
		�AD CNT_I4
		POB INTN_4
		DOD INTN_4
		ODE CNT_I4
		SOZ STOP 
		PZS      
		MSK MASK_4
		PWR
		
MASK_4:	RPA
INTN_4:	RST 4
TEST_1: RST 1		
TEST_2: RST 2		
		