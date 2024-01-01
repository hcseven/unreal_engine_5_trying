// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"


UCLASS()
class DIFFOREST_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OverlapSphere(UPrimitiveComponent* OverlappedComponent, 
					   AActor* OtherActor, 
					   UPrimitiveComponent* OtherComp, 
					   int32 OtherBodyIndex, 
					   bool bFromSweep, 
					   const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OverlapEndSphere(UPrimitiveComponent* OverlappedComponent, 
						 AActor* OtherActor, 
						 UPrimitiveComponent* OtherComp, 
						 int32 OtherBodyIndex);


	UPROPERTY(EditAnywhere)
	USphereComponent* Sphere;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;

	
	

};
