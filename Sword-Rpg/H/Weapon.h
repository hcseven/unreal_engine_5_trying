// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Breakable.h"
#include "CoreMinimal.h"
#include "Item.h"
#include "Treasure.h"
#include "Engine/World.h"
#include "Field/FieldSystemObjects.h"
#include "Field/FieldSystemComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapon.generated.h"

UCLASS()
class DIFFOREST_API AWeapon : public AItem
{
	GENERATED_BODY()

public:

	AWeapon();

	void TakeWeapon(USceneComponent* Comp, FName SocketName, AActor* NewOwner, APawn* Instigator);

	UFUNCTION()
	void OverlapBox(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	FORCEINLINE UBoxComponent* GetWeaponBox() const { return BoxComponent; };

	
	TArray<AActor*> IgnoreToActors;


	UFUNCTION(BlueprintImplementableEvent)
	void DestructWithSword(const FVector FieldLocation);

protected:
	virtual void BeginPlay() override;

	virtual void OverlapSphere(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;

	virtual void OverlapEndSphere(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) override;
	

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComponent;;

	UPROPERTY(EditAnywhere)
	USceneComponent* BoxTraceStart;

	UPROPERTY(EditAnywhere)
	USceneComponent* BoxTraceEnd;

	UPROPERTY(EditAnywhere, Category = "Breakable Properties")
	TArray<TSubclassOf<class ATreasure>> Treasure;
	
};
