// C_Project.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "input.h"


int main(void){
	char* line;
	char** RequestOptions;
	do{
	line = getline();
	//printf("%s\n", line);
	RequestOptions = requestTreatment(line);
	OptionTreatment(RequestOptions);
	} while (strcmp(line, "exit\n") != 0);
	return 0;
}
