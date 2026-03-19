// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GenericTeamAgentInterface.h"
#include "GameplayTagContainer.h"

#include "CCharacter.generated.h"

UCLASS()
class ACCharacter : public ACharacter, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ACCharacter();

	// only called on the server
	virtual void PossessedBy(AController* NewController) override;

	void ServerSideInit();
	void ClientSideInit();
	bool IsLocallyControlledByPlayer() const;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

public:	

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* Generic Team Agent Interface Start */
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	/* Generic Team Agent Interface End */

private:
	UPROPERTY(ReplicatedUsing= OnRep_TeamID)
	FGenericTeamId TeamID;

	UFUNCTION()
	virtual void OnRep_TeamID();
	/**********************************************************************************/
	/*									GAS										      */

public:

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
private:

	void BindGASChangeDelegates();
	void DeathTagUpdated(const FGameplayTag Tag, int32 NewCount);

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
	void SetStatusGaugeEnabled(bool bIsEnabled);
	/**********************************************************************************/

	/* Death And Respawn */
public:
	bool IsDead() const;
	void RespawnImmediately();
private:
	FTransform MeshRelativeTransform;
	UPROPERTY(EditDefaultsOnly, Category = "Death")
	float DeathMontageFinishTimeShift = -0.8f;

	UPROPERTY(EditDefaultsOnly, Category = "Death")
	TObjectPtr<UAnimMontage> DeathMontage;
	
	FTimerHandle DeathMontageTimerHandle;

	void DeathMontageFinished();
	void SetRagdollEnabled(bool bIsEnabled); 

	void PlayDeathAnimation();
	void StartDeathSequence();	
	void Respawn();

	virtual void OnDead();
	virtual void OnRespawn();
	/* Death And Respawn end*/

	/* AI */
private:
	
	void SetAIPerceptionStimuliSourceEnabled(bool bIsEnabled);
	UPROPERTY()
	TObjectPtr<class UAIPerceptionStimuliSourceComponent> PerceptionStimuliSourceComponent;
	/* AI End*/

};
