// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Commands/KScriptCommandBase.h"
#include "KScriptCallCommand.generated.h"

/**
 * サブルーチン呼び出しコマンド [call]
 */
UCLASS()
class KSCRIPT_API UKScriptCallCommand : public UKScriptCommandBase
{
	GENERATED_BODY()

public:
	virtual void Execute(class UKScriptEngine* Engine, const FKScriptCommand* Command, class UKScriptVariable* VariableManager) override;
	virtual EKScriptCommandType GetCommandType() const override { return EKScriptCommandType::Call; }
};
