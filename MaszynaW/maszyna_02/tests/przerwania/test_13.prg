// Test 13 - zdane
// Program wypisujący stale znak '-' lub numer danego przerwania w przypadku jego zgłoszenia
		SOB MAIN
		SOB	INT1
		SOB INT2
		SOB INT3
		SOB INT4
		
MAIN:	POB CHAR
		WYP 2
		SOB MAIN
		
CHAR:	RST '-'
		
INT1:	CZM MASK_1
		MAS 15
		DNS
		POB CHR_1
		WYP 2
		PZS
		MSK MASK_1
		PWR
		
MASK_1:	RPA
CHR_1:	RST '1'

INT2:	CZM MASK_2
		MAS 15
		DNS
		POB CHR_2
		WYP 2
		PZS
		MSK MASK_2
		PWR
		
MASK_2:	RPA
CHR_2:	RST '2'

INT3:	CZM MASK_3
		MAS 15
		DNS
		POB CHR_3
		WYP 2
		PZS
		MSK MASK_3
		PWR
		
MASK_3:	RPA
CHR_3:	RST '3'

INT4:	CZM MASK_4
		MAS 15
		DNS
		POB CHR_4
		WYP 2
		PZS
		MSK MASK_4
		PWR
		
MASK_4:	RPA
CHR_4:	RST '4'
