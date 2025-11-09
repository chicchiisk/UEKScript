// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Commands/KScriptCommandBase.h"
#include "KScriptTransCommand.generated.h"

/**
 * [trans] コマンド - トランジション効果を設定する
 * 次に実行される画像表示コマンド([bg], [chara_show]など)で使用されるトランジション効果を設定
 *
 * 使用例:
 * [trans method="fade" time="500"]
 * [bg storage="bg01.png"]  ; <- この背景表示にフェードが適用される
 *
 * パラメータ:
 * - method: トランジションの種類 (fade, crossfade, slide_left, slide_right, slide_up, slide_down)
 * - time: トランジション時間（ミリ秒）デフォルトは500ms
 */
UCLASS()
class KSCRIPT_API UKScriptTransCommand : public UKScriptCommandBase
{
	GENERATED_BODY()

public:
	virtual void Execute(class UKScriptEngine* Engine, const FKScriptCommand* Command, class UKScriptVariable* VariableManager) override;
};
