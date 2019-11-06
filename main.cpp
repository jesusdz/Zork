#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

#define internal static

#define MAX_ROOMS 16
#define MAX_LINKS (4 * MAX_ROOMS)
#define MAX_ITEMS (4 * MAX_ROOMS)
#define MAX_ENTITIES (MAX_ROOMS + MAX_LINKS + MAX_ITEMS)

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


////////////////////////////////////////////////////////////////////////
// String functions
////////////////////////////////////////////////////////////////////////

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



////////////////////////////////////////////////////////////////////////
// Structs
////////////////////////////////////////////////////////////////////////

struct room;
struct link;
struct item;

struct entity
{
	entity *Parent = nullptr;
	
	room *Room = nullptr;
	link *Link = nullptr;
	item *Item = nullptr;

	const char *Name = "";
	const char *Description = "";
};

struct player
{
	entity * Entity = nullptr;
};

struct room
{
	entity * Entity = nullptr;
};

struct item
{
	entity * Entity = nullptr;
//	const char *location;
	bool Collectable = false;
	bool Openable = false;
	bool Open = false;
};

struct link
{
	entity * Entity = nullptr;
	room * FirstRoom = nullptr;
	room * SecondRoom = nullptr;
	const char * FirstRoomLocation = nullptr;
	const char * SecondRoomLocation = nullptr;
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

	link Links[MAX_LINKS];
	uint16 LinkCount;
};


////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////

internal
entity *CreateEntity(world *World, entity *Parent = nullptr)
{
	entity *Result = &World->Entities[World->EntityCount];
	Result->Parent = Parent;
	World->EntityCount++;
	return Result;
}

internal
void CreatePlayer(world *World, room *InitialRoom)
{
	entity *Entity = CreateEntity(World);
	World->Player.Entity = Entity;
	World->Player.Entity->Parent = InitialRoom->Entity;
}

internal
room *CreateRoom(world *World)
{
	entity *Entity = CreateEntity(World);

	room *Result = &World->Rooms[World->RoomCount];
	World->RoomCount++;

	Result->Entity = Entity;
	Entity->Room = Result;
	
	return Result;
}

internal
link *CreateLink(world *World)
{
	entity *Entity = CreateEntity(World);

	link *Result = &World->Links[World->LinkCount];
	World->LinkCount++;

	Result->Entity = Entity;
	Entity->Link = Result;

	return Result;
}

internal
item *CreateItem(world *World)
{
	entity *Entity = CreateEntity(World);

	item *Result = &World->Items[World->ItemCount];
	World->ItemCount++;

	Result->Entity = Entity;
	Entity->Item = Result;

	return Result;
}

internal
room *GetCurrentRoom(world *World)
{
	assert(World->Player.Entity != nullptr);
	assert(World->Player.Entity->Parent != nullptr);
	assert(World->Player.Entity->Parent->Room != nullptr);
	room *CurrentRoom = World->Player.Entity->Parent->Room;
	return CurrentRoom;
}

internal
void SetCurrentRoom(world *World, room *Room)
{
	assert(Room->Entity != nullptr);
	assert(World->Player.Entity != nullptr);
	World->Player.Entity->Parent = Room->Entity;
}

internal
entity * FindEntityInEntity(world *World, entity *ParentEntity, const char *EntityName)
{
	for (uint16 i = 0; i < World->EntityCount; ++i)
	{
		entity * Entity = &World->Entities[i];

		if (Entity->Parent == ParentEntity)
		{
			if (SameString(Entity->Name, EntityName))
			{
				return &World->Entities[i];
			}
		}
	}
	return nullptr;
}

internal
entity * FindEntityInCurrentRoom(world *World, const char *EntityName)
{
	entity *CurrentRoomEntity = World->Player.Entity->Parent;
	entity *Result = FindEntityInEntity(World, CurrentRoomEntity, EntityName);
	return Result;
}

internal
entity * FindEntityInInventory(world *World, const char *EntityName)
{
	entity *Result = FindEntityInEntity(World, World->Player.Entity, EntityName);
	return Result;
}

internal
link * FindLinkInRoom(world *World, room *Room, const char *LinkNameOrLocation)
{
	for (uint16 i = 0; i < World->LinkCount; ++i)
	{
		link * Link = &World->Links[i];

		if (Link->FirstRoom == Room || Link->SecondRoom == Room)
		{
			if (SameString(Link->Entity->Name, LinkNameOrLocation) ||
				(Link->FirstRoom == Room && SameString(Link->SecondRoomLocation, LinkNameOrLocation)) ||
				(Link->SecondRoom == Room && SameString(Link->FirstRoomLocation, LinkNameOrLocation)))
			{
				return &World->Links[i];
			}
		}
	}
	return nullptr;
}

