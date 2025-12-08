#include "FPSSaveGame.h"

UFPSSaveGame::UFPSSaveGame()
{
	SavedLevelName = NAME_None;
	CurrentLevelIndex = 0;
	PlayerHealth = 100.0f;
	PlayerName = TEXT("PlayerOne");
}
