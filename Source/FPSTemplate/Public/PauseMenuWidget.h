#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

class UButton;
class UWidgetSwitcher;

/**
 * Pause Menu Widget - appears when player presses ESC in-game
 * Has Continue, Options, and Exit to Menu buttons
 */
UCLASS()
class UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	// Close this pause menu (call from outside if needed)
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ClosePauseMenu();

protected:
	// Main pause menu buttons (must match UMG widget names exactly)
	UPROPERTY(meta = (BindWidget))
	UButton* ContinueButton;

	UPROPERTY(meta = (BindWidget))
	UButton* OptionsButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ExitToMenuButton;

	// Widget switcher to swap between pause menu and options panels
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MenuSwitcher;

	// Name of the main menu level to return to
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Levels")
	FName MainMenuLevelName = FName("L_Level_8");

	// Options menu widget class (we'll create this next)
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> OptionsMenuWidgetClass;

private:
	// Button click handlers
	UFUNCTION()
	void OnContinueClicked();

	UFUNCTION()
	void OnOptionsClicked();

	UFUNCTION()
	void OnExitToMenuClicked();

	// Current options menu instance
	UPROPERTY()
	UUserWidget* OptionsMenuWidget;

	// Additional buttons for exit confirmation
	UPROPERTY(meta = (BindWidget))
	UButton* ConfirmExitButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CancelExitButton;

	// Additional button click handlers
	UFUNCTION()
	void OnExitPromptClicked();

	UFUNCTION()
	void OnCancelExitClicked();
};