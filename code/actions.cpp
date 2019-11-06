
internal
void OpenItem(world *World, item *Item)
{
	ASSERT(Item != nullptr);

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
	ASSERT(Item != nullptr);

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
void LookCurrentRoom(world *World)
{
	room *CurrentRoom = GetCurrentRoom(World);

	printf("%s\n", CurrentRoom->Entity->Description);

	uint16 foundItemsCount = 0;
	for (uint16 i = 0; i < World->ItemCount; ++i)
	{
		item * Item = &World->Items[i];
		entity * Entity = Item->Entity;

		if (Entity->Parent == CurrentRoom->Entity)
		{
			if (foundItemsCount == 0)
			{
				printf("You can also see the following objects:\n");
			}
			printf(" - %s\n", Entity->Name);
			foundItemsCount++;
		}
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

