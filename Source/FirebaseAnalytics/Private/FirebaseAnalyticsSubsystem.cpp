// Copyright (C) 2021. Nikita Klimov. All rights reserved.

#include "FirebaseAnalyticsSubsystem.h"

#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"

// Analytics methods
static jmethodID LogEvent_MethodID;
static jmethodID LogEventWithStringParameter_MethodID;
static jmethodID LogEventWithFloatParameter_MethodID;
static jmethodID LogEventWithIntegerParameter_MethodID;
static jmethodID LogEventWithParameters_MethodID;
static jmethodID ResetAnalyticsData_MethodID;
static jmethodID SetAnalyticsCollectionEnabled_MethodID;
static jmethodID SetSessionTimeoutDuration_MethodID;
static jmethodID SetUserID_MethodID;
static jmethodID SetUserProperty_MethodID;
static jmethodID SetDefaultEventParameters_MethodID;

// Bundle methods
static jmethodID Bundle_Constructor_MethodID;
static jmethodID Bundle_PutString_MethodID;
static jmethodID Bundle_PutFloat_MethodID;
static jmethodID Bundle_PutInteger_MethodID;
static jmethodID Bundle_PutParcelableArray_MethodID;
jclass BundleClassID;
jclass ParcelableClassID;

static jmethodID FindMethodInSpecificClass(
	JNIEnv* Env,
	jclass Class,
	const char* Name, 
	const char* Signature)
{
	if (Env && Name && Signature && Class)
	{
		return Env->GetMethodID(Class, Name, Signature);
	}

	return nullptr;
}

static jmethodID FindMethod(JNIEnv* Env, const char* Name, const char* Signature)
{
	if (Env && Name && Signature)
	{
		return Env->GetMethodID(FJavaWrapper::GameActivityClassID, Name, Signature);
	}

	return nullptr;
}

static void CallVoidMethod(JNIEnv* Env, jmethodID Method, ...)
{
    // make sure the function exists
	jobject Object = FJavaWrapper::GameActivityThis;
	if (Method == NULL || Object == NULL || Env == NULL)
	{
		return;
	}

	va_list Args;
	va_start(Args, Method);
	Env->CallVoidMethodV(Object, Method, Args);
	va_end(Args);
}

static void CallVoidObjectMethod(
	JNIEnv* Env, 
	jobject Object, 
	jmethodID Method, ...)
{
    // make sure the function exists
	if (Method == NULL || Object == NULL || Env == NULL)
	{
		return;
	}

	va_list Args;
	va_start(Args, Method);
	Env->CallVoidMethodV(Object, Method, Args);
	va_end(Args);
}

static jobject ConvertBundleToJavaBundle(
	JNIEnv* Env,
	const FBundle& Bundle 
)
{
	// Initialize Bundle class
	auto JBundle = Env->NewObject(BundleClassID, Bundle_Constructor_MethodID);
	
	// Adding string parameter to Bundle
	for (auto& Parameter : Bundle.StringParameters)
	{
	    auto ParameterName = FJavaHelper::ToJavaString(Env, Parameter.Key);
	    auto ParameterValue = FJavaHelper::ToJavaString(Env, Parameter.Value);
		
		CallVoidObjectMethod(
			Env, 
			JBundle, 
			Bundle_PutString_MethodID, 
			*ParameterName, 
			*ParameterValue);
	}

	// Adding float parameter to Bundle
	for (auto& Parameter : Bundle.FloatParameters)
	{
	    auto ParameterName = FJavaHelper::ToJavaString(Env, Parameter.Key);
		CallVoidObjectMethod(
			Env, 
			JBundle, 
			Bundle_PutFloat_MethodID, 
			*ParameterName, 
			Parameter.Value);
	}

	// Adding integer parameter to Bundle
	for (auto& Parameter : Bundle.IntegerParameters)
	{
	    auto ParameterName = FJavaHelper::ToJavaString(Env, Parameter.Key);
		CallVoidObjectMethod(
			Env, 
			JBundle, 
			Bundle_PutInteger_MethodID, 
			*ParameterName, 
			Parameter.Value);
	}

	// Adding bundles parameters to 'JBundle'
	const auto& BundlesParameters = Bundle.BundlesParameters;
	for (const auto& BundleParameter : BundlesParameters)
	{
		const auto JParameterName = FJavaHelper::ToJavaString(Env, BundleParameter.Key);
		const TArray<FBundle> BundlesArray = BundleParameter.Value;
		const int BundlesArraySize = BundlesArray.Num();

		// Create 'Parcelable' java array
		auto JParcelableArray = NewScopedJavaObject(
			Env,
			(jobjectArray)Env->NewObjectArray(
				BundlesArraySize,
				ParcelableClassID,
				NULL));

		// Put bundles to 'JParcelableArray'
		for (int Idx = 0; Idx < BundlesArraySize; Idx++)
		{
			auto JTempBundle = NewScopedJavaObject(Env, ConvertBundleToJavaBundle(Env, BundlesArray[Idx]));
			Env->SetObjectArrayElement(*JParcelableArray, Idx, *JTempBundle);
		}

		// Finally put array of 'JParcelableArray' as parameter to 'JBundle'
		CallVoidObjectMethod(
			Env,
			JBundle,
			Bundle_PutParcelableArray_MethodID,
			*JParameterName,
			*JParcelableArray);
	}

	return JBundle;
}

