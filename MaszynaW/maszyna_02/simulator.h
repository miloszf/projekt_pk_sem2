#ifndef SIMULATOR_H
#define SIMULATOR_H

/** G��wna funkcja aplikacji, inicjalizuje wszystkie elementy symulatora, 
	kieruje wykonaniem programu oraz obs�ug� b��d�w. Pod koniec pracy usuwa
	wszystkie elementy symulatora. 
@param instr_file_name nazwa pliku instrukcji
@param prog_file_name nazwa pliku programu */
void simulator_run(const char* instr_file_name, const char* prog_file_name);

#endif