#include "Widgets/AbilityListView.h"

#include "Abilities/GameplayAbility.h"
#include "Widgets/AbilityGauge.h"

void UAbilityListView::ConfigureAbilities(const TMap<ECAbilityInputID, TSubclassOf<UGameplayAbility>>& Abilities)
{
	OnEntryWidgetGenerated().AddUObject(this, &UAbilityListView::AbilityGaugeGenerated);
	for (const TPair<ECAbilityInputID, TSubclassOf<UGameplayAbility>>& AbilityPair : Abilities)
	{
		AddItem(AbilityPair.Value.GetDefaultObject());
	}
}

void UAbilityListView::AbilityGaugeGenerated(UUserWidget& Widget)
{
	UAbilityGauge* AbilityGauge = Cast<UAbilityGauge>(&Widget);
	if (AbilityGauge)
	{
		AbilityGauge->ConfigureWithWidgetData(FindWidgetDataForAbilty(AbilityGauge->GetListItem<UGameplayAbility>()->GetClass()));
	}
}

const FAbilityWidgetData* UAbilityListView::FindWidgetDataForAbilty(const TSubclassOf<UGameplayAbility>& AbilityClass) const
{
	if (!AbilityDataTable) return nullptr;

	for (auto& AbilityWidgetDataPair : AbilityDataTable->GetRowMap())
	{
		// Key -> RowName,	Vakue-> Data
		const FAbilityWidgetData* WidgetData = AbilityDataTable->FindRow<FAbilityWidgetData>(AbilityWidgetDataPair.Key, "");
		if (WidgetData->AbilityClass == AbilityClass)
		{
			return WidgetData;
		}
	}
	return nullptr;
}
