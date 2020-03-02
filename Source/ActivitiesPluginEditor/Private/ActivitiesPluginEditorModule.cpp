// 2017-2019 Puny Human

#include "ActivitiesPluginEditorModule.h"
#include "Modules/ModuleManager.h"
#include "IAssetTypeActions.h"
#include "EdGraphUtilities.h"
#include "IAssetTools.h"

#include "SGraphNode.h"
#include "EdGraphUtilities.h"
#include "Graph/Nodes/ActivityNode_Base.h"
#include "Widgets/SGraphNode_ActivityStage.h"

#include "DataModel/ActivityTask_Base.h"
#include "DataModel/ActivityTask_StageService.h"
#include "DataModel/ObjectiveTracker.h"

#include "ActivityEditor.h"



IMPLEMENT_MODULE(FActivitiesPluginEditorModule, ActivitiesPluginEditor)

class FGraphPanelNodeFactory_ActivityNodes : public FGraphPanelNodeFactory
{
	virtual TSharedPtr<class SGraphNode> CreateNode(UEdGraphNode* Node) const override
	{
		if (UActivityNode_Base* ActivityNode = Cast<UActivityNode_Base>(Node))
		{
			return SNew(SGraphNode_Activity, ActivityNode);
		}  					  	

		return nullptr;
	}
};

TSharedPtr<FGraphPanelNodeFactory_ActivityNodes> GraphPanelNodeFactory_ActivityNodes;

void FActivitiesPluginEditorModule::StartupModule()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	EAssetTypeCategories::Type ActivitiesCategory = AssetTools.RegisterAdvancedAssetCategory(TEXT("ActivitiesPlugin"), NSLOCTEXT("ActivitiesEditor", "ActivitiesEditorCategory", "Activities"));

	RegisterAction(MakeShareable(new FAssetTypeActions_Activity(ActivitiesCategory)));
	RegisterAction(MakeShareable(new FAssetTypeActions_ObjectiveTracker(ActivitiesCategory)));

	GraphPanelNodeFactory_ActivityNodes = MakeShareable(new FGraphPanelNodeFactory_ActivityNodes());
	FEdGraphUtilities::RegisterVisualNodeFactory(GraphPanelNodeFactory_ActivityNodes);

	MenuExtensibilityManager = MakeShareable(new FExtensibilityManager);
	ToolBarExtensibilityManager = MakeShareable(new FExtensibilityManager);

}

void FActivitiesPluginEditorModule::ShutdownModule()
{
	
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		for (auto AssetAction : AssetTypeActions)
		{
			AssetTools.UnregisterAssetTypeActions(AssetAction.ToSharedRef());
		}

		AssetTypeActions.Empty();
	}

	if (GraphPanelNodeFactory_ActivityNodes.IsValid())
	{
		FEdGraphUtilities::UnregisterVisualNodeFactory(GraphPanelNodeFactory_ActivityNodes);
		GraphPanelNodeFactory_ActivityNodes.Reset();
	}
	
}

TSharedRef<FActivityEditor> FActivitiesPluginEditorModule::CreateActivityEditor(const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, class UActivity* Object)
{
	if (!ClassCache.IsValid())
	{	
		ClassCache = MakeShareable(new FGraphNodeClassHelper(UActivityTask_Base::StaticClass()));
		FGraphNodeClassHelper::AddObservedBlueprintClasses(UActivityTask_StageService::StaticClass());
		FGraphNodeClassHelper::AddObservedBlueprintClasses(UActivityTask_ObjectiveTracker::StaticClass());
		ClassCache->UpdateAvailableBlueprintClasses();	   
	}

	TSharedRef< FActivityEditor> NewActivityEditor(new FActivityEditor());
	NewActivityEditor->InitActivityEditor(Mode, InitToolkitHost, Object);
	return NewActivityEditor;
}

void FActivitiesPluginEditorModule::RegisterAction(TSharedRef<IAssetTypeActions> Action)
{
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();

		AssetTools.RegisterAssetTypeActions(Action);
		AssetTypeActions.Add(Action);
	}
}