// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Cox : ModuleRules
{
	public Cox(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

	
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"UMG",
                "Json",
                "JsonUtilities"
            }
			);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"Nakama"
			}
			);
	}
}
