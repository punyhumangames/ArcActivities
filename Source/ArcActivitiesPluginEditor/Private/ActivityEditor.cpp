// Copyright 2023 Puny Human.  All Rights Reserved

#include "ActivityEditor.h"
#include "DataModel/ArcActivity.h"

#include "Graph/EdGraph_Activity.h"
#include "Graph/EdGraphSchema_Activity.h"

#include "Toolkits/IToolkitHost.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "GraphEditor.h"
#include "GraphEditorActions.h"
#include "EdGraphUtilities.h"
#include "SNodePanel.h"
#include "Widgets/Docking/SDockTab.h"
#include "SSingleObjectDetailsPanel.h"
#include "Framework/Commands/GenericCommands.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "EditorSupportDelegates.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "EdGraph/EdGraph.h"
#include "FileHelpers.h"
#include "HAL/PlatformApplicationMisc.h"

#include "Graph/Nodes/ActivityNode_Base.h"
#include "Graph/Nodes/ActivityNode_Stage.h"
#include "EditorStyleSet.h"

#define LOCTEXT_NAMESPACE "ActivityEditor"

struct FActivityEditorTabs
{
	static const FName Details;
	static const FName Actions;
	static const FName GraphEditor;
};

const FName FActivityEditorTabs::Details = TEXT("Details");
const FName FActivityEditorTabs::Actions = TEXT("Actions");
const FName FActivityEditorTabs::GraphEditor = TEXT("GraphEditor");


FActivityEditor::FActivityEditor()
{

}

FActivityEditor::~FActivityEditor()
{

}

void FActivityEditor::InitActivityEditor(const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, class UArcActivity* ActivityData)
{
	GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->CloseOtherEditors(ActivityData, this);
	//FAssetEditorManager::Get().CloseOtherEditors(ActivityData, this);
	ActivityBeingEdited = ActivityData;

	UEdGraph_Activity* ActivityGraph = Cast<UEdGraph_Activity>(ActivityData->EditorGraph);

	if (!IsValid(ActivityGraph))
	{
		ActivityGraph = Cast<UEdGraph_Activity>(FBlueprintEditorUtils::CreateNewGraph(ActivityData, TEXT("Activity Graph"), UEdGraph_Activity::StaticClass(), UEdGraphSchema_Activity::StaticClass()));  		
		ActivityBeingEdited->EditorGraph = ActivityGraph;
		ActivityGraph->OnCreated();
	}
	else
	{
		ActivityGraph->OnLoaded();
	}
	ActivityGraph->Initialize();

	GraphEditor = CreateGraphEditorWidget(ActivityBeingEdited->EditorGraph);

	UpdateGraphEdPtr = GraphEditor;

	//Default Layout
	const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("ActivityEditor_v1")
		->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.1f)
				->SetHideTabWell(true)
				->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
			)
			->Split
			(
				FTabManager::NewSplitter()
				->SetOrientation(Orient_Horizontal)
				->SetSizeCoefficient(0.8f)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.8f)
					->SetHideTabWell(true)
					->AddTab(FActivityEditorTabs::GraphEditor, ETabState::OpenedTab)
				)
				->Split
				(
					FTabManager::NewSplitter()
					->SetOrientation(Orient_Vertical)
					->SetSizeCoefficient(0.37f)
					->Split
					(
						FTabManager::NewSplitter()
						->SetOrientation(Orient_Horizontal)
						->SetSizeCoefficient(0.33f)
						->Split
						(
							FTabManager::NewStack()
							->SetSizeCoefficient(0.6f)
							->AddTab(FActivityEditorTabs::Details, ETabState::OpenedTab)
						)
						// ...
					)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.6f)
						->AddTab(FActivityEditorTabs::Actions, ETabState::OpenedTab)
					)
				)
			)
		);

	InitAssetEditor(Mode, InitToolkitHost, TEXT("ActivityEditor"), StandaloneDefaultLayout, true, true, ActivityBeingEdited);


	PropertyEditor->SetObject(ActivityBeingEdited);

}



void FActivityEditor::OnGraphEditorFocused(const TSharedRef<SGraphEditor>& InGraphEditor)
{
	UpdateGraphEdPtr = InGraphEditor;

	FGraphPanelSelectionSet CurrentSelection;
	CurrentSelection = InGraphEditor->GetSelectedNodes();
	OnSelectedNodesChanged(CurrentSelection);

}

TSharedRef<class SGraphEditor> FActivityEditor::CreateGraphEditorWidget(UEdGraph* InGraph)
{
	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = LOCTEXT("GraphEdCornerText", "Activity Editor");

	CreateCommandList();

	SGraphEditor::FGraphEditorEvents InEvents;
	InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FActivityEditor::OnSelectedNodesChanged);

	return SNew(SGraphEditor)
		.AdditionalCommands(GraphEditorCommands)
		.Appearance(AppearanceInfo)
		.GraphToEdit(InGraph)
		.GraphEvents(InEvents);
}

