// Copyright 2023 Puny Human.  All Rights Reserved

#include "ArcActivitiesPlugin.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"



class FActivitiesPlugin : public IModuleInterface
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

public:

	/**
	 * Singleton-like access to this module's interface.  This is just for convenience!
	 * Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	 *
	 * @return Returns singleton instance, loading the module on demand if needed
	 */
	static inline FActivitiesPlugin& Get()
	{
		return FModuleManager::LoadModuleChecked< FActivitiesPlugin >("ArcActivitiesPlugin");
	}

	/**
	 * Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	 *
	 * @return True if the module is loaded and ready to use
	 */
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("ArcActivitiesPlugin");
	}
};

IMPLEMENT_MODULE( FActivitiesPlugin, ActivitiesPlugin )



void FActivitiesPlugin::StartupModule()
{
	// This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
}


void FActivitiesPlugin::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}



