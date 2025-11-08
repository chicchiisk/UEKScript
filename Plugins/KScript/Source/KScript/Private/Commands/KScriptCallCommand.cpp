// Copyright Epic Games, Inc. All Rights Reserved.

#include "Commands/KScriptCallCommand.h"
#include "KScript.h"
#include "KScriptEngine.h"
#include "KScriptVariable.h"

void UKScriptCallCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
{
	const FString* TargetLabel = Command.Parameters.Find(TEXT("target"));
	if (TargetLabel && Engine)
	{
		// 現在の位置をコールスタックにプッシュ
		int32 CurrentIndex = Engine->GetCurrentCommandIndex();
		Engine->PushCallStack(CurrentIndex);
		UE_LOG(LogKScript, Log, TEXT("[サブルーチン呼び出し] %s (戻り先インデックス: %d)"), **TargetLabel, CurrentIndex);

		// ラベルにジャンプ
		if (!Engine->JumpToLabel(*TargetLabel))
		{
			// ジャンプに失敗した場合、スタックから削除
			Engine->PopCallStack();
		}
		else
		{
			// ジャンプ後、インデックスが進んでしまうので1つ戻す
			Engine->SetCurrentCommandIndex(Engine->GetCurrentCommandIndex() - 1);
		}
	}
	else
	{
		UE_LOG(LogKScript, Warning, TEXT("Callコマンドに 'target' パラメータがありません"));
	}
}
