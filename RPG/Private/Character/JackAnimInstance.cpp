#include "Character/JackAnimInstance.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"



void UJackAnimInstance::NativeInitializeAnimation()
{
	MyJack = Cast<AJack>(TryGetPawnOwner());
	if (MyJack)
	{
		JackMovement = MyJack->GetCharacterMovement();
	}
}

void UJackAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	if (MyJack && JackMovement)
	{
		
		Speed = UKismetMathLibrary::VSizeXY(JackMovement->Velocity);
		
		if (Speed > 3.f && UKismetMathLibrary::VSizeXY(JackMovement->GetCurrentAcceleration()) != 0)
			ShouldMove = true;
		else
			ShouldMove = false;
			
		MyIsFalling = JackMovement->IsFalling();
		MyVelocity = JackMovement->Velocity;
		MyIsCrouching = JackMovement->IsCrouching();
		
		/* FOR LEAN BUT HAS BUG
		FRotator LastFrameRotation;
		FRotator LeanRotation = UKismetMathLibrary::NormalizedDeltaRotator(MyJack->GetActorRotation(), LastFrameRotation);
		float LeanValue = LeanRotation.Yaw / DeltaTime;
		LeanValue /= 10;
		LeanYaw = FMath::FInterpTo(LeanYaw, LeanValue, DeltaTime, 100);
		LastFrameRotation = MyJack->GetActorRotation();
		*/
	}

}
