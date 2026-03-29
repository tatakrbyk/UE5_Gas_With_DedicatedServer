// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CCharacter.h"
#include "GAS/CGameplayAbilityTypes.h"
#include "CPlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
/**
 * 
 */
UCLASS()
class ACPlayerCharacter : public ACCharacter
{
	GENERATED_BODY()

public:
	ACPlayerCharacter();

	virtual void PawnClientRestart() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	

private:

	UPROPERTY(VisibleDefaultsOnly, Category = "View")
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleDefaultsOnly, Category = "View")
	TObjectPtr<class UCameraComponent> ViewCam;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpIA;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> LookIA;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveIA;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TMap<ECAbilityInputID, TObjectPtr<UInputAction >> GameplayAbilityInputActions;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> GameplayIMC;

	void HandleLookInput(const FInputActionValue& InputActionValue);
	void HandleMoveInput(const FInputActionValue& InputActionValue);
	void HandleAbilityInput(const FInputActionValue& InputActionValue, ECAbilityInputID InputID);
	void SetInputEnabledFromPlayerController(bool bEnabled);

	FVector GetLookRightDirection() const;
	FVector GetLookForwardDirection() const;
	FVector GetMoveForwardDirection() const;

	/* Stun */
	virtual void OnStun() override;
	virtual void OnRecoverFromStun() override;
	/* Stund End*/
	/* Death and Respawn */
	virtual void OnDead() override;
	virtual void OnRespawn() override;
	/* Death and Respawn End*/


};
