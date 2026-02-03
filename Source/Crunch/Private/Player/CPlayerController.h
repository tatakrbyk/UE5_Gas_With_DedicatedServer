// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPlayerController.generated.h"

/**
 * 
 */

class UGameplayWidget;

UCLASS()
class ACPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	// only called on the server
	void OnPossess(APawn* NewPawn) override;

	// only called on the client, also on the listening server
	void AcknowledgePossession(APawn* NewPawn) override;
	
private:

	void SpawnGameplayWidget();
	UPROPERTY()
	TObjectPtr<class ACPlayerCharacter> CPlayerCharacter;

	UPROPERTY(EditDefaultsOnly, Category ="UI")
	TSubclassOf< UGameplayWidget>  GameplayWidgetClass; // WBP_GameplayWidget
	UPROPERTY()
	TObjectPtr< UGameplayWidget> GameplayWidget;
};
