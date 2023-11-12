// Arc Activities Plugin.  Copyright Puny Human and Garrett Fleenor.  Licensed under MIT or Apache 2.0 license.  See LICENSE-MIT and LICENSE-APACHE for more info


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

