#include "Json.h"
#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <jansson.h>
#include <sys/stat.h>

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

void SaveBDD()
{
	json_t *root;

	root = json_array();
	for (int i = 0; i < 5; i++) //pour chaque entrée : jsonobject = hashmap
	{
		json_t* object;
		object = json_object();
		json_t* string = json_string("val");
		json_t* five = json_integer(5 + i);
		json_t* taille = json_real(3.14f + (float)i);
		json_object_set(object, "name", string);
		json_object_set(object, "age", five);
		json_object_set(object, "taille", taille);
		json_array_append(root, object);
	}
	json_dump_file(root, "BDD.json", JSON_INDENT(4));
}
int loadBDD(char* text)
{
	json_error_t error;
	json_t *root;
	root = json_loads(text, 0, &error);
	free(text);

	if (!root)
	{
		fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
		return 1;
	}
	if (!json_is_array(root))
	{
		fprintf(stderr, "error: root is not an array\n");
		json_decref(root);
		return 1;
	}
	int i;
	int test = json_array_size(root);
	for (i = 0; i < json_array_size(root); i++)
	{

		json_t *object, *value;

		object = json_array_get(root, i);
		if (!json_is_object(object))
		{
			fprintf(stderr, "error: commit data %d is not an object\n", i + 1);
			json_decref(root);
			return 1;
		}
		value = json_object_get(object, "name");
		if (json_is_string(value))
		{
			const char* valstr = json_string_value(value);
			printf(valstr);
		}
	}

	return 0;
}