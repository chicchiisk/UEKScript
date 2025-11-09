// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Commands/KScriptCommandBase.h"
#include "KScriptSaveCommand.generated.h"

/**
 * [save] コマンド - ゲームをセーブする
 *
 * 使用例:
 * [save slot="save01"]
 */
UCLASS()
class KSCRIPT_API UKScriptSaveCommand : public UKScriptCommandBase
{
	GENERATED_BODY()

public:
	virtual void Execute(class UKScriptEngine* Engine, const FKScriptCommand* Command, class UKScriptVariable* VariableManager) override;
};
