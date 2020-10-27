// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CoxLoginUserWidget.h"
#include "UI/CoxTextUserWidget.h"
#include "UI/CoxAccountUserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/ScrollBox.h"
#include "Json.h"


void UCoxLoginUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bIsMatching = false;
	UGameInstance* GameInstance = UWidget::GetGameInstance();
	CoxOnlineSubsystem = GameInstance->GetSubsystem<UCoxOnlineSubsystem>();
	CoxUtilitiesSubsystem = GameInstance->GetSubsystem<UCoxUtilitiesSubsystem>();

	if (CoxOnlineSubsystem)
	{
		CoxOnlineSubsystem->OnConnectCallbackEvent.AddDynamic(this, &UCoxLoginUserWidget::OnConnectCallbackEvent);
		CoxOnlineSubsystem->OnAuthenticateErrorCallbackEvent.AddDynamic(this, &UCoxLoginUserWidget::OnAuthenticateErrorCallbackEvent);
		CoxOnlineSubsystem->OnJoinChatErrorCallbackEvent.AddDynamic(this, &UCoxLoginUserWidget::OnJoinChatErrorCallbackEvent);
		CoxOnlineSubsystem->OnMatchmakerTicketCallbackEvent.AddDynamic(this, &UCoxLoginUserWidget::OnMatchmakerTicketCallbackEvent);
		CoxOnlineSubsystem->OnRemoveMatchmakerCallbackEvent.AddDynamic(this, &UCoxLoginUserWidget::OnRemoveMatchmakerCallbackEvent);
	}

	if (Button_Login)
	{
		Button_Login->OnPressed.AddDynamic(this, &UCoxLoginUserWidget::OnButtonLoginPressedEvent);
	}

	if (Button_JoinChat)
	{
		Button_JoinChat->OnPressed.AddDynamic(this, &UCoxLoginUserWidget::OnButtonJoinChatPressedEvent);
	}

	if (Button_Send)
	{
		Button_Send->OnPressed.AddDynamic(this, &UCoxLoginUserWidget::OnButtonSendChatPressedEvent);
	}

	if (Button_Matcher)
	{
		Button_Matcher->OnPressed.AddDynamic(this, &UCoxLoginUserWidget::OnButtonMatcherPressedEvent);
	}
}

void UCoxLoginUserWidget::OnButtonLoginPressedEvent()
{
	if (EditableTextBox_Email && EditableTextBox_Password)
	{
		FString email =  EditableTextBox_Email->GetText().ToString();
		FString password = EditableTextBox_Password->GetText().ToString();

		CoxOnlineSubsystem->AuthenticateEmail(email, password, "", true);
	}
}

void UCoxLoginUserWidget::OnButtonMatcherPressedEvent()
{
	if (Button_Matcher && CoxOnlineSubsystem && TextBlock_Matcher)
	{
		if (!bIsMatching)
		{
			CoxOnlineSubsystem->JoinMatchmaker(2, 2, "*", {}, {});
			Button_Matcher->SetIsEnabled(false);
		}
		else {
			CoxOnlineSubsystem->RemoveMatchmaker(Ticket);
			Button_Matcher->SetIsEnabled(false);
		}
	}
}

void UCoxLoginUserWidget::OnButtonJoinChatPressedEvent()
{
	if (EditableTextBox_ChatName && CoxOnlineSubsystem)
	{
		FString ChatName = EditableTextBox_ChatName->GetText().ToString();
		CoxOnlineSubsystem->JoinChat(ChatName, EChatChannelType::ChannelType_ROOM);
	}
}

void UCoxLoginUserWidget::OnButtonSendChatPressedEvent()
{
	if (EditableTextBox_TEXT && CoxOnlineSubsystem)
	{
		FString Message = EditableTextBox_TEXT->GetText().ToString();

		FString text;

		TSharedPtr<FJsonObject> text_object = MakeShareable(new FJsonObject);
		text_object->SetStringField("text", Message);
		
		TSharedRef<TJsonWriter<TCHAR>> Writer = TJsonWriterFactory<>::Create(&text);
		FJsonSerializer::Serialize(text_object.ToSharedRef(), Writer);
		Writer->Close();

		CoxOnlineSubsystem->SendChatMessage(ChannelId, text);
	}
}

