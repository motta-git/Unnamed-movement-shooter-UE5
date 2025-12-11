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
	bIsMusicMuted = false;
}

void UFPSGameInstance::Init()
{
	Super::Init();

	// Load existing save game if it exists
	CurrentSaveGame = LoadOrCreateSaveGame();

	// If save game was loaded, log the status
	if (CurrentSaveGame)
	{
		UE_LOG(LogTemp, Log, TEXT("FPSGameInstance initialized. Save slot: %s, Current Level Index: %d, Music Muted: %d"), 
			*SaveSlotName, CurrentLevelIndex, bIsMusicMuted);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("FPSGameInstance initialized. Save slot: %s"), *SaveSlotName);
	}

	// Detect the current level and update the index accordingly
	UWorld* World = GetWorld();
	if (World)
	{
		// Get the current level name
		FString CurrentLevelName = World->GetMapName();
		
		// Remove the "UEDPIE_0_" prefix that appears in PIE (Play In Editor) mode
		CurrentLevelName.RemoveFromStart(World->StreamingLevelsPrefix);

		// Try to find the level index in the LevelNames array
		int32 FoundIndex = LevelNames.IndexOfByKey(FName(*CurrentLevelName));
		if (FoundIndex != INDEX_NONE)
		{
			CurrentLevelIndex = FoundIndex;
			UE_LOG(LogTemp, Log, TEXT("Detected current level: %s at index: %d"), *CurrentLevelName, CurrentLevelIndex);
			
			// Update the save game with the current level index
			if (CurrentSaveGame)
			{
				CurrentSaveGame->CurrentLevelIndex = CurrentLevelIndex;
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Current level %s not found in LevelNames array. Using saved index: %d"), 
				*CurrentLevelName, CurrentLevelIndex);
		}
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
			UE_LOG(LogTemp, Log, TEXT("Loaded existing save game [V2]. Last level: %s"), 
				*SaveGame->SavedLevelName.ToString());
			
			// Validation: Ensure the loaded index matches the loaded level name
			// This fixes issues where the save file has desynchronized data (e.g. Index is 10 but Level is "L_Level_0")
			FName SavedLevelName = SaveGame->SavedLevelName;
			UE_LOG(LogTemp, Log, TEXT("SavedLevelName: %s"), 
				*SavedLevelName.ToString());
			int32 IndexFromMsg = LevelNames.IndexOfByKey(SavedLevelName);
			UE_LOG(LogTemp, Log, TEXT("IndexFromMsg: %d"), 
				IndexFromMsg);
			UE_LOG(LogTemp, Warning, TEXT("[DEBUG] LoadOrCreateSaveGame: LevelNames.Num() = %d. SavedLevelName = '%s'. Saved Index = %d. Found Index = %d"), 
				LevelNames.Num(), *SavedLevelName.ToString(), SaveGame->CurrentLevelIndex, IndexFromMsg);

			if (IndexFromMsg != INDEX_NONE && IndexFromMsg != SaveGame->CurrentLevelIndex)
			{
				UE_LOG(LogTemp, Warning, TEXT("Save Data Mismatch! Saved Index: %d, but Saved Level '%s' is at index %d. Correcting index."), 
					SaveGame->CurrentLevelIndex, *SavedLevelName.ToString(), IndexFromMsg);
				
				SaveGame->CurrentLevelIndex = IndexFromMsg;
				// We don't save immediately to disk to avoid perf hit, it will be saved next time user triggers save
			}

			// Update runtime cache if this is the first load
			CurrentLevelIndex = SaveGame->CurrentLevelIndex;
			bIsMusicMuted = SaveGame->bIsMusicMuted;
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
	UE_LOG(LogTemp, Log, TEXT("LoadNextLevel: Next index: %d"), NextIndex);
	// Check if we've reached the end of the level array
	if (NextIndex >= LevelNames.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadNextLevel: Already at the last level (Index: %d)"), CurrentLevelIndex);
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("Loading next level: %s"), *LevelNames[NextIndex].ToString());
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

void UFPSGameInstance::SetMusicMuted(bool bMute)
{
	bIsMusicMuted = bMute;
	UE_LOG(LogTemp, Log, TEXT("SetMusicMuted: %s"), bIsMusicMuted ? TEXT("True") : TEXT("False"));

	// Ensure we have a save game object
	if (!CurrentSaveGame)
	{
		CurrentSaveGame = LoadOrCreateSaveGame();
	}

	if (CurrentSaveGame)
	{
		CurrentSaveGame->bIsMusicMuted = bIsMusicMuted;
		
		// Save to disk immediately so the setting persists
		if (UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SaveSlotName, UserIndex))
		{
			UE_LOG(LogTemp, Log, TEXT("Successfully saved music mute state"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to save music mute state"));
		}
	}
}

bool UFPSGameInstance::GetMusicMuted() const
{
	return bIsMusicMuted;
}