internal
bool IsItemInInventory(world *World, item *Item)
{
	bool result = Item->Entity->Parent == World->Player.Entity;
	return result;
}

internal
bool IsItemInCurrentRoom(world *World, item *Item)
{
	room *CurrentRoom = GetCurrentRoom(World);
	entity *RoomEntity = CurrentRoom->Entity;
	for (entity *CurrentEntity = Item->Entity;
			CurrentEntity != nullptr;
			CurrentEntity = CurrentEntity->Parent)
	{
		if (CurrentEntity == RoomEntity)
		{
			return true;
		}
	}
	return false;
}

internal
void OpenItem(world *World, item *Item)
{
	assert(Item != nullptr);

	if (Item->Openable)
	{
		if (Item->Open)
		{
			printf("It was already open");
		}
		else
		{
			Item->Open = true;
			printf("Done.");
		}
	}
	else
	{
		printf("Cannot be opened.");
	}
}

internal
void CloseItem(world *World, item *Item)
{
	assert(Item != nullptr);

	if (Item->Openable)
	{
		if (Item->Open)
		{
			Item->Open = false;
			printf("Done.");
		}
		else
		{
			printf("It was already closed.");
		}
	}
	else
	{
		printf("Cannot be closed.");
	}
}

internal
item * FindItem(world *World, const char *ItemName)
{
	item *Result = nullptr;

	for (uint16 i = 0; i < World->ItemCount; ++i)
	{
		item * Item = &World->Items[i];

		if (IsItemInInventory(World, Item) ||
				IsItemInCurrentRoom(World, Item))
		{
			if (SameString(ItemName, Item->Entity->Name))
			{
				Result = Item;
				break;
			}
		}
	}

	return Result;
}

internal
room * FindNeighbouringRoom(world *World, const char *LinkNameOrLocation)
{
	room *CurrentRoom = GetCurrentRoom(World);
	link *Link = FindLinkInRoom(World, CurrentRoom, LinkNameOrLocation);
	if (Link != nullptr)
	{
		if (Link->FirstRoom == CurrentRoom)
		{
			return Link->SecondRoom;
		}
		else
		{
			return Link->FirstRoom;
		}
	}
	else
	{
		return nullptr;
	}
}

internal
void LookEntity(world *World, entity *Entity)
{
	printf("%s", Entity->Description);
	if (Entity->Item != nullptr)
	{
		if (Entity->Item->Openable)
		{
			if(Entity->Item->Open)
			{
				printf("It is open.");
			}
			else
			{
				printf("It is closed.");
			}
		}
	}
}

internal
void PrintCurrentRoomDescription(world *World)
{
	room *CurrentRoom = GetCurrentRoom(World);

	printf("%s\n", CurrentRoom->Entity->Description);
	printf("You can also see the following objects:\n");

	uint16 foundItemsCount = 0;
	for (uint16 i = 0; i < World->ItemCount; ++i)
	{
		item * Item = &World->Items[i];
		entity * Entity = Item->Entity;

		if (Entity->Parent == CurrentRoom->Entity)
		{
			foundItemsCount++;
			printf(" - %s\n", Entity->Name);
		}
	}

	if (foundItemsCount == 0)
	{
		printf(" *** No objects at a glance ***\n");
	}
}
internal
void PrintInventory(world *World)
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

internal
void PrintHelp()
{
	printf("You can use the following commands to execute actions:\n");
	printf("- drop <item name>\n");
	printf("- get/take <item name>\n");
	printf("- go [to] <direction or location>\n");
	printf("- help\n");
	printf("- inventory\n");
	printf("- look <item name>\n");
	printf("- quit/exit");
}

int UpdateWorld(world *World, const char *Commandline);

