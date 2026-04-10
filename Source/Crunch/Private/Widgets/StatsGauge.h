#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AttributeSet.h"
#include "GameplayEffectTypes.h"

#include "StatsGauge.generated.h"

/**
 * 
 */
UCLASS()
class UStatsGauge : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Icon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Text_Attribute;
	
	UPROPERTY(EditAnywhere, Category = "Attribute")
	FGameplayAttribute Attribute;

	UPROPERTY(EditAnywhere, Category = "Visual")
	TObjectPtr<UTexture2D> IconTexture;
	
	FNumberFormattingOptions NumberFormattingOptions;

	void SetValue(float NewVal);
	void AttributeChanged(const FOnAttributeChangeData& Data );

};
