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
	for (counter = 0; counter < OptionNum; counter++)
		ShowTreatedOption(TreatedRequest[counter]);
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
	i++;//---------------To remove the '-' char
	TreatedOption->Arguments = (char*)malloc(strlen(Option) - i);
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
	}
	return ERROR;
}

void ShowTreatedOption(Options* OptionToShow)
{
	printf(" Option = %d, Arguments = %s\n", OptionToShow->OptionName, OptionToShow->Arguments);
}