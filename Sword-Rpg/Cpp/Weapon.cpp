// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "MyCharacter.h"
#include "MyInterface.h"



AWeapon::AWeapon()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(GetRootComponent());

	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceStart"));
	BoxTraceStart->SetupAttachment(GetRootComponent());

	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceEnd"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());

}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OverlapBox);
}

void AWeapon::TakeWeapon(USceneComponent* Comp, FName SocketName, AActor* NewOwner, APawn* MyInstigator)
{
	SetOwner(NewOwner);
	SetInstigator(MyInstigator);
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	StaticMesh->AttachToComponent(Comp, TransformRules, SocketName);
}


void AWeapon::OverlapSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OverlapSphere(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

}

void AWeapon::OverlapEndSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OverlapEndSphere(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::OverlapBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());
	FHitResult BoxHit;
	
	for (AActor* Actor : IgnoreToActors)
	{
		ActorsToIgnore.Add(Actor);
	}

	bool hit = UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End, 
		FVector(5.f, 5.f, 5.f), 
		BoxTraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,		
		ActorsToIgnore, 
		EDrawDebugTrace::ForDuration,
		BoxHit,
		true);		
	
	if (BoxHit.GetActor())
	{
		IMyInterface* MyInterface = Cast<IMyInterface>(BoxHit.GetActor());
		if (MyInterface)
		{
			MyInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint);
		}
		IgnoreToActors.AddUnique(BoxHit.GetActor());
		
		UGameplayStatics::ApplyDamage(
			BoxHit.GetActor(),
			20.f,
			GetInstigator()->GetController(),
			this,
			UDamageType::StaticClass()
		);
	}
	
}



