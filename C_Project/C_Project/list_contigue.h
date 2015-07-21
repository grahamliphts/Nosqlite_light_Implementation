#ifndef LIST_CONTIGUE_H
#define LIST_CONTIGUE_H
#define INCREASE_FACTOR 2

#include "Hashmap.h"

enum occurence_strat{FIRST, LAST, ALL};
typedef struct s_list
{
	t_hashmap** data;
	unsigned int length;
	unsigned int max;
}t_list;

t_list* list_create(unsigned int max);
void list_display(t_list* list);
void list_append(t_list* list, t_hashmap* map);
void list_insert(t_list* list, unsigned int index, t_hashmap* map);
void list_remove_index(t_list* list, unsigned int index);
void list_remove_value(t_list* list, enum occurence_strat occ, t_hashmap* map);

t_list* list_copy(t_list* list);
t_list* list_revert_copy(t_list* list);

void list_insert_infinite(t_list* list, unsigned int index, t_hashmap* value);
void list_revert_inside(t_list*);
void list_destroy(t_list* list);

#endif
