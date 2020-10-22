// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CoxTextUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class COX_API UCoxTextUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(Meta = (BindWidget))
		class UTextBlock* TextBlock_UserName;
	UPROPERTY(Meta = (BindWidget))
		class UTextBlock* TextBlock_Message;
};
