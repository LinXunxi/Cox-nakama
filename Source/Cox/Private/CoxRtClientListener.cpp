// Fill out your copyright notice in the Description page of Project Settings.


#include "CoxRtClientListener.h"

void CoxRtClientListener::SetJoinChatSuccessCallback(JoinChatSuccessCallback callback)
{
	JoinChatSuccess = callback;
}

void CoxRtClientListener::SetSendChatMessageSuccessCallback(SendChatMessageSuccessCallback callback)
{
	SendChatMessageSuccess = callback;
}

void CoxRtClientListener::OnJoinChatSuccess(const NChannelPtr& channel)
{
	if (JoinChatSuccess)
	{
		JoinChatSuccess(channel);
	}
	
}

void CoxRtClientListener::OnSendChatMessageSuccess(const NChannelMessageAck& ack)
{
	if (SendChatMessageSuccess)
	{
		SendChatMessageSuccess(ack);
	}
}
