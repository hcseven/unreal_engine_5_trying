// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAttributes.h"

// Sets default values for this component's properties
UCharacterAttributes::UCharacterAttributes()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCharacterAttributes::ReceiveDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0, MaxHealth);
}

float UCharacterAttributes::GetHealthPercent()
{
	return Health / MaxHealth;
}

bool UCharacterAttributes::IsAlive()
{
	if (Health <= 20)
		return false;
	return true;
}

// Called when the game starts
void UCharacterAttributes::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCharacterAttributes::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

