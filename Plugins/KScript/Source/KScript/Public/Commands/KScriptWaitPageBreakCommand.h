// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Commands/KScriptCommandBase.h"
#include "KScriptWaitPageBreakCommand.generated.h"

/**
 * ページ区切り待ちコマンド [p]
 */
UCLASS()
class KSCRIPT_API UKScriptWaitPageBreakCommand : public UKScriptCommandBase
{
	GENERATED_BODY()

public:
	virtual void Execute(class UKScriptEngine* Engine, const FKScriptCommand* Command, class UKScriptVariable* VariableManager) override;
	virtual EKScriptCommandType GetCommandType() const override { return EKScriptCommandType::WaitPageBreak; }
};
