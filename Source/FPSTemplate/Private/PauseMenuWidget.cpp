#include "PauseMenuWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"

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
}

void UPauseMenuWidget::ClosePauseMenu()
{
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