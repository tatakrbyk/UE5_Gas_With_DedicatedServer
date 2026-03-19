#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GenericTeamAgentInterface.h"

#include "MinionBarrack.generated.h"

UCLASS()
class AMinionBarrack : public AActor
{
	GENERATED_BODY()
	

protected:
	virtual void BeginPlay() override;


private:

	UPROPERTY(EditAnywhere, Category = "Spawn")
	FGenericTeamId BarrackTeamID;
	
	UPROPERTY(EditAnywhere, Category = "Spawn")
	int MinionPerGroup = 3;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	float GroupSpawnInterval = 5.f;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	AActor* Goal; // TobjectPtr ?, TWeakObjectPtr ? 

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TSubclassOf<class AMinion> MinionClass;

	UPROPERTY()
	TArray<class AMinion*> MinionPool;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TArray<class APlayerStart*> SpawnSpots;

	int NextSpawnSpotIndex = -1;
	const APlayerStart* GetNextSpawnSpot();

	void SpawnNewGroup();
	void SpawnNewMinions(int Amount);
	AMinion* GetNextAvailableMinion() const;

	FTimerHandle SpawnIntervalTimerHandle;


};
