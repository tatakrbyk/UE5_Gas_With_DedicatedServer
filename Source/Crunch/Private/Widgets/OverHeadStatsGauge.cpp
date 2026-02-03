// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/OverHeadStatsGauge.h"


#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GAS/CAttributeSet.h"
#include "Widgets/ValueGauge.h"

void UOverHeadStatsGauge::ConfigureWithASC(UAbilitySystemComponent* ASC)
{
	if (ASC)
	{
		HealthBar->SetAndBoundToGameplayAttribute(ASC, UCAttributeSet::GetHealthAttribute(), UCAttributeSet::GetMaxHealthAttribute());
		ManaBar->SetAndBoundToGameplayAttribute(ASC, UCAttributeSet::GetManaAttribute(), UCAttributeSet::GetMaxManaAttribute());
	}
}
