// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "KScriptEngine.h"
#include "Commands/KScriptCommandBase.h"
#include "KScriptCommandFactory.generated.h"

/**
 * KScriptコマンドファクトリー
 * コマンドタイプから適切なコマンドインスタンスを生成
 */
UCLASS()
class KSCRIPT_API UKScriptCommandFactory : public UObject
{
	GENERATED_BODY()

public:
	UKScriptCommandFactory();

	/**
	 * コマンドタイプに対応するコマンドインスタンスを取得
	 * @param CommandType コマンドタイプ
	 * @return コマンドインスタンス（シングルトン）
	 */
	UKScriptCommandBase* GetCommand(EKScriptCommandType CommandType);

	/**
	 * すべてのコマンドを登録
	 */
	void RegisterAllCommands();

protected:
	/**
	 * コマンドを登録
	 * @param CommandType コマンドタイプ
	 * @tparam TCommandClass コマンドクラス（UKScriptCommandBaseを継承している必要がある）
	 */
	template <class TCommandClass>
		requires std::is_base_of_v<UKScriptCommandBase, TCommandClass>
	void RegisterCommand(EKScriptCommandType CommandType)
	{
		FString CommandName = FString::Printf(TEXT("Command_%d"), (int32)CommandType);
		UKScriptCommandBase* CommandInstance = CreateDefaultSubobject<TCommandClass>(*CommandName);
		CommandMap.Add(CommandType, CommandInstance);
	}

protected:
	// コマンドタイプとコマンドインスタンスのマップ
	UPROPERTY()
	TMap<EKScriptCommandType, TObjectPtr<UKScriptCommandBase>> CommandMap;
};
