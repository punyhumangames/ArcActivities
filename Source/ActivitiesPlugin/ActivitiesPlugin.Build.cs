// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
	public class ActivitiesPlugin : ModuleRules
	{
		public ActivitiesPlugin(ReadOnlyTargetRules Target) : base(Target)
		{
            PrivatePCHHeaderFile = "Public/ActivitiesPlugin.h";


            PublicIncludePaths.AddRange(
				new string[] {
					// ... add public include paths required here ...
				}
				);

			PrivateIncludePaths.AddRange(
				new string[] {
					// ... add other private include paths required here ...
				}
				);

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
                    "CoreUObject",
                    "Engine", "InputCore",
                    "UMG",
                }
				);

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
                    "Slate",
                    "SlateCore",
                }
				);

			DynamicallyLoadedModuleNames.AddRange(
				new string[]
				{
					// ... add any modules that your module loads dynamically here ...
				}
				);
		}
	}
}
