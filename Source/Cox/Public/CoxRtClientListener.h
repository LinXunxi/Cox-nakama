// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "nakama-cpp/Nakama.h"

using namespace NAKAMA_NAMESPACE;

class COX_API CoxRtClientListener : public NRtDefaultClientListener
{
public:
	using JoinChatSuccessCallback = std::function<void(const NChannelPtr& channel)>;
	void SetJoinChatSuccessCallback(JoinChatSuccessCallback callback);
	void OnJoinChatSuccess(const NChannelPtr& channel);
private:
	JoinChatSuccessCallback JoinChatSuccess;
};
