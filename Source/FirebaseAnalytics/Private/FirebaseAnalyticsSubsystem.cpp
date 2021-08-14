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
jclass BundleClassID;

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
		// Initialize Bundle class
		auto JBundle = NewScopedJavaObject(Env, Env->NewObject(BundleClassID, Bundle_Constructor_MethodID));
		
		// Adding string parameter to Bundle
		for (auto& Parameter : Bundle.StringParameters)
		{
		    auto ParameterName = FJavaHelper::ToJavaString(Env, Parameter.Key);
		    auto ParameterValue = FJavaHelper::ToJavaString(Env, Parameter.Value);
			
			CallVoidObjectMethod(
				Env, 
				*JBundle, 
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
				*JBundle, 
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
				*JBundle, 
				Bundle_PutInteger_MethodID, 
				*ParameterName, 
				Parameter.Value);
		}
		
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
		// Initialize Bundle class
		auto JBundle = NewScopedJavaObject(Env, Env->NewObject(BundleClassID, Bundle_Constructor_MethodID));

		GEngine->AddOnScreenDebugMessage(-1, 1000.0f, FColor::Red, FString::FromInt(Bundle.StringParameters.Num()));
		GEngine->AddOnScreenDebugMessage(-1, 1000.0f, FColor::Red, FString::FromInt(Bundle.FloatParameters.Num()));
		GEngine->AddOnScreenDebugMessage(-1, 1000.0f, FColor::Red, FString::FromInt(Bundle.IntegerParameters.Num()));
		
		// Adding string parameter to Bundle
		for (auto& Parameter : Bundle.StringParameters)
		{
		    auto ParameterName = FJavaHelper::ToJavaString(Env, Parameter.Key);
		    auto ParameterValue = FJavaHelper::ToJavaString(Env, Parameter.Value);
			
			CallVoidObjectMethod(
				Env, 
				*JBundle, 
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
				*JBundle, 
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
				*JBundle, 
				Bundle_PutInteger_MethodID, 
				*ParameterName, 
				Parameter.Value);
		}
		
		CallVoidMethod(
			Env, 
			SetDefaultEventParameters_MethodID,
			*JBundle);
	}
#endif

}

void UFirebaseAnalyticsSubsystem::PutString(
	UPARAM(ref) FBundle& Bundle, 
	const FString& ParameterName, 
	const FString& ParameterValue)
{
	Bundle.StringParameters.Add(ParameterName, ParameterValue);
}

void UFirebaseAnalyticsSubsystem::PutFloat(
	UPARAM(ref) FBundle& Bundle, 
	const FString& ParameterName, 
	const float ParameterValue)
{
	Bundle.FloatParameters.Add(ParameterName, ParameterValue);
}

void UFirebaseAnalyticsSubsystem::PutInteger(
	UPARAM(ref) FBundle& Bundle, 
	const FString& ParameterName, 
	const int ParameterValue)
{
	Bundle.IntegerParameters.Add(ParameterName, ParameterValue);
}

