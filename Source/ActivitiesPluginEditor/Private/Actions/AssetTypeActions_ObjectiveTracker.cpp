// 2017-2019 Puny Human

#include "AssetTypeActions_ObjectiveTracker.h"
#include "ObjectiveTracker.h"


#define LOCTEXT_NAMESPACE "ActivitiesPlugin"

FAssetTypeActions_ObjectiveTracker::FAssetTypeActions_ObjectiveTracker(EAssetTypeCategories::Type InAssetCategory)
	: AssetCategory(InAssetCategory)
{

}

FText FAssetTypeActions_ObjectiveTracker::GetName() const
{
	return LOCTEXT("AssetTypeAction_ObjectiveTracker", "ObjectiveTracker");
}

FColor FAssetTypeActions_ObjectiveTracker::GetTypeColor() const
{
	return FColor::Orange;
}

UClass* FAssetTypeActions_ObjectiveTracker::GetSupportedClass() const
{
	return UActivityTask_ObjectiveTracker::StaticClass();
}

uint32 FAssetTypeActions_ObjectiveTracker::GetCategories()
{
	return AssetCategory | EAssetTypeCategories::Gameplay;
}

#undef LOCTEXT_NAMESPACE