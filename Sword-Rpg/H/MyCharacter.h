// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CharacterAttributes.h"
#include "MyInterface.h"
#include "CoreMinimal.h"
#include "Weapon.h"
#include "Components/CapsuleComponent.h"	
#include "MyHealthBarComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

class AItem;
class USphereComponent;

UENUM(BlueprintType)
enum class EMyCharacterState : uint8 {
	ECS_HasZeroWeapon,
	ECS_HasOneWeapon,
	ECS_HasTwoWeapon,
	ECS_HasZeroWeapon1,// 1 silahý var o da sýrtýnda
	ECS_HasOneWeapon1,//2 silahý var biri sýrtýnda
};

UENUM(BlueprintType)
enum class EMyCharacterDeathState : uint8
{
	DS_Alive,
	DS_Death1,
	DS_Death2,
	DS_Death3,
	DS_Death4
};

UCLASS()
class DIFFOREST_API AMyCharacter : public ACharacter, public IMyInterface
{
	GENERATED_BODY()

public:
	AMyCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void MoveForward(float Scale);
	void MoveRight(float Scale);
	void TurnYaw(float Scale);
	void TurnPitch(float Scale);
	void TakeEquip();
	void Attack();
	void SwordDown();
	void SwordUp();
	void SwordSecondDownUp();

	FORCEINLINE EMyCharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }

	UFUNCTION(BlueprintCallable)
	void SetCollisionWeaponBox(ECollisionEnabled::Type CollisionEnabled);

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

protected:
	virtual void BeginPlay() override;

	EMyCharacterState CharacterState = EMyCharacterState::ECS_HasZeroWeapon;
private:
	UPROPERTY(EditAnywhere);		
	UCameraComponent* Camera;
	
	UPROPERTY(EditAnywhere);
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere);
	AItem* OverlappingItem;
	
	UPROPERTY(EditAnywhere);
	UAnimMontage* AnimMontage;//Atak animasyonlarý //Blueprintten seçtin içerisindeki animasyonlarý 

	UPROPERTY(EditAnywhere);
	UAnimMontage* AnimSwordMontage;//Blueprintten seçtin içerisindeki animasyonlarý

	UPROPERTY(EditAnywhere);
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere);
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere)
	UMyHealthBarComponent* HealthBar;

	UPROPERTY(EditAnywhere)
	USoundBase* HitSounds;

	UPROPERTY(EditAnywhere)
	UParticleSystem* HitBlood;

	AWeapon* Weapon;

	EMyCharacterDeathState DeathStatePose = EMyCharacterDeathState::DS_Alive;

	UCharacterAttributes* Attributes;

	UAnimInstance* AnimInstance;

	uint8 AnimState = 0;

	void PlayHitReactMontage(FName SectionName);

	void PlayDeathMontage();
};
