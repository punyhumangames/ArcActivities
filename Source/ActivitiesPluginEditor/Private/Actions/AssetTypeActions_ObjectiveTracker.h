// 2017-2019 Puny Human

#pragma once

#include "AssetTypeActions_Base.h"

class FAssetTypeActions_ObjectiveTracker : public FAssetTypeActions_Base
{
public:
	FAssetTypeActions_ObjectiveTracker(EAssetTypeCategories::Type InAssetCategory);

	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual bool HasActions(const TArray<UObject*>& InObjects) const override { return false; }

	uint32 GetCategories() override;

private:

	EAssetTypeCategories::Type AssetCategory;
};