#endif

void UFirebaseAnalyticsSubsystem::LogEvent(const FString& EventName)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		auto JEventName = FJavaHelper::ToJavaString(Env, EventName);

		CallVoidMethod(Env, LogEvent_MethodID, *JEventName);
	}
#endif
}

void UFirebaseAnalyticsSubsystem::LogEventWithStringParameter(
	const FString& EventName, 
	const FString& ParameterName, 
	const FString& ParameterValue)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		auto JEventName = FJavaHelper::ToJavaString(Env, EventName);
		auto JParameterName = FJavaHelper::ToJavaString(Env, ParameterName);
		auto JParameterValue = FJavaHelper::ToJavaString(Env, ParameterValue);

		CallVoidMethod(
			Env,
			LogEventWithStringParameter_MethodID,
			*JEventName, 
			*JParameterName, 
			*JParameterValue);
	}
#endif
}

void UFirebaseAnalyticsSubsystem::LogEventWithFloatParameter(
	const FString& EventName, 
	const FString& ParameterName, 
	const float ParameterValue)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		auto JEventName = FJavaHelper::ToJavaString(Env, EventName);
		auto JParameterName = FJavaHelper::ToJavaString(Env, ParameterName);

		CallVoidMethod(
			Env, 
			LogEventWithFloatParameter_MethodID,
			*JEventName, 
			*JParameterName, 
			ParameterValue);
	}
#endif
}

void UFirebaseAnalyticsSubsystem::LogEventWithIntegerParameter(
	const FString& EventName, 
	const FString& ParameterName, 
	const int ParameterValue)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		auto JEventName = FJavaHelper::ToJavaString(Env, EventName);
		auto JParameterName = FJavaHelper::ToJavaString(Env, ParameterName);

		CallVoidMethod(
			Env, 
			LogEventWithIntegerParameter_MethodID,
			*JEventName, 
			*JParameterName, 
			ParameterValue);
	}
#endif
}

void UFirebaseAnalyticsSubsystem::LogEventWithParameters(
	const FString& EventName, 
	const FBundle& Bundle)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		auto JBundle = NewScopedJavaObject(Env, ConvertBundleToJavaBundle(Env, Bundle));
		auto JEventName = FJavaHelper::ToJavaString(Env, EventName);
		
		CallVoidMethod(
			Env, 
			LogEventWithParameters_MethodID,
			*JEventName, 
			*JBundle);
	}
#endif
}

void UFirebaseAnalyticsSubsystem::ResetAnalyticsData()
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		CallVoidMethod(Env, ResetAnalyticsData_MethodID);
	}
#endif
}

void UFirebaseAnalyticsSubsystem::SetAnalyticsCollectionEnabled(const bool bEnabled)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		CallVoidMethod(Env, SetAnalyticsCollectionEnabled_MethodID, bEnabled);
	}
#endif
}

void UFirebaseAnalyticsSubsystem::SetSessionTimeoutDuration(const int Milliseconds)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		CallVoidMethod(Env, SetSessionTimeoutDuration_MethodID, Milliseconds);
	}
#endif
}

