#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>


void inputSentence(char commandline[256])
{
	int ch;
	int i = 0;
	
	do
	{
		ch = getc(stdin);

		if (ch > 31)
		{
			commandline[i] = ch;
			++i;
		}
	} while (i < 255 && ch > 31 && ch != EOF);

	commandline[i] = 0;
}

int tokenize(char commandline[256], char tokens[4][64])
{
	char *cptr = commandline;
	int tokenId = 0;

	while (tokenId < 4 && *cptr != '\0')
	{
		while (*cptr == ' ' || *cptr == '\t') cptr++;

		int charId = 0;
		while (*cptr != ' ' && *cptr != '\t' && *cptr != '\0')
		{
			tokens[tokenId][charId++] = (char)*cptr;
			cptr++;
		}
		tokens[tokenId][charId] = '\0';

		tokenId++;
	}

	return tokenId;
}

int equals(const char *string1, const char *string2)
{
	const char *str1 = string1;
	const char *str2 = string2;

	while (*str1 != '\0' && *str2 != '\n')
	{
		if (*str1 != *str2)
		{
			return 0;
		}

		str1++;
		str2++;
	}

	return *str1 == *str2;
}

int main()
{
	printf("Welcome to Zork!\n");

	char commandline[256];
	char tokens[4][64];

	while (1)
	{
		printf("Type a command:\n");
		inputSentence(commandline);

		int numTokens = tokenize(commandline, tokens);

		if (numTokens > 0)
		{
			if (equals(tokens[0], "quit"))
			{
				break;
			}
		}
	}

	printf("Thanks for playing. See you soon!\n");
	system("pause");

	return 0;
}
