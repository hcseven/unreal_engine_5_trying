// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakable.h"

ABreakable::ABreakable()
{
	PrimaryActorTick.bCanEverTick = false;

	GeometryComp = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryComp"));
	SetRootComponent(GeometryComp);
	GeometryComp->bUseSizeSpecificDamageThreshold = true;
	GeometryComp->SetGenerateOverlapEvents(true);
	GeometryComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GeometryComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	CapsuleComp->SetupAttachment(GetRootComponent());
	CapsuleComp->SetGenerateOverlapEvents(true);
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	CapsuleComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	CapsuleCompBlock = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCompBlock"));
	CapsuleCompBlock->SetupAttachment(GetRootComponent());
	CapsuleCompBlock->SetGenerateOverlapEvents(true);
	CapsuleCompBlock->SetCollisionResponseToAllChannels(ECR_Block);
}

void ABreakable::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABreakable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakable::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (BreakSound)
		UGameplayStatics::PlaySoundAtLocation(this, BreakSound, ImpactPoint);
	if (Treasure.Num() > 0)
	{
		FVector Location = GetActorLocation();
		Location.Z += 77.f;
		int32 Num = FMath::RandRange(0, Treasure.Num() - 1);
		GetWorld()->SpawnActor<ATreasure>(Treasure[Num], Location, GetActorRotation());
		
	}
	CapsuleComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CapsuleCompBlock->SetCollisionResponseToAllChannels(ECR_Ignore);
	SetLifeSpan(5.0f);
}


