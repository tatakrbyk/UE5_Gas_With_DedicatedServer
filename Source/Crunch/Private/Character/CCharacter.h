#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GenericTeamAgentInterface.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "GAS/CGameplayAbilityTypes.h"

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
	const TMap<ECAbilityInputID, TSubclassOf<UGameplayAbility>>& GetAbilities() const;

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
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendGameplayEventToSelf(const FGameplayTag& EventTag, const FGameplayEventData& EventData);

protected:
	void UpgradeAbilityWithInputID(ECAbilityInputID InputID);
private:

	void BindGASChangeDelegates();
	void DeathTagUpdated(const FGameplayTag Tag, int32 NewCount);
	void StunTagUpdated(const FGameplayTag Tag, int32 NewCount);
	void AimTagUpdated(const FGameplayTag Tag, int32 NewCount);

	void MoveSpeedUpdated(const FOnAttributeChangeData& Data);
	void MaxHealthUpdated(const FOnAttributeChangeData& Data);
	void MaxManaUpdated(const FOnAttributeChangeData& Data);
	void SetIsAimming(bool bIsAiming);
	virtual void OnAimStateChanged(bool bIsAimming);

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
	
	/*  Stun */
private:
	UPROPERTY(EditDefaultsOnly, Category = "Stun")
	TObjectPtr<UAnimMontage> StunMontage;

	virtual void OnStun();
	virtual void OnRecoverFromStun();
	/*  Stun End */
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
