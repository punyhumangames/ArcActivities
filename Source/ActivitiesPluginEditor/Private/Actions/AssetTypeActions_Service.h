// 2017-2019 Puny Human

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/IToolkitHost.h"
#include "AssetTypeActions/AssetTypeActions_Blueprint.h"
#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"
#include "Engine/Blueprint.h"
#include "Blueprints/ActivityServiceBlueprint.h"

#include "Factories/Factory.h"
#include "AssetTypeActions_Service.generated.h"

class UFactory;

UCLASS(hidecategories = Object, collapsecategories)
class UActivityServiceBlueprintFactory : public UFactory
{
	GENERATED_BODY()

public:
	UActivityServiceBlueprintFactory();

	// The type of blueprint that will be created
	UPROPERTY(EditAnywhere, Category = Factory)
		TEnumAsByte<enum EBlueprintType> BlueprintType;

	// The parent class of the created blueprint
	UPROPERTY(EditAnywhere, Category = Factory)
		TSubclassOf<class UActivityTask_StageService> ParentClass;

	//Begin UFactory Interface
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	//End UFactory Interface	
};

class FAssetTypeActions_Service : public FAssetTypeActions_Blueprint
{
public:
	FAssetTypeActions_Service(EAssetTypeCategories::Type InAssetCategory)
		: AssetCategory(InAssetCategory)
	{

	}

public:
	// IAssetTypeActions Implementation
	virtual FText GetName() const override { return NSLOCTEXT("ActivitiesEditor", "ServiceTaskName", "Activity Service"); }
	virtual FColor GetTypeColor() const override { return FColor::Orange; }
	virtual uint32 GetCategories() override { return AssetCategory; }
	// End IAssetTypeActions Implementation

	virtual UClass* GetSupportedClass() const override { return UActivityServiceBlueprint::StaticClass(); }
	virtual UFactory* GetFactoryForBlueprintType(UBlueprint* InBlueprint) const override;

private:

	EAssetTypeCategories::Type AssetCategory;
};