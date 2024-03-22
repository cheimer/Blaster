// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/OverheadUserWidget.h"

#include "Components/TextBlock.h"

void UOverheadUserWidget::SetDisplayText(FString TextToDisplay)
{
	if(!DisplayText) return;

	DisplayText->SetText(FText::FromString(TextToDisplay));
}

void UOverheadUserWidget::ShowPlayerNetRole(APawn* InPawn)
{
	if(!InPawn) return;
	ENetRole LocalRole = InPawn->GetLocalRole();
	FString Role;
	switch(LocalRole)
	{
	case ENetRole::ROLE_Authority:
		Role = FString("Authority");
		break;
	case ENetRole::ROLE_AutonomousProxy:
		Role = FString("AutonomousProxy");
		break;
	case ENetRole::ROLE_SimulatedProxy:
		Role = FString("SimulatedProxy");
		break;
	case ENetRole::ROLE_None:
		Role = FString("None");
		break;
	default:
		Role = FString("default??");
		break;
	}
	FString LocalRoleString = FString::Printf(TEXT("Local Role : %s"), *Role);
	SetDisplayText(LocalRoleString);
}

void UOverheadUserWidget::NativeDestruct()
{
	RemoveFromParent();


	Super::NativeDestruct();
}
