#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

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



////////////////////////////////////////////////////////////////////////
// Structs
////////////////////////////////////////////////////////////////////////

struct player
{
	uint16 CurrentRoomIndex;
};

struct room
{
	const char *Description = nullptr;
};

struct item
{
	const char *Description = nullptr;
};

struct door
{
	const char *Description = nullptr;
};

struct world
{
	player Player;

	room Rooms[256];
	uint16 RoomCount;

	item Items[512];
	uint16 ItemCount;

	door Doors[512];
	uint16 DoorCount;
};


////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////

room *CreateRoom(world *World)
{
	room *Result = &World->Rooms[World->RoomCount];
	World->RoomCount++;
	return Result;
}

door *CreateDoor(world *World)
{
	door *Result = &World->Doors[World->DoorCount];
	World->DoorCount++;
	return Result;
}

item *CreateItem(world *World)
{
	item *Result = &World->Items[World->ItemCount];
	World->ItemCount++;
	return Result;
}

void InitializeWorld(world *World)
{
	World->Player.CurrentRoomIndex = 0;

	room *Room = CreateRoom(World);
	Room->Description = "The current room is dark and quiet. You can only see a subtle light coming from a narrow doorway in in the north wall.";
//	door *Door = nullptr;
//	item *Item = nullptr;

	Room = CreateRoom(World);
}

void UpdateWorld(world *World, const char *arg1)
{
	if (equals(arg1, "look"))
	{
		uint16 CurrentRoomIndex = World->Player.CurrentRoomIndex;
		room *CurrentRoom = &World->Rooms[CurrentRoomIndex];
		printf("%s", CurrentRoom->Description);
	}
}

int main()
{
	world World;
	InitializeWorld(&World);

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
			else if (numTokens == 1)
			{
				UpdateWorld(&World, tokens[0]);
			}
			else if (numTokens == 2)
			{
				// TODO(jesus): Function that takes 2 arguments
			}
			else if (numTokens == 3)
			{
				// TODO(jesus): Function that takes 3 arguments
			}
		}
	}

	printf("Thanks for playing. See you soon!\n");

#ifdef _WIN32
	system("pause");
#endif

	return 0;
}

