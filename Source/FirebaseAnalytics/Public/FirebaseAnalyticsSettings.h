// Copyright (C) 2021. Nikita Klimov. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FirebaseAnalyticsSettings.generated.h"

UCLASS(transient, config = Engine)
class UFirebaseAnalyticsSettings : public UObject
{
	GENERATED_BODY()

	UPROPERTY(Config, EditAnywhere, Category = "Firebase Analytics")
	bool bPermanentlyDeactivateCollection = false;
	
	UPROPERTY(Config, EditAnywhere, Category = "Firebase Analytics")
	bool bEnableAnalyticsCollection = true;

	UPROPERTY(Config, EditAnywhere, Category = "Firebase Analytics")
	bool bEnableAdvertisingIDCollection = false;

	UPROPERTY(Config, EditAnywhere, Category = "Firebase Analytics")
	bool bEnableSSAIDCollection = false;

	UPROPERTY(Config, EditAnywhere, Category = "Firebase Analytics")
	bool bAllowAdPersonalizationSignals = false;
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
