// Copyright 2023 Puny Human.  All Rights Reserved

#include "ArcActivitiesPlugin.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"



class FArcActivitiesPlugin : public IModuleInterface
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override { }
	virtual void ShutdownModule() override { }
};

IMPLEMENT_MODULE(FArcActivitiesPlugin, ArcActivitiesPlugin )

