#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "I_Interaction.generated.h"

UENUM()
enum class EInteractableType : uint8
{
	Pickup UMETA(DisplayName="Pickup"),
	NonPlayerCharacter UMETA(DisplayName = "NonPlayerCharacter"),
	Device UMETA(DisplayName = "Device"),
	Toggle UMETA(DisplayName = "Toggle"),
	Container UMETA(DisplayName = "Container")
};

USTRUCT()
struct FInteractableData
{
	GENERATED_USTRUCT_BODY();

	FInteractableData() :
	InteractableType(EInteractableType::Pickup),
	Name(FText::GetEmpty()),
	Action(FText::GetEmpty()),
	Quantity(0),
	InteractionDuration(0.0f)
	{

	};

	UPROPERTY(EditInstanceOnly)
	EInteractableType InteractableType;

	UPROPERTY(EditInstanceOnly)
	FText Name;

	UPROPERTY(EditInstanceOnly)
	FText Action;

	//used only pickup
	UPROPERTY(EditInstanceOnly)
	int8 Quantity;

	//used for things like open door or window etc. an interaction timer
	UPROPERTY(EditInstanceOnly)
	float InteractionDuration;
};


UINTERFACE(MinimalAPI)
class UI_Interaction : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPG_API II_Interaction
{
	GENERATED_BODY()

public:
	//====================================================================================
	//
	//								PROPS AND VARIABLES									
	//
	//====================================================================================
	
	FInteractableData InteractableData;
	
	



	//====================================================================================
	//
	//								FUNCTIONS								
	//
	//====================================================================================
	
	virtual void BeginFocus();

	virtual void EndFocus();
	
	virtual void BeginInteract();
	
	virtual void EndInteract();
	
	virtual void Interact();




};
