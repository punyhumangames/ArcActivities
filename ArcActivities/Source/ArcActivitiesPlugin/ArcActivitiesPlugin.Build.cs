// Arc Activities Plugin.  Copyright Puny Human and Garrett Fleenor.  Licensed under MIT or Apache 2.0 license.  See LICENSE-MIT and LICENSE-APACHE for more info

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
                    "Engine", 
					"InputCore",
                    "UMG",
					"GameplayTags",
					"NetCore",
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
