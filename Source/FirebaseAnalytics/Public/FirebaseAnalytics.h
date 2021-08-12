// Copyright (C) 2021. Nikita Klimov. All rights reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FFirebaseAnalyticsModule : public IModuleInterface
{
public:
	virtual void StartupModule() override {};
	virtual void ShutdownModule() override {};
};