TMap<TEnumAsByte<EBuiltinEventNames>, FName> UFirebaseAnalyticsSubsystem::GetBuiltinEventNames()
{
	TMap<TEnumAsByte<EBuiltinEventNames>, FName> BuiltinNames;
	BuiltinNames.Add(ADD_PAYMENT_INFO,		"add_payment_info");
	BuiltinNames.Add(ADD_SHIPPING_INFO,		"add_shipping_info");
	BuiltinNames.Add(ADD_TO_CART,				"add_to_cart");
	BuiltinNames.Add(ADD_TO_WISHLIST,			"add_to_wishlist");
	BuiltinNames.Add(AD_IMPRESSION,			"ad_impression");
	BuiltinNames.Add(APP_OPEN,				"app_open");
	BuiltinNames.Add(BEGIN_CHECKOUT,			"begin_checkout");
	BuiltinNames.Add(CAMPAIGN_DETAILS,		"campaign_details");
	BuiltinNames.Add(CHECKOUT_PROGRESS,		"checkout_progress");
	BuiltinNames.Add(EARN_VIRTUAL_CURRENCY,	"earn_virtual_currency");
	BuiltinNames.Add(ECOMMERCE_PURCHASE,		"ecommerce_purchase");
	BuiltinNames.Add(GENERATE_LEAD,			"generate_lead");
	BuiltinNames.Add(JOIN_GROUP,				"join_group");
	BuiltinNames.Add(LEVEL_END,				"level_end");
	BuiltinNames.Add(LEVEL_START,				"level_start");
	BuiltinNames.Add(LEVEL_UP,				"level_up");
	BuiltinNames.Add(LOGIN,					"login");
	BuiltinNames.Add(POST_SCORE,				"post_score");
	BuiltinNames.Add(PRESENT_OFFER,			"present_offer");
	BuiltinNames.Add(PURCHASE,				"purchase");
	BuiltinNames.Add(PURCHASE_REFUND,			"purchase_refund");
	BuiltinNames.Add(REFUND,					"refund");
	BuiltinNames.Add(REMOVE_FROM_CART,		"remove_from_cart");
	BuiltinNames.Add(SCREEN_VIEW,				"screen_view");
	BuiltinNames.Add(SEARCH,					"search");
	BuiltinNames.Add(SELECT_CONTENT,			"select_content");
	BuiltinNames.Add(SELECT_ITEM,				"select_item");
	BuiltinNames.Add(SELECT_PROMOTION,		"select_promotion");
	BuiltinNames.Add(SET_CHECKOUT_OPTION,		"set_checkout_option");
	BuiltinNames.Add(SHARE,					"share");
	BuiltinNames.Add(SIGN_UP,					"sign_up");
	BuiltinNames.Add(SPEND_VIRTUAL_CURRENCY,	"spend_virtual_currency");
	BuiltinNames.Add(TUTORIAL_BEGIN,			"tutorial_begin");
	BuiltinNames.Add(TUTORIAL_COMPLETE,		"tutorial_complete");
	BuiltinNames.Add(UNLOCK_ACHIEVEMENT,		"unlock_achievement");
	BuiltinNames.Add(VIEW_CART,				"view_cart");
	BuiltinNames.Add(VIEW_ITEM,				"view_item");
	BuiltinNames.Add(VIEW_ITEM_LIST,			"view_item_list");
	BuiltinNames.Add(VIEW_PROMOTION,			"view_promotion");
	BuiltinNames.Add(VIEW_SEARCH_RESULTS,		"view_search_results");

	return BuiltinNames;
}

