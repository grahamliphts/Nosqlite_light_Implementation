#ifndef LIST_CONTIGUE_H
#define LIST_CONTIGUE_H
#define INCREASE_FACTOR 2

enum occurence_strat{FIRST, LAST, ALL};
typedef struct s_list
{
	int* data;
	unsigned int length;
	unsigned int max;
}t_list;

t_list* list_create(unsigned int max);
void list_display(t_list* list);
void list_append(t_list* list, int nb);
void list_insert(t_list* list, unsigned int index, int data);
void list_remove_index(t_list* list, unsigned int index);
void list_remove_value(t_list* list, enum occurence_strat occ, int value);
t_list* list_copy(t_list* list);
t_list* list_revert_copy(t_list* list);
t_list* list_concat(int count, t_list list*, ...);
void list_insert_infinite(t_list* list, unsigned int index, int value);
void list_revert_inside(t_list*);
void list_destroy(t_list* list);

#endif
