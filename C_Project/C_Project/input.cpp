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

char** requestTreatment(const char* line)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int OptionNum = 0;
	int counter;
	unsigned int sharpCounter = 0;
	char request = line[i];
	char * OptionsArray[10];
	for (k = 0; k < 10; k++)
		OptionsArray[k] = (char*)malloc(200 * sizeof(char)); //-------Option string allocation
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
	printf("%s", "-------Option List--------\n");
	for (counter = 0; counter < OptionNum; counter++)
		printf("%s\n",OptionsArray[counter]);
	putchar('\n');

	return OptionsArray;
}