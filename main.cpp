#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_ROOMS 16
#define MAX_DOORS (4 * MAX_ROOMS)
#define MAX_ITEMS (4 * MAX_ROOMS)
#define MAX_ENTITIES (MAX_ROOMS + MAX_DOORS + MAX_ITEMS)

#define MAX_ARG_COUNT 8
#define MAX_ARG_LENGTH 32
#define MAX_COMMANDLINE_LENGTH 256

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

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
	entity * Entity = nullptr;
	room *CurrentRoom = nullptr;
};

struct room
{
	entity * Entity = nullptr;
};

struct item
{
	entity * Entity = nullptr;
	const char *location;
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

void CreatePlayer(world *World)
{
	entity *Entity = CreateEntity(World);
	World->Player.CurrentRoom = 0;
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
		entity * Entity = &World->Entities[i];

		if (Entity->Parent == Room->Entity)
		{
			if (SameString(Entity->Name, EntityName))
			{
				return &World->Entities[i];
			}
		}
	}
	return nullptr;
}

entity * FindEntityInInventory(world *World, const char *EntityName)
{
	for (uint16 i = 1; i < World->EntityCount; ++i)
	{
		entity * Entity = &World->Entities[i];

		if (Entity->Parent == World->Player.Entity)
		{
			if (SameString(Entity->Name, EntityName))
			{
				return &World->Entities[i];
			}
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

	CreatePlayer(World);

	room *Room = CreateRoom(World);
	Room->Entity->Description = "The current room is dark and quiet. You can only see a subtle light coming from a narrow doorway in in the north wall.";

	door *Door = CreateDoor(World);
	Door->Entity->Name = "doorway";
	Door->Entity->Description = "It is a scary narrow doorway that leads to a moisty corridor. You can see a subtle light on the other side.";

	item *Item = CreateItem(World);
	Item->Entity->Parent = Room->Entity;
	Item->Entity->Name = "dagger";
	Item->Entity->Description = "It's a small shiny and sharp dagger.";
	Item->location = "ground";

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

void UpdateWorld(world *World, uint32 ArgCount, const char *Args[])
{
	room *CurrentRoom = GetCurrentRoom(World);

	if (SameString(Args[0], "look"))
	{
		if (ArgCount == 1)
		{
			printf("%s", CurrentRoom->Entity->Description);
			for (uint16 i = 1; i < World->ItemCount; ++i)
			{
				item * Item = &World->Items[i];
				entity * Entity = Item->Entity;

				if (Entity->Parent == CurrentRoom->Entity)
				{
					printf(" There is a %s on the %s.", Entity->Name, Item->location);
				}
			}
		}
		else if (ArgCount == 2)
		{
			entity *FoundEntity = FindEntityInRoom(World, CurrentRoom, Args[1]);
			if (FoundEntity == nullptr)
			{
				FoundEntity = FindEntityInInventory(World, Args[1]);
			}
			if (FoundEntity != nullptr)
			{
				printf("%s", FoundEntity->Description);
			}
			else
			{
				printf("Cannot find anything such as '%s'", Args[1]);
			}
		}
		else
		{
			printf("Invalid look command.");
		}
	}
	else if (SameString(Args[0], "get"))
	{
		if (ArgCount == 2)
		{
			entity *FoundEntity = FindEntityInInventory(World, Args[1]);
			if (FoundEntity == nullptr)
			{
				entity *FoundEntity = FindEntityInRoom(World, CurrentRoom, Args[1]);
				FoundEntity->Parent = World->Player.Entity;
				printf("Taken.");
			}
			else
			{
				printf("You already have this.");
			}
		}
		else
		{
			printf("Invalid get command.");
		}
	}
	else if (SameString(Args[0], "drop"))
	{
		if (ArgCount == 2)
		{
			entity *FoundEntity = FindEntityInInventory(World, Args[1]);
			if (FoundEntity != nullptr)
			{
				FoundEntity->Parent = CurrentRoom->Entity;
				printf("Dropped.");
			}
			else
			{
				printf("You don't own this.");
			}
		}
		else
		{
			printf("Invalid drop command.");
		}
	}
	else if (SameString(Args[0], "inventory"))
	{
		entity *PlayerEntity = World->Player.Entity;

		printf("You are carrying the following items:");
		for (uint32 i = 0; i < World->ItemCount; ++i)
		{
			entity *ItemEntity = World->Items[i].Entity;
			if (ItemEntity != nullptr && ItemEntity->Parent == PlayerEntity)
			{
				printf("\n - %s\n", ItemEntity->Name);
			}
		}
	}
	else if (SameString(Args[0], "help"))
	{
		printf("You can use the following commands to execute actions:\n");
		printf("- drop\n");
		printf("- get\n");
		printf("- help\n");
		printf("- inventory\n");
		printf("- look\n");
		printf("- quit/exit");
	}
	else
	{
		printf("I don't understand that command...");
	}

	printf("\n");
}

void UpdateWorld(world *World, uint32 ArgCount, const char Args[MAX_ARG_COUNT][MAX_ARG_LENGTH])
{
	const char *NewArgs[MAX_ARG_COUNT];
	for (uint32 i = 0; i < ArgCount; ++i)
	{
		NewArgs[i] = Args[i];
	}
	UpdateWorld(World, ArgCount, NewArgs);
}

int UpdateWorld(world *World, const char *Commandline)
{
	char Tokens[MAX_ARG_COUNT][MAX_ARG_LENGTH];
	int NumTokens = Tokenize(Commandline, Tokens);

	if (NumTokens > 0)
	{
		if (SameString(Tokens[0], "quit") ||
			SameString(Tokens[0], "exit"))
		{
			return 0;
		}
		else
		{
			UpdateWorld(World, NumTokens, Tokens);
		}
	}

	return 1;
}

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

		int Continue = UpdateWorld(&World, Commandline);

		if (Continue == 0)
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

