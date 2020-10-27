// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ImageWrapper/Public/IImageWrapper.h"
#include "ImageWrapper/Public/IImageWrapperModule.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CoxUtilitiesSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class COX_API UCoxUtilitiesSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override { return !IsTemplate(); }
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UCoxUtilitiesSubsystem, STATGROUP_Tickables); }
public:
	bool Base64EncodeData(const TArray<uint8>& data, FString& dest);
	bool Base64DecodeData(const FString& source, TArray<uint8>& dest);
	FString MD5Encode(const FString& Name);
	UTexture2D* LoadTexture2D(const TArray<uint8>& data);
	bool LoadFileToData(TArray<uint8>& data, const FString& dialog, const FString& path, const FString& types);
};
