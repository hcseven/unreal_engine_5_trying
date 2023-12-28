// Fill out your copyright notice in the Description page of Project Settings.


#include "Geralt.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"


AGeralt::AGeralt()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->TargetArmLength = 300.f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetupAttachment(RootComponent);
	

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
}

void AGeralt::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem
			<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(GeraltMappingContext, 0);
		}
	}

}

void AGeralt::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(GeraltMoveAction, ETriggerEvent::Triggered, this, &AGeralt::Move);
		EnhancedInput->BindAction(GeraltLookAction, ETriggerEvent::Triggered, this, &AGeralt::Look);
	}
	
	
	/*
	* //With old input type
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &AGeralt::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &AGeralt::MoveRight);
	PlayerInputComponent->BindAxis(FName("LookRight"), this, &AGeralt::LookRight);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &AGeralt::LookUp);
	*/
}
void AGeralt::Move(const FInputActionValue& Scale)
{
	FVector2D MoveVector = Scale.Get<FVector2D>();
	
	const FRotator Rotation = GetControlRotation();
	FRotator YawRotation = FRotator(0.f, Rotation.Yaw, 0.f);
	
	FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Forward, MoveVector.Y);

	FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Right, MoveVector.X);
}
void AGeralt::Look(const FInputActionValue& Scale)
{
	FVector2D LookAxis = Scale.Get<FVector2D>();

	AddControllerYawInput(LookAxis.X);
	AddControllerPitchInput(LookAxis.Y * -1);
}












/*
* //With old input type
void AGeralt::MoveForward(float Scale)
{
	if (Controller && Scale)
		AddMovementInput(GetActorForwardVector(), Scale);
}

void AGeralt::MoveRight(float Scale)
{
	if (Controller && Scale)
		AddMovementInput(GetActorRightVector(), Scale);
}
void AGeralt::LookRight(float Scale)
{
	if (Controller && Scale)
		AddControllerYawInput(Scale);
}
void AGeralt::LookUp(float Scale)
{
	if (Controller && Scale)
		AddControllerPitchInput(Scale);
}
*/