/*
 * singly_linked_list.c - Simple example of Singly linked list
 *
 * Author: Evgeniy Sennikov <sennikov.work@ya.ru>
 *
 * Licence: GPLv3
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define MAX_NAME_LEN 32
#define MAX_DATA_LEN 256


typedef struct list_s {
	int    id;
	char   name[MAX_NAME_LEN];
	char   data[MAX_DATA_LEN];
	struct list_s * next;
} list_t;


void print_all_list (list_t * head)
{
	list_t * cur = head;

	while(cur)
	{
		printf("id: %3d\tname: %s  data: %s\n", cur->id, cur->name, cur->data);
		cur = cur->next;
	}
	printf("\n");
}

list_t * init_first (list_t * head, int id, char * name, char * data)
{
	if(head)
	{
		printf("list already inited\n");
	}

	// printf("init list with params (id=%d name=%s data=%s)\n", id, name, data);

	list_t * cur = calloc(1, sizeof(list_t));
	cur->next = NULL;
	cur->id = id;
	strcpy(cur->name, name);
	strcpy(cur->data, data);

	return cur;
}

list_t * insert_at_end (list_t * head, int id, char * name, char * data)
{
	if(!head)
	{
		head = init_first(head, id, name, data);
	}
	else
	{
		list_t * cur = head;
		while(cur)
		{
			if(!cur->next)
			{
				list_t * tmp = calloc(1, sizeof(list_t));
				tmp->next = NULL;
				tmp->id = id;
				strcpy(tmp->name, name);
				strcpy(tmp->data, data);
				cur->next = tmp;
				break;
			}
			cur = cur->next;
		}
	}

	return head;
}

list_t * insert_at_begin (list_t * head, int id, char * name, char * data)
{
	if(!head)
	{
		head = init_first(head, id, name, data);
	}
	else
	{
		list_t * tmp = calloc(1, sizeof(list_t));
		tmp->next = head;
		tmp->id = id;
		strcpy(tmp->name, name);
		strcpy(tmp->data, data);
		head = tmp;
	}

	return head;
}

list_t * delete_by_id (list_t * head, int id)
{
	list_t * cur = head;
	list_t * prev = NULL;

	while(cur)
	{
		if(cur->id == id)
		{
			if(prev)
			{
				prev->next = cur->next;
				free(cur);
				break;
			}
			else
			{
				head = cur->next;
				free(cur);
				break;
			}
		}
		prev = cur;
		cur = cur->next;
	}

	return head;
}

int main (int argc, char *argv[])
{
	list_t * head = NULL;

	head = insert_at_begin (head, 0, "first_name", "first_data");
	head = insert_at_end   (head, 1, "second_name", "second_data");
	head = insert_at_begin (head, 2, "foo", "bar");
	head = insert_at_end   (head, 3, "foo", "bar");
	head = insert_at_begin (head, 4, "foo", "bar");
	print_all_list (head);

	head = delete_by_id (head, 0);
	print_all_list (head);

	head = delete_by_id (head, 3);
	print_all_list (head);

	head = delete_by_id (head, 4);
	print_all_list (head);

	return 0;
}
