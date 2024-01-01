// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Animation/AnimMontage.h"

AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.f;	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	AnimInstance = GetMesh()->GetAnimInstance();

	Tags.Add(FName("MyCharacter"));
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CharacterState == EMyCharacterState::ECS_HasZeroWeapon1 && !AnimInstance->Montage_IsPlaying(AnimSwordMontage) && AnimState == 2)
	{
		AnimInstance->Montage_Play(AnimSwordMontage);
		AnimInstance->Montage_JumpToSection(FName("SwordDown2"), AnimSwordMontage);
		Weapon->TakeWeapon(GetMesh(), FName("Socket_SpineForSword"), this, this);
		AnimState = 1;
	}
	if (CharacterState == EMyCharacterState::ECS_HasOneWeapon && !AnimInstance->Montage_IsPlaying(AnimSwordMontage) && AnimState == 3)
	{
		AnimInstance->Montage_Play(AnimSwordMontage);
		AnimInstance->Montage_JumpToSection(FName("SwordUp2"), AnimSwordMontage);
		Weapon->TakeWeapon(GetMesh(), FName("Socket_HandR"), this, this);
		AnimState = 1;
	}

}

void AMyCharacter::GetHit_Implementation(const FVector& ImpactPoint)
{
	
	FVector Forward = GetActorForwardVector();
	FVector ImpactLow(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	FVector AfterHit = (ImpactPoint - GetActorLocation()).GetSafeNormal();
	double CosTheta = FVector::DotProduct(Forward, AfterHit);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);
	FVector CrossProd = FVector::CrossProduct(Forward, AfterHit);
	//UE_LOG(LogTemp, Display, TEXT(" THETAAAA : %f  "), Theta);
	if (CrossProd.Z < 0.f)
		Theta *= -1.f;
	//UE_LOG(LogTemp, Display, TEXT(" THETA : %f  "), Theta);

	FName SectionNameForHit = FName(TEXT("React_Back"));
	if (Theta >= 45.f && Theta < 135.f)
		SectionNameForHit = FName("React_Right");
	else if (Theta >= -135.f && Theta < -45.f)
		SectionNameForHit = FName("React_Left");
	else if (Theta >= -45.f && Theta < 45.f)
		SectionNameForHit = FName("React_Front");
	PlayHitReactMontage(SectionNameForHit);
	//if (HealthBar)
		//HealthBar->SetVisibility(true);
	if (HitSounds)
		UGameplayStatics::PlaySoundAtLocation(this, HitSounds, ImpactPoint);
	if (HitBlood)
		UGameplayStatics::SpawnEmitterAtLocation(this, HitBlood, ImpactPoint);
}

void AMyCharacter::PlayHitReactMontage(FName SectionName)
{
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
	if (DeathMontage && !(Attributes->IsAlive()))
		PlayDeathMontage();
}

void AMyCharacter::PlayDeathMontage()
{
	int a = FMath::RandRange(1, 4);
	FName DeathMontageName;
	if (a == 1)
	{
		DeathMontageName = FName("Death1");
		DeathStatePose = EMyCharacterDeathState::DS_Death1;
	}
	else if (a == 2)
	{
		DeathMontageName = FName("Death2");
		DeathStatePose = EMyCharacterDeathState::DS_Death2;
	}
	else if (a == 3)
	{
		DeathMontageName = FName("Death3");
		DeathStatePose = EMyCharacterDeathState::DS_Death3;
	}
	else if (a == 4)
	{
		DeathMontageName = FName("Death4");
		DeathStatePose = EMyCharacterDeathState::DS_Death4;
	}
	AnimInstance->Montage_Play(DeathMontage);
	AnimInstance->Montage_JumpToSection(DeathMontageName, DeathMontage);
	HealthBar->SetVisibility(false);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	SetLifeSpan(3.0f);
}

// Bind Keyboard Input, Move, Attack and Sword Anims
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName("TurnYaw"), this, &AMyCharacter::TurnYaw);
	PlayerInputComponent->BindAxis(FName("TurnPitch"), this, &AMyCharacter::TurnPitch);
	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed , this, &AMyCharacter::Jump);
	PlayerInputComponent->BindAction(FName("TakeEquip"), IE_Pressed, this, &AMyCharacter::TakeEquip);
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &AMyCharacter::Attack);
	PlayerInputComponent->BindAction(FName("SwordDown"), IE_Pressed, this, &AMyCharacter::SwordDown);
	PlayerInputComponent->BindAction(FName("SwordUp"), IE_Pressed, this, &AMyCharacter::SwordUp);
}

