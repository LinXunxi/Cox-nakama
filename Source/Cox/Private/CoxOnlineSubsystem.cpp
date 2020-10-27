// Fill out your copyright notice in the Description page of Project Settings.

#include "CoxOnlineSubsystem.h"

void UCoxOnlineSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	InitClient("47.100.47.82");
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
				this->OnAuthenticateErrorCallbackEvent.Broadcast(FString(UTF8_TO_TCHAR(error.message.c_str())));
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
				this->OnJoinChatErrorCallbackEvent.Broadcast(FString(UTF8_TO_TCHAR(error.message.c_str())));
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
			if (this->RtClientListener) {
				this->RtClientListener->OnSendChatMessageSuccess(ack);
			}
		},
		[this](NRtError error)
		{
			this->OnSendChatMessageErrorCallbackEvent.Broadcast(FString(UTF8_TO_TCHAR(error.message.c_str())));
		}
	);
}

void UCoxOnlineSubsystem::JoinMatchmaker(int32 minCount, int32 maxCount, const FString& query,const NStringMap& stringProperties, const NStringDoubleMap& numericProperties)
{
	if (RtClientPtr)
	{
		
		RtClientPtr->addMatchmaker(
			minCount, 
			maxCount, 
			TCHAR_TO_UTF8(*query), 
			stringProperties, 
			numericProperties, 
			[this](const NMatchmakerTicket& ticket)
			{
				this->OnMatchmakerTicketCallbackEvent.Broadcast(FString(UTF8_TO_TCHAR(ticket.ticket.c_str())));
			},
			[this](NRtError error)
			{
				this->OnJoinMatchmakerErrorCallbackEvent.Broadcast(FString(UTF8_TO_TCHAR(error.message.c_str())));
			}
		);
	}
}

void UCoxOnlineSubsystem::RemoveMatchmaker(const FString& ticket)
{
	if (RtClientPtr)
	{
		RtClientPtr->removeMatchmaker(
			TCHAR_TO_UTF8(*ticket),
			[this]()
			{
				this->OnRemoveMatchmakerCallbackEvent.Broadcast();
			},
			[this](NRtError error)
			{
				this->OnRemoveMatchmakerErrorCallbackEvent.Broadcast(FString(UTF8_TO_TCHAR(error.message.c_str())));
			}
		);
	
	}
}

void UCoxOnlineSubsystem::JoinMatchByToken(const FString& token)
{
	if (RtClientPtr)
	{
		RtClientPtr->joinMatchByToken(
			TCHAR_TO_UTF8(*token),
			[this](const NMatch& match)
			{
				if (this->RtClientListener) {
					this->RtClientListener->OnJoinMatchByTokenSuccess(match);
				}
			},
			[this](NRtError error)
			{
				this->OnJoinMatchByTokenErrorCallbackEvent.Broadcast(FString(UTF8_TO_TCHAR(error.message.c_str())));
			}
		);
	}
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
 