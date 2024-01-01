	// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"	
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "MyCharacter.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Overlap);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	Attributes = CreateDefaultSubobject<UCharacterAttributes>(TEXT("CharacterAttributes"));
	HealthBar = CreateDefaultSubobject<UMyHealthBarComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(GetRootComponent());

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 4500;
	PawnSensing->SetPeripheralVisionAngle(43.f);

	//GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}


void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	AnimInstance = GetMesh()->GetAnimInstance();
	
	if (HealthBar)
		HealthBar->SetVisibility(false);

	EnemyController = Cast<AAIController>(GetController());
	MoveToPatrolTarget(PatrolTarget);
	if (PawnSensing)
		PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);

	if (WeaponClass)
	{
		EquippedWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
		EquippedWeapon->TakeWeapon(GetMesh(), FName("Socket_RightHand"), this, this);
		EquippedWeapon->SetActorEnableCollision(true);
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (EnemyState != EEnemyState::EES_Patrolling)
		CheckCombatState();
	else
		CheckPatrolTarget();

	if (AttackAnimState == 1 && !(AnimInstance->Montage_IsPlaying(AttackMontage)) && Attributes->Health != 0 && !(AnimInstance->Montage_IsPlaying(HitReactMontage)))
		AttackAnimState = 0;

}

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) 
		return false;
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= Radius;

}

void AEnemy::MoveToPatrolTarget(AActor* Target)
{
	if (EnemyController == nullptr || PatrolTarget == nullptr || !Attributes->IsAlive())
		return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(50.f);
	EnemyController->MoveTo(MoveRequest);
}

AActor* AEnemy::ChooseToTarget()
{
	const int32 NumPatrolTargets = PatrolTargets.Num();
	if (NumPatrolTargets > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
		return PatrolTargets[TargetSelection];
	}
	return nullptr;
}

void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, 300.f))
	{
		PatrolTarget = ChooseToTarget();
		float WaitTime = FMath::RandRange(4, 10);
		GetWorldTimerManager().SetTimer(TargetTimer, this, &AEnemy::TimerFunc, WaitTime);
	}
}

void AEnemy::TimerFunc()
{
	MoveToPatrolTarget(PatrolTarget);
}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
	if (SeenPawn->ActorHasTag(FName("MyCharacter")))
	{
		GetWorldTimerManager().ClearTimer(TargetTimer);
		GetCharacterMovement()->MaxWalkSpeed = 350.f;
		CombatTarget = SeenPawn;
		if (EnemyState != EEnemyState::EES_Attacking)
		{
			EnemyState = EEnemyState::EES_Chasing;
			MoveToPatrolTarget(SeenPawn);
		}
	}
}

void AEnemy::CheckCombatState()
{
	if (CombatTarget == nullptr && Attributes->Health == 0)
		return;
	if (!InTargetRange(CombatTarget, CombatDist) && !(AnimInstance->Montage_IsPlaying(AttackMontage)))
	{
		CombatTarget = nullptr;
		if (HealthBar)
			HealthBar->SetVisibility(false);
		EnemyState = EEnemyState::EES_Patrolling;
		GetCharacterMovement()->MaxWalkSpeed = 125.f;
		MoveToPatrolTarget(PatrolTarget);
	}
	else if (!InTargetRange(CombatTarget, AttackDist) && EnemyState != EEnemyState::EES_Chasing && !(AnimInstance->Montage_IsPlaying(AttackMontage)))
	{
		EnemyState = EEnemyState::EES_Chasing;
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		MoveToPatrolTarget(CombatTarget);
	}
	else if (InTargetRange(CombatTarget, AttackDist) && EnemyState != EEnemyState::EES_Attacking)
	{
		EnemyState = EEnemyState::EES_Attacking;
		Attack();
	}
}

void AEnemy::Attack()
{
	if (AnimInstance && AttackMontage && AttackAnimState == 0 &&  !(AnimInstance->Montage_IsPlaying(HitReactMontage)))
	{
		AnimInstance->Montage_Play(AttackMontage);
		FName AttackName;
		int a = FMath::RandRange(0, 4);
		switch (a)
		{
			case 0:
				AttackName = FName("Attack1");
			case 1:
				AttackName = FName("Attack2");
			case 2:
				AttackName = FName("Attack3");
			case 3:
				AttackName = FName("Attack4");
			case 4:
				AttackName = FName("Attack5");
			default:
				break;
		}
		AnimInstance->Montage_JumpToSection(AttackName, AttackMontage);
		AttackAnimState = 1;
	}
	
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::PlayDeathMontage()
{
	int a = FMath::RandRange(1, 4);
	FName DeathMontageName;
	if (a == 1)
	{
		DeathMontageName = FName("Death1");
		DeathStatePose = EDeathState::DS_Death1;
	}
	else if (a == 2)
	{
		DeathMontageName = FName("Death2");
		DeathStatePose = EDeathState::DS_Death2;
	}
	else if (a == 3)
	{
		DeathMontageName = FName("Death3");
		DeathStatePose = EDeathState::DS_Death3;
	}
	else if (a == 4)
	{
		DeathMontageName = FName("Death4");
		DeathStatePose = EDeathState::DS_Death4;
	}
	AnimInstance->Montage_Play(DeathMontage);
	AnimInstance->Montage_JumpToSection(DeathMontageName, DeathMontage);
	HealthBar->SetVisibility(false);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	SetLifeSpan(10.0f);
	EquippedWeapon->SetLifeSpan(10.0f);
	
}

void AEnemy::PlayHitReactMontage(const FName &SectionName)
{
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
	if (DeathMontage && !(Attributes->IsAlive()))
		PlayDeathMontage();
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	//UE_LOG(LogTemp, Display, TEXT("%f , %f , %f : "), ImpactPoint.X, ImpactPoint.Y, ImpactPoint.Z);

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
	if (HealthBar)
		HealthBar->SetVisibility(true);
	if (HitSounds)
		UGameplayStatics::PlaySoundAtLocation(this, HitSounds, ImpactPoint);
	if (HitBlood)
		UGameplayStatics::SpawnEmitterAtLocation(this, HitBlood, ImpactPoint);
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Attributes->ReceiveDamage(DamageAmount);
	if (HealthBar)
		HealthBar->SetHealthPercent(Attributes->GetHealthPercent());
	CombatTarget = EventInstigator->GetPawn();
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = 350.f;
	MoveToPatrolTarget(CombatTarget);
	return 0.0f;
}

void AEnemy::SetCollisionWeaponBox(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreToActors.Empty();
	}
}

