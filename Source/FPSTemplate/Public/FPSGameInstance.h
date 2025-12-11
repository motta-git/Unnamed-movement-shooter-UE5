#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FPSGameInstance.generated.h"

class UFPSSaveGame;

/**
 * Custom Game Instance that manages save/load operations
 * Persists across level transitions
 */
UCLASS()
class FPSTEMPLATE_API UFPSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFPSGameInstance();

	virtual void Init() override;

	/** Save the current level name and index to disk */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	void SaveCurrentLevel();

	/** Load the last saved level */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	void LoadSavedLevel();

	/** Get the name of the last saved level without loading it */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	FName GetLastSavedLevelName() const;

	/** Check if a save file exists */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	bool DoesSaveGameExist() const;

	/** Load a level by index from the LevelNames array */
	UFUNCTION(BlueprintCallable, Category = "Level Management")
	void LoadLevelByIndex(int32 LevelIndex);

	/** Load the next level in the sequence */
	UFUNCTION(BlueprintCallable, Category = "Level Management")
	void LoadNextLevel();

	/** Load the previous level in the sequence */
	UFUNCTION(BlueprintCallable, Category = "Level Management")
	void LoadPreviousLevel();

	/** Get the current level index */
	UFUNCTION(BlueprintCallable, Category = "Level Management")
	int32 GetCurrentLevelIndex() const;

	/** Set the current level index (doesn't load the level, just updates the index) */
	UFUNCTION(BlueprintCallable, Category = "Level Management")
	void SetCurrentLevelIndex(int32 NewIndex);

protected:
	/** The name of the save slot */
	UPROPERTY(EditDefaultsOnly, Category = "Save System")
	FString SaveSlotName;

	/** The user index for the save slot (usually 0 for single player) */
	UPROPERTY(EditDefaultsOnly, Category = "Save System")
	int32 UserIndex;

	/** Array of level names in order (configure this in the editor) */
	UPROPERTY(EditDefaultsOnly, Category = "Level Management")
	TArray<FName> LevelNames;

	/** Current level index in the LevelNames array */
	UPROPERTY(BlueprintReadOnly, Category = "Level Management")
	int32 CurrentLevelIndex;

	/** Whether the music is muted */
	UPROPERTY(BlueprintReadOnly, Category = "Audio Settings")
	bool bIsMusicMuted;

	/** Set the music mute state and save it */
	UFUNCTION(BlueprintCallable, Category = "Audio Settings")
	void SetMusicMuted(bool bMute);

	/** Get the current music mute state */
	UFUNCTION(BlueprintCallable, Category = "Audio Settings")
	bool GetMusicMuted() const;

	/** Cached reference to the current save game object */
	UPROPERTY()
	UFPSSaveGame* CurrentSaveGame;

private:
	/** Load save game from disk or create a new one */
	UFPSSaveGame* LoadOrCreateSaveGame();
};
