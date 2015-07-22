#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream> 
#include "input.h"
#include <string.h>
#include "list_contigue.h"


char * getline(void) {
	char * line = (char*)malloc(100), *linep = line;
	size_t lenmax = 100, len = lenmax;
	int c;

	if (line == NULL)
		return NULL;

	for (;;) {
		c = fgetc(stdin);
		if (c == EOF)
			break;

		if (--len == 0) {
			len = lenmax;
			char * linen = (char*)realloc(linep, lenmax *= 2);

			if (linen == NULL) {
				free(linep);
				return NULL;
			}
			line = linen + (line - linep);
			linep = linen;
		}

		if ((*line++ = c) == '\n')
			break;
	}
	*line = '\0';
	return linep;
}

void requestTreatment(const char* line, Options** TreatedRequest)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int OptionNum = 0;
	int counter;
	unsigned int sharpCounter = 0;
	char request = line[i];
	char * OptionsArray[10];

	for (counter = 0; counter < 10; counter++)
	{
		TreatedRequest[counter] = (Options*)malloc(sizeof(Options));
		TreatedRequest[counter]->OptionName = ERROR;
	}
	for (k = 0; k < 10; k++)
	{
		OptionsArray[k] = (char*)malloc(200 * sizeof(char)); //-------Option string allocation
		OptionsArray[k][0] = '\0';
	}
	unsigned char IsinOption = 0;

	while (request != '\n')
	{
		
		if (request == '"')
			sharpCounter++;

		if (IsinOption == 1)
		{
			if (sharpCounter < 2)
			{
				OptionsArray[OptionNum][j] = request;
				j++;
			}
			else
			{
				OptionsArray[OptionNum][j] = '"';
				OptionsArray[OptionNum][j+1] = '\0';
				OptionNum++;
				IsinOption = 0;
				j = 0;
				sharpCounter = 0;
			}
		}
		if (request == '-')
		{
			IsinOption = 1;
		}
		//putchar(request); //-------------------Debug Line value-----------------
		i++;
		request = line[i];
	}
	//-----------------------------End Treatments----------------------------
	//OptionNum++;
	if (IsinOption)
	{
		OptionsArray[OptionNum][j] = '\0';
		OptionNum++;
	}
	/*printf("%s", "-------Option List--------\n"); //--------------------------------------Print Bloc for debugging purpose--------------------
	for (counter = 0; counter < OptionNum; counter++)
		printf("%s\n",OptionsArray[counter]);
	putchar('\n');
	*/
	for (counter = 0; counter < OptionNum; counter++)
		OptionTreatment(OptionsArray[counter],TreatedRequest[counter]);

}

void OptionTreatment(char* Option,Options* TreatedOption)
{
	int i = 0;
	int j = 0;

	mode Active_mode;

	Active_mode = OptionExtract((const char*)Option);
	if (Active_mode == ERROR)
	{
		printf("%s\n", "no suitable option found");
		return;
	}
	TreatedOption->OptionName = Active_mode;
	while (Option[i] != '\0' && Option[i] != '=')
		i++;
	//i++;//---------------To remove the '-' char
	if (strlen(Option) - i < 0)
		TreatedOption->Arguments = (char*)malloc((strlen(Option) - i)*sizeof(char));
	else
		TreatedOption->Arguments = (char*)malloc(sizeof(char));
	TreatedOption->Arguments[0] = '\0';
	while (Option[i] != '\0')
	{
		TreatedOption->Arguments[j] = Option[i];
		i++;
		j++;
	}
	TreatedOption->Arguments[j] = '\0';
}

mode OptionExtract(const char* OptionLine)
{
	char* OptionName = (char*)malloc(20 * sizeof(char));
	int i = 0;
	while (OptionLine[i] != '=' && OptionLine[i] != '\0')
	{
		OptionName[i] = OptionLine[i];
		i++;
	}
	OptionName[i] = '\0';

	if (OptionName)
	{
		if (strcmp(OptionName, "collection") == 0)
			return COLLECTION;
		if (strcmp(OptionName, "insert") == 0)
			return INSERT;
		if (strcmp(OptionName, "where") == 0)
			return WHERE;
		if (strcmp(OptionName, "projection") == 0)
			return PROJECTION;
		if (strcmp(OptionName, "remove") == 0)
			return REMOVE;
		if (strcmp(OptionName, "find") == 0)
			return FIND;
		if (strcmp(OptionName, "sort") == 0)
			return SORT;
		if (strcmp(OptionName, "set") == 0)
			return SET;
	}
	return ERROR;
}

