// Copyright Epic Games, Inc. All Rights Reserved.

#include "Commands/KScriptStopBgmCommand.h"
#include "KScript.h"
#include "KScriptEngine.h"
#include "KScriptVariable.h"
#include "KScriptUIManager.h"
#include "KScriptAudioManager.h"

void UKScriptStopBgmCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command,
                                     UKScriptVariable* VariableManager)
{
	const FString* FadeOut = Command.Parameters.Find(TEXT("fadeout"));

	UE_LOG(LogKScript, Log, TEXT("[BGM停止] フェードアウト: %s"),
	       FadeOut ? **FadeOut : TEXT("即座"));

	// フェードアウト時間を取得（オプション、デフォルト: 0.0）
	float FadeOutDuration = 0.0f;
	if (FadeOut)
	{
		FadeOutDuration = FCString::Atof(**FadeOut) / 1000.0f; // ミリ秒から秒に変換
	}

	// UIManagerを使用してBGMを停止
	UKScriptAudioManager* AudioManager = GetWorld()->GetSubsystem<UKScriptAudioManager>();
	if (AudioManager)
	{
		AudioManager->StopBGM(FadeOutDuration);
		UE_LOG(LogKScript, Log, TEXT("BGMの停止に成功しました"));
	}
	else
	{
		UE_LOG(LogKScript, Error, TEXT("AudioManagerが初期化されていません"));
	}
}
