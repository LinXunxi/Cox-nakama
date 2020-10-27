// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CoxOnlineSubsystem.h"
#include "CoxUtilitiesSubsystem.h"
#include "CoxLoginUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class COX_API UCoxLoginUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;



	UFUNCTION()
		void OnButtonLoginPressedEvent();
	UFUNCTION()
		void OnButtonMatcherPressedEvent();
	UFUNCTION()
		void OnButtonJoinChatPressedEvent();
	UFUNCTION()
		void OnButtonSendChatPressedEvent();
	UFUNCTION()
		void OnConnectCallbackEvent();
	UFUNCTION()
		void OnAuthenticateErrorCallbackEvent(const FString& message);
	UFUNCTION()
		void OnJoinChatErrorCallbackEvent(const FString& message);
	UFUNCTION()
		void OnMatchmakerTicketCallbackEvent(const FString& ticket);
	UFUNCTION()
		void OnRemoveMatchmakerCallbackEvent();



	void OnChannelMessageCallbackEvent(const NChannelMessage& message);
	void OnJoinChatSuccessCallbackEvent(const NChannelPtr& channel);
	void OnSendChatMessageSuccessCallbackEvent(const NChannelMessageAck& ack);
	void OnChannelPresenceCallbackEvent(const NChannelPresenceEvent& presence);
	void OnMatchmakerMatchedCallbackEvent(const NMatchmakerMatchedPtr& matched);
public:
	/*UMG Start*/
	UPROPERTY(Meta = (BindWidget))
		class UButton* Button_Login;
	UPROPERTY(Meta = (BindWidget))
		class UButton* Button_Quit;
	UPROPERTY(Meta = (BindWidget))
		class UEditableTextBox* EditableTextBox_Email;
	UPROPERTY(Meta = (BindWidget))
		class UEditableTextBox* EditableTextBox_Password;
	UPROPERTY(Meta = (BindWidget))
		class UTextBlock* TextBlock_INFO;
	UPROPERTY(Meta = (BindWidget))
		class UVerticalBox* VerticalBox_LoginBox;

	UPROPERTY(Meta = (BindWidget))
		class UHorizontalBox* HorizontalBox_JoinChat;
	UPROPERTY(Meta = (BindWidget))
		class UButton* Button_JoinChat;
	UPROPERTY(Meta = (BindWidget))
		class UEditableTextBox* EditableTextBox_ChatName;


	UPROPERTY(Meta = (BindWidget))
		class UVerticalBox* VerticalBox_Chat;
	UPROPERTY(Meta = (BindWidget))
		class UButton* Button_Send;
	UPROPERTY(Meta = (BindWidget))
		class UEditableTextBox* EditableTextBox_TEXT;
	UPROPERTY(Meta = (BindWidget))
		class UScrollBox* ScrollBox_Chat;

	UPROPERTY(Meta = (BindWidget))
		class UScrollBox* ScrollBox_Presences;

	UPROPERTY(Meta = (BindWidget))
		class UButton* Button_Matcher;
	UPROPERTY(Meta = (BindWidget))
		class UHorizontalBox* HorizontalBox_Top;
	UPROPERTY(Meta = (BindWidget))
		class UTextBlock* TextBlock_Matcher;

	/*UMG End*/

	UPROPERTY()
		UCoxOnlineSubsystem* CoxOnlineSubsystem;
	UPROPERTY()
		UCoxUtilitiesSubsystem* CoxUtilitiesSubsystem;

	UPROPERTY(EditDefaultsOnly, Category = NAKAMA)
		TSubclassOf<class UCoxTextUserWidget> CoxTextUserWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = NAKAMA)
		TSubclassOf<class UCoxAccountUserWidget> CoxAccountUserWidgetClass;
private:
	TMap<FString, class UCoxAccountUserWidget*> UsersMap;
	FString ChannelId;
	bool bIsMatching;
	FString Ticket;
	CoxRtClientListener* RtClientListener;
	void AddUser(const NUserPresence& presence);
};
