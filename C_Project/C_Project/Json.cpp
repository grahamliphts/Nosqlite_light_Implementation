#include "Json.h"
#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <jansson.h>
#include <sys/stat.h>
#include "list_contigue.h"

void CreateHashmap()
{
	t_hashmap* bdd = hashmap_create(50, 1.0f, 2.0f);

	//Create a collection
	t_list* collection0 = list_create(100); //100 => max entry count in the collection
	hashmap_put(bdd, "students", collection0);
	//new entries
	t_hashmap* entry0 = hashmap_create(50, 1.0f, 5.0f);
	hashmap_put(entry0, "name", "Gabriel");
	hashmap_put(entry0, "rate", "12");
	hashmap_put(entry0, "group", "AL2");
	list_append(collection0, entry0);
	t_hashmap* entry1 = hashmap_create(50, 1.0f, 5.0f);
	hashmap_put(entry1, "title", "SSD 1To CRUCIAL");
	hashmap_put(entry1, "ref", "SSD-CR-1TO");
	hashmap_put(entry1, "price", "52");
	list_append(collection0, entry1);

	//Create a collection
	t_list* collection1 = list_create(100); //100 => max entry count in the collection
	hashmap_put(bdd, "profs", collection1);
	//new entries
	t_hashmap* entry2 = hashmap_create(50, 1.0f, 5.0f);
	hashmap_put(entry2, "name", "Martel");
	hashmap_put(entry2, "score", "42");
	hashmap_put(entry2, "game", "HL3");
	list_append(collection1, entry2);
	t_hashmap* entry3 = hashmap_create(50, 1.0f, 5.0f);
	hashmap_put(entry3, "ohoh", "SSD 1ko USELESS");
	hashmap_put(entry3, "code", "SSD-CR-1B00B");
	list_append(collection1, entry3);
	t_hashmap* entry4 = hashmap_create(50, 1.0f, 5.0f);
	hashmap_put(entry4, "aaaa", "bbbb");
	list_append(collection1, entry4);

	SaveBDD(bdd);
	loadBDD(readJson());
}
char* readJson()
{
	FILE *file;
	fopen_s(&file, "BDD.json", "r");
	struct stat st;
	stat("BDD.json", &st);
	long size = st.st_size;
	char* bdd = (char*)calloc(1, size);
	fread(bdd, 1, size, file);

	fclose(file);
	return bdd;
}

void SaveBDD(t_hashmap* map)
{
	json_t *root, *table;
	t_hashmap_key* tablekeys = hashmap_keys(map);
	int i, j;
	root = json_array();
	for (i = 0; i < tablekeys->length; i++)
	{
		json_t* name, *elements;
		table = json_object();
		name = json_string(tablekeys->keys[i]);
		elements = json_array();
		json_object_set(table, "name", name);
		

		t_list* list = (t_list*)hashmap_get(map, tablekeys->keys[i]);
		for (j = 0; j < list->length; j++)
		{		
			json_t* entry;
			entry = json_object();
			
			t_hashmap* hashtable = list->data[j];
			t_hashmap_key* entrykeys = hashmap_keys(hashtable);
			int k = 0;
			for (k = 0; k < entrykeys->length; k++)
			{
				json_t* value;
				char* valuestr = (char*)hashmap_get(hashtable, entrykeys->keys[k]);
				value = json_string(valuestr);
				json_object_set(entry, entrykeys->keys[k], value);
			}
			json_array_append(elements, entry);
		}
		json_object_set(table, "values", elements);
		json_array_append(root, table);
	}
	json_dump_file(root, "BDD.json", JSON_INDENT(4));
}
t_hashmap* loadBDD(char* text)
{
	json_error_t error;
	json_t *root;
	root = json_loads(text, 0, &error);
	free(text);
	t_hashmap* map;
	map = hashmap_create(50, 1.0f, 5.0f);

	if (!root)
		fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
	if (!json_is_array(root))
	{
		fprintf(stderr, "error: root is not an array\n");
		json_decref(root);
	}
	int i,j;
	for (i = 0; i < json_array_size(root); i++)
	{
		json_t *table;

		table = json_array_get(root, i);
		if (!json_is_object(table))
		{
			fprintf(stderr, "error: table data %d is not an object\n", i + 1);
			json_decref(table);
		}
		
		json_t *name, *elements;
		
		name = json_object_get(table, "name");
		const char* namestr = json_string_value(name);
		printf("Table name : %s \n", namestr);
		elements = json_object_get(table, "values");

		for (j = 0; j < json_array_size(elements); j++)
		{
			json_t *entry;
			entry = json_array_get(elements, j);

			void *iter = json_object_iter(entry);
			while (iter)
			{
				json_t *value;
				const char* key = json_object_iter_key(iter);
				value = json_object_iter_value(iter);
				if (json_is_string(value))
				{
					const char* valstr = json_string_value(value);
					printf("\t%s %s ", key, valstr);
				}
				else if (json_is_integer(value))
				{
					long long valstr = json_integer_value(value);
					printf("\t%s %d ", key, valstr);
				}
				else if (json_is_real(value))
				{
					double valstr = json_real_value(value);
					printf("\t%s %f ", key, valstr);
				}
				printf("\n");
				iter = json_object_iter_next(entry, iter);
			}
			printf("\n");
		}
		printf("\n\n\n");
	}

	return 0;
}