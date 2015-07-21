#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include "input.h"
#include <string.h>

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
			return SORT;
	}
	return ERROR;
}

void ShowTreatedOption(Options* OptionToShow)
{
	if (OptionToShow->OptionName != ERROR)
		printf(" Option = %d, Arguments = %s\n", OptionToShow->OptionName, OptionToShow->Arguments);
}

int ApplyTreatedRequest(Options** TreatedRequest, int ArraySize)
{
	int counter;
	unsigned char IsCollectionSelected = 0;
	unsigned char IsOptionActive = 0;
	
	char* WorkingCollection = (char*)malloc(20 * sizeof(char));
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
			return insertInto(WorkingCollection, TreatedRequest[counter]->Arguments);
			break;
		case REMOVE:
			break;
		case PROJECTION:
			break;
		case FIND:
			// ----------------------Dependance Reasearch
			if (lookatOption(PROJECTION, TreatedRequest, ArraySize) != 0)
			{
				;
			}
			else
				printf("%s\n", "Option Dependency error");
			break;
		case SET:
			// ----------------------Dependance Reasearch
			if (lookatOption(WHERE, TreatedRequest, ArraySize) != 0)
			{
				;
			}
			else
				printf("%s\n", "Option Dependency error");
			break;
		case SORT:
			// ----------------------Dependance Reasearch
			if (lookatOption(PROJECTION, TreatedRequest, ArraySize) != 0)
			{
				;
			}
			else
				printf("%s\n", "Option Dependency error");
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

int insertInto(char* collection, char* Insert_arguments)
{
	return 0;
}

int removeFrom(char* collection, char* Remove_arguments, char* Where_arguments)
{
	return 0;
}

int Set(char* collection, char* Set_arguments, char* Where_arguments)
{
	return 0;
}

int Projection(char* collection, char* Projection_arguments, char* Where_arguments)
{
	return 0;
}

int find(char* collection, char* Find_arguments, char Where_arguments, char* Projection_arguments)
{
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

