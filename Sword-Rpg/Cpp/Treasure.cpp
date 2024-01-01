// Fill out your copyright notice in the Description page of Project Settings.


#include "Treasure.h"
#include "MyCharacter.h"


ATreasure::ATreasure()
{
	NiagaraCompTreasure = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraCompTreasure"));
	NiagaraCompTreasure->SetupAttachment(GetRootComponent());
}

void ATreasure::BeginPlay()
{
	Super::BeginPlay();
	this->Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OverlapSphere);
}

void ATreasure::OverlapSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMyCharacter* Character = Cast<AMyCharacter>(OtherActor);
	if(Character)
	{
		if (CoinSound)
			UGameplayStatics::PlaySoundAtLocation(this, CoinSound, SweepResult.ImpactPoint);
		NiagaraCompTreasure->Deactivate();
		Destroy();	
	}
}
