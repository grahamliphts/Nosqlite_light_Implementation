enum mode { COLLECTION, INSERT, WHERE, PROJECTION, FIND, ERROR ,REMOVE};

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
