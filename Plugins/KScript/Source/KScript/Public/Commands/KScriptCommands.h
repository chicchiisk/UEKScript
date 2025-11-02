// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Commands/KScriptCommandBase.h"
#include "KScriptCommands.generated.h"

/**
 * テキスト表示コマンド
 */
UCLASS()
class KSCRIPT_API UKScriptTextCommand : public UKScriptCommandBase
{
	GENERATED_BODY()

public:
	virtual void Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager) override;
	virtual EKScriptCommandType GetCommandType() const override { return EKScriptCommandType::Text; }
};

/**
 * クリック待ちコマンド [l]
 */
UCLASS()
class KSCRIPT_API UKScriptWaitClickCommand : public UKScriptCommandBase
{
	GENERATED_BODY()

public:
	virtual void Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager) override;
	virtual EKScriptCommandType GetCommandType() const override { return EKScriptCommandType::WaitClick; }
};

/**
 * ページ区切り待ちコマンド [p]
 */
UCLASS()
class KSCRIPT_API UKScriptWaitPageBreakCommand : public UKScriptCommandBase
{
	GENERATED_BODY()

public:
	virtual void Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager) override;
	virtual EKScriptCommandType GetCommandType() const override { return EKScriptCommandType::WaitPageBreak; }
};

/**
 * 改行コマンド [r]
 */
UCLASS()
class KSCRIPT_API UKScriptLineBreakCommand : public UKScriptCommandBase
{
	GENERATED_BODY()

public:
	virtual void Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager) override;
	virtual EKScriptCommandType GetCommandType() const override { return EKScriptCommandType::LineBreak; }
};

/**
 * メッセージクリアコマンド [cm]
 */
UCLASS()
class KSCRIPT_API UKScriptClearMessageCommand : public UKScriptCommandBase
{
	GENERATED_BODY()

public:
	virtual void Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager) override;
	virtual EKScriptCommandType GetCommandType() const override { return EKScriptCommandType::ClearMessage; }
};

/**
 * ジャンプコマンド [jump]
 */
UCLASS()
class KSCRIPT_API UKScriptJumpCommand : public UKScriptCommandBase
{
	GENERATED_BODY()

public:
	virtual void Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager) override;
	virtual EKScriptCommandType GetCommandType() const override { return EKScriptCommandType::Jump; }
};

/**
 * 条件分岐コマンド [if]
 */
UCLASS()
class KSCRIPT_API UKScriptIfCommand : public UKScriptCommandBase
{
	GENERATED_BODY()

public:
	virtual void Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager) override;
	virtual EKScriptCommandType GetCommandType() const override { return EKScriptCommandType::If; }
};

/**
 * Elseコマンド [else]
 */
UCLASS()
class KSCRIPT_API UKScriptElseCommand : public UKScriptCommandBase
{
	GENERATED_BODY()

public:
	virtual void Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager) override;
	virtual EKScriptCommandType GetCommandType() const override { return EKScriptCommandType::Else; }
};

/**
 * EndIfコマンド [endif]
 */
UCLASS()
class KSCRIPT_API UKScriptEndIfCommand : public UKScriptCommandBase
{
	GENERATED_BODY()

public:
	virtual void Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager) override;
	virtual EKScriptCommandType GetCommandType() const override { return EKScriptCommandType::EndIf; }
};

/**
 * 式評価コマンド [eval]
 */
UCLASS()
class KSCRIPT_API UKScriptEvalCommand : public UKScriptCommandBase
{
	GENERATED_BODY()

public:
	virtual void Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager) override;
	virtual EKScriptCommandType GetCommandType() const override { return EKScriptCommandType::Eval; }
};

/**
 * サブルーチン呼び出しコマンド [call]
 */
UCLASS()
class KSCRIPT_API UKScriptCallCommand : public UKScriptCommandBase
{
	GENERATED_BODY()

public:
	virtual void Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager) override;
	virtual EKScriptCommandType GetCommandType() const override { return EKScriptCommandType::Call; }
};

/**
 * サブルーチンから戻るコマンド [return]
 */
UCLASS()
class KSCRIPT_API UKScriptReturnCommand : public UKScriptCommandBase
{
	GENERATED_BODY()

public:
	virtual void Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager) override;
	virtual EKScriptCommandType GetCommandType() const override { return EKScriptCommandType::Return; }
};

/**
 * 背景表示コマンド [bg]
 */
UCLASS()
class KSCRIPT_API UKScriptBgCommand : public UKScriptCommandBase
{
	GENERATED_BODY()

public:
	virtual void Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager) override;
	virtual EKScriptCommandType GetCommandType() const override { return EKScriptCommandType::Bg; }
};

/**
 * キャラクター表示コマンド [chara_show]
 */
UCLASS()
class KSCRIPT_API UKScriptCharaShowCommand : public UKScriptCommandBase
{
	GENERATED_BODY()

public:
	virtual void Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager) override;
	virtual EKScriptCommandType GetCommandType() const override { return EKScriptCommandType::CharaShow; }
};

/**
 * キャラクター非表示コマンド [chara_hide]
 */
UCLASS()
class KSCRIPT_API UKScriptCharaHideCommand : public UKScriptCommandBase
{
	GENERATED_BODY()

public:
	virtual void Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager) override;
	virtual EKScriptCommandType GetCommandType() const override { return EKScriptCommandType::CharaHide; }
};

/**
 * BGM再生コマンド [playbgm]
 */
UCLASS()
class KSCRIPT_API UKScriptPlayBgmCommand : public UKScriptCommandBase
{
	GENERATED_BODY()

public:
	virtual void Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager) override;
	virtual EKScriptCommandType GetCommandType() const override { return EKScriptCommandType::PlayBgm; }
};

/**
 * BGM停止コマンド [stopbgm]
 */
UCLASS()
class KSCRIPT_API UKScriptStopBgmCommand : public UKScriptCommandBase
{
	GENERATED_BODY()

public:
	virtual void Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager) override;
	virtual EKScriptCommandType GetCommandType() const override { return EKScriptCommandType::StopBgm; }
};

/**
 * SE再生コマンド [playse]
 */
UCLASS()
class KSCRIPT_API UKScriptPlaySeCommand : public UKScriptCommandBase
{
	GENERATED_BODY()

public:
	virtual void Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager) override;
	virtual EKScriptCommandType GetCommandType() const override { return EKScriptCommandType::PlaySe; }
};
