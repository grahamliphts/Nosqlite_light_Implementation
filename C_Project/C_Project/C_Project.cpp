// C_Project.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "input.h"
#include "list_contigue.h"
#include "Json.h"


int main(void)
{
	char* line;
	const int OptionArrayMaxSize = 10;
	Options* TreatedRequest[OptionArrayMaxSize];


	/* Zone de test */
	t_hashmap* BDD = (t_hashmap*)hashmap_create(20, 0.6, 2.1);

	insertInto(BDD, "student", "{ name : 'Gabriel', rate : 12, group :'AL2'}");
	insertInto(BDD, "student", "{ name : 'Loic', rate : 18.2, group :'AL2'}");
	insertInto(BDD, "student", "{ name : 'Benoit', rate : 11, group :'AL1'}");
	insertInto(BDD, "student", "{ name : 'Jean', rate : 11, group :'AL1'}");
	insertInto(BDD, "student", "{ name : 'Toto', rate : 14, group :'AL3'}");
	insertInto(BDD, "student", "{ name : 'Gru', rate : 15, group :'AL3'}");

	int* FindResult = (int*)malloc(sizeof(int) * 30);
	printf("-collection=\"student\" -find=\"{rate:11}\" -projection=\"{name:1,group:1}\"\n");
	find(BDD, "student", "{rate:11}", FindResult);
	Projection(BDD, "student", "{name:1,group:1}", FindResult);

	printf("-collection=\"student\" -set=\"{name:'lol'}\" -where=\"{rate:11}\"\n");
	Set(BDD, "student", "{name:'lol'}", "{rate:11}");

	printf("-collection=\"student\" -find=\"{rate:11}\" -projection=\"{name:1,group:1}\"\n");
	FindResult = (int*)malloc(sizeof(int) * 30);
	find(BDD, "student", "{rate:11}", FindResult);
	Projection(BDD, "student", "{name:1,group:1}", FindResult);

	removeFrom(BDD, "student", "{ name : 'Loic'}");
	FindResult = (int*)malloc(sizeof(int) * 30);
	find(BDD, "student", "{group :'AL2'}", FindResult);
	Projection(BDD, "student", "{name:1,group:1}", FindResult);

	printf("\n\n");

	find(BDD, "student", "{}", FindResult);
	//Sort(BDD, "student", "{rate:-1}", FindResult);
	Projection(BDD, "student", "{name:1,rate:1}", FindResult);

	// -------------------------------------------------

	do
	{
		line = getline();
		if (strcmp(line, "save\n") == 0)
		{
			SaveBDD(BDD);
		}
		else
		{
			requestTreatment(line, TreatedRequest);
			ApplyTreatedRequest(BDD, TreatedRequest, OptionArrayMaxSize);
		}
	} while (strcmp(line, "exit\n") != 0);
	return 0;
}
