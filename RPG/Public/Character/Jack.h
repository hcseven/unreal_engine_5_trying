#pragma once

#include "I_Interaction.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Jack.generated.h"


USTRUCT()
struct FInteractionData
{
	GENERATED_USTRUCT_BODY();

	FInteractionData() : CurrentInteractionActor(nullptr), LastInteractionCheckTime(0.0f) {};

	UPROPERTY()
	AActor* CurrentInteractionActor;

	UPROPERTY()
	float LastInteractionCheckTime;
};
/**
 * 
 */
UCLASS()
class AJack : public ABaseCharacter
{
	GENERATED_BODY()
	
public:

	//====================================================================================
	//
	//								PROPS AND VARIABLES									
	//
	//====================================================================================

	UCameraComponent* MyCamera;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* MySpringArm;





	//====================================================================================
	//
	//								FUNCTIONS								
	//
	//====================================================================================

	AJack();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Scale);
	void MoveRight(float Scale);
	void StartStopCrouch();

	FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(InteractionTimerHandle); };



protected:

	//====================================================================================
	//
	//								PROPS AND VARIABLES									
	//
	//====================================================================================

	UPROPERTY(VisibleAnywhere, Category="Character | Interaction")
	TScriptInterface<II_Interaction> TargetInteractable;


	float InteractionCheckFrequences;

	float InteractionCheckDistance;

	FTimerHandle InteractionTimerHandle;

	FInteractionData InteractionData;

	//====================================================================================
	//
	//								FUNCTIONS								
	//
	//====================================================================================
	virtual void BeginPlay() override;

	void PerformInteractionCheck();
	
	void FoundInteractable(AActor* NewInteractable);

	void NoInteractableFound();

	void BeginInteract();

	void EndInteract();

	void Interact();




};
