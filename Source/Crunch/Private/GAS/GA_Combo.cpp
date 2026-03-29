#include "GAS/GA_Combo.h"

#include "GAS/CAbilitySystemStatics.h"
#include "Abilities/Tasks//AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks//AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagsManager.h"

UGA_Combo::UGA_Combo()
{
	//AbilityTags.AddTag(UCAbilitySystemStatics::GetBasicAttackAbilityTag());

	FGameplayTagContainer AssetTags;
	AssetTags.AddTag(UCAbilitySystemStatics::GetBasicAttackAbilityTag());
	SetAssetTags(AssetTags);
	BlockAbilitiesWithTag.AddTag(UCAbilitySystemStatics::GetBasicAttackAbilityTag());	// Meaning -> Block Double Combo Attack
}

void UGA_Combo::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!K2_CommitAbility()) // Apply cost, Start cooldown, etc..., It returns false if the cost is not met or if it is in cooldown etc.... apply End Ability
	{
		//UE_LOG(LogTemp, Warning, TEXT("[GA COMBO] (ActivateAbility) !CommitAbility "));

		K2_EndAbility();
		return;
	}
	
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		// Play Combo01
		UAbilityTask_PlayMontageAndWait* PlayComboMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, ComboMontage);
		PlayComboMontageTask->OnBlendOut.AddDynamic(this, &UGA_Combo::K2_EndAbility);
		PlayComboMontageTask->OnCancelled.AddDynamic(this, &UGA_Combo::K2_EndAbility);
		PlayComboMontageTask->OnCompleted.AddDynamic(this, &UGA_Combo::K2_EndAbility);
		PlayComboMontageTask->OnInterrupted.AddDynamic(this, &UGA_Combo::K2_EndAbility);
		PlayComboMontageTask->ReadyForActivation();

		// Wait for the Combo Change event. 
		// If the next combo (combo02, combo03, etc.) occurs, play the corresponding animation.
		// If the combo change ends, stop the combo.
		UAbilityTask_WaitGameplayEvent* WaitComboChangeEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, GetComboChangedEventTag() , nullptr, false, false);
		WaitComboChangeEventTask->EventReceived.AddDynamic(this, &UGA_Combo::ComboChangedEventReceived);
		WaitComboChangeEventTask->ReadyForActivation();
	}

	if (K2_HasAuthority())
	{
		UAbilityTask_WaitGameplayEvent* WaitTargettingEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, GetComboTargetEventTag());
		WaitTargettingEventTask->EventReceived.AddDynamic(this, &UGA_Combo::TryDamage);
		WaitTargettingEventTask->ReadyForActivation();
	}

	SetupWaitComboInputPress();
}

FGameplayTag UGA_Combo::GetComboChangedEventTag()
{
	return FGameplayTag::RequestGameplayTag("Ability.Combo.Change");
}

FGameplayTag UGA_Combo::GetComboChangedEventEndTag()
{
	return FGameplayTag::RequestGameplayTag("Ability.Combo.Change.End");
}

FGameplayTag UGA_Combo::GetComboTargetEventTag()
{
	return FGameplayTag::RequestGameplayTag("Ability.Combo.Damage");
}

void UGA_Combo::SetupWaitComboInputPress()
{
	UAbilityTask_WaitInputPress* WaitInputPress = UAbilityTask_WaitInputPress::WaitInputPress(this);
	WaitInputPress->OnPress.AddDynamic(this, &UGA_Combo::HandleInputPress);
	WaitInputPress->ReadyForActivation();
}

void UGA_Combo::HandleInputPress(float TimeWaited)
{
	SetupWaitComboInputPress();
	TryCommitCombo();
}

void UGA_Combo::TryCommitCombo()
{
	if (NextComboName == NAME_None) return;

	UAnimInstance* OwnerAnimInst = GetOwnerAnimInstance();
	if (!OwnerAnimInst) return;
	OwnerAnimInst->Montage_SetNextSection(OwnerAnimInst->Montage_GetCurrentSection(ComboMontage), NextComboName, ComboMontage);

		
}

TSubclassOf<UGameplayEffect> UGA_Combo::GetDamageEffectForCurrentCombo() const
{
	UAnimInstance* OwnerAnimInstance = GetOwnerAnimInstance();
	if (OwnerAnimInstance)
	{
		FName CurrentSectionName = OwnerAnimInstance->Montage_GetCurrentSection(ComboMontage);
		const TSubclassOf<UGameplayEffect>* FoundEffectPtr = DamageEffectMap.Find(CurrentSectionName);
		if (FoundEffectPtr)
		{
			return *FoundEffectPtr;
		}
	}
	return DefaultDamageEffect;
}

void UGA_Combo::ComboChangedEventReceived(FGameplayEventData Data)
{
	FGameplayTag EventTag = Data.EventTag;

	if (EventTag == GetComboChangedEventEndTag())
	{
		NextComboName = NAME_None;
		//UE_LOG(LogTemp, Warning, TEXT("Next Combo is END:"));

		return;
	}

	TArray<FName> TagNames;
	UGameplayTagsManager::Get().SplitGameplayTagFName(EventTag, TagNames);
	NextComboName = TagNames.Last();

	//UE_LOG(LogTemp, Warning, TEXT("Next Combo is now: %s"), *NextComboName.ToString());
	
}

void UGA_Combo::TryDamage(FGameplayEventData Data)
{
	TArray<FHitResult> HitResults = GetHitResultFromSweepLocationTargetData(Data.TargetData, TargetSweepSphereRadius );

	for (const FHitResult& HitResult : HitResults)
	{
		TSubclassOf<UGameplayEffect> GameplayEffect = GetDamageEffectForCurrentCombo();		
		ApplyGameplayEffectToHitResultActor(HitResult, GameplayEffect, GetAbilityLevel(CurrentSpecHandle, CurrentActorInfo));
	}
}
