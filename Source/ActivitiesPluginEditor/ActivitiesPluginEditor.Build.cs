// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
	public class ActivitiesPluginEditor : ModuleRules
	{
		public ActivitiesPluginEditor(ReadOnlyTargetRules Target) : base(Target)
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
					"ActivitiesPlugin",
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
