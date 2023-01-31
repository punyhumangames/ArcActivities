// Copyright 2023 Puny Human.  All Rights Reserved

namespace UnrealBuildTool.Rules
{
	public class ArcActivitiesPlugin : ModuleRules
	{
		public ArcActivitiesPlugin(ReadOnlyTargetRules Target) : base(Target)
		{
            PrivatePCHHeaderFile = "Public/ArcActivitiesPlugin.h";


			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
                    "CoreUObject",
                    "Engine", "InputCore",
                    "UMG",
					"GameplayTags",
                });

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
                    "Slate",
                    "SlateCore",
                });
		}
	}
}
