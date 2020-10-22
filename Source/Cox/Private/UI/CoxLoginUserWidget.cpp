// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CoxLoginUserWidget.h"
#include "UI/CoxTextUserWidget.h"
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

	UGameInstance* GameInstance = UWidget::GetGameInstance();
	CoxOnlineSubsystem = GameInstance->GetSubsystem<UCoxOnlineSubsystem>();

	if (CoxOnlineSubsystem)
	{
		CoxOnlineSubsystem->OnConnectCallbackEvent.AddDynamic(this, &UCoxLoginUserWidget::OnConnectCallbackEvent);
		CoxOnlineSubsystem->OnAuthenticateErrorCallbackEvent.AddDynamic(this, &UCoxLoginUserWidget::OnAuthenticateErrorCallbackEvent);
		CoxOnlineSubsystem->OnJoinChatErrorCallbackEvent.AddDynamic(this, &UCoxLoginUserWidget::OnJoinChatErrorCallbackEvent);
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

		//FString Data = "{\"text\":\" "+ Message + "\"}";

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
		}
	}

	if (VerticalBox_LoginBox)
	{
		VerticalBox_LoginBox->RemoveFromParent();

		if (HorizontalBox_JoinChat)
		{
			HorizontalBox_JoinChat->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
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

void UCoxLoginUserWidget::OnChannelMessageCallbackEvent(const NChannelMessage& message)
{
	//接收到消息的回调函数
	//UE_LOG(LogClass,Warning, TEXT("OnChannelMessageCallbackEvent: %s"),*FString(message.content.c_str()));
	if (CoxTextUserWidgetClass)
	{
		UCoxTextUserWidget* CoxTextUserWidget = CreateWidget<UCoxTextUserWidget>(this, CoxTextUserWidgetClass);
		if (CoxTextUserWidget)
		{
			CoxTextUserWidget->TextBlock_UserName->SetText(FText::FromString(FString(message.senderId.c_str())));

			FString message_text;

			FString JsonValue = FString(message.content.c_str());

			TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);
			TSharedPtr<FJsonObject> Root;

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
		HorizontalBox_JoinChat->RemoveFromParent();
		VerticalBox_Chat->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ChannelId = FString(channel->id.c_str());
	}
}
