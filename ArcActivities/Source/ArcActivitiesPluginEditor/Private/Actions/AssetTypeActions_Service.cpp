// Arc Activities Plugin.  Copyright Puny Human and Garrett Fleenor.  Licensed under MIT or Apache 2.0 license.  See LICENSE-MIT and LICENSE-APACHE for more info


#include "Actions/AssetTypeActions_Service.h"
#include "DataModel/ArcActivityTask_ObjectiveTracker.h"

#include "Kismet2/KismetEditorUtilities.h"
#include "Misc/MessageDialog.h"


#define LOCTEXT_NAMESPACE "ActivityService"



UActivityServiceBlueprintFactory::UActivityServiceBlueprintFactory()
{
	bCreateNew = true;
	bEditAfterNew = false;
	SupportedClass = UArcActivityTask_StageServiceBlueprint::StaticClass();
	ParentClass = UArcActivityTask_StageService::StaticClass();
}

UObject* UActivityServiceBlueprintFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	// Make sure we are trying to factory a gameplay ability blueprint, then create and init one
	check(Class->IsChildOf(UArcActivityTask_StageServiceBlueprint::StaticClass()));

	// If they selected an interface, force the parent class to be UInterface
	if (BlueprintType == BPTYPE_Interface)
	{
		ParentClass = UInterface::StaticClass();
	}

	if ((ParentClass == nullptr) || !FKismetEditorUtilities::CanCreateBlueprintOfClass(ParentClass) || !ParentClass->IsChildOf(UArcActivityTask_StageService::StaticClass()))
	{
		FFormatNamedArguments Args;
		Args.Add(TEXT("ClassName"), (ParentClass != nullptr) ? FText::FromString(ParentClass->GetName()) : LOCTEXT("Null", "(null)"));
		FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT( "CannotCreateActivityBlueprint", "Cannot create an Activity Service based on the class '{ClassName}'."), Args));
		return nullptr;
	}
	else
	{
		return CastChecked<UArcActivityTask_StageServiceBlueprint>(FKismetEditorUtilities::CreateBlueprint(ParentClass, InParent, Name, BlueprintType, UArcActivityTask_StageServiceBlueprint::StaticClass(), UBlueprintGeneratedClass::StaticClass(), CallingContext));
	}
}

UObject* UActivityServiceBlueprintFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return FactoryCreateNew(Class, InParent, Name, Flags, Context, Warn, NAME_None);
}

UFactory* FAssetTypeActions_Service::GetFactoryForBlueprintType(UBlueprint* InBlueprint) const
{
	UActivityServiceBlueprintFactory* Factory = NewObject< UActivityServiceBlueprintFactory>();
	Factory->ParentClass = TSubclassOf<UArcActivityTask_StageService>(*InBlueprint->GeneratedClass);
	return Factory;
}


#undef LOCTEXT_NAMESPACE