void ShowTreatedOption(Options* OptionToShow)
{
	if (OptionToShow->OptionName != ERROR)
		printf(" Option = %d, Arguments = %s\n", OptionToShow->OptionName, OptionToShow->Arguments);
}

int ApplyTreatedRequest(t_hashmap* BDD, Options** TreatedRequest, int ArraySize)
{
	int counter;
	unsigned char IsCollectionSelected = 0;
	unsigned char IsOptionActive = 0;
	
	char* WorkingCollection = (char*)malloc(20 * sizeof(char));

	int* FindResult = (int*)malloc(sizeof(int) * 30);

	for (counter = 0; counter < ArraySize; counter++)
	{
		switch (TreatedRequest[counter]->OptionName)
		{
		case COLLECTION:
			if (IsCollectionSelected == 0)
			{
				IsCollectionSelected = 1;
				WorkingCollection = extractBetween('"', '"', TreatedRequest[counter]->Arguments);
				printf("Working Collection = %s\n", WorkingCollection);
			}
			else
			{
				printf("%s\n", "Error : Two Collection selected");
				return 1;
			}
			break;

		case INSERT:
			return insertInto(BDD, WorkingCollection, extractBetween('"', '"', TreatedRequest[counter]->Arguments));
			break;
		case REMOVE:
			return removeFrom(BDD, WorkingCollection, extractBetween('"', '"', TreatedRequest[counter]->Arguments));
			break;
		case PROJECTION:
			// ----------------------Dependance Reasearch
			if (lookatOption(FIND, TreatedRequest, ArraySize) != 0)
			{
				if (FindResult[0] > 0)
				{
					if (lookatOption(SORT, TreatedRequest, ArraySize) != 0)
						Sort(BDD, WorkingCollection, extractBetween('"', '"', TreatedRequest[counter]->Arguments), FindResult);
					Projection(BDD, WorkingCollection, extractBetween('"', '"', TreatedRequest[counter]->Arguments), FindResult);
				}
			}
			else
				printf("%s\n", "'-find' Option Dependency error");
			break;
		case FIND:
			// ----------------------Dependance Reasearch
			if (lookatOption(PROJECTION, TreatedRequest, ArraySize) != 0)
			{
				find(BDD, WorkingCollection, extractBetween('"', '"', TreatedRequest[counter]->Arguments), FindResult);

				if (FindResult[0] == -1)
					printf("Aucun Resultat trouvé !");
			}
			else
				printf("%s\n", "'-projection' Option Dependency error");
			break;
		case SET:
			// ----------------------Dependance Reasearch
			if (lookatOption(WHERE, TreatedRequest, ArraySize) != 0)
			{
				char* setArgu = extractBetween('"', '"', TreatedRequest[counter]->Arguments);
				char* whereArgu = extractBetween('"', '"', TreatedRequest[counter + 1]->Arguments);
				Set(BDD, WorkingCollection, setArgu, whereArgu);
			}
			else
				printf("%s\n", "'-where' Option Dependency error");
			break;
		case SORT:
			// ----------------------Dependance Reasearch
			if (lookatOption(PROJECTION, TreatedRequest, ArraySize) != 0)
			{
				;
			}
			else
				printf("%s\n", "'-projection' Option Dependency error");
			break;
		}
	}
	//	ShowTreatedOption(TreatedRequest[counter]);
	
	
	return 0;
}
	
char* extractBetween(const char start,const char end,const char* Line)
{
	int i = 0;
	int j = 0;
	char* toreturn = (char*)malloc(strlen(Line)*sizeof(char));
	toreturn[0] = '\0';
	while (Line[i] != '\0' && Line[i] != start)
		i++;
	if (Line[i] == start && Line[i] != '\0') // ---------------------Case of fisrt "start" occurence occure on 1st character of Line----------------------
		i++;

	while (Line[i] != '\0' && Line[i] != end)
	{
		toreturn[j] = Line[i];
		i++;
		j++;
	}
	toreturn[j] = '\0';
	return toreturn;
}

