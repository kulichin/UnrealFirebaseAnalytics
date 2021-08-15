// Copyright (C) 2021. Nikita Klimov. All rights reserved.

#include "FirebaseAnalytics.h"
#include "FirebaseAnalyticsSettings.h"
#include "Settings/Public/ISettingsModule.h"

#define LOCTEXT_NAMESPACE "FFirebaseAnalyticsModule"

void FFirebaseAnalyticsModule::StartupModule()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings(
			TEXT("Project"), 
			TEXT("Plugins"), 
			TEXT("Firebase Analytics"),
			LOCTEXT("Firebase Analytics", "Firebase Analytics"), 
			LOCTEXT("Facebook", "Settings for Facebook Authentication"),
			GetMutableDefault<UFirebaseAnalyticsSettings>());
	}
}

void FFirebaseAnalyticsModule::ShutdownModule()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings(
			TEXT("Project"), 
			TEXT("Plugins"), 
			TEXT("Firebase Analytics"));
	}
}

#undef LOCTEXT_NAMESPACE
IMPLEMENT_MODULE(FFirebaseAnalyticsModule, FirebaseAnalytics)
