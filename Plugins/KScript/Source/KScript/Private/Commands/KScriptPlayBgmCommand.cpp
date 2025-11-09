// Copyright Epic Games, Inc. All Rights Reserved.

#include "Commands/KScriptPlayBgmCommand.h"
#include "KScript.h"
#include "KScriptEngine.h"
#include "KScriptVariable.h"
#include "KScriptUIManager.h"
#include "KScriptAudioManager.h"

void UKScriptPlayBgmCommand::Execute(class UKScriptEngine* Engine, const FKScriptCommand* Command,
                                     class UKScriptVariable* VariableManager)
{
	const FString* Storage = Command->Parameters.Find(TEXT("storage"));
	const FString* Loop = Command->Parameters.Find(TEXT("loop"));

	if (Storage)
	{
		bool bLoop = Loop ? Loop->Equals(TEXT("true"), ESearchCase::IgnoreCase) : true;
		UE_LOG(LogKScript, Log, TEXT("[BGM再生] ファイル: %s, ループ: %s"),
		       **Storage, bLoop ? TEXT("有効") : TEXT("無効"));

		// ボリュームパラメータを取得（オプション、デフォルト: 1.0）
		float Volume = 1.0f;
		const FString* VolumeStr = Command->Parameters.Find(TEXT("volume"));
		if (VolumeStr)
		{
			Volume = FCString::Atof(**VolumeStr) / 100.0f; // ティラノスクリプトでは0-100の範囲
		}

		// UIManagerを使用してBGMを再生
		UKScriptAudioManager* AudioManager = GetWorld()->GetSubsystem<UKScriptAudioManager>();
		if (AudioManager)
		{
			if (AudioManager->PlayBGM(*Storage, bLoop, Volume))
			{
				UE_LOG(LogKScript, Log, TEXT("BGMの再生に成功しました"));
			}
			else
			{
				UE_LOG(LogKScript, Warning, TEXT("BGMの再生に失敗しました: %s"), **Storage);
			}
		}
		else
		{
			UE_LOG(LogKScript, Error, TEXT("AudioManagerが初期化されていません"));
		}
	}
	else
	{
		UE_LOG(LogKScript, Warning, TEXT("BGM再生コマンドに 'storage' パラメータがありません"));
	}
}
