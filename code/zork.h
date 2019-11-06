#ifndef ZORK_H
#define ZORK_H

#define MAX_ROOMS 16
#define MAX_LINKS (4 * MAX_ROOMS)
#define MAX_ITEMS (4 * MAX_ROOMS)
#define MAX_ENTITIES (MAX_ROOMS + MAX_LINKS + MAX_ITEMS)

#define MAX_ARG_COUNT 8
#define MAX_ARG_LENGTH 32
#define MAX_COMMANDLINE_LENGTH 256

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

	uint8 ExitFlag;
};

#endif
