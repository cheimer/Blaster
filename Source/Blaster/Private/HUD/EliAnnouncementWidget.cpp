// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/EliAnnouncementWidget.h"

#include "Components/TextBlock.h"

void UEliAnnouncementWidget::SetEliAnnouncementText(FString AttackerName, FString VictimName)
{
	FString EliAnnouncementText = FString::Printf(TEXT("    %s Eliminate %s"), *AttackerName, *VictimName);

	if(AnnouncementText)
	{
		AnnouncementText->SetText(FText::FromString(EliAnnouncementText));
	}
}
