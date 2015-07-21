// C_Project.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "input.h"


int main(int argc, char** argv)
{
	if (argc < 3)
	{
		printf("Pas assez de parametres");
		return 1;
	}

	char* line;

	do
	{
		line = getline();
		printf("%s\n", line);

		if (line[0] == '{')
		{
			while (line[0] != '}' || strcmp(line, "exit\n") != 0)
			{

			}
		}
	} 
	while (strcmp(line, "exit\n") != 0);
	
	return 0;
}
