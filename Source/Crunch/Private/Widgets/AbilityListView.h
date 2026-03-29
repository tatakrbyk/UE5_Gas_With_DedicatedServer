#pragma once

#include "CoreMinimal.h"
#include "Components/ListView.h"
#include "GAS/CGameplayAbilityTypes.h"

#include "AbilityListView.generated.h"

/**
 * 
 */
UCLASS()
class UAbilityListView : public UListView
{
	GENERATED_BODY()
	
public:
	void ConfigureAbilities(const TMap<ECAbilityInputID, TSubclassOf<class UGameplayAbility>>& Abilities);


private:

	UPROPERTY(EditAnywhere, Category = "Data")
	TObjectPtr<UDataTable> AbilityDataTable;

	void AbilityGaugeGenerated(UUserWidget& Widget);

	const struct FAbilityWidgetData* FindWidgetDataForAbilty(const TSubclassOf<UGameplayAbility>& AbilityClass) const;
};
