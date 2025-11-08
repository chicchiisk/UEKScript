// Copyright Epic Games, Inc. All Rights Reserved.

#include "Commands/KScriptCharaHideCommand.h"
#include "KScript.h"
#include "KScriptEngine.h"
#include "KScriptVariable.h"
#include "KScriptUIManager.h"
#include "KScriptImageManager.h"

void UKScriptCharaHideCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command,
                                       UKScriptVariable* VariableManager)
{
	const FString* Name = Command.Parameters.Find(TEXT("name"));
	const FString* Layer = Command.Parameters.Find(TEXT("layer"));

	if (Name)
	{
		UE_LOG(LogKScript, Log, TEXT("[キャラクター非表示] 名前: %s, レイヤー: %s"),
		       **Name, Layer ? **Layer : TEXT("デフォルト"));

		// UIManagerを使用してキャラクター画像を非表示
		UKScriptImageManager* ImageManager = GetWorld()->GetSubsystem<UKScriptImageManager>();
		if (ImageManager)
		{
			if (ImageManager->HideCharacter(*Name))
			{
				UE_LOG(LogKScript, Log, TEXT("キャラクター画像の非表示に成功しました"));
			}
			else
			{
				UE_LOG(LogKScript, Warning, TEXT("キャラクター '%s' が見つかりません"), **Name);
			}
		}
		else
		{
			UE_LOG(LogKScript, Error, TEXT("ImageManagerが初期化されていません"));
		}
	}
	else
	{
		UE_LOG(LogKScript, Warning, TEXT("キャラクター非表示コマンドに 'name' パラメータがありません"));
	}
}
