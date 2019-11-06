
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
void InitializeWorld(world *World)
{
	World->EntityCount = 0;
	World->RoomCount = 0;
	World->LinkCount = 0;
	World->ItemCount = 0;
	World->ExitFlag = 0;

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

