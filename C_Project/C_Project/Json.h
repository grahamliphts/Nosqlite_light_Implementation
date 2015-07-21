#include "Hashmap.h"
void CreateHashmap();
char* readJson();
t_hashmap* loadBDD(char* text);
t_hashmap* JSON_parse(char* string);
char* JSON_stringify(t_hashmap* map);
void SaveBDD(t_hashmap* map);