// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BlasterHUD.generated.h"

USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()

public:
	class UTexture2D* CrosshairsCenter;
	class UTexture2D* CrosshairsLeft;
	class UTexture2D* CrosshairsRight;
	class UTexture2D* CrosshairsTop;
	class UTexture2D* CrosshairsBottom;
	float CrosshairSpread;
	FLinearColor CrosshairsColor;
};

/**
 *
 */
UCLASS()
class BLASTER_API ABlasterHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

	UPROPERTY(EditAnywhere, Category = "Player States")
	TSubclassOf<class UUserWidget> CharacterOverlayClass;
	TObjectPtr<class UCharacterOverlayWidget> CharacterOverlay;
	void AddCharacterOverlay();

	UPROPERTY(EditAnywhere, Category = "Announcement")
	TSubclassOf<class UUserWidget> AnnouncementClass;
	TObjectPtr<class UAnnouncement> Announcement;
	void AddAnnouncement();

	UPROPERTY(EditAnywhere, Category = "EliAnnouncement")
	TSubclassOf<class UEliAnnouncementWidget> EliAnnouncementClass;
	//TObjectPtr<class UEliAnnouncementWidget> EliAnnouncement;
	void AddEliAnnouncement(FString AttackerName, FString VictimName);

protected:
	virtual void BeginPlay() override;

private:
	TObjectPtr<APlayerController> OwningPlayerController;

	FHUDPackage HUDPackage;

	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrossHairColor);

	UPROPERTY(EditAnywhere)
	float CrosshairSpreadMax = 16.0f;

	UPROPERTY(EditAnywhere)
	float EliAnnouncementTime = 3.0f;

	UFUNCTION()
	void EliAnnouncementTimerFinished(UEliAnnouncementWidget* MsgToRemove);

	UPROPERTY()
	TArray<UEliAnnouncementWidget*> EliMessages;

public:
	void SetHUDPackage(const FHUDPackage& Package) {HUDPackage = Package;}

};
