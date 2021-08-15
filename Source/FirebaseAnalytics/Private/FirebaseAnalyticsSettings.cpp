// Copyright (C) 2021. Nikita Klimov. All rights reserved.

#include "FirebaseAnalyticsSettings.h"

#if WITH_EDITOR
void UFirebaseAnalyticsSettings::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	SaveConfig(CPF_Config, *GetDefaultConfigFilename());
}
#endif
