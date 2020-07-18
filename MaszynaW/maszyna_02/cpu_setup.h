#ifndef CPU_SETUP_H
#define CPU_SETUP_H

/** Funkcja inicjalizuj�ca obiekt struktury CPUPreference.
@param name nazwa ustawienia
@return nowy obiekt */
struct CPUPreference cpu_preference_init(const char* name);
/** Funkcja usuwaj�ca podany obiekt
@param pref obiekt do usuni�cia */
void cpu_preference_delete(struct CPUPreference* pref);

/** Funkcja inicjalizuj�ca obiekt struktury CPUSetup.
@return nowy obiekt */
struct CPUSetup cpu_setup_init();
/** Funkcja usuwaj�ca podany obiekt
@param setup obiekt do usuni�cia */
void cpu_setup_delete(struct CPUSetup* setup);

#endif