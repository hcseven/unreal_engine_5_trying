#pragma once

#include "Character/Jack.h"
#include "ItemDataStructs.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemBase.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UItemBase : public UObject
{
	GENERATED_BODY()

public:

	//====================================================================================
	//
	//								PROPS AND VARIABLES									
	//
	//====================================================================================

	//UPROPERTY()
	//UInventoryComponent* OwningInventory;

	UPROPERTY(EditAnywhere, Category = "Item Data", meta=(UIMin=1, UIMax=100))
	int32 Quantity;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FName ID;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemQuality ItemQuality;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemStatistics ItemStatistics;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemTextData ItemTextData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemNumericData ItemNumericData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemAssetData ItemAssetData;




	//====================================================================================
	//
	//								FUNCTIONS								
	//
	//====================================================================================

	UItemBase();

	UFUNCTION(Category = "Item")
	UItemBase* CreatedItemCopy();

	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemStackWeight() const { return Quantity * ItemNumericData.Weight; };

	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemSingleWeight() const { return ItemNumericData.Weight; };

	UFUNCTION(Category = "Item")
	FORCEINLINE bool IsFullItemStack() const { return Quantity == ItemNumericData.MaxStackSize; };

	UFUNCTION(Category = "Item")
	void SetQuantity(const int32 NewQuantity);

	UFUNCTION(Category = "Item")
	virtual void Use(AJack* Jack);


protected :
	bool operator== (const FName& OtherID) const
	{
		return ID == OtherID;
	}

};
