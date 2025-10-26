#include "PauseMenuWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Initialize bindings
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
		ExitToMenuButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnExitToMenuClicked);
	}

	OptionsMenuWidget = nullptr;
}

void UPauseMenuWidget::ClosePauseMenu()
{
	// Remove from viewport and unpause the game
	RemoveFromParent();

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
	// Switch to options panel if a widget switcher is wired up
	if (MenuSwitcher)
	{
		// Attempt to switch to the second widget (index1) which is commonly the Options panel
		MenuSwitcher->SetActiveWidgetIndex(1);
	}
}

void UPauseMenuWidget::OnExitToMenuClicked()
{
	// Open the main menu level
	if (GetWorld())
	{
		UGameplayStatics::OpenLevel(this, MainMenuLevelName);
	}
}