// 2017-2019 Puny Human

#include "AssetTypeActions_Activity.h"
#include "DataModel/Activity.h"

#include "ActivityEditor.h"
#include "ActivitiesPluginEditorModule.h"

#define LOCTEXT_NAMESPACE "ActivitiesPlugin"

FAssetTypeActions_Activity::FAssetTypeActions_Activity(EAssetTypeCategories::Type InAssetCategory)
	: AssetCategory(InAssetCategory)
{

}

FText FAssetTypeActions_Activity::GetName() const
{
	return LOCTEXT("AssetTypeAction_Activity", "Activity");
}

FColor FAssetTypeActions_Activity::GetTypeColor() const
{
	return FColor::Emerald;
}

UClass* FAssetTypeActions_Activity::GetSupportedClass() const
{
	return UActivity::StaticClass();
}

void FAssetTypeActions_Activity::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor /*= TSharedPtr<IToolkitHost>()*/)
{
	const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (UObject* Object : InObjects)
	{
		if (UActivity* Activity = Cast<UActivity>(Object))
		{
			FActivityEditor* ExistingInstance = static_cast<FActivityEditor*>(GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->FindEditorForAsset(Object, false));
			if (ExistingInstance)
			{
				ExistingInstance->InitActivityEditor(Mode, EditWithinLevelEditor, Activity);
			}
			else
			{
				FActivitiesPluginEditorModule& Module = FModuleManager::GetModuleChecked<FActivitiesPluginEditorModule>(TEXT("ActivitiesPluginEditor"));
				TSharedRef<FActivityEditor> NewEditor = Module.CreateActivityEditor(Mode, EditWithinLevelEditor, Activity);
			}						 									
		}
	}
}

uint32 FAssetTypeActions_Activity::GetCategories()
{
	return AssetCategory | EAssetTypeCategories::Gameplay;
}

#undef LOCTEXT_NAMESPACE
