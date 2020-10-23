// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CoxAccountUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class COX_API UCoxAccountUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(Meta = (BindWidget))
		class UImage* Image_Photo;
	UPROPERTY(Meta = (BindWidget))
		class UTextBlock* TextBlock_Name;
};
