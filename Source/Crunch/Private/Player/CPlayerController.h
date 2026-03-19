// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "CPlayerController.generated.h"

/**
 * 
 */

class UGameplayWidget;

UCLASS()
class ACPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:

	// only called on the server
	void OnPossess(APawn* NewPawn) override;

	// only called on the client, also on the listening server
	void AcknowledgePossession(APawn* NewPawn) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/* Generic Team Agent Interface Start */
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	/* Generic Team Agent Interface End */
private:

	void SpawnGameplayWidget();
	UPROPERTY()
	TObjectPtr<class ACPlayerCharacter> CPlayerCharacter;

	UPROPERTY(EditDefaultsOnly, Category ="UI")
	TSubclassOf< UGameplayWidget>  GameplayWidgetClass; // WBP_GameplayWidget
	UPROPERTY()
	TObjectPtr< UGameplayWidget> GameplayWidget;

	UPROPERTY(Replicated)
	FGenericTeamId TeamID;
};
