// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "KScriptEngine.h"
#include "KScriptCommandBase.generated.h"

class UKScriptEngine;
class UKScriptVariable;

/**
 * KScriptコマンドの基底クラス
 * Commandパターンを使用して、各コマンドタイプを独立したクラスとして実装
 */
UCLASS(Abstract, BlueprintType)
class KSCRIPT_API UKScriptCommandBase : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * コマンドを実行
	 * @param Engine スクリプトエンジン
	 * @param Command 実行するコマンド情報
	 * @param VariableManager 変数管理システム
	 */
	virtual void Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
	{
		// 基底クラスでは何もしない（サブクラスでオーバーライド）
	}

	/**
	 * このコマンドが処理できるコマンドタイプを返す
	 */
	virtual EKScriptCommandType GetCommandType() const
	{
		return EKScriptCommandType::Unknown;
	}
};
