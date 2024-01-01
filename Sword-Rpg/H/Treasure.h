// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Item.h"
#include "Treasure.generated.h"

/**
 * 
 */
UCLASS()
class DIFFOREST_API ATreasure : public AItem
{
	GENERATED_BODY()


public:
	ATreasure();

	UPROPERTY(EditAnywhere)
	USoundBase* CoinSound;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* NiagaraCompTreasure;


protected:
	virtual void BeginPlay() override;

private:		

	virtual void OverlapSphere(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;
	
	
};
