#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_ROOMS 64
#define MAX_DOORS 1024
#define MAX_ITEMS 2042
#define MAX_ENTITIES (MAX_ROOMS + MAX_DOORS + MAX_ITEMS)

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

void InputCommandline(char commandline[256])
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

int Tokenize(char commandline[256], char tokens[4][64])
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



////////////////////////////////////////////////////////////////////////
// Structs
////////////////////////////////////////////////////////////////////////

struct room;
struct door;
struct item;

struct entity
{
	entity *Parent = nullptr;
	
	room *Room = nullptr;
	door *Door = nullptr;
	item *Item = nullptr;

	const char *Name = "";
	const char *Description = "";
};

struct player
{
	room *CurrentRoom = nullptr;
};

struct room
{
	entity * Entity = nullptr;
};

struct item
{
	entity * Entity = nullptr;
};

struct door
{
	entity * Entity = nullptr;
};

struct world
{
	player Player;

	entity Entities[MAX_ENTITIES];
	uint16 EntityCount;

	room Rooms[MAX_ROOMS];
	uint16 RoomCount;

	item Items[MAX_ITEMS];
	uint16 ItemCount;

	door Doors[MAX_DOORS];
	uint16 DoorCount;
};


////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////

entity *CreateEntity(world *World, entity *Parent = nullptr)
{
	entity *Result = &World->Entities[World->EntityCount];
	Result->Parent = Parent;
	World->EntityCount++;
	return Result;
}

room *CreateRoom(world *World)
{
	entity *Entity = CreateEntity(World);

	room *Result = &World->Rooms[World->RoomCount];
	World->RoomCount++;

	Result->Entity = Entity;
	Entity->Room = Result;
	
	return Result;
}

door *CreateDoor(world *World)
{
	entity *Entity = CreateEntity(World);

	door *Result = &World->Doors[World->DoorCount];
	World->DoorCount++;

	Result->Entity = Entity;
	Entity->Door = Result;

	return Result;
}

item *CreateItem(world *World)
{
	entity *Entity = CreateEntity(World);

	item *Result = &World->Items[World->ItemCount];
	World->ItemCount++;

	Result->Entity = Entity;
	Entity->Item = Result;

	return Result;
}

entity * FindEntityInRoom(world *World, room *Room, const char *EntityName)
{
	for (uint16 i = 1; i < World->EntityCount; ++i)
	{
		if (SameString(World->Entities[i].Name, EntityName))
		{
			return &World->Entities[i];
		}
	}
	return nullptr;
}

void InitializeWorld(world *World)
{
	World->EntityCount = 1;
	World->RoomCount = 1;
	World->DoorCount = 1;
	World->ItemCount = 1;

	room *Room = CreateRoom(World);
	Room->Entity->Description = "The current room is dark and quiet. You can only see a subtle light coming from a narrow doorway in in the north wall.\n";

	World->Player.CurrentRoom = Room;

	Room = CreateRoom(World);
}

void PrintWelcomeMessage()
{
	printf("**********************\n");
	printf("   Welcome to Zork!\n");
	printf("**********************\n");

	printf("\n");

	printf("This is a text adventure in the spirit of the classic game Zork.\n");
	printf("You will advance through the story by means of executing actions.\n");
	printf("Type 'help' in the commandline to see the available commands.\n");
	printf("Have fun!\n\n");
}

room *GetCurrentRoom(world *World)
{
	room *CurrentRoom = World->Player.CurrentRoom;
	return CurrentRoom;
}

void UpdateWorld(world *World, const char *Arg1)
{
	if (SameString(Arg1, "look"))
	{
		room *CurrentRoom = GetCurrentRoom(World);
		printf("%s", CurrentRoom->Entity->Description);
	}
	else if (SameString(Arg1, "help"))
	{
		printf("You can use the following commands to execute actions:\n");
		printf("- look\n");
		printf("- help\n");
		printf("- quit/exit\n");
	}
}

void UpdateWorld(world *World, const char *Arg1, const char *Arg2)
{
	if (SameString(Arg1, "look"))
	{
		room *CurrentRoom = GetCurrentRoom(World);
		entity *FoundEntity = FindEntityInRoom(World, CurrentRoom, Arg2);
		if (FoundEntity != nullptr)
		{
			printf("%s\n", CurrentRoom->Entity->Description);
		}
	}
}

int main()
{
	world World;
	InitializeWorld(&World);

	PrintWelcomeMessage();

	UpdateWorld(&World, "look");

	char Commandline[256];
	char Tokens[4][64];

	while (1)
	{
		printf("\n>> ");
		InputCommandline(Commandline);

		int NumTokens = Tokenize(Commandline, Tokens);

		if (NumTokens > 0)
		{
			if (SameString(Tokens[0], "quit") ||
				SameString(Tokens[0], "exit"))
			{
				break;
			}
			else if (NumTokens == 1)
			{
				UpdateWorld(&World, Tokens[0]);
			}
			else if (NumTokens == 2)
			{
				// TODO(jesus): Function that takes 2 arguments
			}
			else if (NumTokens == 3)
			{
				// TODO(jesus): Function that takes 3 arguments
			}
		}
	}

	printf("\nThanks for playing. See you soon!\n");

#ifdef _WIN32
	system("pause");
#endif

	return 0;
}

