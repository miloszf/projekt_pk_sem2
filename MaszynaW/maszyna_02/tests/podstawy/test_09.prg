// Test 09 - zdane
// Zamiana parami element�w tablicy N-elementowej (N parzyste) (ABCDEF->BADCFE)
		POB �AD_P
		ODE POB_P
		�AD �AD_P
		POB POB_P
		�AD P_ADR
		POB ARR_L
LOOP:	�AD VAR_I
		SOZ STOP
P_ADR:	RPA
		�AD TEMP
		POB P_ADR
		DOD CST_1
		�AD P_ADR1
P_ADR1:	RPA
		�AD TEMP2
		POB P_ADR
		DOD �AD_P
		�AD �_ADR
		POB TEMP2
�_ADR:	RPA
		POB �_ADR
		DOD CST_1
		�AD �_ADR1
		POB TEMP
�_ADR1:	RPA
		POB P_ADR
		DOD CST_2
		�AD P_ADR
		POB VAR_I
		ODE CST_2
		SOB LOOP
		
STOP:	STP


CST_1:	RST 1
CST_2:	RST 2
ARR_L:	RST 6
ARRAY:	RST 10
		RST 11
		RST 12
		RST 13
		RST 14
		RST 15
POB_P:	POB ARRAY
�AD_P:	�AD ARRAY
VAR_I:	RPA
TEMP:	RPA
TEMP2: 	RPA

	