enum mode { COLLECTION, INSERT, WHERE, PROJECTION, FIND, ERROR ,REMOVE,SORT,SET};

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
int ApplyTreatedRequest(Options** TreatedRequest, int ArraySize);
char* extractBetween(const char start, const char end, const char* Line);
int insertInto(char* collection, char* Insert_arguments);
int removeFrom(char* collection, char* Remove_arguments, char* Where_arguments);
int Set(char* collection, char* Set_arguments, char* Where_arguments);
int Projection(char* collection, char* Projection_arguments, char* Where_arguments);
int find(char* collection, char* Find_arguments, char Where_arguments, char* Projection_arguments);
unsigned int lookatOption(mode Research, Options** TreatedRequest, int ArraySize);
