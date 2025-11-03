// Copyright Epic Games, Inc. All Rights Reserved.

#include "Commands/KScriptCommands.h"
#include "KScript.h"
#include "KScriptEngine.h"
#include "KScriptVariable.h"

// テキスト表示コマンド
void UKScriptTextCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
{
	UE_LOG(LogKScript, Log, TEXT("[テキスト] %s"), *Command.Text);
	// TODO: UIシステムと連携してテキストを表示
}

// クリック待ちコマンド
void UKScriptWaitClickCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
{
	UE_LOG(LogKScript, Log, TEXT("[クリック待ち]"));
	if (Engine)
	{
		Engine->SetExecutionState(EKScriptExecutionState::WaitingInput);
	}
	// TODO: UIにクリック待ちアイコンを表示
}

// ページ区切り待ちコマンド
void UKScriptWaitPageBreakCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
{
	UE_LOG(LogKScript, Log, TEXT("[ページ区切り待ち]"));
	if (Engine)
	{
		Engine->SetExecutionState(EKScriptExecutionState::WaitingInput);
	}
	// TODO: ページ区切り処理
}

// 改行コマンド
void UKScriptLineBreakCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
{
	UE_LOG(LogKScript, Log, TEXT("[改行]"));
	// TODO: UIに改行を送る
}

// メッセージクリアコマンド
void UKScriptClearMessageCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
{
	UE_LOG(LogKScript, Log, TEXT("[メッセージクリア]"));
	// TODO: UIのメッセージをクリア
}

// ジャンプコマンド
void UKScriptJumpCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
{
	const FString* TargetLabel = Command.Parameters.Find(TEXT("target"));
	if (TargetLabel && Engine)
	{
		Engine->JumpToLabel(*TargetLabel);
	}
	else
	{
		UE_LOG(LogKScript, Warning, TEXT("ジャンプコマンドに 'target' パラメータがありません"));
	}
}

// 条件分岐コマンド
void UKScriptIfCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
{
	const FString* Expression = Command.Parameters.Find(TEXT("exp"));
	if (Expression && VariableManager && Engine)
	{
		FKScriptVariableValue Result;
		if (VariableManager->EvaluateExpression(*Expression, Result))
		{
			bool Condition = Result.AsBool();
			UE_LOG(LogKScript, Log, TEXT("[条件分岐] %s = %s"), **Expression, Condition ? TEXT("真") : TEXT("偽"));

			// 条件が偽の場合、対応する[else]または[endif]までスキップ
			if (!Condition)
			{
				Engine->SkipToElseOrEndIf();
			}
		}
		else
		{
			UE_LOG(LogKScript, Warning, TEXT("条件式の評価に失敗しました: %s"), **Expression);
		}
	}
	else
	{
		UE_LOG(LogKScript, Warning, TEXT("Ifコマンドに 'exp' パラメータまたは変数マネージャーがありません"));
	}
}

// Elseコマンド
void UKScriptElseCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
{
	// elseに到達したということは、ifの条件が真だった場合
	// endifまでスキップする
	if (Engine)
	{
		Engine->SkipToEndIf();
	}
	UE_LOG(LogKScript, Log, TEXT("[Else] - endifまでスキップ"));
}

// EndIfコマンド
void UKScriptEndIfCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
{
	// endifは何もしない（条件分岐の終了マーカー）
	UE_LOG(LogKScript, Log, TEXT("[EndIf]"));
}

// 式評価コマンド
void UKScriptEvalCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
{
	const FString* Expression = Command.Parameters.Find(TEXT("exp"));
	if (Expression && VariableManager)
	{
		// 式を評価
		// evalタグは代入式を想定（例: f.hp=100, f.name="太郎"）
		FString TrimmedExpr = Expression->TrimStartAndEnd();
		int32 EqualPos = INDEX_NONE;
		if (TrimmedExpr.FindChar(TEXT('='), EqualPos))
		{
			FString VarName = TrimmedExpr.Left(EqualPos).TrimStartAndEnd();
			FString ValueExpr = TrimmedExpr.Mid(EqualPos + 1).TrimStartAndEnd();

			FKScriptVariableValue Result;
			if (VariableManager->EvaluateExpression(ValueExpr, Result))
			{
				VariableManager->SetVariableValue(VarName, Result);
				UE_LOG(LogKScript, Log, TEXT("[式評価] %s = %s"), *VarName, *Result.AsString());
			}
			else
			{
				UE_LOG(LogKScript, Warning, TEXT("式の評価に失敗しました: %s"), *ValueExpr);
			}
		}
		else
		{
			// 単純な式評価（代入なし）
			FKScriptVariableValue Result;
			if (VariableManager->EvaluateExpression(TrimmedExpr, Result))
			{
				UE_LOG(LogKScript, Log, TEXT("[式評価] 結果: %s"), *Result.AsString());
			}
		}
	}
	else
	{
		UE_LOG(LogKScript, Warning, TEXT("Evalコマンドに 'exp' パラメータまたは変数マネージャーがありません"));
	}
}

// サブルーチン呼び出しコマンド
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

