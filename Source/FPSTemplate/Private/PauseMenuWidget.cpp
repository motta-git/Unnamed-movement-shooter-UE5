#include "PauseMenuWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ContinueButton)
	{
		ContinueButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnContinueClicked);
	}

	if (OptionsButton)
	{
		OptionsButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnOptionsClicked);
	}

	if (ExitToMenuButton)
	{
		ExitToMenuButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnExitPromptClicked);
	}

	if (ConfirmExitButton)
	{
		ConfirmExitButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnExitToMenuClicked);
	}

	if (CancelExitButton)
	{
		CancelExitButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnCancelExitClicked);
	}

	OptionsMenuWidget = nullptr;

	// Ensure we are in UI mode while the menu is open
	if (APlayerController* PC = GetOwningPlayer())
	{
		FInputModeGameAndUI Mode; // allows ESC/game input if needed
		Mode.SetWidgetToFocus(TakeWidget());
		Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(Mode);
		PC->bShowMouseCursor = true;
		PC->SetIgnoreLookInput(true);
		PC->SetIgnoreMoveInput(true);
	}
}

void UPauseMenuWidget::ClosePauseMenu()
{
	RemoveFromParent();

	if (APlayerController* PC = GetOwningPlayer())
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->bShowMouseCursor = false;
		PC->SetIgnoreLookInput(false);
		PC->SetIgnoreMoveInput(false);
	}

	if (GetWorld())
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);
	}
}

void UPauseMenuWidget::OnContinueClicked()
{
	ClosePauseMenu();

}

void UPauseMenuWidget::OnOptionsClicked()
{
	if (MenuSwitcher)
	{
		MenuSwitcher->SetActiveWidgetIndex(1); // Show options panel
	}
}

void UPauseMenuWidget::OnExitPromptClicked()
{
	if (MenuSwitcher)
	{
		MenuSwitcher->SetActiveWidgetIndex(2); // Show exit confirmation panel
	}
}

void UPauseMenuWidget::OnExitToMenuClicked()
{
	if (GetWorld())
	{
		UGameplayStatics::OpenLevel(this, MainMenuLevelName);
	}
}

void UPauseMenuWidget::OnCancelExitClicked()
{
	if (MenuSwitcher)
	{
		MenuSwitcher->SetActiveWidgetIndex(0); // Return to main menu panel
	}
}