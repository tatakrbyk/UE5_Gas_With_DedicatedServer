#include "AI/MinionBarrack.h"

#include "GameFramework/PlayerStart.h"
#include "AI/Minion.h"

void AMinionBarrack::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(SpawnIntervalTimerHandle, this, &AMinionBarrack::SpawnNewGroup, GroupSpawnInterval, true);
	}
}

const APlayerStart* AMinionBarrack::GetNextSpawnSpot()
{
	if(SpawnSpots.Num() == 0) return nullptr;
	++NextSpawnSpotIndex; // -! to 0

	if (NextSpawnSpotIndex > SpawnSpots.Num()-1)
	{
		NextSpawnSpotIndex = 0;
	}

	return SpawnSpots[NextSpawnSpotIndex];
}

void AMinionBarrack::SpawnNewGroup()
{
	int i = MinionPerGroup;

	while (i > 0)
	{
		FTransform SpawnTransform = GetActorTransform();
		if (const APlayerStart* NextSpawnSpot = GetNextSpawnSpot())
		{
			SpawnTransform = NextSpawnSpot->GetActorTransform();
		}

		AMinion* NextAvailableMinion = GetNextAvailableMinion();
		if (!NextAvailableMinion) break;

		NextAvailableMinion->SetActorTransform(SpawnTransform);
		NextAvailableMinion->Activate();
		--i;
	}
	SpawnNewMinions(i);
}

void AMinionBarrack::SpawnNewMinions(int Amount)
{
	for (int i = 0; i < Amount; i++)
	{
		FTransform SpawnTransform = GetActorTransform();
		if (const APlayerStart* NextSpawnSpot = GetNextSpawnSpot())
		{
			SpawnTransform = NextSpawnSpot->GetActorTransform();
		}

		AMinion* NewMinion = GetWorld()->SpawnActorDeferred<AMinion>(MinionClass, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
		NewMinion->SetGenericTeamId(BarrackTeamID);
		NewMinion->FinishSpawning(SpawnTransform);
		NewMinion->SetGoal(Goal);
		MinionPool.Add(NewMinion);
	}
}

AMinion* AMinionBarrack::GetNextAvailableMinion() const
{
	for (AMinion* Minion : MinionPool)
	{
		if (!Minion->IsActive())
		{
			return Minion;
		}
	}
	return nullptr;
}

