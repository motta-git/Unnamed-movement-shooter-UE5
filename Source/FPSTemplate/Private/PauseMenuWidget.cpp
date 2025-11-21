#include "PauseMenuWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Engine/GameViewportClient.h"
#include "InputCoreTypes.h"
#include "Input/Reply.h"

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

FReply UPauseMenuWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	// Close the pause menu when Escape is pressed
	if (InKeyEvent.GetKey() == EKeys::Escape)
	{
		ClosePauseMenu();
		return FReply::Handled();
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UPauseMenuWidget::ClosePauseMenu()
{
	RemoveFromParent();

	if (APlayerController* PC = GetOwningPlayer())
	{
		// Use a game-only input mode that does not require clicking to capture the mouse
		FInputModeGameOnly Mode;
		// Ensure the mouse capture isn't tied to mouse-down (helps with immediate look after closing)
		Mode.SetConsumeCaptureMouseDown(false);
		PC->SetInputMode(Mode);
		PC->bShowMouseCursor = false;

		// Disable UI-specific click/mouse over events so the controller receives look/move normally
		PC->bEnableClickEvents = false;
		PC->bEnableMouseOverEvents = false;

		PC->SetIgnoreLookInput(false);
		PC->SetIgnoreMoveInput(false);

		// Center the mouse in the viewport so look input is captured reliably
		if (GetWorld() && GetWorld()->GetGameViewport())
		{
			FVector2D ViewportSize;
			GetWorld()->GetGameViewport()->GetViewportSize(ViewportSize);
			PC->SetMouseLocation((int32)(ViewportSize.X * 0.5f), (int32)(ViewportSize.Y * 0.5f));
		}
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