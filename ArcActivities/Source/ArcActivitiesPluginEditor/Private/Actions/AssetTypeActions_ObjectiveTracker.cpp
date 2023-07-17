// Copyright 2023 Puny Human.  All Rights Reserved


#include "Actions/AssetTypeActions_ObjectiveTracker.h"
#include "DataModel/ArcActivityTask_ObjectiveTracker.h"

#include "Kismet2/KismetEditorUtilities.h"
#include "Misc/MessageDialog.h"


#define LOCTEXT_NAMESPACE "ObjectiveTrackerFactory"



UObjectiveTrackerBlueprintFactory::UObjectiveTrackerBlueprintFactory()
{
	bCreateNew = true;
	bEditAfterNew = false;
	SupportedClass = UArcActivityTask_ObjectiveTrackerBlueprint::StaticClass();
	ParentClass = UArcActivityTask_ObjectiveTracker::StaticClass();
}

UObject* UObjectiveTrackerBlueprintFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	// Make sure we are trying to factory a gameplay ability blueprint, then create and init one
	check(Class->IsChildOf(UArcActivityTask_ObjectiveTrackerBlueprint::StaticClass()));

	// If they selected an interface, force the parent class to be UInterface
	if (BlueprintType == BPTYPE_Interface)
	{
		ParentClass = UInterface::StaticClass();
	}

	if ((ParentClass == nullptr) || !FKismetEditorUtilities::CanCreateBlueprintOfClass(ParentClass) || !ParentClass->IsChildOf(UArcActivityTask_ObjectiveTracker::StaticClass()))
	{
		FFormatNamedArguments Args;
		Args.Add(TEXT("ClassName"), (ParentClass != nullptr) ? FText::FromString(ParentClass->GetName()) : LOCTEXT("Null", "(null)"));
		FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT( "CannotCreateActivityBlueprint", "Cannot create an Objective T racker based on the class '{ClassName}'."), Args));
		return nullptr;
	}
	else
	{
		return CastChecked<UArcActivityTask_ObjectiveTrackerBlueprint>(FKismetEditorUtilities::CreateBlueprint(ParentClass, InParent, Name, BlueprintType, UArcActivityTask_ObjectiveTrackerBlueprint::StaticClass(), UBlueprintGeneratedClass::StaticClass(), CallingContext));
	}
}

UObject* UObjectiveTrackerBlueprintFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return FactoryCreateNew(Class, InParent, Name, Flags, Context, Warn, NAME_None);
}

UFactory* FAssetTypeActions_ObjectiveTracker::GetFactoryForBlueprintType(UBlueprint* InBlueprint) const
{
	UObjectiveTrackerBlueprintFactory* Factory = NewObject< UObjectiveTrackerBlueprintFactory>();
	Factory->ParentClass = TSubclassOf<UArcActivityTask_ObjectiveTracker>(*InBlueprint->GeneratedClass);
	return Factory;
}


#undef LOCTEXT_NAMESPACE