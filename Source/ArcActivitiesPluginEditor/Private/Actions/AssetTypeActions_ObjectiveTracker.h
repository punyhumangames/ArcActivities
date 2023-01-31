// Copyright 2023 Puny Human.  All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/IToolkitHost.h"
#include "AssetTypeActions/AssetTypeActions_Blueprint.h"
#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"
#include "Engine/Blueprint.h"
#include "Blueprints/ArcActivityTask_ObjectiveTrackerBlueprint.h"

#include "Factories/Factory.h"
#include "AssetTypeActions_ObjectiveTracker.generated.h"

class UFactory;

UCLASS(hidecategories = Object, collapsecategories)
class UObjectiveTrackerBlueprintFactory : public UFactory
{
	GENERATED_BODY()

public:
	UObjectiveTrackerBlueprintFactory();

	// The type of blueprint that will be created
	UPROPERTY(EditAnywhere, Category = Factory)
		TEnumAsByte<enum EBlueprintType> BlueprintType;

	// The parent class of the created blueprint
	UPROPERTY(EditAnywhere, Category = Factory)
		TSubclassOf<class UArcActivityTask_ObjectiveTracker> ParentClass;

	//Begin UFactory Interface
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	//End UFactory Interface	
};

class FAssetTypeActions_ObjectiveTracker : public FAssetTypeActions_Blueprint
{
public:
	FAssetTypeActions_ObjectiveTracker(EAssetTypeCategories::Type InAssetCategory)
		: AssetCategory(InAssetCategory)
	{

	}

public:
	// IAssetTypeActions Implementation
	virtual FText GetName() const override { return NSLOCTEXT("ActivitiesEditor", "ObjectiveTrackerName", "Objective Tracker"); }
	virtual FColor GetTypeColor() const override { return FColor::Yellow; }
	virtual uint32 GetCategories() override { return AssetCategory; }
	// End IAssetTypeActions Implementation

	virtual UClass* GetSupportedClass() const override { return UArcActivityTask_ObjectiveTrackerBlueprint::StaticClass(); }
	virtual UFactory* GetFactoryForBlueprintType(UBlueprint* InBlueprint) const override;

private:

	EAssetTypeCategories::Type AssetCategory;
};