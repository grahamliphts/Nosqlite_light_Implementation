#include "list_contigue.h"
#include <stdio.h>
#include <stdlib.h>

t_list* list_create(unsigned int max)
{
	t_list* list = (t_list*)malloc(sizeof(*list));
	list->data = (int*)malloc(sizeof(int) * max);
	list->length = 0;
	list->max = max;
	return list;
}

void list_display(t_list* list)
{
	int i = 0;
	if(list && 0 < list->length)
	{
		for(i = 0; i < list->length - 1; i++)
			printf("%d,", list->data[i]);	

		printf("%d", list->data[i]);
		printf("\n");
	}
}

void list_append(t_list* list, int nb)
{
	if(list->length < list->max)
		list->data[list->length++] = nb;
}

void list_insert(t_list* list, unsigned int index, int data)
{ 
	int i = 0;
	if(list && list->max > list->length && index < list->length)
	{
		for(i = list->length; i > index; i--)
			list->data[i] = list->data[i - 1];
		list->data[index] = data;
		list->length++;
	}
}

void list_remove_index(t_list* list, unsigned int index)
{
	int i = 0;
	if(list && index < list->length)
	{
		for(i = index; i < list->length; i++)
		{
			list->data[i] = list->data[i + 1];
		}
		list->length--;
	}
}

void list_remove_value(t_list* list, enum occurence_strat occ, int value)
{
	int first = -1;
	int last = 0;
	int found = 0;
	int i = 0;
	int j = 0;
	if(list)
	{
		for(i = 0; i < list->length; i++)
		{
			
			if(occ == ALL && list->data[i] != value)
			{
				list->data[j] = list->data[i];
				j++;
				found ++;
			}
			if(list->data[i] ==  value && first == -1)
				first = i;
			else if (list->data[i] ==  value)
				last = i;
		}

		if(occ == FIRST)
			list_remove_index(list, first);
		else if(occ == LAST)
			list_remove_index(list, last);
		else
			list->length = list->length - found;
	}
}

t_list* list_copy(t_list* list)
{
	int i;
	if(list)
	{	
		t_list* newlist = list_create(list->length);
		newlist->length = list->length;
		for(i = 0; i < list->length; i++)
		{
			newlist->data[i] = list->data[i];
		}
		return newlist;
	}
	return NULL;
}

t_list* list_revert_copy(t_list* list)
{
	int i = 0;
	if(list)
	{	
		t_list* newlist = list_create(list->length);
		newlist->length = list->length;
		while(i < list->length)
		{
			newlist->data[i] = list->data[list->length - 1 - i];
			i++;
		}
		return newlist;
	}
	return NULL;
}

void list_revert_inside(t_list* list)
{
	int i = 0;
	int j;
	int temp;
	if(list)
	{
		for(i = 0, j = list->length - 1; i < list->length/2; i++, j--)
		{
			temp = list->data[j];
			list->data[j] = list->data[i];
			list->data[i] = temp;
		}
	}
}

t_list list_concat(int count, t_list* list, ...)
{
	if(list && list->data)
	{
		va_list ap;
		va_start(ap, list);
		int idx;
		t_list* list_app = NULL;
		int newsize = 0;
		for(idx++ < count && (list_arg = va_arg(ap, t_list*)))
		{
			newsize += list_arg->length;
		}
		va_end(ap);
		va_start(ap, list)
		int i = 0, j = 0;
		idx = 0;
		list_new = list_create(new_size);
		list_arg = list;
		do
		{
			for(j = 0; j < list_arg->length; i++, j++)
				list_new->data[i] = list_arg->data[j];
			idx++;
		}while(idx <= count && (list = va_arg(ap,t_list)))
		va_end(ap);
	}
	return list_new;
}

void list_insert_infinite(t_list* list, unsigned int index, int value)
{
	if(list->max <= list->length)
	{
		list = realloc(list, INCREASE_FACTOR * sizeof(*list));
		list->max += INCREASE_FACTOR;
	}
	list_insert(list, index, value);
}

void list_destroy(t_list* list)
{
	free(list->data);
	free(list);
}
