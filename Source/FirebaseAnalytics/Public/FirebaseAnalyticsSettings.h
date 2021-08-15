// Copyright (C) 2021. Nikita Klimov. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FirebaseAnalyticsSettings.generated.h"

UCLASS(transient, config = Engine)
class UFirebaseAnalyticsSettings : public UObject
{
	GENERATED_BODY()

	UPROPERTY(Config, EditAnywhere)
	bool bPermanentlyDeactivateCollection = false;
	
	UPROPERTY(Config, EditAnywhere)
	bool bEnableAnalyticsCollection = true;

	UPROPERTY(Config, EditAnywhere)
	bool bEnableAdvertisingIDCollection = false;

	UPROPERTY(Config, EditAnywhere)
	bool bEnableSSAIDCollection = false;

	UPROPERTY(Config, EditAnywhere)
	bool bAllowAdPersonalizationSignals = false;
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