void UFirebaseAnalyticsSubsystem::SetUserID(const FString& UserID)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		auto JUserID = FJavaHelper::ToJavaString(Env, UserID);
		
		CallVoidMethod(Env, SetUserID_MethodID, *JUserID);
	}
#endif
}

void UFirebaseAnalyticsSubsystem::SetUserProperty(
	const FString& PropertyName, 
	const FString& PropertyValue)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		auto JPropertyName = FJavaHelper::ToJavaString(Env, PropertyName);
		auto JPropertyValue = FJavaHelper::ToJavaString(Env, PropertyValue);

		CallVoidMethod(
			Env, 
			SetUserProperty_MethodID,
			*JPropertyName, 
			*JPropertyValue);
	}
#endif
}

void UFirebaseAnalyticsSubsystem::SetDefaultEventParameters(const FBundle& Bundle)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		auto JBundle = NewScopedJavaObject(Env, ConvertBundleToJavaBundle(Env, Bundle));
		CallVoidMethod(
			Env, 
			SetDefaultEventParameters_MethodID,
			*JBundle);
	}
#endif

}

void UFirebaseAnalyticsSubsystem::PutString(
	FBundle& Bundle, 
	const FString& ParameterName, 
	const FString& ParameterValue)
{
	Bundle.StringParameters.Add(ParameterName, ParameterValue);
}

void UFirebaseAnalyticsSubsystem::PutFloat(
	FBundle& Bundle, 
	const FString& ParameterName, 
	const float ParameterValue)
{
	Bundle.FloatParameters.Add(ParameterName, ParameterValue);
}

void UFirebaseAnalyticsSubsystem::PutInteger(
	FBundle& Bundle, 
	const FString& ParameterName, 
	const int ParameterValue)
{
	Bundle.IntegerParameters.Add(ParameterName, ParameterValue);
}

void UFirebaseAnalyticsSubsystem::PutBundles(
	FBundle& Bundle,
	const FString& ParameterName,
	const TArray<FBundle>& ParameterValue)
{
	Bundle.BundlesParameters.Add(ParameterName, ParameterValue);
}

