// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "CCharacter.generated.h"

UCLASS()
class ACCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACCharacter();

	// only called on the server
	virtual void PossessedBy(AController* NewController) override;

	void ServerSideInit();
	void ClientSideInit();
	bool IsLocallyControlledByPlayer() const;

protected:
	virtual void BeginPlay() override;

public:	

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	/**********************************************************************************/
	/*									GAS										      */

public:

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
private:

	UPROPERTY(VisibleDefaultsOnly, Category = "Game Ability")
	TObjectPtr<class UCAbilitySystemComponent> CAbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<class UCAttributeSet> CAttributeSet;

	/**********************************************************************************/

	/**********************************************************************************/
	/*									UI										      */

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "UI")
	TObjectPtr<class UWidgetComponent> OverHeadWidgetComponent;

	void ConfigureOverHeadStatusWidget();

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	float HeadStatGaugeVisibilityCheckUpdateGap = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	float HeadStatGaugeVisibilityRangeSquared = 10000000.f;

	FTimerHandle HeadStatsGaugeVisibilityUpdateTimerHandle;
	void UpdateHeadGaugeVisibility();
	/**********************************************************************************/

};
