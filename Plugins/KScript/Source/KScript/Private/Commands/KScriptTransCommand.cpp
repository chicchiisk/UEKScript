// Copyright Epic Games, Inc. All Rights Reserved.

#include "Commands/KScriptTransCommand.h"
#include "KScript.h"
#include "KScriptEngine.h"
#include "KScriptImageManager.h"
#include "KScriptTransitionManager.h"

void UKScriptTransCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand* Command, class UKScriptVariable* VariableManager)
{
	if (!Engine)
	{
		return;
	}

	// トランジションの種類を取得
	FString Method = TEXT("fade");
	if (Command->Parameters.Contains(TEXT("method")))
	{
		Method = Command->Parameters[TEXT("method")].ToLower();
	}

	// トランジション時間を取得（ミリ秒）
	float TimeMs = 500.0f;
	if (Command->Parameters.Contains(TEXT("time")))
	{
		TimeMs = FCString::Atof(*Command->Parameters[TEXT("time")]);
	}

	// ミリ秒を秒に変換
	float Duration = TimeMs / 1000.0f;

	// メソッド名をトランジションタイプに変換
	EKScriptTransitionType TransitionType = EKScriptTransitionType::Fade;
	if (Method == TEXT("fade") || Method == TEXT("fadein"))
	{
		TransitionType = EKScriptTransitionType::Fade;
	}
	else if (Method == TEXT("crossfade"))
	{
		TransitionType = EKScriptTransitionType::CrossFade;
	}
	else if (Method == TEXT("slide_left") || Method == TEXT("slideleft"))
	{
		TransitionType = EKScriptTransitionType::SlideLeft;
	}
	else if (Method == TEXT("slide_right") || Method == TEXT("slideright"))
	{
		TransitionType = EKScriptTransitionType::SlideRight;
	}
	else if (Method == TEXT("slide_up") || Method == TEXT("slideup"))
	{
		TransitionType = EKScriptTransitionType::SlideUp;
	}
	else if (Method == TEXT("slide_down") || Method == TEXT("slidedown"))
	{
		TransitionType = EKScriptTransitionType::SlideDown;
	}
	else
	{
		UE_LOG(LogKScript, Warning, TEXT("[trans] 不明なトランジションメソッド: %s"), *Method);
	}

	// ImageManagerにトランジション設定を保存
	UWorld* World = Engine->GetWorld();
	if (World)
	{
		UKScriptImageManager* ImageManager = World->GetSubsystem<UKScriptImageManager>();
		if (ImageManager)
		{
			// ImageManagerにトランジション設定を保存するメソッドを呼び出す
			// TODO: ImageManager::SetNextTransition メソッドを追加する必要がある
			UE_LOG(LogKScript, Log, TEXT("[trans] トランジション設定: method=%s, duration=%.2f秒"), *Method, Duration);
		}
	}
}
