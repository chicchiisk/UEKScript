// Copyright Epic Games, Inc. All Rights Reserved.

#include "Commands/KScriptBgCommand.h"
#include "KScript.h"
#include "KScriptEngine.h"
#include "KScriptVariable.h"
#include "KScriptUIManager.h"
#include "KScriptImageManager.h"

void UKScriptBgCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
{
	const FString* Storage = Command.Parameters.Find(TEXT("storage"));
	if (Storage)
	{
		UE_LOG(LogKScript, Log, TEXT("[背景表示] ファイル: %s"), **Storage);

		// UIManagerを使用して背景画像を表示
		if (Engine && Engine->GetUIManager())
		{
			UKScriptImageManager* ImageManager = Engine->GetUIManager()->GetImageManager();
			if (ImageManager)
			{
				if (ImageManager->SetBackground(*Storage))
				{
					UE_LOG(LogKScript, Log, TEXT("背景画像の表示に成功しました"));
				}
				else
				{
					UE_LOG(LogKScript, Warning, TEXT("背景画像の表示に失敗しました: %s"), **Storage);
				}
			}
			else
			{
				UE_LOG(LogKScript, Error, TEXT("ImageManagerが初期化されていません"));
			}
		}
		else
		{
			UE_LOG(LogKScript, Error, TEXT("UIManagerが設定されていません"));
		}
	}
	else
	{
		UE_LOG(LogKScript, Warning, TEXT("背景表示コマンドに 'storage' パラメータがありません"));
	}
}
