// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "MyHealthBarComponent.generated.h"

/**
 * 
 */
UCLASS()
class DIFFOREST_API UMyHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	void SetHealthPercent(float Percent);


	UPROPERTY();
	class UHealthBar* HealthBarWidget;
};