internal
void UpdateWorld(world *World, uint32 ArgCount, const char *Args[])
{
	room *CurrentRoom = GetCurrentRoom(World);

	if (SameString(Args[0], "go"))
	{
		if (ArgCount > 1 && ArgCount < 4)
		{
			const char *Location = Args[1];
			
			if (ArgCount == 3 && SameString(Args[1], "to"))
			{
				Location = Args[2];
			}

			room * NextRoom = FindNeighbouringRoom(World, Location);

			if (NextRoom != nullptr)
			{
				SetCurrentRoom(World, NextRoom);
				UpdateWorld(World, "look");
			}
			else
			{
				printf("I don't know how to go there.");
			}
		}
		else if (ArgCount >= 4)
		{
			printf("Too much information, be more specific.");
		}
		else
		{
			printf("Where do you want to go?");
		}
	}
	else if (SameString(Args[0], "look", "examine"))
	{
		if (ArgCount == 1)
		{
			PrintCurrentRoomDescription(World);
		}
		else if (ArgCount == 2)
		{
			entity *FoundEntity = FindEntityInCurrentRoom(World, Args[1]);
			if (FoundEntity == nullptr)
			{
				FoundEntity = FindEntityInInventory(World, Args[1]);
			}
			if (FoundEntity != nullptr)
			{
				LookEntity(World, FoundEntity);
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
	else if (SameString(Args[0], "get", "take"))
	{
		if (ArgCount == 2)
		{
			entity *FoundEntity = FindEntityInInventory(World, Args[1]);
			if (FoundEntity == nullptr)
			{
				entity *FoundEntity = FindEntityInCurrentRoom(World, Args[1]);

				if (FoundEntity != nullptr)
				{
					if (FoundEntity->Item->Collectable)
					{
						FoundEntity->Parent = World->Player.Entity;
						printf("Taken.");
					}
					else
					{
						printf("I cannot take that.");
					}
				}
				else
				{
					printf("I cannot find that.");
				}
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
		PrintInventory(World);
	}
	else if (SameString(Args[0], "open"))
	{
		if (ArgCount == 2)
		{
			item *Item = FindItem(World, Args[1]);
			OpenItem(World, Item);
		}
		else if (ArgCount < 2)
		{
			printf("What do I have to open?");
		}
		else
		{
			printf("You will need to be more specific");
		}
	}
	else if (SameString(Args[0], "close"))
	{
		if (ArgCount == 2)
		{
			item *Item = FindItem(World, Args[1]);
			CloseItem(World, Item);
		}
		else if (ArgCount < 2)
		{
			printf("What do I have to close?");
		}
		else
		{
			printf("You will need to be more specific");
		}
	}
	else if (SameString(Args[0], "help"))
	{
		PrintHelp();
	}
	else
	{
		printf("I don't understand that command...");
	}

	printf("\n");
}

internal
void UpdateWorld(world *World, uint32 ArgCount, const char Args[MAX_ARG_COUNT][MAX_ARG_LENGTH])
{
	const char *NewArgs[MAX_ARG_COUNT];
	for (uint32 i = 0; i < ArgCount; ++i)
	{
		NewArgs[i] = Args[i];
	}
	UpdateWorld(World, ArgCount, NewArgs);
}

internal
int UpdateWorld(world *World, const char *Commandline)
{
	char Tokens[MAX_ARG_COUNT][MAX_ARG_LENGTH];
	int NumTokens = Tokenize(Commandline, Tokens);

	if (NumTokens > 0)
	{
		if (SameString(Tokens[0], "quit", "exit", "q"))
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

internal
void InitializeWorld(world *World)
{
	World->EntityCount = 0;
	World->RoomCount = 0;
	World->LinkCount = 0;
	World->ItemCount = 0;

	// Room 1

	room *Room1 = CreateRoom(World);
	Room1->Entity->Description =
		"The current room is dark and quiet. "
		"You can only see a subtle light coming from a narrow doorway in in the north wall.";

	item *Item = CreateItem(World);
	Item->Collectable = true;
	Item->Entity->Parent = Room1->Entity;
	Item->Entity->Name = "dagger";
	Item->Entity->Description = "It's a small shiny and sharp dagger.";
	//Item->location = "ground";


	// Room 2

	room *Room2 = CreateRoom(World);
	Room2->Entity->Description =
		"This room is subtlely lit by a chandelier on an elegant wooden table. "
		"As you try to inspect details in the penumbra, you can see several paintings on the walls. "
		"What seems a secret passageway is open leading towards the southern, darker room."
		"There is big wooden closed door in the right wall. "
		"An open door to the left leads to another, more illuminated room.";

	Item = CreateItem(World);
	Item->Entity->Parent = Room2->Entity;
	Item->Entity->Name = "table";
	Item->Entity->Description = "It's an old and elegant wooden table.";
	//Item->location = "ground";

	Item = CreateItem(World);
	Item->Entity->Parent = Room2->Entity;
	Item->Entity->Name = "chandelier";
	Item->Entity->Description = "The chandelier is made of silver. It's candles are burning and emmiting an eerie light.";
	//Item->location = "table";

	Item = CreateItem(World);
	Item->Entity->Parent = Room2->Entity;
	Item->Entity->Name = "door";
	Item->Entity->Description = "Looks like this door is well closed. I'm going to need a key to open it.";
	Item->Openable = true;
	Item->Open = false;
	//Item->location = "ground";


	// Links between rooms

	link *Link = CreateLink(World);
	Link->FirstRoom = Room1;
	Link->FirstRoomLocation = "south";
	Link->SecondRoom = Room2;
	Link->SecondRoomLocation = "north";
	Link->Entity->Name = "passageway";
	Link->Entity->Description = "It is a scary narrow passageway that leads to another room. You can see a subtle light on the other side.";


	// Player

	CreatePlayer(World, Room1);
}

internal
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