TMap<EBuiltinEventNames, FString> UFirebaseAnalyticsSubsystem::GetBuiltinEventNames()
{
	TMap<EBuiltinEventNames, FString> BuiltinNames;
	BuiltinNames.Add(EBuiltinEventNames::ADD_PAYMENT_INFO,		"add_payment_info");
	BuiltinNames.Add(EBuiltinEventNames::ADD_SHIPPING_INFO,		"add_shipping_info");
	BuiltinNames.Add(EBuiltinEventNames::ADD_TO_CART,			"add_to_cart");
	BuiltinNames.Add(EBuiltinEventNames::ADD_TO_WISHLIST,		"add_to_wishlist");
	BuiltinNames.Add(EBuiltinEventNames::AD_IMPRESSION,			"ad_impression");
	BuiltinNames.Add(EBuiltinEventNames::APP_OPEN,				"app_open");
	BuiltinNames.Add(EBuiltinEventNames::BEGIN_CHECKOUT,		"begin_checkout");
	BuiltinNames.Add(EBuiltinEventNames::CAMPAIGN_DETAILS,		"campaign_details");
	BuiltinNames.Add(EBuiltinEventNames::CHECKOUT_PROGRESS,		"checkout_progress");
	BuiltinNames.Add(EBuiltinEventNames::EARN_VIRTUAL_CURRENCY,	"earn_virtual_currency");
	BuiltinNames.Add(EBuiltinEventNames::ECOMMERCE_PURCHASE,	"ecommerce_purchase");
	BuiltinNames.Add(EBuiltinEventNames::GENERATE_LEAD,			"generate_lead");
	BuiltinNames.Add(EBuiltinEventNames::JOIN_GROUP,			"join_group");
	BuiltinNames.Add(EBuiltinEventNames::LEVEL_END,				"level_end");
	BuiltinNames.Add(EBuiltinEventNames::LEVEL_START,			"level_start");
	BuiltinNames.Add(EBuiltinEventNames::LEVEL_UP,				"level_up");
	BuiltinNames.Add(EBuiltinEventNames::LOGIN,					"login");
	BuiltinNames.Add(EBuiltinEventNames::POST_SCORE,			"post_score");
	BuiltinNames.Add(EBuiltinEventNames::PRESENT_OFFER,			"present_offer");
	BuiltinNames.Add(EBuiltinEventNames::PURCHASE,				"purchase");
	BuiltinNames.Add(EBuiltinEventNames::PURCHASE_REFUND,		"purchase_refund");
	BuiltinNames.Add(EBuiltinEventNames::REFUND,				"refund");
	BuiltinNames.Add(EBuiltinEventNames::REMOVE_FROM_CART,		"remove_from_cart");
	BuiltinNames.Add(EBuiltinEventNames::SCREEN_VIEW,			"screen_view");
	BuiltinNames.Add(EBuiltinEventNames::SEARCH,				"search");
	BuiltinNames.Add(EBuiltinEventNames::SELECT_CONTENT,		"select_content");
	BuiltinNames.Add(EBuiltinEventNames::SELECT_ITEM,			"select_item");
	BuiltinNames.Add(EBuiltinEventNames::SELECT_PROMOTION,		"select_promotion");
	BuiltinNames.Add(EBuiltinEventNames::SET_CHECKOUT_OPTION,	"set_checkout_option");
	BuiltinNames.Add(EBuiltinEventNames::SHARE,					"share");
	BuiltinNames.Add(EBuiltinEventNames::SIGN_UP,				"sign_up");
	BuiltinNames.Add(EBuiltinEventNames::SPEND_VIRTUAL_CURRENCY,"spend_virtual_currency");
	BuiltinNames.Add(EBuiltinEventNames::TUTORIAL_BEGIN,		"tutorial_begin");
	BuiltinNames.Add(EBuiltinEventNames::TUTORIAL_COMPLETE,		"tutorial_complete");
	BuiltinNames.Add(EBuiltinEventNames::UNLOCK_ACHIEVEMENT,	"unlock_achievement");
	BuiltinNames.Add(EBuiltinEventNames::VIEW_CART,				"view_cart");
	BuiltinNames.Add(EBuiltinEventNames::VIEW_ITEM,				"view_item");
	BuiltinNames.Add(EBuiltinEventNames::VIEW_ITEM_LIST,		"view_item_list");
	BuiltinNames.Add(EBuiltinEventNames::VIEW_PROMOTION,		"view_promotion");
	BuiltinNames.Add(EBuiltinEventNames::VIEW_SEARCH_RESULTS,	"view_search_results");

	return BuiltinNames;
}

