
#include "GeraltAnimInstance.h"
#include "Geralt.h"
#include "Kismet/KismetMathLibrary.h"

void UGeraltAnimInstance::NativeInitializeAnimation()
{
	MyGeraltActor = Cast<AGeralt>(TryGetPawnOwner());
	if (MyGeraltActor)
	{
		GeraltMovement = MyGeraltActor->GetCharacterMovement();
	}
}

void UGeraltAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	if (GeraltMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(GeraltMovement->Velocity);

		if (GroundSpeed > 3.f && UKismetMathLibrary::VSize(GeraltMovement->GetCurrentAcceleration()) != 0)
			ShouldMove = true;
		else
			ShouldMove = false;
	}
		

}
