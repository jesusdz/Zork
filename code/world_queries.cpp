
internal
room *GetCurrentRoom(world *World)
{
	ASSERT(World->Player.Entity != nullptr);
	ASSERT(World->Player.Entity->Parent != nullptr);
	ASSERT(World->Player.Entity->Parent->Room != nullptr);
	room *CurrentRoom = World->Player.Entity->Parent->Room;
	return CurrentRoom;
}

internal
void SetCurrentRoom(world *World, room *Room)
{
	ASSERT(Room->Entity != nullptr);
	ASSERT(World->Player.Entity != nullptr);
	World->Player.Entity->Parent = Room->Entity;
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