void UCoxLoginUserWidget::OnConnectCallbackEvent()
{
	if (CoxOnlineSubsystem)
	{
		RtClientListener = CoxOnlineSubsystem->GetCoxRtClientListener();

		if (RtClientListener)
		{
			RtClientListener->setChannelMessageCallback([this](const NChannelMessage& message)
				{
					this->OnChannelMessageCallbackEvent(message);
				});
			RtClientListener->SetJoinChatSuccessCallback([this](const NChannelPtr& channel)
				{
					this->OnJoinChatSuccessCallbackEvent(channel);
				});
			RtClientListener->SetSendChatMessageSuccessCallback([this](const NChannelMessageAck& ack)
				{
					this->OnSendChatMessageSuccessCallbackEvent(ack);
				});
			RtClientListener->setChannelPresenceCallback([this](const NChannelPresenceEvent& presence)
				{
					this->OnChannelPresenceCallbackEvent(presence);
				});
			RtClientListener->setMatchmakerMatchedCallback([this](const NMatchmakerMatchedPtr& matched)
				{
					this->OnMatchmakerMatchedCallbackEvent(matched);
				});
			
		}
	}

	if (VerticalBox_LoginBox)
	{
		VerticalBox_LoginBox->RemoveFromParent();

		if (HorizontalBox_Top)
		{
			HorizontalBox_Top->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
}

void UCoxLoginUserWidget::OnAuthenticateErrorCallbackEvent(const FString& message)
{
	if (TextBlock_INFO)
	{
		TextBlock_INFO->SetText(FText::FromString(message));
	}
}

void UCoxLoginUserWidget::OnJoinChatErrorCallbackEvent(const FString& message)
{
	if (TextBlock_INFO)
	{
		TextBlock_INFO->SetText(FText::FromString(message));
	}
}

void UCoxLoginUserWidget::OnMatchmakerTicketCallbackEvent(const FString& ticket)
{
	if (Button_Matcher && TextBlock_Matcher)
	{
		bIsMatching = true;
		TextBlock_Matcher->SetText(FText::FromString("Matching..."));
		Button_Matcher->SetIsEnabled(true);
		Ticket = ticket;
	}
}

void UCoxLoginUserWidget::OnRemoveMatchmakerCallbackEvent()
{
	if (Button_Matcher && TextBlock_Matcher)
	{
		TextBlock_Matcher->SetText(FText::FromString("Matchmaker"));
		Button_Matcher->SetIsEnabled(true);
		bIsMatching = false;
	}
}

void UCoxLoginUserWidget::OnChannelMessageCallbackEvent(const NChannelMessage& message)
{
	if (CoxTextUserWidgetClass)
	{
		UCoxTextUserWidget* CoxTextUserWidget = CreateWidget<UCoxTextUserWidget>(this, CoxTextUserWidgetClass);
		if (CoxTextUserWidget)
		{
			CoxTextUserWidget->TextBlock_UserName->SetText(FText::FromString(FString(UTF8_TO_TCHAR(message.username.c_str()))));

			FString JsonValue = FString(UTF8_TO_TCHAR(message.content.c_str()));
			TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);
			TSharedPtr<FJsonObject> Root;

			FString message_text;
			if (FJsonSerializer::Deserialize(JsonReader, Root))
			{
				if (Root->HasField(TEXT("text")))
				{
					message_text = Root->GetStringField(TEXT("text"));
				}
			}
			CoxTextUserWidget->TextBlock_Message->SetText(FText::FromString(message_text));

			if (ScrollBox_Chat)
			{
				ScrollBox_Chat->AddChild(CoxTextUserWidget);
			}
		}
	}

}

void UCoxLoginUserWidget::OnJoinChatSuccessCallbackEvent(const NChannelPtr& channel)
{
	if (HorizontalBox_JoinChat && VerticalBox_Chat)
	{
		VerticalBox_Chat->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ChannelId = FString(channel->id.c_str());

		for (auto user : channel->presences)
		{
			AddUser(user);
		}
	}
}

void UCoxLoginUserWidget::OnSendChatMessageSuccessCallbackEvent(const NChannelMessageAck& ack)
{
	if (EditableTextBox_TEXT)
	{
		EditableTextBox_TEXT->SetText(FText());
	}
}

void UCoxLoginUserWidget::OnChannelPresenceCallbackEvent(const NChannelPresenceEvent& presence)
{
	if (ScrollBox_Presences)
	{
		for (auto user : presence.leaves)
		{
			FString name = FString(UTF8_TO_TCHAR(user.username.c_str()));
			if (UsersMap.Contains(name))
			{
				UsersMap[name]->RemoveFromParent();
				UsersMap.Remove(name);
			}
		}
		for (auto user : presence.joins)
		{
			AddUser(user);
		}
	}
}

void UCoxLoginUserWidget::OnMatchmakerMatchedCallbackEvent(const NMatchmakerMatchedPtr& matched)
{
	if (CoxUtilitiesSubsystem && CoxOnlineSubsystem && HorizontalBox_Top)
	{
		FString MatchID = FString(UTF8_TO_TCHAR(matched->matchId.c_str()));
		CoxOnlineSubsystem->JoinChat(CoxUtilitiesSubsystem->MD5Encode(MatchID), EChatChannelType::ChannelType_ROOM);
		HorizontalBox_Top->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCoxLoginUserWidget::AddUser(const NUserPresence& presence)
{	
	if (CoxAccountUserWidgetClass)
	{
		FString name = FString(UTF8_TO_TCHAR(presence.username.c_str()));
		UCoxAccountUserWidget* CoxAccountUserWidget = CreateWidget<UCoxAccountUserWidget>(this, CoxAccountUserWidgetClass);
		if (CoxAccountUserWidget)
		{
			CoxAccountUserWidget->TextBlock_Name->SetText(FText::FromString(name));
			UsersMap.Add(name, CoxAccountUserWidget);
			if (ScrollBox_Presences)
			{
				ScrollBox_Presences->AddChild(CoxAccountUserWidget);
			}
		}
	}
}
