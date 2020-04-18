#ifndef ERROR_H
#define ERROR_H
// Struktura zajmuj¹ca siê obs³ug¹ b³êdów
// - jedna na ca³y program
// - koñczy pracê programu w razie wykrycia b³êdu
// - komunikat?

typedef int Error;
// b³êdy krytyczne, wyst¹pienie oznacza natychmiastowe zamkniêcie programu
// - b³¹d alokacji pamiêci
// - wy³uskanie wskaŸnika NULL
// - ?inne?

// b³êdy niekrytyczne, program wyœwietla stosowny komunikat
// i czeka na reakcjê u¿ytkownika:
// - przetwarzania podanych plików:
//   - b³¹d otwierania pliku
//   - nierozpoznane znaczniki
//   - niepoprawne wartoœci
// - b³¹d podczas wykonywania instrukcji/programu:
//   - nierozpoznana wartoœæ w rejestrze instrukcji

Error error();
void set_error(Error error, const char* message);
void* malloc_s(size_t size);
void* calloc_s(size_t count, size_t size);
void* realloc_s(void* ptr, size_t new_size);
void check_for_NULL(void* ptr);

#endif