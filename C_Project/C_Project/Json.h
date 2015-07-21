#include "Hashmap.h"

char* readJson();
int loadBDD(char* text);
t_hashmap* JSON_parse(char* string);
char* JSON_stringify(t_hashmap* map);
void SaveBDD();