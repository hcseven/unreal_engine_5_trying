// Fill out your copyright notice in the Description page of Project Settings.

#include "MyHealthBarComponent.h"
#include "HealthBar.h"
#include "Components/ProgressBar.h"


void UMyHealthBarComponent::SetHealthPercent(float Percent)
{
	if (HealthBarWidget == nullptr)
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	if (HealthBarWidget && HealthBarWidget->HealthBar)
		HealthBarWidget->HealthBar->SetPercent(Percent);
}
