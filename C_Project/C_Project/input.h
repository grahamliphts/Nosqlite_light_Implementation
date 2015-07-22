enum mode { COLLECTION, INSERT, WHERE, PROJECTION, FIND, ERROR ,REMOVE,SORT,SET};

#include "Hashmap.h"
#include "list_contigue.h"

typedef struct Options Options;
struct Options
{
	mode OptionName;
	char* Arguments;
};


char * getline(void);
void requestTreatment(const char*, Options**);
void OptionTreatment(char* Option, Options*);
mode OptionExtract(const char* OptionLine);
void ShowTreatedOption(Options* OptionToShow);
int ApplyTreatedRequest(t_hashmap* BDD, Options** TreatedRequest, int ArraySize);
char* extractBetween(const char start, const char end, const char* Line);
int insertInto(t_hashmap* BDD, char* collection, char* Insert_arguments);
int removeFrom(t_hashmap* BDD, char* collection, char* Remove_arguments);
int Set(t_hashmap* BDD, char* collection, char* Set_arguments, char* Where_arguments);
int Projection(t_hashmap* BDD, char* collection, char* Projection_arguments, int* findResult);
int find(t_hashmap* BDD, char* collection, char* Find_arguments, int* findResult);
int Sort(t_hashmap* BDD, char* collection, char* Sort_arguments, int* findResult);
unsigned int lookatOption(mode Research, Options** TreatedRequest, int ArraySize);

t_list_arg* ReadArguments(char* Insert_arguments);