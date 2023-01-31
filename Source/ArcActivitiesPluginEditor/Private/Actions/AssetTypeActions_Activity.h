// Copyright 2023 Puny Human.  All Rights Reserved

#pragma once

#include "AssetTypeActions_Base.h"


class FAssetTypeActions_Activity : public FAssetTypeActions_Base
{
public:
	FAssetTypeActions_Activity(EAssetTypeCategories::Type InAssetCategory);

	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual bool HasActions(const TArray<UObject*>& InObjects) const override { return false; }
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;

	uint32 GetCategories() override;



private:

	EAssetTypeCategories::Type AssetCategory;
};

