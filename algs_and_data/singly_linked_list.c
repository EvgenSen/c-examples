/*
 * singly_linked_list.c - Simple example of Singly linked list
 *
 * `list_t * head` may be global var. This will make the program easier.
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
		printf("id: %2d, name: %-12s, data: %s\n", cur->id, cur->name, cur->data);
		cur = cur->next;
	}
	printf("\n");
}

list_t * insert_at_begin (list_t * head, int id, char * name, char * data)
{
	list_t * new = calloc(1, sizeof(list_t));
	new->next = head;
	new->id = id;
	strcpy(new->name, name);
	strcpy(new->data, data);
	head = new;

	return head;
}

list_t * insert_at_end (list_t * head, int id, char * name, char * data)
{
	if(!head)
	{
		return (head = insert_at_begin(head, id, name, data));
	}

	list_t * cur = head;
	while(cur)
	{
		if(!cur->next)
		{
			list_t * new = calloc(1, sizeof(list_t));
			new->next = NULL;
			new->id = id;
			strcpy(new->name, name);
			strcpy(new->data, data);
			cur->next = new;
			break;
		}
		cur = cur->next;
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

list_t * delete_all (list_t * head)
{
	list_t * cur = head;
	list_t * rm = NULL;

	while(cur)
	{
		rm = cur;
		cur = cur->next;
		free(rm);
		rm = NULL;
	}

	head = NULL;

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

	delete_all(head);

	return 0;
}
