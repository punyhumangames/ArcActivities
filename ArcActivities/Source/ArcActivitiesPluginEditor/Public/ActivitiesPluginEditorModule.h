// Arc Activities Plugin.  Copyright Puny Human and Garrett Fleenor.  Licensed under MIT or Apache 2.0 license.  See LICENSE-MIT and LICENSE-APACHE for more info


#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "IAssetTypeActions.h"
#include "EdGraphUtilities.h"
#include "Toolkits/IToolkitHost.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "AssetTypeActions_Base.h"

struct FArcGraphNodeClassHelper;

class FActivitiesPluginEditorModule : public IModuleInterface,
	public IHasMenuExtensibility, public IHasToolBarExtensibility
{
public:

	// IModuleInterface implementation
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	TSharedPtr<FArcGraphNodeClassHelper> GetClassCache() { return ClassCache; }

	TSharedPtr<FGraphPanelNodeFactory> GraphPanelNodeFactory;

	virtual TSharedRef<FActivityEditor> CreateActivityEditor(const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, class UArcActivity* Object);

	virtual TSharedPtr<FExtensibilityManager> GetMenuExtensibilityManager() override { return MenuExtensibilityManager; }
	virtual TSharedPtr<FExtensibilityManager> GetToolBarExtensibilityManager() override { return ToolBarExtensibilityManager; }

private:

	virtual void RegisterAction(TSharedRef<IAssetTypeActions> Action); 	

	TArray< TSharedPtr<IAssetTypeActions> > AssetTypeActions;

	TSharedPtr<FArcGraphNodeClassHelper> ClassCache;

	TSharedPtr<FExtensibilityManager> MenuExtensibilityManager;
	TSharedPtr<FExtensibilityManager> ToolBarExtensibilityManager;
};
