#include "FPSGameInstance.h"
#include "FPSSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

UFPSGameInstance::UFPSGameInstance()
{
	SaveSlotName = TEXT("PlayerSaveSlot");
	UserIndex = 0;
	CurrentSaveGame = nullptr;
	CurrentLevelIndex = 0;
	LevelNames.Add("L_Level_0");
	LevelNames.Add("L_Level_1");
	LevelNames.Add("L_Level_2");
	LevelNames.Add("L_Level_3");
	LevelNames.Add("L_Level_4");
	LevelNames.Add("L_Level_5");
	LevelNames.Add("L_Level_6");
	LevelNames.Add("L_Level_7");
	LevelNames.Add("L_Run");
	LevelNames.Add("L_Level_9");
	LevelNames.Add("L_Level_8");
}

void UFPSGameInstance::Init()
{
	Super::Init();

	// Load existing save game if it exists
	CurrentSaveGame = LoadOrCreateSaveGame();

	// Restore the current level index from save game
	if (CurrentSaveGame)
	{
		CurrentLevelIndex = CurrentSaveGame->CurrentLevelIndex;
		UE_LOG(LogTemp, Log, TEXT("FPSGameInstance initialized. Save slot: %s, Current Level Index: %d"), 
			*SaveSlotName, CurrentLevelIndex);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("FPSGameInstance initialized. Save slot: %s"), *SaveSlotName);
	}
}

void UFPSGameInstance::SaveCurrentLevel()
{
	// Get the current world
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("SaveCurrentLevel: No valid world found"));
		return;
	}

	// Get the current level name
	FString CurrentLevelName = World->GetMapName();
	
	// Remove the "UEDPIE_0_" prefix that appears in PIE (Play In Editor) mode
	CurrentLevelName.RemoveFromStart(World->StreamingLevelsPrefix);

	// Ensure we have a save game object
	if (!CurrentSaveGame)
	{
		CurrentSaveGame = LoadOrCreateSaveGame();
	}

	if (CurrentSaveGame)
	{
		// Update the saved level name
		CurrentSaveGame->SavedLevelName = FName(*CurrentLevelName);
		
		// Try to find the level index in the LevelNames array
		int32 FoundIndex = LevelNames.IndexOfByKey(FName(*CurrentLevelName));
		if (FoundIndex != INDEX_NONE)
		{
			CurrentLevelIndex = FoundIndex;
			CurrentSaveGame->CurrentLevelIndex = CurrentLevelIndex;
			UE_LOG(LogTemp, Log, TEXT("Found level index: %d for level: %s"), CurrentLevelIndex, *CurrentLevelName);
		}
		else
		{
			// Level not in array, just save the current index as-is
			CurrentSaveGame->CurrentLevelIndex = CurrentLevelIndex;
			UE_LOG(LogTemp, Warning, TEXT("Level %s not found in LevelNames array. Saving current index: %d"), 
				*CurrentLevelName, CurrentLevelIndex);
		}

		// Save to disk
		if (UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SaveSlotName, UserIndex))
		{
			UE_LOG(LogTemp, Log, TEXT("Successfully saved level: %s (Index: %d)"), *CurrentLevelName, CurrentLevelIndex);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to save level: %s"), *CurrentLevelName);
		}
	}
}

void UFPSGameInstance::LoadSavedLevel()
{
	// Load the save game
	if (!CurrentSaveGame)
	{
		CurrentSaveGame = LoadOrCreateSaveGame();
	}

	if (CurrentSaveGame && CurrentSaveGame->SavedLevelName != NAME_None)
	{
		FString LevelName = CurrentSaveGame->SavedLevelName.ToString();
		UE_LOG(LogTemp, Log, TEXT("Loading saved level: %s"), *LevelName);

		// Open the saved level
		UGameplayStatics::OpenLevel(this, CurrentSaveGame->SavedLevelName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No saved level found to load"));
	}
}

FName UFPSGameInstance::GetLastSavedLevelName() const
{
	if (CurrentSaveGame)
	{
		return CurrentSaveGame->SavedLevelName;
	}

	return NAME_None;
}

bool UFPSGameInstance::DoesSaveGameExist() const
{
	return UGameplayStatics::DoesSaveGameExist(SaveSlotName, UserIndex);
}

UFPSSaveGame* UFPSGameInstance::LoadOrCreateSaveGame()
{
	UFPSSaveGame* SaveGame = nullptr;

	// Try to load existing save game
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, UserIndex))
	{
		SaveGame = Cast<UFPSSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, UserIndex));
		
		if (SaveGame)
		{
			UE_LOG(LogTemp, Log, TEXT("Loaded existing save game. Last level: %s"), 
				*SaveGame->SavedLevelName.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load save game from slot: %s"), *SaveSlotName);
		}
	}

	// If loading failed or no save exists, create a new one
	if (!SaveGame)
	{
		SaveGame = Cast<UFPSSaveGame>(UGameplayStatics::CreateSaveGameObject(UFPSSaveGame::StaticClass()));
		UE_LOG(LogTemp, Log, TEXT("Created new save game object"));
	}

	return SaveGame;
}

void UFPSGameInstance::LoadLevelByIndex(int32 LevelIndex)
{
	// Validate the index
	if (!LevelNames.IsValidIndex(LevelIndex))
	{
		UE_LOG(LogTemp, Error, TEXT("LoadLevelByIndex: Invalid level index %d. LevelNames array has %d elements."), 
			LevelIndex, LevelNames.Num());
		return;
	}

	// Update the current level index
	CurrentLevelIndex = LevelIndex;

	// Get the level name
	FName LevelName = LevelNames[LevelIndex];
	
	UE_LOG(LogTemp, Log, TEXT("Loading level by index %d: %s"), LevelIndex, *LevelName.ToString());

	// Load the level
	UGameplayStatics::OpenLevel(this, LevelName);
}

void UFPSGameInstance::LoadNextLevel()
{
	int32 NextIndex = CurrentLevelIndex + 1;
	
	// Check if we've reached the end of the level array
	if (NextIndex >= LevelNames.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadNextLevel: Already at the last level (Index: %d)"), CurrentLevelIndex);
		return;
	}

	LoadLevelByIndex(NextIndex);
}

void UFPSGameInstance::LoadPreviousLevel()
{
	int32 PrevIndex = CurrentLevelIndex - 1;
	
	// Check if we're at the first level
	if (PrevIndex < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadPreviousLevel: Already at the first level (Index: %d)"), CurrentLevelIndex);
		return;
	}

	LoadLevelByIndex(PrevIndex);
}

int32 UFPSGameInstance::GetCurrentLevelIndex() const
{
	return CurrentLevelIndex;
}

void UFPSGameInstance::SetCurrentLevelIndex(int32 NewIndex)
{
	if (LevelNames.IsValidIndex(NewIndex))
	{
		CurrentLevelIndex = NewIndex;
		UE_LOG(LogTemp, Log, TEXT("Set current level index to: %d"), CurrentLevelIndex);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SetCurrentLevelIndex: Invalid index %d. LevelNames array has %d elements."), 
			NewIndex, LevelNames.Num());
	}
}
