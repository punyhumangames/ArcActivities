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
