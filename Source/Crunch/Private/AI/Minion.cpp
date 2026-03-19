#include "AI/Minion.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

void AMinion::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	Super::SetGenericTeamId(NewTeamID);
	PickSkinBadeOnTeamID();
}

bool AMinion::IsActive() const
{
	return !IsDead();
}

void AMinion::Activate()
{
	RespawnImmediately();
}

void AMinion::SetGoal(AActor* Goal)
{
	if (AAIController* AIController = GetController<AAIController>())
	{
		if (UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent())
		{
			BlackboardComponent->SetValueAsObject(GoalBlackboardKeyName, Goal);
		}
	}
}

void AMinion::PickSkinBadeOnTeamID()
{
	USkeletalMesh** Skin = SkinMap.Find(GetGenericTeamId());
	if (Skin)
	{
		GetMesh()->SetSkeletalMesh(*Skin);
	}
}

void AMinion::OnRep_TeamID()
{
	PickSkinBadeOnTeamID();
}