TMap<TEnumAsByte<EBuiltinParamNames>, FName> UFirebaseAnalyticsSubsystem::GetBuiltinParamNames()
{
	TMap<TEnumAsByte<EBuiltinParamNames>, FName> BuiltinNames;
	BuiltinNames.Add(ACHIEVEMENT_ID,			"achievement_id");
	BuiltinNames.Add(ACLID,					"aclid");
	BuiltinNames.Add(AD_FORMAT,				"ad_format");
	BuiltinNames.Add(AD_PLATFORM,				"ad_platform");
	BuiltinNames.Add(AD_SOURCE,				"ad_source");
	BuiltinNames.Add(AD_UNIT_NAME,			"ad_unit_name");
	BuiltinNames.Add(AFFILIATION,				"affiliation");
	BuiltinNames.Add(CAMPAIGN,				"campaign");
	BuiltinNames.Add(CHARACTER,				"character");
	BuiltinNames.Add(CHECKOUT_OPTION,			"checkout_option");
	BuiltinNames.Add(CHECKOUT_STEP,			"checkout_step");
	BuiltinNames.Add(CONTENT,					"content");
	BuiltinNames.Add(CONTENT_TYPE,			"content_type");
	BuiltinNames.Add(COUPON,					"coupon");
	BuiltinNames.Add(CP1,						"cp1");
	BuiltinNames.Add(CREATIVE_NAME,			"creative_name");
	BuiltinNames.Add(CREATIVE_SLOT,			"creative_slot");
	BuiltinNames.Add(CURRENCY,				"currency");
	BuiltinNames.Add(DESTINATION,				"destination");
	BuiltinNames.Add(DISCOUNT,				"discount");
	BuiltinNames.Add(END_DATE,				"end_date");
	BuiltinNames.Add(EXTEND_SESSION,			"extend_session");
	BuiltinNames.Add(FLIGHT_NUMBER,			"flight_number");
	BuiltinNames.Add(GROUP_ID,				"group_id");
	BuiltinNames.Add(INDEX,					"index");
	BuiltinNames.Add(ITEMS,					"items");
	BuiltinNames.Add(ITEM_BRAND,				"item_brand");
	BuiltinNames.Add(ITEM_CATEGORY,			"item_category");
	BuiltinNames.Add(ITEM_CATEGORY2,			"item_category2");
	BuiltinNames.Add(ITEM_CATEGORY3,			"item_category3");
	BuiltinNames.Add(ITEM_CATEGORY4,			"item_category4");
	BuiltinNames.Add(ITEM_CATEGORY5,			"item_category5");
	BuiltinNames.Add(ITEM_ID,					"item_id");
	BuiltinNames.Add(ITEM_LIST,				"item_list");
	BuiltinNames.Add(ITEM_LIST_ID,			"item_list_id");
	BuiltinNames.Add(ITEM_LIST_NAME,			"item_list_name");
	BuiltinNames.Add(ITEM_LOCATION_ID,		"item_location_id");
	BuiltinNames.Add(ITEM_NAME,				"item_name");
	BuiltinNames.Add(ITEM_VARIANT,			"item_variant");
	BuiltinNames.Add(LEVEL,					"level");
	BuiltinNames.Add(LEVEL_NAME,				"level_name");
	BuiltinNames.Add(LOCATION,				"location");
	BuiltinNames.Add(LOCATION_ID,				"location_id");
	BuiltinNames.Add(MEDIUM,					"medium");
	BuiltinNames.Add(METHOD,					"method");
	BuiltinNames.Add(NUMBER_OF_NIGHTS,		"number_of_nights");
	BuiltinNames.Add(NUMBER_OF_PASSENGERS,	"number_of_passengers");
	BuiltinNames.Add(NUMBER_OF_ROOMS,			"number_of_rooms");
	BuiltinNames.Add(ORIGIN,					"origin");
	BuiltinNames.Add(PAYMENT_TYPE,			"payment_type");
	BuiltinNames.Add(PRICE,					"price");
	BuiltinNames.Add(PROMOTION_ID,			"promotion_id");
	BuiltinNames.Add(PROMOTION_NAME,			"promotion_name");
	BuiltinNames.Add(QUANTITY,				"quantity");
	BuiltinNames.Add(SCORE,					"score");
	BuiltinNames.Add(SCREEN_CLASS,			"screen_class");
	BuiltinNames.Add(SCREEN_NAME,				"screen_name");
	BuiltinNames.Add(SEARCH_TERM,				"search_term");
	BuiltinNames.Add(SHIPPING,				"shipping");
	BuiltinNames.Add(SHIPPING_TIER,			"shipping_tier");
	BuiltinNames.Add(SIGN_UP_METHOD,			"sign_up_method");
	BuiltinNames.Add(SOURCE,					"source");
	BuiltinNames.Add(START_DATE,				"start_date");
	BuiltinNames.Add(SUCCESS,					"success");
	BuiltinNames.Add(TAX,						"tax");
	BuiltinNames.Add(TERM,					"term");
	BuiltinNames.Add(TRANSACTION_ID,			"transaction_id");
	BuiltinNames.Add(TRAVEL_CLASS,			"travel_class");
	BuiltinNames.Add(VALUE,					"value");
	BuiltinNames.Add(VIRTUAL_CURRENCY_NAME,	"virtual_currency_name");

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
	BundleClassID							= FJavaWrapper::FindClassGlobalRef(Env, "android/os/Bundle", false);
	Bundle_Constructor_MethodID				= FindMethodInSpecificClass(Env, BundleClassID, "<init>",			"()V");
	Bundle_PutString_MethodID				= FindMethodInSpecificClass(Env, BundleClassID, "putString",		"(Ljava/lang/String;Ljava/lang/String;)V");
	Bundle_PutFloat_MethodID				= FindMethodInSpecificClass(Env, BundleClassID, "putFloat",			"(Ljava/lang/String;F)V");
	Bundle_PutInteger_MethodID				= FindMethodInSpecificClass(Env, BundleClassID, "putInt",			"(Ljava/lang/String;I)V");
}

#endif
