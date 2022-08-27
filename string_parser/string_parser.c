/*
 * string_parser.c - Simple examples of string parsing
 *
 * Author: Evgeniy Sennikov <sennikov.work@ya.ru>
 *
 * Licence: GPLv3
 */

#include <stdio.h>
#include <string.h>

#define MAX_BUF_SIZE 128

int parce_sip_uri(char *sip_uri)
{
	char *addr = NULL, *port = NULL, *number = NULL;

	printf("Before:\n\tString  = %s\n", sip_uri);

	if ((addr = strchr(sip_uri, '@')))
	{
		*addr = '\0';
		addr++;
		number = strsep(&sip_uri, "@");
		if (number == NULL)
		{
			printf(" Error\n");
			return -1;
		}
		if ((port = strchr(addr, ':')))
		{
			*port = '\0';
			port++;
		}
	}
	else
		number = strdup(sip_uri);

	printf("After: \n\tNumber  = %s\n", number);
	printf("\tAddress = %s\n", addr ? addr : "None");
	printf("\tPort    = %s\n", port ? port : "None");

	return 0;
}

void split_string_to_words(char *p)
{
	char *word;
	int   i = 0;

	if (!strlen(p))
		return;

	printf("Before:\n\tString  = %s\nAfter:\n", p);

	while (1)
	{
		word = strsep(&p, " ");
		if (word != NULL)
		{
			printf("\tSlovo %d = %s\n", i + 1, word);
			switch (i)
			{
			case 0:
				// action with first word
				break;
			case 1:
				// action with second word...
				break;
			case 2:
				break;
			default:
				break;
			}
			i++;
		}
		else
		{
			break;
		}
	}
}

int main()
{
	char buffer[MAX_BUF_SIZE] = {0};

	// --------------------------------------------------------------
	char *p = buffer;
	strcpy(buffer, "word1 second_word word3");
	printf("\nStart split_string_to_words()\n");
	split_string_to_words(p);

	// --------------------------------------------------------------
	strcpy(buffer, "*1234#@192.168.1.2:5060");
	printf("\nStart parce_sip_uri()\n");
	parce_sip_uri(buffer);

	return 0;
}
