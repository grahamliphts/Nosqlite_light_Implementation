#ifndef HASHMAP_H
#define HASHMAP_H
// Header Hashmap

// --------------------------------------------------
// Structures Hashmaps
// --------------------------------------------------

// Entrée Hashmap
typedef struct s_hashmap_entry
{
	void* value;
	char* key;
	struct s_hashmap_entry* next;
} t_hashmap_entry;

// Structure Hashmap
typedef struct s_hashmap
{
	t_hashmap_entry** entries;
	unsigned int slots;
	double load_factor;
	double grow_factor;
	int size;
} t_hashmap;

// Key Hashmap
typedef struct s_hashmap_key
{
	char** keys;
	unsigned int length;
} t_hashmap_key;
// --------------------------------------------------


// --------------------------------------------------
// Fonctions Hashmap
// --------------------------------------------------

t_hashmap* hashmap_create(unsigned int slots, double lf, double gf);
t_hashmap_entry* hashmap_entry_create(char* key, void* value);
t_hashmap_key* hashmap_keys(t_hashmap* map);

void hashmap_put(t_hashmap* map, char* key, void* value);
void* hashmap_get(t_hashmap* map, char* key);
unsigned int hashmap_hashcode(char* key, unsigned int max);
void hashmap_remove(t_hashmap * map, char* key);

void hashmap_display(t_hashmap* map, void(*displayer)(void*));
// --------------------------------------------------

#endif