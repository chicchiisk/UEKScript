// Copyright Epic Games, Inc. All Rights Reserved.

#include "Commands/KScriptPlaySeCommand.h"
#include "KScript.h"
#include "KScriptEngine.h"
#include "KScriptVariable.h"
#include "KScriptUIManager.h"
#include "KScriptAudioManager.h"

void UKScriptPlaySeCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command,
                                    UKScriptVariable* VariableManager)
{
	const FString* Storage = Command.Parameters.Find(TEXT("storage"));
	const FString* Loop = Command.Parameters.Find(TEXT("loop"));
	const FString* Buf = Command.Parameters.Find(TEXT("buf"));

	if (Storage)
	{
		bool bLoop = Loop ? Loop->Equals(TEXT("true"), ESearchCase::IgnoreCase) : false;
		UE_LOG(LogKScript, Log, TEXT("[SE再生] ファイル: %s, ループ: %s, バッファ: %s"),
		       **Storage, bLoop ? TEXT("有効") : TEXT("無効"), Buf ? **Buf : TEXT("デフォルト"));

		// ボリュームパラメータを取得（オプション、デフォルト: 1.0）
		float Volume = 1.0f;
		const FString* VolumeStr = Command.Parameters.Find(TEXT("volume"));
		if (VolumeStr)
		{
			Volume = FCString::Atof(**VolumeStr) / 100.0f; // ティラノスクリプトでは0-100の範囲
		}

		// UIManagerを使用してSEを再生
		UKScriptAudioManager* AudioManager = GetWorld()->GetSubsystem<UKScriptAudioManager>();
		if (AudioManager)
		{
			if (AudioManager->PlaySE(*Storage, Volume))
			{
				UE_LOG(LogKScript, Log, TEXT("SEの再生に成功しました"));
			}
			else
			{
				UE_LOG(LogKScript, Warning, TEXT("SEの再生に失敗しました: %s"), **Storage);
			}
		}
		else
		{
			UE_LOG(LogKScript, Error, TEXT("AudioManagerが初期化されていません"));
		}
	}
	else
	{
		UE_LOG(LogKScript, Warning, TEXT("SE再生コマンドに 'storage' パラメータがありません"));
	}
}
