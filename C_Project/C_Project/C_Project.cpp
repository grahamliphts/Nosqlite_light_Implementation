// C_Project.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "input.h"


int main(void){
	char* line;
	const int OptionArrayMaxSize = 10;
	Options* TreatedRequest[OptionArrayMaxSize];
	do{
	line = getline();
	requestTreatment(line, TreatedRequest);
	ApplyTreatedRequest(TreatedRequest, OptionArrayMaxSize);
	} while (strcmp(line, "exit\n") != 0);
	return 0;
}
