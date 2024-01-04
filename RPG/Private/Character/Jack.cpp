#include "Character/Jack.h"

#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "DrawDebugHelpers.h"


AJack::AJack()
{
	PrimaryActorTick.bCanEverTick = true;

	MySpringArm = CreateDefaultSubobject<USpringArmComponent>("MySpringArm");
	MySpringArm->SetupAttachment(RootComponent);
	MySpringArm->TargetArmLength = 330.f;

	MyCamera = CreateDefaultSubobject<UCameraComponent>("MyCamera");
	MyCamera->SetupAttachment(MySpringArm);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	MySpringArm->bUsePawnControlRotation = true;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	InteractionCheckFrequences = 0.1f;
	InteractionCheckDistance = 246.f;

	BaseEyeHeight = 76.f;
}

void AJack::BeginPlay()
{
	Super::BeginPlay();

}

void AJack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequences)
	{
		PerformInteractionCheck();
	}
}



//====================================================================================
//
//								INTERACTION FUNCTIONS								
//
//====================================================================================

void AJack::PerformInteractionCheck()
{
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	FVector TraceStart{ GetPawnViewLocation()};
	FVector TraceEnd{ TraceStart + (GetViewRotation().Vector() * InteractionCheckDistance)};

	float Direction = FVector::DotProduct(GetActorForwardVector(), GetViewRotation().Vector());
	if (Direction > 0)
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Black, false, 5.f, 0, 2.f);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		FHitResult TraceHitResult;

		if (GetWorld()->LineTraceSingleByChannel(TraceHitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
		{
			if (TraceHitResult.GetActor()->GetClass()->ImplementsInterface(UI_Interaction::StaticClass()))
			{
				const float Distance = (TraceStart - TraceHitResult.ImpactPoint).Size();

				if (TraceHitResult.GetActor() != InteractionData.CurrentInteractionActor && Distance <= InteractionCheckDistance)
				{
					FoundInteractable(TraceHitResult.GetActor());
					return;
				}
				if (TraceHitResult.GetActor() == InteractionData.CurrentInteractionActor)
					return;
			}
			
			else
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
					FString::Printf(TEXT("%s"), *(TraceHitResult.GetActor()->GetActorNameOrLabel())));
			
		}
	}

	
	NoInteractableFound();
}

void AJack::FoundInteractable(AActor* NewInteractable)
{
	if (IsInteracting())
		EndInteract();
	if (InteractionData.CurrentInteractionActor)
	{
		TargetInteractable = InteractionData.CurrentInteractionActor;
		TargetInteractable->EndFocus();
	}

	InteractionData.CurrentInteractionActor = NewInteractable;
	TargetInteractable = NewInteractable;

	TargetInteractable->BeginFocus();

}

void AJack::NoInteractableFound()
{
	if (IsInteracting())
		GetWorldTimerManager().ClearTimer(InteractionTimerHandle);

	if (InteractionData.CurrentInteractionActor)
	{
		if (IsValid(TargetInteractable.GetObject()))
			TargetInteractable->EndFocus();



		InteractionData.CurrentInteractionActor = nullptr;
		TargetInteractable = nullptr;
	}
}

void AJack::BeginInteract()
{
	PerformInteractionCheck();

	if (InteractionData.CurrentInteractionActor)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->BeginInteract();

			if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
				Interact();

			else
				GetWorldTimerManager().SetTimer(InteractionTimerHandle,
					this, 
					&AJack::Interact, 
					TargetInteractable->InteractableData.InteractionDuration,
					false);

		}
			
	
	}
}

void AJack::EndInteract()
{
	GetWorldTimerManager().ClearTimer(InteractionTimerHandle);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndInteract();
	}
}

void AJack::Interact()
{
	GetWorldTimerManager().ClearTimer(InteractionTimerHandle);
	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->Interact();
	}
}




//====================================================================================
//
//								INPUT AND MOVE FUNCTIONS								
//
//====================================================================================

void AJack::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AJack::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AJack::MoveRight);
	PlayerInputComponent->BindAxis("LookRight", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AJack::StartStopCrouch);
	PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &AJack::BeginInteract);
	PlayerInputComponent->BindAction("Interaction", IE_Released, this, &AJack::EndInteract);
}

void AJack::MoveForward(float Scale)
{
	const FRotator Rotation = GetControlRotation();
	FRotator YawRotation = FRotator(0.f, Rotation.Yaw, 0.f);
	FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	AddMovementInput(Forward, Scale);
}

void AJack::MoveRight(float Scale)
{
	const FRotator Rotation = GetControlRotation();
	FRotator YawRotation = FRotator(0.f, Rotation.Yaw,0.f);
	FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(Right, Scale);
}

void AJack::StartStopCrouch()
{
	if (GetCharacterMovement()->IsCrouching())
		UnCrouch();
	else
		Crouch();
}







