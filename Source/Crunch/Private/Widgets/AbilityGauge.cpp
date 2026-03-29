#include "Widgets/AbilityGauge.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GAS/CAbilitySystemStatics.h"

void UAbilityGauge::NativeConstruct()
{
	Super::NativeConstruct();
	TEXT_CooldownCounter->SetVisibility(ESlateVisibility::Hidden);

	UAbilitySystemComponent* OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningPlayerPawn());
	if (OwnerASC)
	{
		OwnerASC->AbilityCommittedCallbacks.AddUObject(this, &UAbilityGauge::AbilityCommitted);
	}
	
	WholeNumberFormattionOptions.MaximumFractionalDigits = 0;
	TwoDigitNumberFormattionOptions.MaximumFractionalDigits = 2;
}

void UAbilityGauge::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// NOTE(t): ListItemObject Assing this class in WBP_GameplayWidget as Entry 
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	AbilityCDO = Cast<UGameplayAbility>(ListItemObject);

	float CooldownDuration = UCAbilitySystemStatics::GetStaticCooldownDurationForAbility(AbilityCDO) ;
	float Cost = UCAbilitySystemStatics::GetStaticCostForAbility(AbilityCDO);
	
	TEXT_CooldownDuration->SetText(FText::AsNumber(CooldownDuration));
	TEXT_Cost->SetText(FText::AsNumber(Cost));
}

void UAbilityGauge::ConfigureWithWidgetData(const FAbilityWidgetData* WidgetData)
{
	if (Icon && WidgetData)
	{
		Icon->GetDynamicMaterial()->SetTextureParameterValue(IconMaterialParamName, WidgetData->Icon.LoadSynchronous());
	}
}

void UAbilityGauge::AbilityCommitted(UGameplayAbility* Ability)
{
	if (Ability->GetClass()->GetDefaultObject() == AbilityCDO)
	{
		float CooldownTimeRemaining = 0.f;
		float CooldownDuration = 0.f;

		Ability->GetCooldownTimeRemainingAndDuration(Ability->GetCurrentAbilitySpecHandle(), Ability->GetCurrentActorInfo(), CooldownTimeRemaining, CooldownDuration);

		StartCooldown(CooldownTimeRemaining, CooldownDuration	);
	}
}

void UAbilityGauge::StartCooldown(float CooldownTimeRemaining, float CooldownDuration)
{
	TEXT_CooldownDuration->SetText(FText::AsNumber(CooldownDuration));
	CachedCooldownDuration = CooldownDuration;
	CachedCooldownTimeRemaing = CooldownTimeRemaining;

	TEXT_CooldownCounter->SetVisibility(ESlateVisibility::Visible);

	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UAbilityGauge::CooldownFinished, CachedCooldownTimeRemaing);
	GetWorld()->GetTimerManager().SetTimer(CooldownTimerUpdateHandle, this, &UAbilityGauge::UpdateCooldown, CooldownUpdateInterval, true, 0.f);

}

void UAbilityGauge::CooldownFinished()
{
	CachedCooldownDuration = CachedCooldownTimeRemaing = 0.f;
	TEXT_CooldownCounter->SetVisibility(ESlateVisibility::Hidden);
	GetWorld()->GetTimerManager().ClearTimer(CooldownTimerUpdateHandle);
	Icon->GetDynamicMaterial()->SetScalarParameterValue(CooldownPercentParamName, 1.f);

}

void UAbilityGauge::UpdateCooldown()
{
	CachedCooldownTimeRemaing -= CooldownUpdateInterval;
	FNumberFormattingOptions* FormattingOptions = CachedCooldownTimeRemaing > 1 ? &WholeNumberFormattionOptions : &TwoDigitNumberFormattionOptions;
	TEXT_CooldownCounter->SetText(FText::AsNumber(CachedCooldownTimeRemaing, FormattingOptions));

	Icon->GetDynamicMaterial()->SetScalarParameterValue(CooldownPercentParamName, 1.0f - CachedCooldownTimeRemaing / CachedCooldownDuration);
}
