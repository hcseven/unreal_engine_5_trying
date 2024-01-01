// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
	
#include "Kismet/KismetMathLibrary.h"
#include "CoreMinimal.h"
#include "MyCharacter.h"
#include "Animation/AnimInstance.h"
#include "MyCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DIFFOREST_API UMyCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()


public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	class AMyCharacter* Character;

	UPROPERTY(BlueprintReadOnly)
	class UCharacterMovementComponent* MovementComponent;

	UPROPERTY(BlueprintReadOnly)
	float Speed;

	UPROPERTY(BlueprintReadOnly)
	bool ZVector;

	UPROPERTY(BlueprintReadOnly)
	EMyCharacterState CharacterState;
};