int insertInto(t_hashmap* BDD, char* collection, char* Insert_arguments)
{
	t_hashmap* table_h = (t_hashmap*)hashmap_create(15, 0.6, 2.1);
	t_list* collection_l = (t_list*)hashmap_get(BDD, collection);
	t_list_arg* arguments = ReadArguments(Insert_arguments);

	if (collection_l != NULL)
	{
		int i = 0;
		unsigned char found = 0;
		for (i = 0; i < collection_l->length ; i++)
		{
			t_hashmap* table = collection_l->data[i];
			if (strcmp((char*)hashmap_get(table, (char*)arguments->data[0]->key), (char*)arguments->data[0]->value) == 0)
			{
				found = 1;
				printf("La cle %s de valeur %s existe deja, voulez-vous la remplacer ? Y/N\n", (char*)arguments->data[0]->key, (char*)arguments->data[0]->value);
				break;
			}
		}

		if (found == 0)
		{
			unsigned int i = 0;
			for (i; i < arguments->length; i++)
			{
				hashmap_put(table_h, (char*)arguments->data[i]->key, arguments->data[i]->value);
			}

			list_append(collection_l, table_h);
			hashmap_put(BDD, collection, collection_l);
		}
	}
	else
	{
		collection_l = list_create(10);

		unsigned int i = 0;
		for (i; i < arguments->length; i++)
		{
			hashmap_put(table_h, (char*)arguments->data[i]->key, arguments->data[i]->value);
		}

		list_append(collection_l, table_h);
		hashmap_put(BDD, collection, collection_l);
	}

	return 0;
}

int removeFrom(t_hashmap* BDD, char* collection, char* Remove_arguments)
{
	t_list_arg* arguments = ReadArguments(Remove_arguments);
	t_list* collection_l = (t_list*)hashmap_get(BDD, collection);

	int nbResult = 0;

	if (collection_l != NULL)
	{
		unsigned int i = 0;
		for (i; i < collection_l->length; i++)
		{
			t_hashmap* table = collection_l->data[i];
			unsigned char found = 0;

			int j = 0;
			for (j; j < arguments->length; j++)
			{
				char* value = (char*)hashmap_get(table, (char*)arguments->data[j]->key);
				if (value != NULL && strcmp(value, (char*)arguments->data[j]->value) == 0)
				{
					found = 1;
					break;
				}
			}

			if (found == 1)
			{
				list_remove_index(collection_l, i);
			}
		}
	}
	else
	{
		printf("Collection doesn't exist");
	}

	return 0;
}

int Set(t_hashmap* BDD, char* collection, char* Set_arguments, char* Where_arguments)
{
	t_list_arg* setArguments = ReadArguments(Set_arguments);
	t_list_arg* whereArguments = ReadArguments(Where_arguments);

	t_list* collection_l = (t_list*)hashmap_get(BDD, collection);

	int nbResult = 0;

	if (collection_l != NULL)
	{
		unsigned int i = 0;
		for (i; i < collection_l->length; i++)
		{
			t_hashmap* table = collection_l->data[i];
			unsigned char found = 1;

			int j = 0;
			for (j; j < whereArguments->length; j++)
			{
				char* value = (char*)hashmap_get(table, (char*)whereArguments->data[j]->key);
				if (value == NULL || strcmp(value, (char*)whereArguments->data[j]->value) != 0)
				{
					found = 0;
					break;
				}
			}

			if (found == 1)
			{
				int k = 0;
				for (k; k < setArguments->length; k++)
				{
					char* value = (char*)hashmap_get(table, (char*)setArguments->data[k]->key);
					if (value != NULL)
					{
						table->entries[hashmap_hashcode((char*)setArguments->data[k]->key, 15)]->value = (char*)setArguments->data[k]->value;
					}
				}
			}
		}
	}
	else
	{
		printf("Collection doesn't exist");
	}

	return 0;
}

int Projection(t_hashmap* BDD, char* collection, char* Projection_arguments, int* findResult)
{
	t_list* collection_l = (t_list*)hashmap_get(BDD, collection);
	t_list_arg* arguments = ReadArguments(Projection_arguments);

	if (collection_l != NULL)
	{
		unsigned int i = 1;
		if (findResult[0] > 0)
		{
			for (i; i <= findResult[0]; i++)
			{
				//printf("{\n");
				t_hashmap* table = collection_l->data[findResult[i]];

				int j = 0;
				for (j; j < arguments->length; j++)
				{
					char* value = (char*)hashmap_get(table, (char*)arguments->data[j]->key);
					printf("%s\t", value);
				}
				printf("\n");
				//printf("}\n");
			}
			printf("\n");
		}
	}
	else
	{
		printf("Collection doesn't exist");
	}
	return 0;
}

