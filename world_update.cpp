
void UpdateWorld(world *World, const char *Commandline);

internal
void UpdateWorld(world *World, uint32 ArgCount, const char *Args[])
{
	if (ArgCount == 0)
	{
		return;
	}

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
			LookCurrentRoom(World);
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
	else if (SameString(Args[0], "quit", "exit", "q"))
	{
		World->ExitFlag = 1;
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
void UpdateWorld(world *World, const char *Commandline)
{
	char Tokens[MAX_ARG_COUNT][MAX_ARG_LENGTH];
	int NumTokens = Tokenize(Commandline, Tokens);
	UpdateWorld(World, NumTokens, Tokens);
}