void AMyCharacter::MoveForward(float Scale)
{
	if (!AnimInstance->Montage_IsPlaying(AnimMontage) && !(AnimInstance->Montage_IsPlaying(AnimSwordMontage)))
		AnimState = 0;
	if (Controller && Scale	&& AnimState == 0)
	{
		const FRotator controlrot = GetControlRotation();
		const FRotator mycontrolrotator(0.f, controlrot.Yaw, 0.f);
		FVector Direction = FRotationMatrix(mycontrolrotator).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Scale);
	}
}

void AMyCharacter::MoveRight(float Scale)
{
	if (!AnimInstance->Montage_IsPlaying(AnimMontage) && !(AnimInstance->Montage_IsPlaying(AnimSwordMontage)))
		AnimState = 0;
	
	if (Controller && Scale && AnimState == 0)
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Scale);	
	}
}

void AMyCharacter::TurnYaw(float Scale)
{
	AddControllerYawInput(Scale);
}

void AMyCharacter::TurnPitch(float Scale)
{
	AddControllerPitchInput(Scale);
}

void AMyCharacter::TakeEquip()
{
	Weapon = Cast<AWeapon>(OverlappingItem);
	if (Weapon && CharacterState == EMyCharacterState::ECS_HasZeroWeapon)
	{
		OverlappingItem->SetInstigator(this);
		Weapon->TakeWeapon(GetMesh(), FName("Socket_HandR"), GetOwner(), this);
		CharacterState = EMyCharacterState::ECS_HasOneWeapon;
	}
	/*
	else if (Weapon && CharacterState == EMyCharacterState::ECS_HasOneWeapon)
	{
		Weapon->TakeWeapon(GetMesh(), FName("Socket_HandL"));
		CharacterState = EMyCharacterState::ECS_HasTwoWeapon;
	}*/
}

void AMyCharacter::Attack() 
{	
	if (AnimInstance && AnimMontage && AnimState == 0 && (CharacterState != EMyCharacterState::ECS_HasZeroWeapon) && (CharacterState != EMyCharacterState::ECS_HasZeroWeapon1))
	{	
		AnimInstance->Montage_Play(AnimMontage);
		FName SectionName = FName();
		int32 Select = FMath::RandRange(0, 1);
		if (Select == 0)
			SectionName = FName("Attack1");
		else if (Select == 1)
			SectionName = FName("Attack2");
		AnimInstance->Montage_JumpToSection(SectionName, AnimMontage);
		AnimState = 1;
	}
	if (!(AnimInstance->Montage_IsPlaying(AnimMontage)))
		AnimState = 0;
}

void AMyCharacter::SwordSecondDownUp()
{
	UE_LOG(LogTemp, Display, TEXT("advsjdsd"));
}

void AMyCharacter::SwordDown()
{
	//float a = AnimSwordMontage->GetPlayLength();
	if (CharacterState == EMyCharacterState::ECS_HasOneWeapon)
	{
		AnimInstance->Montage_Play(AnimSwordMontage);
		AnimInstance->Montage_JumpToSection(FName("SwordDown"), AnimSwordMontage);
		FTimerHandle Handle;
		//GetWorldTimerManager().SetTimer(Handle,this, &AMyCharacter::SwordSecondDownUp, AnimSwordMontage->GetSectionLength(0), false);
		AnimState = 2;
		CharacterState = EMyCharacterState::ECS_HasZeroWeapon1;
	}
	if (!(AnimInstance->Montage_IsPlaying(AnimSwordMontage)))
		AnimState = 0;
}

void AMyCharacter::SwordUp()
{
	if (CharacterState == EMyCharacterState::ECS_HasZeroWeapon1)
	{
		AnimInstance->Montage_Play(AnimSwordMontage);
		AnimInstance->Montage_JumpToSection(FName("SwordUp"), AnimSwordMontage);
		AnimState = 3;
		CharacterState = EMyCharacterState::ECS_HasOneWeapon;
	}
	if (!(AnimInstance->Montage_IsPlaying(AnimSwordMontage)))
		AnimState = 0;

}

//blueprint içerisinden çaðýrýlýyor
void AMyCharacter::SetCollisionWeaponBox(ECollisionEnabled::Type CollisionEnabled)
{
	if (Weapon && Weapon->GetWeaponBox())
	{
		Weapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		Weapon->IgnoreToActors.Empty();
	}
}



