// Copyright 2023 Puny Human.  All Rights Reserved

namespace UnrealBuildTool.Rules
{
	public class ArcActivitiesPluginEditor : ModuleRules
	{
		public ArcActivitiesPluginEditor(ReadOnlyTargetRules Target) : base(Target)
		{
            PrivatePCHHeaderFile = "Public/ActivitiesPluginEditor.h";

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
                    "ApplicationCore",
                    "Engine",
					"Slate",
					"EditorStyle",
					"UnrealEd",
					"KismetWidgets",
					"GraphEditor",
                    "AIGraph",
                    "ToolMenus",
                });

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
                    "AssetTools",
                    "InputCore",
					"SlateCore",
					"PropertyEditor",
					"EditorStyle",
					"ContentBrowser",
					"ArcActivitiesPlugin",
                    "BlueprintGraph",
                    "ClassViewer",
                    "MessageLog",
                    "Kismet",					
					"KismetCompiler",
                });

			DynamicallyLoadedModuleNames.AddRange(
				new string[]
				{
                   "AssetRegistry",
                }
				);
		}
	}
}
