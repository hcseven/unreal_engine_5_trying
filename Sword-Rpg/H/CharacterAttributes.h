// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterAttributes.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DIFFOREST_API UCharacterAttributes : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCharacterAttributes();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	float Health;

	UPROPERTY(EditAnywhere)
	float MaxHealth;

	void ReceiveDamage(float Damage);

	float GetHealthPercent();

	bool IsAlive();

protected:
	virtual void BeginPlay() override;

		
};
