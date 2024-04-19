// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EliAnnouncementWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UEliAnnouncementWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetEliAnnouncementText(FString AttackerName, FString VictimName);

	UPROPERTY(meta = (BindWiidget))
	class UHorizontalBox* AnnouncementBox;

	UPROPERTY(meta = (BindWiidget))
	class UTextBlock* AnnouncementText;

};
