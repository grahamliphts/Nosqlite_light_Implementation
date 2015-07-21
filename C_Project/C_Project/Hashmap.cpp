#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Hashmap.h"

// ------------------------------
// Function:    hashmap_create
// Description: Create an hashmap and return pointer
// Return:      hashmap struct
// ------------------------------
t_hashmap* hashmap_create(unsigned int slots, double lf, double gf)
{
	t_hashmap* map = (t_hashmap*)malloc(sizeof(t_hashmap));
	map->slots = slots;
	map->load_factor = lf;
	map->grow_factor = gf;

	map->size = 0;
	map->entries = (t_hashmap_entry**)calloc(slots, sizeof(t_hashmap_entry*));

	return map;
}


// ------------------------------
// Function:    hashmap_put
// Description: add entry to hashmap
// Return:      null
// ------------------------------
void hashmap_put(t_hashmap* map, char* key, void* value)
{
	if (map->size >= (map->load_factor * map->slots))
	{
		t_hashmap_entry** new_entries = (t_hashmap_entry**)calloc(map->slots * map->grow_factor, sizeof(t_hashmap_entry*));

		int i;
		for (i = 0; i < map->slots; i++)
		{
			t_hashmap_entry* entry = map->entries[i];

			while (entry)
			{
				t_hashmap_entry* next = entry->next;
				unsigned int code = hashmap_hashcode(entry->key, map->slots * map->grow_factor);
				entry->next = new_entries[code];
				new_entries[code] = entry;
				entry = next;
			}
		}

		free(map->entries);
		map->entries = new_entries;

		printf("new entries created\n");
	}

	unsigned int hashcode = hashmap_hashcode(key, map->slots);

	t_hashmap_entry** current = &map->entries[hashcode];
	while (*current)
	{
		if (!strcmp((*current)->key, key))
		{
			(*current)->value = value;
			return;
		}
		*current = (*current)->next;
	}
	*current = hashmap_entry_create(key, value);
	map->size++;
}


// ------------------------------
// Function:    hashmap_hashcode
// Description: create an hashcode based on the key
// Return:      hashcode
// ------------------------------
unsigned int hashmap_hashcode(char* key, unsigned int max)
{
	unsigned int hashcode = 0;
	while (*key)
	{
		hashcode += *key;
		key++;
	}
	hashcode %= max;

	return hashcode;
}


// ------------------------------
// Function:    hashmap_get
// Description: get the value from the key
// Return:      value
// ------------------------------
void* hashmap_get(t_hashmap* map, char* key)
{
	unsigned int hashcode = hashmap_hashcode(key, map->slots);

	t_hashmap_entry* current = map->entries[hashcode];
	while (current)
	{
		if (!strcmp(current->key, key))
		{
			return current->value;
		}
		current = current->next;
	}
	return NULL;
}


// ------------------------------
// Function:    hashmap_entry_create
// Description: Create an entry for the hashmap
// Return:      hashmap entry
// ------------------------------
t_hashmap_entry* hashmap_entry_create(char* key, void* value)
{
	t_hashmap_entry* entry = (t_hashmap_entry*)malloc(sizeof(t_hashmap_entry));
	entry->key = key;
	entry->value = value;
	entry->next = NULL;

	return entry;
}


// ------------------------------
// Function:    displayer
// Description: Display d
// Return:      None
// ------------------------------
void displayer(void *d)
{
	printf("%d\n", (int)d);
}


// ------------------------------
// Function:    hashmap_keys
// Description:
// Return:
// ------------------------------
t_hashmap_key* hashmap_keys(t_hashmap* map)
{
	t_hashmap_key* hkeys = (t_hashmap_key*)malloc(sizeof(t_hashmap_key));

	int i;
	for (i = 0; i < map->slots; i++)
	{
		t_hashmap_entry* current = map->entries[i];
		while (current)
		{
			if (map->entries[i]->key != NULL)
			{
				hkeys->keys[hkeys->length] = map->entries[i]->key;
				hkeys->length++;
				current = current->next;
			}
		}
	}
	return hkeys;
}


// ------------------------------
// Function:    hashmap_display
// Description:
// Return:      None
// ------------------------------
void hashmap_display(t_hashmap* map, void(*displayer)(void*))
{
	t_hashmap_key* hkeys = hashmap_keys(map);
}

// ------------------------------
// Function:    hashmap_remove
// Description:
// Return:      None
// ------------------------------
void hashmap_remove(t_hashmap * map, char* key)
{
	unsigned int hash = hashmap_hashcode(key, map->slots);
	t_hashmap_entry* tmp;
	t_hashmap_entry** entry = &map->entries[hash];

	while (*entry)
	{
		if (!strcmp(key, (*entry)->key))
		{
			tmp = *entry;
			*entry = (*entry)->next;
			free(tmp);
			map->size--;
			break;
		}
		entry = &(*entry)->next;
	}
}
