// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Chaos/ChaosGameplayEventDispatcher.h"
#include "GameFramework/Actor.h"
#include "Treasure.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyInterface.h"
#include "Breakable.generated.h"

class UGeometryCollectionComponent;


UCLASS()
class DIFFOREST_API ABreakable : public AActor , public IMyInterface
{
	GENERATED_BODY()
	
public:	
	ABreakable();

	virtual void Tick(float DeltaTime) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

	UPROPERTY(EditAnywhere)
	USoundBase* BreakSound;

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CapsuleCompBlock;

	void OnBreak(const FChaosBreakEvent& BreakEvent);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UGeometryCollectionComponent* GeometryComp;
	
	UPROPERTY(EditAnywhere, Category = "Breakable Properties")
	TArray<TSubclassOf<class ATreasure>> Treasure;
	

};
