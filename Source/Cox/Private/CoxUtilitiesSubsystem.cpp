// Fill out your copyright notice in the Description page of Project Settings.

#include "CoxUtilitiesSubsystem.h"
#include "Misc/Base64.h"
#include "Developer/DesktopPlatform/Public/IDesktopPlatform.h"
#include "Developer/DesktopPlatform/Public/DesktopPlatformModule.h"



void UCoxUtilitiesSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UCoxUtilitiesSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UCoxUtilitiesSubsystem::Tick(float DeltaTime)
{

}

bool UCoxUtilitiesSubsystem::Base64EncodeData(const TArray<uint8>& data, FString& dest)
{
	if (data.Num() > 0)
	{
		dest = FBase64::Encode(data);
		return true;
	}

	dest = FString();
	return false;
}

bool UCoxUtilitiesSubsystem::Base64DecodeData(const FString& source, TArray<uint8>& dest)
{
	return FBase64::Decode(source, dest);
}

FString UCoxUtilitiesSubsystem::MD5Encode(const FString& Name)
{
	return FMD5::HashAnsiString(*Name);
}

UTexture2D* UCoxUtilitiesSubsystem::LoadTexture2D(const TArray<uint8>& data)
{
	UTexture2D* LoadedT2D = nullptr;
	
	IImageWrapperModule& ImageWrapperModule = FModuleManager::Get().LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	EImageFormat Format = ImageWrapperModule.DetectImageFormat(data.GetData(), data.Num());
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(Format);

	if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(data.GetData(), data.Num()))
	{
		TArray<uint8> UncompressedBGRA;
		if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))
		{
			LoadedT2D = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);
			if (!LoadedT2D)
				return nullptr;

			void* TextureData = LoadedT2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
			FMemory::Memcpy(TextureData, UncompressedBGRA.GetData(), UncompressedBGRA.Num());
			LoadedT2D->PlatformData->Mips[0].BulkData.Unlock();
			LoadedT2D->UpdateResource();
		}
	}
	return LoadedT2D;
}

bool UCoxUtilitiesSubsystem::LoadFileToData(TArray<uint8>& data, const FString& dialog, const FString& path, const FString& types)
{
	TArray<FString> OutFileNames;
	if (GEngine)
	{
		if (GEngine->GameViewport)
		{
			void* ParentWindowHandle = GEngine->GameViewport->GetWindow()->GetNativeWindow()->GetOSWindowHandle();
			IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
			if (DesktopPlatform)
			{
				uint32 SelectionFlag = 0;
				//TEXT("Image files(*.jpg; *.png; *.bmp; *.jpeg) | *.jpg; *.png; *.bmp; *.jpeg")
				if (DesktopPlatform->OpenFileDialog(ParentWindowHandle, dialog, path, FString(""), types, SelectionFlag, OutFileNames))
				{
					if (FFileHelper::LoadFileToArray(data, *OutFileNames[0]))
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}
