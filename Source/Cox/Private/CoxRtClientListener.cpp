// Fill out your copyright notice in the Description page of Project Settings.


#include "CoxRtClientListener.h"

void CoxRtClientListener::SetJoinChatSuccessCallback(JoinChatSuccessCallback callback)
{
	JoinChatSuccess = callback;
}

void CoxRtClientListener::OnJoinChatSuccess(const NChannelPtr& channel)
{
	if (JoinChatSuccess) {
		JoinChatSuccess(channel);
	}
	
}
