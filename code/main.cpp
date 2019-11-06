#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

#define internal static
#define local_persist static
#define ASSERT(test) assert(test)

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

#include "zork.h"
#include "string_utils.cpp"
#include "world_init.cpp"
#include "world_queries.cpp"
#include "actions.cpp"
#include "world_update.cpp"

int main()
{
	world World;
	InitializeWorld(&World);

	PrintWelcomeMessage();

	UpdateWorld(&World, "look");

	char Commandline[MAX_COMMANDLINE_LENGTH];

	while (1)
	{
		InputCommandline(Commandline);

		UpdateWorld(&World, Commandline);

		if (World.ExitFlag == 1)
		{
			break;
		}
	}

	printf("\nThanks for playing Zork.\n");
	printf("See you soon!\n\n");

#ifdef _WIN32
	system("pause");
#endif

	return 0;
}

