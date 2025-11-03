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

		// UIManagerを使用して背景画像を表示
		if (Engine && Engine->GetUIManager())
		{
			UKScriptImageManager* ImageManager = Engine->GetUIManager()->GetImageManager();
			if (ImageManager)
			{
				if (ImageManager->SetBackground(*Storage))
				{
					UE_LOG(LogKScript, Log, TEXT("背景画像の表示に成功しました"));
				}
				else
				{
					UE_LOG(LogKScript, Warning, TEXT("背景画像の表示に失敗しました: %s"), **Storage);
				}
			}
			else
			{
				UE_LOG(LogKScript, Error, TEXT("ImageManagerが初期化されていません"));
			}
		}
		else
		{
			UE_LOG(LogKScript, Error, TEXT("UIManagerが設定されていません"));
		}
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

		// 位置パラメータを取得（オプション）
		FVector2D Position = FVector2D::ZeroVector;
		const FString* Left = Command.Parameters.Find(TEXT("left"));
		const FString* Top = Command.Parameters.Find(TEXT("top"));
		if (Left)
		{
			Position.X = FCString::Atof(**Left);
		}
		if (Top)
		{
			Position.Y = FCString::Atof(**Top);
		}

		// UIManagerを使用してキャラクター画像を表示
		if (Engine && Engine->GetUIManager())
		{
			UKScriptImageManager* ImageManager = Engine->GetUIManager()->GetImageManager();
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
			UE_LOG(LogKScript, Error, TEXT("UIManagerが設定されていません"));
		}
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

		// UIManagerを使用してキャラクター画像を非表示
		if (Engine && Engine->GetUIManager())
		{
			UKScriptImageManager* ImageManager = Engine->GetUIManager()->GetImageManager();
			if (ImageManager)
			{
				if (ImageManager->HideCharacter(*Name))
				{
					UE_LOG(LogKScript, Log, TEXT("キャラクター画像の非表示に成功しました"));
				}
				else
				{
					UE_LOG(LogKScript, Warning, TEXT("キャラクター '%s' が見つかりません"), **Name);
				}
			}
			else
			{
				UE_LOG(LogKScript, Error, TEXT("ImageManagerが初期化されていません"));
			}
		}
		else
		{
			UE_LOG(LogKScript, Error, TEXT("UIManagerが設定されていません"));
		}
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

		// ボリュームパラメータを取得（オプション、デフォルト: 1.0）
		float Volume = 1.0f;
		const FString* VolumeStr = Command.Parameters.Find(TEXT("volume"));
		if (VolumeStr)
		{
			Volume = FCString::Atof(**VolumeStr) / 100.0f; // ティラノスクリプトでは0-100の範囲
		}

		// UIManagerを使用してBGMを再生
		if (Engine && Engine->GetUIManager())
		{
			UKScriptAudioManager* AudioManager = Engine->GetUIManager()->GetAudioManager();
			if (AudioManager)
			{
				if (AudioManager->PlayBGM(*Storage, bLoop, Volume))
				{
					UE_LOG(LogKScript, Log, TEXT("BGMの再生に成功しました"));
				}
				else
				{
					UE_LOG(LogKScript, Warning, TEXT("BGMの再生に失敗しました: %s"), **Storage);
				}
			}
			else
			{
				UE_LOG(LogKScript, Error, TEXT("AudioManagerが初期化されていません"));
			}
		}
		else
		{
			UE_LOG(LogKScript, Error, TEXT("UIManagerが設定されていません"));
		}
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

	// フェードアウト時間を取得（オプション、デフォルト: 0.0）
	float FadeOutDuration = 0.0f;
	if (FadeOut)
	{
		FadeOutDuration = FCString::Atof(**FadeOut) / 1000.0f; // ミリ秒から秒に変換
	}

	// UIManagerを使用してBGMを停止
	if (Engine && Engine->GetUIManager())
	{
		UKScriptAudioManager* AudioManager = Engine->GetUIManager()->GetAudioManager();
		if (AudioManager)
		{
			AudioManager->StopBGM(FadeOutDuration);
			UE_LOG(LogKScript, Log, TEXT("BGMの停止に成功しました"));
		}
		else
		{
			UE_LOG(LogKScript, Error, TEXT("AudioManagerが初期化されていません"));
		}
	}
	else
	{
		UE_LOG(LogKScript, Error, TEXT("UIManagerが設定されていません"));
	}
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

		// ボリュームパラメータを取得（オプション、デフォルト: 1.0）
		float Volume = 1.0f;
		const FString* VolumeStr = Command.Parameters.Find(TEXT("volume"));
		if (VolumeStr)
		{
			Volume = FCString::Atof(**VolumeStr) / 100.0f; // ティラノスクリプトでは0-100の範囲
		}

		// UIManagerを使用してSEを再生
		if (Engine && Engine->GetUIManager())
		{
			UKScriptAudioManager* AudioManager = Engine->GetUIManager()->GetAudioManager();
			if (AudioManager)
			{
				if (AudioManager->PlaySE(*Storage, Volume))
				{
					UE_LOG(LogKScript, Log, TEXT("SEの再生に成功しました"));
				}
				else
				{
					UE_LOG(LogKScript, Warning, TEXT("SEの再生に失敗しました: %s"), **Storage);
				}
			}
			else
			{
				UE_LOG(LogKScript, Error, TEXT("AudioManagerが初期化されていません"));
			}
		}
		else
		{
			UE_LOG(LogKScript, Error, TEXT("UIManagerが設定されていません"));
		}
	}
	else
	{
		UE_LOG(LogKScript, Warning, TEXT("SE再生コマンドに 'storage' パラメータがありません"));
	}
}

