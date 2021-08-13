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
	//if (Env && Name && Signature && Class)
	//{
		return Env->GetMethodID(Class, Name, Signature);
	//}

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
	UPARAM(ref) FBundle& Bundle)
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

	// Find methods in Bundle class
	BundleClassID							= FJavaWrapper::FindClassGlobalRef(Env, "android/os/Bundle", false);
	Bundle_Constructor_MethodID				= FindMethodInSpecificClass(Env, BundleClassID, "<init>",			"()V");
	Bundle_PutString_MethodID				= FindMethodInSpecificClass(Env, BundleClassID, "putString",		"(Ljava/lang/String;Ljava/lang/String;)V");
	Bundle_PutFloat_MethodID				= FindMethodInSpecificClass(Env, BundleClassID, "putFloat",			"(Ljava/lang/String;F)V");
	Bundle_PutInteger_MethodID				= FindMethodInSpecificClass(Env, BundleClassID, "putInt",			"(Ljava/lang/String;I)V");
}

#endif