int find(t_hashmap* BDD, char* collection, char* Find_arguments, int* findResult)
{
	t_list* collection_l = (t_list*)hashmap_get(BDD, collection);

	if (strcmp(Find_arguments, "{}") == 0)
	{
		int i = 1;
		findResult[0] = collection_l->length;
		for (i; i <= collection_l->length; i++)
		{
			findResult[i] = i - 1;
		}
		return 0;
	}

	t_list_arg* arguments = ReadArguments(Find_arguments);

	int nbResult = 0;

	if (collection_l != NULL)
	{
		unsigned int i = 0;
		for (i; i < collection_l->length; i++)
		{
			t_hashmap* table = collection_l->data[i];
			unsigned char found = 1;

			int j = 0;
			for (j; j < arguments->length; j++)
			{
				char* value = (char*)hashmap_get(table, (char*)arguments->data[j]->key);
				if (value == NULL || strcmp(value, (char*)arguments->data[j]->value) != 0)
				{
					found = 0;
					break;
				}
			}

			if (found == 1)
			{
				findResult[nbResult + 1] = i;
				nbResult++;
			}
		}

		findResult[0] = nbResult == 0 ? -1 : nbResult;
	}
	else
	{
		printf("Collection doesn't exist");
	}

	return 0;
}

int Sort(t_hashmap* BDD, char* collection, char* Sort_arguments, int* findResult)
{
	t_list* collection_l = (t_list*)hashmap_get(BDD, collection);
	t_list_arg* arguments = ReadArguments(Sort_arguments);

	int* new_find = (int*)malloc(sizeof(int) * 30);

	if (collection_l != NULL)
	{
		unsigned int i = 1;
		if (findResult[0] > 0)
		{
			float lower_value;
			int lower_index = -1;

			new_find[0] = findResult[0];
			int i = 1;
			for (i; i <= findResult[0]; i++)
			{
				int j = 1;

				t_hashmap* table = collection_l->data[findResult[i]];
				char* value = (char*)hashmap_get(table, (char*)arguments->data[0]->key);

				try
				{
					lower_value = atof(value);
				}
				catch (int e)
				{
					printf("Impossible de trier sur cette cle !");
				}

				for (j; j <= findResult[0]; j++)
				{
					t_hashmap* table = collection_l->data[findResult[j]];
					char* value = (char*)hashmap_get(table, (char*)arguments->data[0]->key);
					float value_float;

					try
					{
						value_float = atof(value);
					}
					catch (int e)
					{
						printf("Impossible de trier sur cette cle !");
					}

					if (lower_value > value_float)
					{
						lower_value = value_float;
						lower_index = findResult[j];
					}
				}

				if (lower_index != -1)
				{
					findResult[lower_index] = findResult[i];
					findResult[i] = lower_index;
				}
			}
		}
	}
	else
	{
		printf("Collection doesn't exist");
	}
	return 0;
}

unsigned int lookatOption(mode Research, Options** TreatedRequest, int ArraySize)
{
	int counter = 0;
	for (counter = 0; counter < ArraySize; counter++)
	{
		if (TreatedRequest[counter]->OptionName == Research)
			return counter;
	}
	return 0;
}

t_list_arg* ReadArguments(char* Insert_arguments) 
{
	t_list_arg* arguments = (t_list_arg*)malloc(sizeof(t_list_arg));
	arguments->data = (t_arg**)malloc(sizeof(t_arg*) * 10);
	arguments->length = 0;
	arguments->max = 10;

	int i = -1;
	char c;
	char* key   = (char*)malloc(sizeof(char) * 200);
	char* value = (char*)malloc(sizeof(char) * 200);

	int keyF = 0;
	int valueF = 0;

	int nbCharKey = 0;
	int nbCharValue = 0;
	unsigned char equal = 0;

	do
	{
		i++;
		c = Insert_arguments[i];

		if (c == '{' || c == ' ' || c == '\'')
			continue;

		if (c == ',' || c == '}')
		{
			key[nbCharKey] = '\0';
			value[nbCharValue] = '\0';

			t_arg* argument = (t_arg*)malloc(sizeof(t_arg));
			argument->key = key;
			argument->value = value;

			arguments->data[arguments->length] = argument;
			arguments->length++;

			nbCharKey = 0;
			nbCharValue = 0;
			keyF = 0;
			valueF = 0;
			equal = 0;

			key = (char*)malloc(sizeof(char) * 200);
			value = (char*)malloc(sizeof(char) * 200);
			continue;
		}

		if (valueF == 0 && c != ':' && equal == 0)
		{
			if (keyF == 0)
				keyF = i;

			key[i - keyF] = c;
			nbCharKey++;
			continue;
		}

		if (c == ':')
		{
			equal = 1;
			continue;
		}

		if (valueF == 0)
		{
			valueF = i;
		}

		value[i - valueF] = c;
		nbCharValue++;
	}
	while (c != '\0');

	return arguments;
}