void FActivityEditor::OnSelectedNodesChanged(const TSet<class UObject*>& NewSelection)
{					   

	if (NewSelection.Num() == 0)
	{
		PropertyEditor->SetObject(ActivityBeingEdited);
	}
	else if(NewSelection.Num() == 1)
	{
		TArray<UObject*> SelectionArr = NewSelection.Array();

		if (UActivityNode_Base* BaseNode = Cast<UActivityNode_Base>(SelectionArr[0]))
		{
			PropertyEditor->SetObject(BaseNode->GetNodeInstance());
		}	  	  
	}

}

void FActivityEditor::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_ActivityEditor", "Activity Editor"));
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(FActivityEditorTabs::Details, FOnSpawnTab::CreateSP(this, &FActivityEditor::SpawnTab_Details))
		.SetDisplayName(LOCTEXT("DetailsTab_Label", "Details"))
		.SetGroup(WorkspaceMenuCategoryRef);

	InTabManager->RegisterTabSpawner(FActivityEditorTabs::Actions, FOnSpawnTab::CreateSP(this, &FActivityEditor::SpawnTab_Actions))
		.SetDisplayName(LOCTEXT("ActionsTab_Label", "Actions"))
		.SetGroup(WorkspaceMenuCategoryRef);

	InTabManager->RegisterTabSpawner(FActivityEditorTabs::GraphEditor, FOnSpawnTab::CreateSP(this, &FActivityEditor::SpawnTab_GraphEditor))
		.SetDisplayName(LOCTEXT("GraphEditorTab_Label", "Graph Editor"))
		.SetGroup(WorkspaceMenuCategoryRef);
}

void FActivityEditor::UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(FActivityEditorTabs::Details);
	InTabManager->UnregisterTabSpawner(FActivityEditorTabs::Actions);
	InTabManager->UnregisterTabSpawner(FActivityEditorTabs::GraphEditor);
}

TSharedRef<SDockTab> FActivityEditor::SpawnTab_GraphEditor(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab)
		.Label(LOCTEXT("GraphEditorTab", "Activity Graph"))
		.TabColorScale(GetTabColorScale())
		[
			GraphEditor.ToSharedRef()
		];
}

TSharedRef<SDockTab> FActivityEditor::SpawnTab_Details(const FSpawnTabArgs& Args)
{
	//FMissionObjectivesEditorEdMode* EdMode = FAssetEditorToolkit::GetEditorMode();
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	const FDetailsViewArgs DetailsViewArgs(false, false, true, FDetailsViewArgs::HideNameArea, true, this);
	TSharedPtr<IDetailsView> PropertyEditorRef = PropertyEditorModule.CreateDetailView(DetailsViewArgs);

	PropertyEditor = PropertyEditorRef;

	// Spawn the tab
	return SNew(SDockTab)
		.Label(LOCTEXT("DetailsTab_Title", "Details"))
		[
			PropertyEditorRef.ToSharedRef()
		];
}

TSharedRef<SDockTab> FActivityEditor::SpawnTab_Actions(const FSpawnTabArgs& Args)
{
	
	//ActionPalette = SNew(SCCGraphPalette, SharedThis(this));

	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("Kismet.Tabs.Palette"))
		.Label(LOCTEXT("ActionsPaletteTitle", "Activity Nodes"))
		[
			SNew(SBox)
		/*[
			ActionPalette.ToSharedRef()
		] */
		];

	return SpawnedTab;
	
}

FName FActivityEditor::GetToolkitFName() const
{
	return TEXT("Activity Editor");
}

FText FActivityEditor::GetBaseToolkitName() const
{
	return LOCTEXT("ActivityEditorAPP", "Activity Editor");
}  


FText FActivityEditor::GetToolkitName() const
{
	const bool bDirtyState = ActivityBeingEdited->GetOutermost()->IsDirty();

	FFormatNamedArguments Args;
	Args.Add(TEXT("ActivityName"), FText::FromString(ActivityBeingEdited->GetName()));
	Args.Add(TEXT("DirtyState"), bDirtyState ? FText::FromString(TEXT("*")) : FText::GetEmpty());
	return FText::Format(LOCTEXT("ActivityEditorAppLabel", "{ActivityName}{DirtyState}"), Args);
}

FLinearColor FActivityEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor::White;
}

FString FActivityEditor::GetWorldCentricTabPrefix() const
{
	return TEXT("ActivityEditor");
}




#undef LOCTEXT_NAMESPACE