TMap<EBuiltinParamNames, FString> UFirebaseAnalyticsSubsystem::GetBuiltinParamNames()
{
	TMap<EBuiltinParamNames, FString> BuiltinNames;
	BuiltinNames.Add(EBuiltinParamNames::ACHIEVEMENT_ID,		"achievement_id");
	BuiltinNames.Add(EBuiltinParamNames::ACLID,					"aclid");
	BuiltinNames.Add(EBuiltinParamNames::AD_FORMAT,				"ad_format");
	BuiltinNames.Add(EBuiltinParamNames::AD_PLATFORM,			"ad_platform");
	BuiltinNames.Add(EBuiltinParamNames::AD_SOURCE,				"ad_source");
	BuiltinNames.Add(EBuiltinParamNames::AD_UNIT_NAME,			"ad_unit_name");
	BuiltinNames.Add(EBuiltinParamNames::AFFILIATION,			"affiliation");
	BuiltinNames.Add(EBuiltinParamNames::CAMPAIGN,				"campaign");
	BuiltinNames.Add(EBuiltinParamNames::CHARACTER,				"character");
	BuiltinNames.Add(EBuiltinParamNames::CHECKOUT_OPTION,		"checkout_option");
	BuiltinNames.Add(EBuiltinParamNames::CHECKOUT_STEP,			"checkout_step");
	BuiltinNames.Add(EBuiltinParamNames::CONTENT,				"content");
	BuiltinNames.Add(EBuiltinParamNames::CONTENT_TYPE,			"content_type");
	BuiltinNames.Add(EBuiltinParamNames::COUPON,				"coupon");
	BuiltinNames.Add(EBuiltinParamNames::CP1,					"cp1");
	BuiltinNames.Add(EBuiltinParamNames::CREATIVE_NAME,			"creative_name");
	BuiltinNames.Add(EBuiltinParamNames::CREATIVE_SLOT,			"creative_slot");
	BuiltinNames.Add(EBuiltinParamNames::CURRENCY,				"currency");
	BuiltinNames.Add(EBuiltinParamNames::DESTINATION,			"destination");
	BuiltinNames.Add(EBuiltinParamNames::DISCOUNT,				"discount");
	BuiltinNames.Add(EBuiltinParamNames::END_DATE,				"end_date");
	BuiltinNames.Add(EBuiltinParamNames::EXTEND_SESSION,		"extend_session");
	BuiltinNames.Add(EBuiltinParamNames::FLIGHT_NUMBER,			"flight_number");
	BuiltinNames.Add(EBuiltinParamNames::GROUP_ID,				"group_id");
	BuiltinNames.Add(EBuiltinParamNames::INDEX,					"index");
	BuiltinNames.Add(EBuiltinParamNames::ITEMS,					"items");
	BuiltinNames.Add(EBuiltinParamNames::ITEM_BRAND,			"item_brand");
	BuiltinNames.Add(EBuiltinParamNames::ITEM_CATEGORY,			"item_category");
	BuiltinNames.Add(EBuiltinParamNames::ITEM_CATEGORY2,		"item_category2");
	BuiltinNames.Add(EBuiltinParamNames::ITEM_CATEGORY3,		"item_category3");
	BuiltinNames.Add(EBuiltinParamNames::ITEM_CATEGORY4,		"item_category4");
	BuiltinNames.Add(EBuiltinParamNames::ITEM_CATEGORY5,		"item_category5");
	BuiltinNames.Add(EBuiltinParamNames::ITEM_ID,				"item_id");
	BuiltinNames.Add(EBuiltinParamNames::ITEM_LIST,				"item_list");
	BuiltinNames.Add(EBuiltinParamNames::ITEM_LIST_ID,			"item_list_id");
	BuiltinNames.Add(EBuiltinParamNames::ITEM_LIST_NAME,		"item_list_name");
	BuiltinNames.Add(EBuiltinParamNames::ITEM_LOCATION_ID,		"item_location_id");
	BuiltinNames.Add(EBuiltinParamNames::ITEM_NAME,				"item_name");
	BuiltinNames.Add(EBuiltinParamNames::ITEM_VARIANT,			"item_variant");
	BuiltinNames.Add(EBuiltinParamNames::LEVEL,					"level");
	BuiltinNames.Add(EBuiltinParamNames::LEVEL_NAME,			"level_name");
	BuiltinNames.Add(EBuiltinParamNames::LOCATION,				"location");
	BuiltinNames.Add(EBuiltinParamNames::LOCATION_ID,			"location_id");
	BuiltinNames.Add(EBuiltinParamNames::MEDIUM,				"medium");
	BuiltinNames.Add(EBuiltinParamNames::METHOD,				"method");
	BuiltinNames.Add(EBuiltinParamNames::NUMBER_OF_NIGHTS,		"number_of_nights");
	BuiltinNames.Add(EBuiltinParamNames::NUMBER_OF_PASSENGERS,	"number_of_passengers");
	BuiltinNames.Add(EBuiltinParamNames::NUMBER_OF_ROOMS,		"number_of_rooms");
	BuiltinNames.Add(EBuiltinParamNames::ORIGIN,				"origin");
	BuiltinNames.Add(EBuiltinParamNames::PAYMENT_TYPE,			"payment_type");
	BuiltinNames.Add(EBuiltinParamNames::PRICE,					"price");
	BuiltinNames.Add(EBuiltinParamNames::PROMOTION_ID,			"promotion_id");
	BuiltinNames.Add(EBuiltinParamNames::PROMOTION_NAME,		"promotion_name");
	BuiltinNames.Add(EBuiltinParamNames::QUANTITY,				"quantity");
	BuiltinNames.Add(EBuiltinParamNames::SCORE,					"score");
	BuiltinNames.Add(EBuiltinParamNames::SCREEN_CLASS,			"screen_class");
	BuiltinNames.Add(EBuiltinParamNames::SCREEN_NAME,			"screen_name");
	BuiltinNames.Add(EBuiltinParamNames::SEARCH_TERM,			"search_term");
	BuiltinNames.Add(EBuiltinParamNames::SHIPPING,				"shipping");
	BuiltinNames.Add(EBuiltinParamNames::SHIPPING_TIER,			"shipping_tier");
	BuiltinNames.Add(EBuiltinParamNames::SIGN_UP_METHOD,		"sign_up_method");
	BuiltinNames.Add(EBuiltinParamNames::SOURCE,				"source");
	BuiltinNames.Add(EBuiltinParamNames::START_DATE,			"start_date");
	BuiltinNames.Add(EBuiltinParamNames::SUCCESS,				"success");
	BuiltinNames.Add(EBuiltinParamNames::TAX,					"tax");
	BuiltinNames.Add(EBuiltinParamNames::TERM,					"term");
	BuiltinNames.Add(EBuiltinParamNames::TRANSACTION_ID,		"transaction_id");
	BuiltinNames.Add(EBuiltinParamNames::TRAVEL_CLASS,			"travel_class");
	BuiltinNames.Add(EBuiltinParamNames::VALUE,					"value");
	BuiltinNames.Add(EBuiltinParamNames::VIRTUAL_CURRENCY_NAME,	"virtual_currency_name");

	return BuiltinNames;
}

