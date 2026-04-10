#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GAS/CGameplayAbilityTypes.h"
#include "GameplayWidget.generated.h"

/**
 * 
 */
UCLASS()
class UGameplayWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	void ConfigureAbilities(const TMap<ECAbilityInputID, TSubclassOf<class UGameplayAbility>>& Abilities);
private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UValueGauge> HealthBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UValueGauge> ManaBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UAbilityListView> ListView_Ability;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UStatsGauge> AttackDamageGauge;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UStatsGauge> ArmorGauge;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UStatsGauge> MoveSpeedGauge;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UStatsGauge> IntelligenceGauge;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UStatsGauge> StrengthGauge;

	UPROPERTY()
	TObjectPtr<class UAbilitySystemComponent> OwnerAbilitySystemComponent;

};
