#include "Json.h"
#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>

char* loadBDD()
{
	char *source = NULL;
	FILE *fp = fopen("BDD.json", "r");
	if (fp != NULL) 
	{
		/* Go to the end of the file. */
		if (fseek(fp, 0L, SEEK_END) == 0) 
		{
			/* Get the size of the file. */
			long bufsize = ftell(fp);
			if (bufsize == -1) 
			{
				printf("Error length file");
				return 0;
			}

			/* Allocate our buffer to that size. */
			source = (char*)malloc(sizeof(char)* (bufsize + 1));
			/* Go back to the start of the file. */
			if (fseek(fp, 0L, SEEK_SET) == 0) 
			{ 
				printf("Error start file");
				return 0;
			}

			/* Read the entire file into memory. */
			size_t newLen = fread(source, sizeof(char), bufsize, fp);
			if (newLen == 0)
				fputs("Error reading file", stderr);
			else
				source[++newLen] = '\0'; 
		}
		fclose(fp);
	}
	printf(source);
	free(source); /* Don't forget to call free() later! */
	return 0;
}