// サブルーチンから戻るコマンド
void UKScriptReturnCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
{
	if (Engine)
	{
		int32 ReturnIndex = Engine->PopCallStack();
		if (ReturnIndex >= 0)
		{
			Engine->SetCurrentCommandIndex(ReturnIndex);
			UE_LOG(LogKScript, Log, TEXT("[Return] インデックス %d に戻ります"), ReturnIndex);
		}
		else
		{
			UE_LOG(LogKScript, Warning, TEXT("コールスタックが空の状態でReturnコマンドが呼ばれました"));
		}
	}
}

// 背景表示コマンド
void UKScriptBgCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
{
	const FString* Storage = Command.Parameters.Find(TEXT("storage"));
	if (Storage)
	{
		UE_LOG(LogKScript, Log, TEXT("[背景表示] ファイル: %s"), **Storage);
		// TODO: 背景画像の読み込みと表示
		// - リソースパスの解決
		// - テクスチャの読み込み
		// - 背景レイヤーへの適用
		// - トランジション効果の適用（time, method パラメータなど）
	}
	else
	{
		UE_LOG(LogKScript, Warning, TEXT("背景表示コマンドに 'storage' パラメータがありません"));
	}
}

// キャラクター表示コマンド
void UKScriptCharaShowCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
{
	const FString* Name = Command.Parameters.Find(TEXT("name"));
	const FString* Storage = Command.Parameters.Find(TEXT("storage"));
	const FString* Layer = Command.Parameters.Find(TEXT("layer"));
	
	if (Name && Storage)
	{
		UE_LOG(LogKScript, Log, TEXT("[キャラクター表示] 名前: %s, ファイル: %s, レイヤー: %s"), 
			**Name, **Storage, Layer ? **Layer : TEXT("デフォルト"));
		// TODO: キャラクター画像の読み込みと表示
		// - リソースパスの解決
		// - テクスチャの読み込み
		// - キャラクターレイヤーへの配置
		// - 位置、サイズ、透明度などの設定（pos, size, opacity パラメータなど）
	}
	else
	{
		UE_LOG(LogKScript, Warning, TEXT("キャラクター表示コマンドに必要なパラメータがありません"));
	}
}

// キャラクター非表示コマンド
void UKScriptCharaHideCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
{
	const FString* Name = Command.Parameters.Find(TEXT("name"));
	const FString* Layer = Command.Parameters.Find(TEXT("layer"));
	
	if (Name)
	{
		UE_LOG(LogKScript, Log, TEXT("[キャラクター非表示] 名前: %s, レイヤー: %s"), 
			**Name, Layer ? **Layer : TEXT("デフォルト"));
		// TODO: キャラクターの非表示処理
		// - 指定されたキャラクターの検索
		// - フェードアウトなどのトランジション適用
		// - レイヤーからの削除
	}
	else
	{
		UE_LOG(LogKScript, Warning, TEXT("キャラクター非表示コマンドに 'name' パラメータがありません"));
	}
}

// BGM再生コマンド
void UKScriptPlayBgmCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
{
	const FString* Storage = Command.Parameters.Find(TEXT("storage"));
	const FString* Loop = Command.Parameters.Find(TEXT("loop"));
	
	if (Storage)
	{
		bool bLoop = Loop ? Loop->Equals(TEXT("true"), ESearchCase::IgnoreCase) : true;
		UE_LOG(LogKScript, Log, TEXT("[BGM再生] ファイル: %s, ループ: %s"), 
			**Storage, bLoop ? TEXT("有効") : TEXT("無効"));
		// TODO: BGMの再生処理
		// - オーディオファイルの読み込み
		// - サウンドコンポーネントでの再生
		// - ループ設定
		// - フェードイン処理（fadein パラメータなど）
	}
	else
	{
		UE_LOG(LogKScript, Warning, TEXT("BGM再生コマンドに 'storage' パラメータがありません"));
	}
}

// BGM停止コマンド
void UKScriptStopBgmCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
{
	const FString* FadeOut = Command.Parameters.Find(TEXT("fadeout"));
	
	UE_LOG(LogKScript, Log, TEXT("[BGM停止] フェードアウト: %s"), 
		FadeOut ? **FadeOut : TEXT("即座"));
	// TODO: BGMの停止処理
	// - 現在再生中のBGMの取得
	// - フェードアウト処理（fadeout パラメータ指定時）
	// - サウンドの停止
}

// SE再生コマンド
void UKScriptPlaySeCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
{
	const FString* Storage = Command.Parameters.Find(TEXT("storage"));
	const FString* Loop = Command.Parameters.Find(TEXT("loop"));
	const FString* Buf = Command.Parameters.Find(TEXT("buf"));
	
	if (Storage)
	{
		bool bLoop = Loop ? Loop->Equals(TEXT("true"), ESearchCase::IgnoreCase) : false;
		UE_LOG(LogKScript, Log, TEXT("[SE再生] ファイル: %s, ループ: %s, バッファ: %s"), 
			**Storage, bLoop ? TEXT("有効") : TEXT("無効"), Buf ? **Buf : TEXT("デフォルト"));
		// TODO: SEの再生処理
		// - オーディオファイルの読み込み
		// - サウンドコンポーネントでの再生
		// - ループ設定
		// - バッファ管理（複数のSEを同時再生する場合）
	}
	else
	{
		UE_LOG(LogKScript, Warning, TEXT("SE再生コマンドに 'storage' パラメータがありません"));
	}
}

