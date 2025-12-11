#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "FPSSaveGame.generated.h"

/**
 * Custom Save Game class to store persistent data like Level Name.
 */
UCLASS()
class FPSTEMPLATE_API UFPSSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UFPSSaveGame();

	/** The name of the level to load when "Continue" is pressed */
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString PlayerName;

	/** The name of the level to load when "Continue" is pressed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	FName SavedLevelName;

	/** The index of the current level in the level sequence */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	int32 CurrentLevelIndex;

	/** Example: You could add Health, Inventory, etc. here later */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	float PlayerHealth;

	/** Whether the music is muted */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	bool bIsMusicMuted;
};
