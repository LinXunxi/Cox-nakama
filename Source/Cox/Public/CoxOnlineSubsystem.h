// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoxRtClientListener.h"
#include "CoxTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CoxOnlineSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConnectCallbackEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRemoveMatchmakerCallbackEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAuthenticateErrorCallbackEvent, const FString&, message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJoinChatErrorCallbackEvent, const FString&, message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSendChatMessageErrorCallbackEvent, const FString&, message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMatchmakerTicketCallbackEvent, const FString&, ticket);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRemoveMatchmakerErrorCallbackEvent, const FString&, message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJoinMatchmakerErrorCallbackEvent, const FString&, message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJoinMatchByTokenErrorCallbackEvent, const FString&, message);

UCLASS()
class COX_API UCoxOnlineSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()
public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override { return !IsTemplate(); }
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UCoxOnlineSubsystem, STATGROUP_Tickables); }

	void AuthenticateEmail(const FString& email, const FString& password, const FString& account, bool bIsCreate);
	void InitClient(const FString& host = "127.0.0.1", int32 port = -1, const FString& key = "defaultkey");
	void JoinChat(const FString& channel, EChatChannelType chatChannelType, bool bIsPersistence = true, bool bIsHidden = false);
	void SendChatMessage(const FString& channel, const FString& message);
	void JoinMatchmaker(int32 minCount, int32 maxCount, const FString& query, const NStringMap& stringProperties, const NStringDoubleMap& numericProperties);
	void RemoveMatchmaker(const FString& ticket);
	void JoinMatchByToken(const FString& token);

	CoxRtClientListener* GetCoxRtClientListener();
public:
	FOnConnectCallbackEvent OnConnectCallbackEvent;
	FOnAuthenticateErrorCallbackEvent OnAuthenticateErrorCallbackEvent;
	FOnJoinChatErrorCallbackEvent OnJoinChatErrorCallbackEvent;
	FOnSendChatMessageErrorCallbackEvent OnSendChatMessageErrorCallbackEvent;
	FOnMatchmakerTicketCallbackEvent OnMatchmakerTicketCallbackEvent;
	FOnRemoveMatchmakerCallbackEvent OnRemoveMatchmakerCallbackEvent;
	FOnRemoveMatchmakerErrorCallbackEvent OnRemoveMatchmakerErrorCallbackEvent;
	FOnJoinMatchmakerErrorCallbackEvent OnJoinMatchmakerErrorCallbackEvent;
	FOnJoinMatchByTokenErrorCallbackEvent OnJoinMatchByTokenErrorCallbackEvent;
private:
	void InitRtClient(int32 port = -1, bool bIsInvisible = false);
	void InitRtClientListener();
private:
	NClientPtr ClientPtr;
	NRtClientPtr RtClientPtr;
	NSessionPtr SessionPtr;
	CoxRtClientListener* RtClientListener;
};
