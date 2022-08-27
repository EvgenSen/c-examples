/*
 * doubly_linked_list.c - Simple example of Doubly linked list
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

typedef struct list_s
{
	int            id;
	char           name[MAX_NAME_LEN];
	char           data[MAX_DATA_LEN];
	struct list_s *next;
	struct list_s *prev;
} list_t;

enum position_e
{
	P_BEGIN = 0,
	P_END   = 1
};

void print_all_list(list_t *head)
{
	list_t *cur = head;
	int     i   = 0;

	printf(" +--------------------------------\n");
	printf(" |\n");
	while (cur)
	{
		printf(" | %02d: %p <-- %p --> %p\n", i, cur->prev, cur, cur->next);
		printf(" |     [id=%d, name='%s', data='%s']\n", cur->id, cur->name, cur->data);
		printf(" |\n");
		cur = cur->next;
		i++;
	}
	printf(" +--------------------------------\n");
}

list_t *init_first(list_t *head, int id, char *name, char *data)
{
	if (head)
	{
		printf("Warn: list already inited\n");
		return head;
	}

	list_t *cur = calloc(1, sizeof(list_t));
	cur->next   = NULL;
	cur->prev   = NULL;
	cur->id     = id;
	strcpy(cur->name, name);
	strcpy(cur->data, data);

	return cur;
}

void destroy_list(list_t *head)
{
	list_t *cur  = head;
	list_t *next = NULL;

	if (!head)
	{
		printf("Warn: list already destroyed\n");
		return;
	}

	while (cur)
	{
		if (cur->next)
			next = cur->next;
		else
			next = NULL;

		free(cur);
		cur = next;
	}
}

list_t *insert_data(list_t *head, int id, char *name, char *data, int position)
{
	if (!head)
	{
		head = init_first(head, id, name, data);
		return head;
	}
	else
	{
		list_t *cur = head;
		/* If insert at the end, then go to last element */
		while ((position == P_END) && cur && cur->next)
		{
			cur = cur->next;
		}

		list_t *tmp = calloc(1, sizeof(list_t));
		tmp->next   = (position == P_END) ? NULL : cur;
		tmp->prev   = (position == P_END) ? cur : NULL;
		tmp->id     = id;
		strcpy(tmp->name, name);
		strcpy(tmp->data, data);

		if (position == P_END)
		{
			cur->next = tmp;
		}
		else
		{
			head->prev = tmp;
			head       = tmp;
		}
	}

	return head;
}

list_t *delete_by_id(list_t *head, int id)
{
	list_t *cur = head;

	while (cur)
	{
		if (cur->id == id)
		{
			if (cur->prev && cur->next)
			{
				cur->prev->next = cur->next;
				cur->next->prev = cur->prev;
				free(cur);
				return head;
			}
			else if (cur->next == NULL && cur->prev != NULL)
			{
				/* Remove last elem */
				cur->prev->next = cur->next;
				free(cur);
				return head;
			}
			else if (cur->prev == NULL && cur->next != NULL)
			{
				/* Remove first elem */
				head       = cur->next;
				head->prev = NULL;
				free(cur);
				return head;
			}
			else
				printf("Error: elem with NULL prev and next link (id=%d, name='%s', data='%s')\n",
				        cur->id, cur->name, cur->data);
		}
		cur = cur->next;
	}

	printf("Error: elem with id %d not found\n", id);

	return head;
}

list_t *change_by_id(list_t *head, int id, char *name, char *data)
{
	list_t *cur = head;

	while (cur)
	{
		if (cur->id == id)
		{
			strcpy(cur->name, name);
			strcpy(cur->data, data);
			return head;
		}
		cur = cur->next;
	}

	printf("Error: elem with id %d not found\n", id);

	return head;
}

int main()
{
	list_t *head = NULL;

	head = insert_data(head, 0, "first_name", "first_data", P_BEGIN);
	head = insert_data(head, 1, "second_name", "second_data", P_END);
	head = insert_data(head, 2, "foo", "bar", P_BEGIN);
	head = insert_data(head, 3, "foo", "bar", P_END);
	head = insert_data(head, 4, "foo", "bar", P_BEGIN);
	print_all_list(head);

	head = delete_by_id(head, 0);
	print_all_list(head);

	head = delete_by_id(head, 3);
	print_all_list(head);

	head = delete_by_id(head, 4);
	print_all_list(head);

	head = delete_by_id(head, 123);
	print_all_list(head);

	head = change_by_id(head, 1, "new_name", "new_data");
	print_all_list(head);

	destroy_list(head);

	return 0;
}
