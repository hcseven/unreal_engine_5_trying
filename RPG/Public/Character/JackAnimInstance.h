// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Jack.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "JackAnimInstance.generated.h"


/**
 * 
 */
UCLASS()
class RPG_API UJackAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

public: 

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;



public: 

	UCharacterMovementComponent* JackMovement;

	AJack* MyJack;

	UPROPERTY(BlueprintReadOnly)
	float Speed;

	UPROPERTY(BlueprintReadOnly)
	bool ShouldMove;

	UPROPERTY(BlueprintReadOnly)
	bool MyIsFalling;

	UPROPERTY(BlueprintReadOnly)
	FVector MyVelocity;

	UPROPERTY(BlueprintReadOnly)
	bool MyIsCrouching;

	
	//UPROPERTY(BlueprintReadOnly)
	//float LeanYaw;

};
