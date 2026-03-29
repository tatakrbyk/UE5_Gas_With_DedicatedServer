#include "GAS/GA_UpperCut.h"

#include "Abilities/Tasks//AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks//AbilityTask_WaitGameplayEvent.h"
#include "GAS/GA_Combo.h"
#include "GAS/CAbilitySystemStatics.h"
#include "GameplayTagsManager.h"

UGA_UpperCut::UGA_UpperCut()
{
	BlockAbilitiesWithTag.AddTag(UCAbilitySystemStatics::GetBasicAttackAbilityTag());
}

void UGA_UpperCut::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!K2_CommitAbility())
	{
		K2_EndAbility();
		return;
	}

	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		UAbilityTask_PlayMontageAndWait* PlayUpperCutMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, UpperCutMontage);
		PlayUpperCutMontageTask->OnBlendOut.AddDynamic(this, &UGA_UpperCut::K2_EndAbility);
		PlayUpperCutMontageTask->OnCancelled.AddDynamic(this, &UGA_UpperCut::K2_EndAbility);
		PlayUpperCutMontageTask->OnInterrupted.AddDynamic(this, &UGA_UpperCut::K2_EndAbility);
		PlayUpperCutMontageTask->OnCompleted.AddDynamic(this, &UGA_UpperCut::K2_EndAbility);
		PlayUpperCutMontageTask->ReadyForActivation();

		UAbilityTask_WaitGameplayEvent* WaitLaunchEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, GetUpperCutLaunchTag());
		WaitLaunchEventTask->EventReceived.AddDynamic(this, &UGA_UpperCut::StartLaunching);
		WaitLaunchEventTask->ReadyForActivation();
	}

	NextComboName = NAME_None;
}

FGameplayTag UGA_UpperCut::GetUpperCutLaunchTag()
{
	return FGameplayTag::RequestGameplayTag("Ability.Uppercut.Launch");
}

const FGenericDamageEffectDef* UGA_UpperCut::GetDamageEffectDefForCurrentCombo() const
{
	UAnimInstance* OwnerAnimInstance = GetOwnerAnimInstance();
	if (OwnerAnimInstance)
	{
		FName CurrentComboName = OwnerAnimInstance->Montage_GetCurrentSection(UpperCutMontage);
		const FGenericDamageEffectDef* EffectDef = ComboDamageMap.Find(CurrentComboName);
		return EffectDef;
	}
	return nullptr;
}

void UGA_UpperCut::StartLaunching(FGameplayEventData EventData)
{

	if (K2_HasAuthority())
	{
		TArray<FHitResult> TargetHitResults = GetHitResultFromSweepLocationTargetData(EventData.TargetData, TargetSweepSphereRadius, ETeamAttitude::Hostile, ShouldDrawDebug());
		PushTarget(GetAvatarActorFromActorInfo(), FVector::UpVector * UpperCutLaunchSpeed);
		for (FHitResult& HitResult : TargetHitResults)
		{
			PushTarget(HitResult.GetActor(), FVector::UpVector * UpperCutLaunchSpeed);
			ApplyGameplayEffectToHitResultActor(HitResult, LaunchDamageEffect, GetAbilityLevel(CurrentSpecHandle, CurrentActorInfo));
		}
	}

	UAbilityTask_WaitGameplayEvent* WaitComboChangeEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, UGA_Combo::GetComboChangedEventTag(), nullptr, false, false);
	WaitComboChangeEvent->EventReceived.AddDynamic(this, &UGA_UpperCut::HandleComboChangeEvent);
	WaitComboChangeEvent->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitComboCommitEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, UCAbilitySystemStatics::GetBasicAttackInputPressedTag() );
	WaitComboCommitEvent->EventReceived.AddDynamic(this, &UGA_UpperCut::HandleComboCommitEvent);
	WaitComboCommitEvent->ReadyForActivation();


	UAbilityTask_WaitGameplayEvent* WaitComboDamageEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, UGA_Combo::GetComboTargetEventTag());
	WaitComboDamageEvent->EventReceived.AddDynamic(this, &UGA_UpperCut::HandleComboDamageEvent);
	WaitComboDamageEvent->ReadyForActivation();
}

void UGA_UpperCut::HandleComboChangeEvent(FGameplayEventData EventData)
{

	FGameplayTag EventTag = EventData.EventTag;
	if (EventTag == UGA_Combo::GetComboChangedEventEndTag())
	{
		NextComboName = NAME_None;
		UE_LOG(LogTemp, Warning, TEXT(" [GA_UpperCut] Next Combo is cleared"));
		return;
	}

	TArray<FName> TagNames;
	UGameplayTagsManager::Get().SplitGameplayTagFName(EventTag, TagNames);

	NextComboName = TagNames.Last();
	UE_LOG(LogTemp, Warning, TEXT(" [GA_UpperCut] Next Combo is: %s"), *NextComboName.ToString());

}

void UGA_UpperCut::HandleComboCommitEvent(FGameplayEventData EventData)
{
	if (NextComboName == NAME_None) return;

	UAnimInstance* OwnerAnimInst = GetOwnerAnimInstance();
	if (!OwnerAnimInst) return;
	OwnerAnimInst->Montage_SetNextSection(OwnerAnimInst->Montage_GetCurrentSection(UpperCutMontage), NextComboName, UpperCutMontage);	

}

void UGA_UpperCut::HandleComboDamageEvent(FGameplayEventData EventData)
{
	if (K2_HasAuthority())
	{
		TArray<FHitResult> TargetHitResults = GetHitResultFromSweepLocationTargetData(EventData.TargetData, TargetSweepSphereRadius, ETeamAttitude::Hostile, ShouldDrawDebug());
		PushTarget(GetAvatarActorFromActorInfo(), FVector::UpVector * UpperCuComboHoldSpeed);
		const FGenericDamageEffectDef* EffectDef = GetDamageEffectDefForCurrentCombo();

		if (!EffectDef) return;

		for (FHitResult& HitResult : TargetHitResults)
		{
			FVector PushVel = GetAvatarActorFromActorInfo()->GetActorTransform().TransformVector(EffectDef->PushVelocity);
			PushTarget(HitResult.GetActor(), PushVel);
			ApplyGameplayEffectToHitResultActor(HitResult, EffectDef->DamageEffect, GetAbilityLevel(CurrentSpecHandle, CurrentActorInfo));
		}
	}

}

