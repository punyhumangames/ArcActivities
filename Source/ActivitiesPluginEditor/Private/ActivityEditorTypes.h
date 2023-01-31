// 2017-2019 Puny Human

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/WeakObjectPtr.h"
#include "ActivityEditorTypes.generated.h"

UENUM()
enum class EActivitySubnodeType : uint8
{
	Service,
	ObjectiveTracker,
};

USTRUCT()
struct FArcGraphNodeClassData
{
	GENERATED_BODY()

	FArcGraphNodeClassData() {}
	FArcGraphNodeClassData(UClass* InClass, const FString& InDeprecatedMessage);
	FArcGraphNodeClassData(const FString& InAssetName, const FString& InGeneratedClassPackage, const FString& InClassName, UClass* InClass);

	FString ToString() const;
	FString GetClassName() const;
	FText GetCategory() const;
	FString GetDisplayName() const;
	UClass* GetClass(bool bSilent = false);
	bool IsAbstract() const;

	FORCEINLINE bool IsBlueprint() const { return AssetName.Len() > 0; }
	FORCEINLINE bool IsDeprecated() const { return DeprecatedMessage.Len() > 0; }
	FORCEINLINE FString GetDeprecatedMessage() const { return DeprecatedMessage; }
	FORCEINLINE FString GetPackageName() const { return GeneratedClassPackage; }

	/** set when child class masked this one out (e.g. always use game specific class instead of engine one) */
	uint32 bIsHidden : 1;

	/** set when class wants to hide parent class from selection (just one class up hierarchy) */
	uint32 bHideParent : 1;

private:

	/** pointer to uclass */
	TWeakObjectPtr<UClass> Class;

	/** path to class if it's not loaded yet */
	UPROPERTY()
		FString AssetName;

	UPROPERTY()
		FString GeneratedClassPackage;

	/** resolved name of class from asset data */
	UPROPERTY()
		FString ClassName;

	/** User-defined category for this class */
	UPROPERTY()
		FText Category;

	/** message for deprecated class */
	FString DeprecatedMessage;
};


struct FArcGraphNodeClassNode
{
	FArcGraphNodeClassData Data;
	FString ParentClassName;

	TSharedPtr<FArcGraphNodeClassNode> ParentNode;
	TArray<TSharedPtr<FArcGraphNodeClassNode> > SubNodes;

	void AddUniqueSubNode(TSharedPtr<FArcGraphNodeClassNode> SubNode);
};

struct FArcGraphNodeClassHelper
{
	using ThisClass = FArcGraphNodeClassHelper;
	DECLARE_MULTICAST_DELEGATE(FOnPackageListUpdated);

	FArcGraphNodeClassHelper(UClass* InRootClass);
	~FArcGraphNodeClassHelper();

	void GatherClasses(const UClass* BaseClass, TArray<FArcGraphNodeClassData>& AvailableClasses);
	static FString GetDeprecationMessage(const UClass* Class);

	void OnAssetAdded(const struct FAssetData& AssetData);
	void OnAssetRemoved(const struct FAssetData& AssetData);
	void InvalidateCache();
	void OnReloadComplete(EReloadCompleteReason Reason);

	static void AddUnknownClass(const FArcGraphNodeClassData& ClassData);
	static bool IsClassKnown(const FArcGraphNodeClassData& ClassData);
	static FOnPackageListUpdated OnPackageListUpdated;

	static int32 GetObservedBlueprintClassCount(UClass* BaseNativeClass);
	static void AddObservedBlueprintClasses(UClass* BaseNativeClass);
	void UpdateAvailableBlueprintClasses();

private:

	UClass* RootNodeClass;
	TSharedPtr<FArcGraphNodeClassNode> RootNode;
	static TArray<FName> UnknownPackages;
	static TMap<UClass*, int32> BlueprintClassCount;

	TSharedPtr<FArcGraphNodeClassNode> CreateClassDataNode(const struct FAssetData& AssetData);
	TSharedPtr<FArcGraphNodeClassNode> FindBaseClassNode(TSharedPtr<FArcGraphNodeClassNode> Node, const FString& ClassName);
	void FindAllSubClasses(TSharedPtr<FArcGraphNodeClassNode> Node, TArray<FArcGraphNodeClassData>& AvailableClasses);

	UClass* FindAssetClass(const FString& GeneratedClassPackage, const FString& AssetName);
	void BuildClassGraph();
	void AddClassGraphChildren(TSharedPtr<FArcGraphNodeClassNode> Node, TArray<TSharedPtr<FArcGraphNodeClassNode> >& NodeList);

	bool IsHidingClass(UClass* Class);
	bool IsHidingParentClass(UClass* Class);
	bool IsPackageSaved(FName PackageName);
};
