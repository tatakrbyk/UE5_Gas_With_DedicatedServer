#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include <Perception/AIPerceptionTypes.h>
#include "GameplayTagContainer.h"
#include "CAIController.generated.h"

/**
 * 
 */
UCLASS()
class ACAIController : public AAIController
{
	GENERATED_BODY()

public:
	ACAIController();

	virtual void OnPossess(APawn* NewPawn) override;
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditDefaultsOnly, Category = "AI Behaviour")
	FName TargetBlackboardKeyName = "Target";

	UPROPERTY(EditDefaultsOnly, Category = "AI Behaviour")
	TObjectPtr<class UBehaviorTree> BehaviorTree;

	UPROPERTY(VisibleDefaultsOnly, Category = "Perception")
	TObjectPtr<class UAIPerceptionComponent> AIPerceptionComponent;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Perception")
	TObjectPtr<class UAISenseConfig_Sight> SightConfig;

	UFUNCTION()
	void TargetPerceptionUpdated(AActor* TargetActor, FAIStimulus Stimulus);

	UFUNCTION()
	void TargetForgotten(AActor* ForgottenActor);

	const UObject* GetCurrentTarget() const;
	void SetCurrentTarget(AActor* NewTarget);

	AActor* GetNextPerceivedActor() const;
	void ForgetActorIfDead(AActor* ActorToForget);

	void ClearAndDisableAllSenses();
	void EnableAllSenses();

	void PawnDeadTagUpdated(const FGameplayTag Tag, int32 Count);
};
