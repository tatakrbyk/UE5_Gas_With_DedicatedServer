#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayEffectTypes.h"
#include "LevelGauge.generated.h"


/**
 * 
 */
UCLASS()
class ULevelGauge : public UUserWidget
{
	GENERATED_BODY()
	
public:

	//virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
private:

	UPROPERTY(EditDefaultsOnly, Category = "Visual")
	FName PercentMaterialParamName = "Percent";

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image_LevelProgress;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Text_Level;

	FNumberFormattingOptions NumberFormattingOptions;

	const class UAbilitySystemComponent* OwnerASC;
	
	void UpdateGauge(const FOnAttributeChangeData& Data);
};
