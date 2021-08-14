// Copyright (C) 2021. Nikita Klimov. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FirebaseAnalyticsSubsystem.generated.h"

UENUM(Blueprintable)
enum EBuiltinParamNames
{
	ACHIEVEMENT_ID,
	ACLID,
	AD_FORMAT,
	AD_PLATFORM,
	AD_SOURCE,
	AD_UNIT_NAME,
	AFFILIATION	,
	CAMPAIGN,
	CHARACTER,
	CHECKOUT_OPTION,
	CHECKOUT_STEP,
	CONTENT,
	CONTENT_TYPE,
	COUPON,
	CP1,
	CREATIVE_NAME,
	CREATIVE_SLOT,
	CURRENCY,
	DESTINATION,
	DISCOUNT,
	END_DATE,
	EXTEND_SESSION,
	FLIGHT_NUMBER,
	GROUP_ID,
	INDEX,
	ITEMS,
	ITEM_BRAND,
	ITEM_CATEGORY,
	ITEM_CATEGORY2,
	ITEM_CATEGORY3,
	ITEM_CATEGORY4,
	ITEM_CATEGORY5,
	ITEM_ID,
	ITEM_LIST,
	ITEM_LIST_ID,
	ITEM_LIST_NAME,
	ITEM_LOCATION_ID,
	ITEM_NAME,
	ITEM_VARIANT,
	LEVEL,
	LEVEL_NAME,
	LOCATION,
	LOCATION_ID,
	MEDIUM,
	METHOD,
	NUMBER_OF_NIGHTS,
	NUMBER_OF_PASSENGERS,
	NUMBER_OF_ROOMS,
	ORIGIN,
	PAYMENT_TYPE,
	PRICE,
	PROMOTION_ID,
	PROMOTION_NAME,
	QUANTITY,
	SCORE,
	SCREEN_CLASS,
	SCREEN_NAME,
	SEARCH_TERM,
	SHIPPING,
	SHIPPING_TIER,
	SIGN_UP_METHOD,
	SOURCE,
	START_DATE,
	SUCCESS,
	TAX,
	TERM,
	TRANSACTION_ID,
	TRAVEL_CLASS,
	VALUE,
	VIRTUAL_CURRENCY_NAME,
};

UENUM(Blueprintable)
enum EBuiltinEventNames
{
	ADD_PAYMENT_INFO,
	ADD_SHIPPING_INFO,
	ADD_TO_CART,
	ADD_TO_WISHLIST,
	AD_IMPRESSION,
	APP_OPEN,
	BEGIN_CHECKOUT,
	CAMPAIGN_DETAILS,
	CHECKOUT_PROGRESS,
	EARN_VIRTUAL_CURRENCY,
	ECOMMERCE_PURCHASE,
	GENERATE_LEAD,
	JOIN_GROUP,
	LEVEL_END,
	LEVEL_START,
	LEVEL_UP,
	LOGIN,
	POST_SCORE,
	PRESENT_OFFER,
	PURCHASE,
	PURCHASE_REFUND,
	REFUND,
	REMOVE_FROM_CART,
	SCREEN_VIEW,
	SEARCH,
	SELECT_CONTENT,
	SELECT_ITEM,
	SELECT_PROMOTION,
	SET_CHECKOUT_OPTION,
	SHARE,
	SIGN_UP,
	SPEND_VIRTUAL_CURRENCY,
	TUTORIAL_BEGIN,
	TUTORIAL_COMPLETE,
	UNLOCK_ACHIEVEMENT,
	VIEW_CART,
	VIEW_ITEM,
	VIEW_ITEM_LIST,
	VIEW_PROMOTION,
	VIEW_SEARCH_RESULTS,
};

USTRUCT(BlueprintType)
struct FBundle
{
	GENERATED_BODY()

	TMap<FString, FString> StringParameters;
	TMap<FString, float> FloatParameters;
	TMap<FString, int> IntegerParameters;
};

// TODO: finish up:
//					Measure Ecommerce
//					Log events
//					Configure Analytics Data Collection and Usage
//					Home
//					README

