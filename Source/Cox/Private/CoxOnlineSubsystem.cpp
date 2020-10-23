// Fill out your copyright notice in the Description page of Project Settings.


#include "CoxOnlineSubsystem.h"

void UCoxOnlineSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	InitClient();
}

void UCoxOnlineSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UCoxOnlineSubsystem::Tick(float DeltaTime)
{
	if (ClientPtr)
	{
		ClientPtr->tick();
	}
	if (RtClientPtr)
	{
		RtClientPtr->tick();
	}
}

void UCoxOnlineSubsystem::AuthenticateEmail(const FString& email, const FString& password, const FString& account, bool bIsCreate)
{
	if (ClientPtr)
	{
		ClientPtr->authenticateEmail(TCHAR_TO_UTF8(*email), TCHAR_TO_UTF8(*password), TCHAR_TO_UTF8(*account), bIsCreate, {},
			[this](NSessionPtr session)
			{
				this->SessionPtr = session;
				this->InitRtClient();
			}, 
			[this](const NError& error)
			{
				this->OnAuthenticateErrorCallbackEvent.Broadcast(FString(error.message.c_str()));
			});
	}
}

void UCoxOnlineSubsystem::InitClient(const FString& host /*= "127.0.0.1"*/, int32 port /*= -1*/, const FString& key /*= "defaultkey"*/)
{
	NClientParameters parameters;
	parameters.serverKey = TCHAR_TO_UTF8(*key);
	parameters.host = TCHAR_TO_UTF8(*host);
	parameters.port = port;
	if (!ClientPtr)
	{
		ClientPtr = createDefaultClient(parameters);
	}
}



void UCoxOnlineSubsystem::JoinChat(const FString& channel, EChatChannelType chatChannelType, bool bIsPersistence /*= true*/, bool bIsHidden /*= false*/)
{
	if (RtClientPtr)
	{
		RtClientPtr->joinChat(
			TCHAR_TO_UTF8(*channel),
			(NChannelType)chatChannelType,
			bIsPersistence,
			bIsHidden,
			[this](NChannelPtr channel)
			{
				if (this->RtClientListener) {
					this->RtClientListener->OnJoinChatSuccess(channel);
				}
					
			},
			[this](NRtError error)
			{
				this->OnJoinChatErrorCallbackEvent.Broadcast(FString(error.message.c_str()));
			}
		);
	}
	
}

void UCoxOnlineSubsystem::SendChatMessage(const FString& channel, const FString& message)
{
	RtClientPtr->writeChatMessage(
		TCHAR_TO_UTF8(*channel),
		TCHAR_TO_UTF8(*message),
		[this](const NChannelMessageAck& ack)
		{
			//UE_LOG(LogClass, Warning, TEXT("SendChatMessage: channelId = %s"), *FString(ack.channelId.c_str()));
		},
		[this](NRtError error)
		{
			this->OnSendChatMessageErrorCallbackEvent.Broadcast(FString(error.message.c_str()));
		}
	);
}

CoxRtClientListener* UCoxOnlineSubsystem::GetCoxRtClientListener()
{
	return RtClientListener;
}

void UCoxOnlineSubsystem::InitRtClient(int32 port /*= -1*/, bool bIsInvisible /*= false*/)
{
	if (!ClientPtr)
		return;
	if (!RtClientListener)
		InitRtClientListener();
	if (!RtClientPtr)
	{
		RtClientPtr = ClientPtr->createRtClient(port);
		RtClientPtr->setListener(RtClientListener);
		RtClientPtr->connect(SessionPtr, bIsInvisible);
	}
}

void UCoxOnlineSubsystem::InitRtClientListener()
{
	RtClientListener = new CoxRtClientListener();
	RtClientListener->setConnectCallback([this]()
		{
			this->OnConnectCallbackEvent.Broadcast();
		});
}
