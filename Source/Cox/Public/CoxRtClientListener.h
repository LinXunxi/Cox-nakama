// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "nakama-cpp/Nakama.h"

using namespace NAKAMA_NAMESPACE;

class COX_API CoxRtClientListener : public NRtDefaultClientListener
{
public:
	using JoinChatSuccessCallback = std::function<void(const NChannelPtr& channel)>;
	using SendChatMessageSuccessCallback = std::function<void(const NChannelMessageAck& ack)>;

	void SetJoinChatSuccessCallback(JoinChatSuccessCallback callback);
	void SetSendChatMessageSuccessCallback(SendChatMessageSuccessCallback callback);


	void OnJoinChatSuccess(const NChannelPtr& channel);
	void OnSendChatMessageSuccess(const NChannelMessageAck& ack);
private:
	JoinChatSuccessCallback JoinChatSuccess;
	SendChatMessageSuccessCallback SendChatMessageSuccess;
};
