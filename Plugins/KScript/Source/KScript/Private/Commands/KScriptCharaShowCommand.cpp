// Copyright Epic Games, Inc. All Rights Reserved.

#include "Commands/KScriptCharaShowCommand.h"
#include "KScript.h"
#include "KScriptEngine.h"
#include "KScriptVariable.h"
#include "KScriptUIManager.h"
#include "KScriptImageManager.h"

void UKScriptCharaShowCommand::Execute(class UKScriptEngine* Engine, const FKScriptCommand* Command,
                                       class UKScriptVariable* VariableManager)
{
	const FString* Name = Command->Parameters.Find(TEXT("name"));
	const FString* Storage = Command->Parameters.Find(TEXT("storage"));
	const FString* Layer = Command->Parameters.Find(TEXT("layer"));

	if (Name && Storage)
	{
		UE_LOG(LogKScript, Log, TEXT("[キャラクター表示] 名前: %s, ファイル: %s, レイヤー: %s"),
		       **Name, **Storage, Layer ? **Layer : TEXT("デフォルト"));

		// 位置パラメータを取得（オプション）
		FVector2D Position = FVector2D::ZeroVector;
		const FString* Left = Command->Parameters.Find(TEXT("left"));
		const FString* Top = Command->Parameters.Find(TEXT("top"));
		if (Left)
		{
			Position.X = FCString::Atof(**Left);
		}
		if (Top)
		{
			Position.Y = FCString::Atof(**Top);
		}

		// UIManagerを使用してキャラクター画像を表示
		UKScriptImageManager* ImageManager = GetWorld()->GetSubsystem<UKScriptImageManager>();
		if (ImageManager)
		{
			if (ImageManager->ShowCharacter(*Name, *Storage, Position))
			{
				UE_LOG(LogKScript, Log, TEXT("キャラクター画像の表示に成功しました"));
			}
			else
			{
				UE_LOG(LogKScript, Warning, TEXT("キャラクター画像の表示に失敗しました: %s"), **Name);
			}
		}
		else
		{
			UE_LOG(LogKScript, Error, TEXT("ImageManagerが初期化されていません"));
		}
	}
	else
	{
		UE_LOG(LogKScript, Warning, TEXT("キャラクター表示コマンドに必要なパラメータがありません"));
	}
}
