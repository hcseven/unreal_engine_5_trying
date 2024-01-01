// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterAnimInstance.h"

void UMyCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<AMyCharacter>(TryGetPawnOwner());
	if (Character)
		MovementComponent = Character->GetCharacterMovement();
}

void UMyCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (Character)
	{
		Speed = UKismetMathLibrary::VSizeXY(MovementComponent->Velocity);
		ZVector = MovementComponent->IsFalling();
		CharacterState = Character->GetCharacterState()	;
	}


}
