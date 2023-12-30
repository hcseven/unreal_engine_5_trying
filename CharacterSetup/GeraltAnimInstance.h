// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GeraltAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERSETUP_API UGeraltAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	//like begin play's different version for animations
	virtual void NativeInitializeAnimation() override;

	//like tick's different version for animations
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	class AGeralt* MyGeraltActor;
	UCharacterMovementComponent* GeraltMovement;


	UPROPERTY(BlueprintReadOnly)
	bool ShouldMove;

	UPROPERTY(BlueprintReadOnly)
	float GroundSpeed;
};
