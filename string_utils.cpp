
internal
void InputCommandline(char commandline[MAX_COMMANDLINE_LENGTH])
{
	printf("\n>> ");

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
	} while (i < MAX_COMMANDLINE_LENGTH && ch > 31 && ch != EOF);

	commandline[i] = 0;
}

internal
int Tokenize(const char commandline[MAX_COMMANDLINE_LENGTH], char tokens[MAX_ARG_COUNT][MAX_ARG_LENGTH])
{
	const char *cptr = commandline;
	int tokenId = 0;

	while (tokenId < MAX_ARG_COUNT && *cptr != '\0')
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

internal
int SameString(const char *string1, const char *string2)
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

internal
int SameString(const char *toCompare, const char *opt1, const char *opt2)
{
	if (SameString(toCompare, opt1)) return 1;
	if (SameString(toCompare, opt2)) return 1;
	return false;
}

internal
int SameString(const char *toCompare, const char *opt1, const char *opt2, const char *opt3)
{
	if (SameString(toCompare, opt1, opt2)) return 1;
	if (SameString(toCompare, opt3)) return 1;
	return false;
}