#if PLATFORM_ANDROID

JNI_METHOD void Java_com_epicgames_ue4_GameActivity_NativeInitialize(
	JNIEnv* Env,
	jobject Thiz)
{
	// Find methods in game activity
    LogEvent_MethodID						= FindMethod(Env, "AndroidThunkJava_LogEvent",						"(Ljava/lang/String;)V");
    LogEventWithStringParameter_MethodID	= FindMethod(Env, "AndroidThunkJava_LogEventWithParameter",			"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    LogEventWithFloatParameter_MethodID		= FindMethod(Env, "AndroidThunkJava_LogEventWithParameter",			"(Ljava/lang/String;Ljava/lang/String;F)V");
    LogEventWithIntegerParameter_MethodID	= FindMethod(Env, "AndroidThunkJava_LogEventWithParameter",			"(Ljava/lang/String;Ljava/lang/String;I)V");
    LogEventWithParameters_MethodID			= FindMethod(Env, "AndroidThunkJava_LogEventWithParameters",		"(Ljava/lang/String;Landroid/os/Bundle;)V");
    ResetAnalyticsData_MethodID				= FindMethod(Env, "AndroidThunkJava_ResetAnalyticsData",			"()V");
    SetAnalyticsCollectionEnabled_MethodID	= FindMethod(Env, "AndroidThunkJava_SetAnalyticsCollectionEnabled", "(Z)V");
    SetSessionTimeoutDuration_MethodID		= FindMethod(Env, "AndroidThunkJava_SetSessionTimeoutDuration",		"(I)V");
    SetUserID_MethodID						= FindMethod(Env, "AndroidThunkJava_SetUserID",						"(Ljava/lang/String;)V");
    SetUserProperty_MethodID				= FindMethod(Env, "AndroidThunkJava_SetUserProperty",				"(Ljava/lang/String;Ljava/lang/String;)V");
	SetDefaultEventParameters_MethodID		= FindMethod(Env, "AndroidThunkJava_SetDefaultEventParameters",		"(Landroid/os/Bundle;)V");
	
	// Find methods in Bundle class
	ParcelableClassID						= FJavaWrapper::FindClassGlobalRef(Env, "android/os/Parcelable", false);
	BundleClassID							= FJavaWrapper::FindClassGlobalRef(Env, "android/os/Bundle", false);
	Bundle_Constructor_MethodID				= FindMethodInSpecificClass(Env, BundleClassID, "<init>",				"()V");
	Bundle_PutString_MethodID				= FindMethodInSpecificClass(Env, BundleClassID, "putString",			"(Ljava/lang/String;Ljava/lang/String;)V");
	Bundle_PutFloat_MethodID				= FindMethodInSpecificClass(Env, BundleClassID, "putFloat",				"(Ljava/lang/String;F)V");
	Bundle_PutInteger_MethodID				= FindMethodInSpecificClass(Env, BundleClassID, "putInt",				"(Ljava/lang/String;I)V");
	Bundle_PutParcelableArray_MethodID		= FindMethodInSpecificClass(Env, BundleClassID, "putParcelableArray",	"(Ljava/lang/String;[Landroid/os/Parcelable;)V");
}

#endif
