#ifndef CPU_SETUP_H
#define CPU_SETUP_H

/** Funkcja inicjalizująca obiekt struktury CPUPreference.
@param name nazwa ustawienia
@return nowy obiekt */
struct CPUPreference cpu_preference_init(const char* name);
/** Funkcja usuwająca podany obiekt
@param pref obiekt do usunięcia */
void cpu_preference_delete(struct CPUPreference* pref);

/** Funkcja inicjalizująca obiekt struktury CPUSetup.
@return nowy obiekt */
struct CPUSetup cpu_setup_init();
/** Funkcja usuwająca podany obiekt
@param setup obiekt do usunięcia */
void cpu_setup_delete(struct CPUSetup* setup);

#endif