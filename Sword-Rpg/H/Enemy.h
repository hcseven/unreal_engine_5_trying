// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "MyHealthBarComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "CharacterAttributes.h"
#include "MyInterface.h"
#include "Enemy.generated.h"

class UAnimMontage;
class UCharacterAttributes;

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_Patrolling,
	EES_Chasing,
	EES_Attacking
};

UENUM(BlueprintType)
enum class EDeathState : uint8
{
	DS_Alive,
	DS_Death1,
	DS_Death2,
	DS_Death3,
	DS_Death4
};

UCLASS()
class DIFFOREST_API AEnemy : public ACharacter , public IMyInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(VisibleAnywhere)
	UCharacterAttributes* Attributes;

	UPROPERTY(EditAnywhere)
	UMyHealthBarComponent* HealthBar;

	UPROPERTY(EditAnywhere)
	UPawnSensingComponent* PawnSensing;

	UFUNCTION(BlueprintCallable)
	void SetCollisionWeaponBox(ECollisionEnabled::Type CollisionEnabled);

protected:

	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

	virtual void BeginPlay() override;

	/*
		MONTAGES, PARTICLES AND SOUNDS
	*/
	UPROPERTY(EditAnywhere)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* DeathMontage;

	void PlayHitReactMontage(const FName& SectionName);

	void PlayDeathMontage();
	
	UPROPERTY(EditAnywhere)
	USoundBase* HitSounds;

	UPROPERTY(EditAnywhere)
	UParticleSystem* HitBlood;

	UPROPERTY(BlueprintReadOnly)
	EDeathState DeathStatePose = EDeathState::DS_Alive;


	/* HEALTHBAR ACTIVATE*/
	AActor* CombatTarget;


	/* 
		PATROLS
	*/
	UPROPERTY(EditInstanceOnly)
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly)
	TArray<AActor*> PatrolTargets;

	class AAIController* EnemyController;
	
	bool InTargetRange(AActor* Target, double Radius);

	void MoveToPatrolTarget(AActor* Target);

	AActor* ChooseToTarget();

	void CheckPatrolTarget();
	
	void TimerFunc();

	void AttackTimerFunc();

	float CombatDist = 500.f;
	float AttackDist = 200.f;

	FTimerHandle TargetTimer;

	FTimerHandle AttackTimer;

	UFUNCTION()
	void PawnSeen(APawn* Pawn);

	void CheckCombatState();

	void Attack();

	int AttackAnimState = 1;

	UAnimInstance* AnimInstance;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;

	AWeapon* EquippedWeapon;//spawn ettiðimiz kýlýcý buna atayýp destroy edeceðiz.
	
};
