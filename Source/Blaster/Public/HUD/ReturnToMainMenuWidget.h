// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReturnToMainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UReturnToMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void MenuSetup();
	void MenuTearDown();

protected:
	virtual bool Initialize() override;

	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);

	UFUNCTION()
	void OnPlayerLeftGame();

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* ReturnButton;

	TObjectPtr<APlayerController> PlayerController;

	UFUNCTION()
	void ReturnButtonClicked();

	TObjectPtr<class UMultiplayerSessionsSubsystem> MultiplayerSessionsSubsystem;

};
