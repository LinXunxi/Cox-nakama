// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoxTypes.generated.h"


UENUM(BlueprintType)
enum class EChatChannelType : uint8
{
	ChannelType_ROOM = 0,			  ///< A chat room which can be created dynamically with a name.
	ChannelType_DIRECT_MESSAGE = 1,   ///< A private chat between two users.
	ChannelType_GROUP = 2			  ///< A chat within a group on the server.
};