// TODO: add 'SetDefaultEventParameters'
// TODO: add examples
UCLASS()
class UFirebaseAnalyticsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/** Log an event with associated parameters.
	 *  @param EventName	Name of the event to log. Should contain 1 to 40 alphanumeric
	 *						characters or underscores. The name must start with an
	 *						alphabetic character. Some event names are reserved.
	 *						See Analytics Events (https://firebase.google.com/docs/reference/
	 *						cpp/group/event-names.html#group__event__names) for the list
	 *  					of reserved event names.
	 */
	UFUNCTION(BlueprintCallable, Category = "FirebaseAnalytics")
	static void LogEvent(const FString& EventName);

	/** Log an event with one string parameter.
	 *  @param EventName		Name of the event to log. Should contain 1 to 40 alphanumeric
	 *							characters or underscores. The name must start with an
	 *							alphabetic character. Some event names are reserved.
	 *							See Analytics Events (https://firebase.google.com/docs/reference/
	 *							cpp/group/event-names.html#group__event__names) for the list
	 *  						of reserved event names.
	 *
	 *  @param ParameterName	Name of the parameter to log.
	 *  @param ParameterValue	Value of the parameter to log.
	 */
	UFUNCTION(BlueprintCallable, Category = "FirebaseAnalytics")
	static void LogEventWithStringParameter(
		const FString& EventName, 
		const FString& ParameterName, 
		const FString& ParameterValue);

	/** Log an event with one float parameter.
     *  @param EventName		Name of the event to log. Should contain 1 to 40 alphanumeric
	 *							characters or underscores. The name must start with an
	 *							alphabetic character. Some event names are reserved.
	 *							See Analytics Events (https://firebase.google.com/docs/reference/
	 *							cpp/group/event-names.html#group__event__names) for the list
	 *  						of reserved event names.
	 *
	 *  @param ParameterName	Name of the parameter to log.
	 *  @param ParameterValue	Value of the parameter to log.
	 */
	UFUNCTION(BlueprintCallable, Category = "FirebaseAnalytics")
	static void LogEventWithFloatParameter(
		const FString& EventName, 
		const FString& ParameterName, 
		const float ParameterValue);

	/** Log an event with one integer parameter.
     *  @param EventName		Name of the event to log. Should contain 1 to 40 alphanumeric
	 *							characters or underscores. The name must start with an
	 *							alphabetic character. Some event names are reserved.
	 *							See Analytics Events (https://firebase.google.com/docs/reference/
	 *							cpp/group/event-names.html#group__event__names) for the list
	 *  						of reserved event names.
	 *
	 *  @param ParameterName	Name of the parameter to log.
	 *  @param ParameterValue	Value of the parameter to log.
	 */
	UFUNCTION(BlueprintCallable, Category = "FirebaseAnalytics")
	static void LogEventWithIntegerParameter(
		const FString& EventName, 
		const FString& ParameterName, 
		const int ParameterValue);

	/** Log an event with associated parameters.
     *  @param EventName	Name of the event to log. Should contain 1 to 40 alphanumeric
	 *						characters or underscores. The name must start with an
	 *						alphabetic character. Some event names are reserved.
	 *						See Analytics Events (https://firebase.google.com/docs/reference/
	 *						cpp/group/event-names.html#group__event__names) for the list
	 *  					of reserved event names.
	 *
	 *  @param Bundle		Array of FParameter structures.
	 */
	UFUNCTION(BlueprintCallable, Category = "FirebaseAnalytics")
	static void LogEventWithParameters(
		const FString& EventName, 
		UPARAM(ref) FBundle& Bundle);

	/** Clears all analytics data for this app from the device and resets the app instance id. */
	UFUNCTION(BlueprintCallable, Category = "FirebaseAnalytics")
	static void ResetAnalyticsData();

	/** Sets whether analytics collection is enabled for this app on this device.
	 *
	 *  @param bEnabled	"true" to enable analytics collection, "false" to disable.
	 */
	UFUNCTION(BlueprintCallable, Category = "FirebaseAnalytics")
	static void SetAnalyticsCollectionEnabled(const bool bEnabled);

	/** Sets the duration of inactivity that terminates the current session.
	 *
	 *  @param Milliseconds	The duration of inactivity
	 *						that terminates the current session.
	 */
	UFUNCTION(BlueprintCallable, Category = "FirebaseAnalytics")
	static void SetSessionTimeoutDuration(const int Milliseconds);

	/** Sets the user ID property.
	 *
	 *  @param UserID	The user ID associated with the user of this app on
	 *					this device. The user ID must be non-empty and no more
	 *					than 256 characters long. Setting UserID to NULL or
	 *					nullptr removes the user ID.
	 */
	UFUNCTION(BlueprintCallable, Category = "FirebaseAnalytics")
	static void SetUserID(const FString& UserID);

	/** Set a user property to the given value.
	 *  @param PropertyName		Name of the user property to set. This must be a
	 *							combination of letters and digits (matching the
	 *							regular expression [a-zA-Z0-9] between 1 and 40
	 *							characters long starting with a letter [a-zA-Z] character.
	 *
	 *  @param PropertyValue	Value to set the user property to. Set this argument
	 *							to NULL or nullptr to remove the user property.
	 *							The value can be between 1 to 100 characters long.
	 */
	UFUNCTION(BlueprintCallable, Category = "FirebaseAnalytics")
	static void SetUserProperty(
		const FString& PropertyName, 
		const FString& PropertyValue);

	/** Add a string parameter to Bundle.
	 *	@param Bundle			Bundle reference
	 *  @param ParameterName	Name of the parameter to log.
	 *  @param ParameterValue	Value of the parameter to log.
	 */
	UFUNCTION(BlueprintCallable, Category = "FirebaseAnalytics | Bundle")
	static void PutString(
		UPARAM(ref) FBundle& Bundle, 
		const FString& ParameterName, 
		const FString& ParameterValue);
	
	/** Add a floating point parameter to Bundle.
	 *	@param Bundle			Bundle reference
	 *  @param ParameterName	Name of the parameter to log.
	 *  @param ParameterValue	Value of the parameter to log.
	 */
	UFUNCTION(BlueprintCallable, Category = "FirebaseAnalytics | Bundle")
	static void PutFloat(
		UPARAM(ref) FBundle& Bundle, 
		const FString& ParameterName, 
		const float ParameterValue);
	
	/** Add a integer parameter to Bundle.
	 *	@param Bundle			Bundle reference
	 *  @param ParameterName	Name of the parameter to log.
	 *  @param ParameterValue	Value of the parameter to log.
	 */
	UFUNCTION(BlueprintCallable, Category="FirebaseAnalytics | Bundle")
	static void PutInteger(
		UPARAM(ref) FBundle& Bundle, 
		const FString& ParameterName, 
		const int ParameterValue);

	/** Return a built-in event names.
	 */
	UFUNCTION(BlueprintCallable, Category="FirebaseAnalytics")
	static TMap<TEnumAsByte<EBuiltinEventNames>, FName> GetBuiltinEventNames();

	/** Return a built-in param names.
	 */
	UFUNCTION(BlueprintCallable, Category="FirebaseAnalytics")
	static TMap<TEnumAsByte<EBuiltinParamNames>, FName> GetBuiltinParamNames();
};
