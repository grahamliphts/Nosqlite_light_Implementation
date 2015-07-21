#include "Json.h"
#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

int loadBDD()
{
	FILE* 		fp;
	char*		title = NULL;
	char*		str_keys = NULL;
	char**		keys = NULL;
	size_t 	read = 0;
	int		i;
	int		nb_keys = 0;
	size_t		len = 0;

	fp = fopen("BDD.json", "r");

	if (NULL == fp)
	{
		fprintf(stderr, "Fail to open file in reading\n");
		return NULL;
	}

	while (read != -1)
	{
		read = getdelim(&title, &len, '{', fp);
		if (read == -1)
		{
			free(title);
			free(str_keys);
			return 0;
		}
		read = getdelim(&str_keys, &len, '}', fp);
		if (read != -1)
		{
			title = delete_space(title);
			str_keys = delete_space(str_keys);
			if (keys == NULL)
				return NULL;
		}
		else
		{
			free(title);
			free(str_keys);
		}
	}
	return 0;
}

char* delete_space(char* str)
{
	int	i = 0;
	int	j = 0;
	int	size_result;
	char*	result = NULL;

	while (str[i] != '\0')
	{
		if (str[i] != '\n' && str[i] != '\t' && str[i] != ' ' && str[i] != '{')
		{
			str[j] = str[i];
			j++;
		}
		i++;
	}
	str[j] = '\0';
	size_result = strlen(str) + 1;
	result = (char*)malloc(size_result);
	if (result != NULL)
		my_memcpy(result, str, size_result);
	free(str);
	return result;
}

void my_memcpy(char* dst, char* src, int len)
{
	int	i = 0;

	for (i = 0; i < len; i++)
	{
		dst[i] = src[i];
	}
}
