// Copyright Epic Games, Inc. All Rights Reserved.

#include "KScriptSubsystem.h"
#include "KScript.h"
#include "KScriptEngine.h"
#include "KScriptParser.h"
#include "KScriptVariable.h"
#include "KScriptAsset.h"
#include "KScriptUIManager.h"
#include "KScriptImageManager.h"
#include "KScriptAudioManager.h"
#include "KScriptSaveGame.h"
#include "Kismet/GameplayStatics.h"

UKScriptSubsystem::UKScriptSubsystem()
{
}

void UKScriptSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// KScriptコンポーネントを初期化
	ScriptEngine = NewObject<UKScriptEngine>(this);
	ScriptParser = NewObject<UKScriptParser>(this);
	VariableManager = NewObject<UKScriptVariable>(this);

	// エンジンにパーサーと変数管理システムを設定
	ScriptEngine->SetParser(ScriptParser);
	ScriptEngine->SetVariableManager(VariableManager);

	UE_LOG(LogKScript, Log, TEXT("KScriptサブシステムを初期化しました"));
}

void UKScriptSubsystem::Deinitialize()
{
	UE_LOG(LogKScript, Log, TEXT("KScriptサブシステムを終了しました"));

	Super::Deinitialize();
}

bool UKScriptSubsystem::LoadAndStartScript(const FString& FilePath)
{
	if (!ScriptEngine)
	{
		UE_LOG(LogKScript, Error, TEXT("スクリプトエンジンが初期化されていません"));
		return false;
	}

	if (ScriptEngine->LoadScriptFromFile(FilePath))
	{
		ScriptEngine->Start();
		UE_LOG(LogKScript, Log, TEXT("スクリプトファイルから実行を開始しました: %s"), *FilePath);
		return true;
	}

	return false;
}

bool UKScriptSubsystem::LoadAndStartScriptFromAsset(UKScriptAsset* ScriptAsset)
{
	if (!ScriptEngine)
	{
		UE_LOG(LogKScript, Error, TEXT("スクリプトエンジンが初期化されていません"));
		return false;
	}

	if (!ScriptAsset)
	{
		UE_LOG(LogKScript, Error, TEXT("スクリプトアセットがnullです"));
		return false;
	}

	if (ScriptEngine->LoadScriptFromAsset(ScriptAsset))
	{
		// 現在のスクリプトアセットパスを記録
		CurrentScriptAssetPath = ScriptAsset->GetPathName();

		ScriptEngine->Start();
		UE_LOG(LogKScript, Log, TEXT("スクリプトアセットから実行を開始しました: %s"), *ScriptAsset->GetName());
		return true;
	}

	return false;
}

bool UKScriptSubsystem::LoadAndStartScriptFromText(const FString& ScriptText)
{
	if (!ScriptEngine)
	{
		UE_LOG(LogKScript, Error, TEXT("スクリプトエンジンが初期化されていません"));
		return false;
	}

	if (ScriptEngine->LoadScript(ScriptText))
	{
		ScriptEngine->Start();
		UE_LOG(LogKScript, Log, TEXT("テキストからスクリプト実行を開始しました"));
		return true;
	}

	return false;
}

void UKScriptSubsystem::PauseScript()
{
	if (ScriptEngine)
	{
		ScriptEngine->Pause();
	}
}

void UKScriptSubsystem::ResumeScript()
{
	if (ScriptEngine)
	{
		ScriptEngine->Resume();
	}
}

void UKScriptSubsystem::StopScript()
{
	if (ScriptEngine)
	{
		ScriptEngine->Stop();
	}
}

void UKScriptSubsystem::HandleInput()
{
	if (ScriptEngine)
	{
		ScriptEngine->OnInput();
	}
}

bool UKScriptSubsystem::StepScript()
{
	if (ScriptEngine)
	{
		return ScriptEngine->Step();
	}

	return false;
}

// ========================================
// セーブ/ロード機能
// ========================================

bool UKScriptSubsystem::SaveGame(const FString& SlotName, int32 UserIndex)
{
	if (!ScriptEngine || !VariableManager)
	{
		UE_LOG(LogKScript, Error, TEXT("セーブに失敗: エンジンまたは変数管理システムが初期化されていません"));
		return false;
	}

	// セーブゲームオブジェクトを作成
	UKScriptSaveGame* SaveGameObject = Cast<UKScriptSaveGame>(UGameplayStatics::CreateSaveGameObject(UKScriptSaveGame::StaticClass()));
	if (!SaveGameObject)
	{
		UE_LOG(LogKScript, Error, TEXT("セーブゲームオブジェクトの作成に失敗しました"));
		return false;
	}

	// 現在の状態をキャプチャ
	CaptureCurrentState(SaveGameObject);

	// スロット名を設定
	SaveGameObject->SlotName = SlotName;

	// セーブを実行
	if (UGameplayStatics::SaveGameToSlot(SaveGameObject, SlotName, UserIndex))
	{
		UE_LOG(LogKScript, Log, TEXT("ゲームをセーブしました: スロット=%s, ユーザー=%d"), *SlotName, UserIndex);
		return true;
	}
	else
	{
		UE_LOG(LogKScript, Error, TEXT("ゲームのセーブに失敗しました: スロット=%s"), *SlotName);
		return false;
	}
}

bool UKScriptSubsystem::LoadGame(const FString& SlotName, int32 UserIndex)
{
	if (!ScriptEngine || !VariableManager)
	{
		UE_LOG(LogKScript, Error, TEXT("ロードに失敗: エンジンまたは変数管理システムが初期化されていません"));
		return false;
	}

	// セーブデータが存在するかチェック
	if (!UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
	{
		UE_LOG(LogKScript, Warning, TEXT("セーブデータが見つかりません: スロット=%s, ユーザー=%d"), *SlotName, UserIndex);
		return false;
	}

	// セーブデータをロード
	UKScriptSaveGame* SaveGameObject = Cast<UKScriptSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
	if (!SaveGameObject)
	{
		UE_LOG(LogKScript, Error, TEXT("セーブデータのロードに失敗しました: スロット=%s"), *SlotName);
		return false;
	}

	// 状態を復元
	if (RestoreFromSaveGame(SaveGameObject))
	{
		UE_LOG(LogKScript, Log, TEXT("ゲームをロードしました: スロット=%s, ユーザー=%d"), *SlotName, UserIndex);
		UE_LOG(LogKScript, Log, TEXT("%s"), *SaveGameObject->GetSaveDataSummary());
		return true;
	}
	else
	{
		UE_LOG(LogKScript, Error, TEXT("セーブデータからの状態復元に失敗しました"));
		return false;
	}
}

bool UKScriptSubsystem::DoesSaveGameExist(const FString& SlotName, int32 UserIndex) const
{
	return UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex);
}

bool UKScriptSubsystem::DeleteSaveGame(const FString& SlotName, int32 UserIndex)
{
	if (UGameplayStatics::DeleteGameInSlot(SlotName, UserIndex))
	{
		UE_LOG(LogKScript, Log, TEXT("セーブデータを削除しました: スロット=%s, ユーザー=%d"), *SlotName, UserIndex);
		return true;
	}
	else
	{
		UE_LOG(LogKScript, Warning, TEXT("セーブデータの削除に失敗しました: スロット=%s"), *SlotName);
		return false;
	}
}

void UKScriptSubsystem::CaptureCurrentState(UKScriptSaveGame* SaveGameObject)
{
	if (!SaveGameObject)
	{
		return;
	}

	// 基本情報
	SaveGameObject->SaveDateTime = FDateTime::Now();

	// スクリプト実行状態
	SaveGameObject->CurrentScriptAssetPath = CurrentScriptAssetPath;
	SaveGameObject->CurrentCommandIndex = ScriptEngine->GetCurrentCommandIndex();
	SaveGameObject->CallStack = TArray<int32>(); // コールスタックはエンジンから直接取得できないため空
	// TODO: KScriptEngineにGetCallStackメソッドを追加する

	// 変数データ
	if (VariableManager)
	{
		// 変数マップをコピー
		SaveGameObject->Variables.Empty();
		// TODO: KScriptVariableにGetAllVariables()メソッドを追加する
		// 現在は変数マップに直接アクセスできないため、将来的に改善
	}

	// 画像状態
	UWorld* World = GetWorld();
	if (World)
	{
		UKScriptImageManager* ImageManager = World->GetSubsystem<UKScriptImageManager>();
		if (ImageManager)
		{
			// TODO: ImageManagerから現在の画像状態を取得
			// 現在はImageManagerに状態取得用のAPIがないため、将来的に追加
		}

		// 音声状態
		UKScriptAudioManager* AudioManager = World->GetSubsystem<UKScriptAudioManager>();
		if (AudioManager)
		{
			// TODO: AudioManagerから現在のBGM情報を取得
			// 現在はAudioManagerに状態取得用のAPIがないため、将来的に追加
		}

		// メッセージ状態
		UKScriptUIManager* UIManager = World->GetSubsystem<UKScriptUIManager>();
		if (UIManager)
		{
			// TODO: UIManagerから現在のメッセージテキストを取得
			// 現在はUIManagerに状態取得用のAPIがないため、将来的に追加
		}
	}

	UE_LOG(LogKScript, Log, TEXT("現在の状態をセーブデータにキャプチャしました"));
}

bool UKScriptSubsystem::RestoreFromSaveGame(UKScriptSaveGame* SaveGameObject)
{
	if (!SaveGameObject)
	{
		return false;
	}

	// スクリプトアセットをロード
	if (!SaveGameObject->CurrentScriptAssetPath.IsEmpty())
	{
		UKScriptAsset* ScriptAsset = LoadObject<UKScriptAsset>(nullptr, *SaveGameObject->CurrentScriptAssetPath);
		if (ScriptAsset)
		{
			// スクリプトをロード（実行は開始しない）
			if (!ScriptEngine->LoadScriptFromAsset(ScriptAsset))
			{
				UE_LOG(LogKScript, Error, TEXT("スクリプトアセットのロードに失敗しました: %s"), *SaveGameObject->CurrentScriptAssetPath);
				return false;
			}
			CurrentScriptAssetPath = SaveGameObject->CurrentScriptAssetPath;
		}
		else
		{
			UE_LOG(LogKScript, Error, TEXT("スクリプトアセットが見つかりません: %s"), *SaveGameObject->CurrentScriptAssetPath);
			return false;
		}
	}

	// 実行位置を復元
	ScriptEngine->SetCurrentCommandIndex(SaveGameObject->CurrentCommandIndex);
	ScriptEngine->SetExecutionState(EKScriptExecutionState::WaitingInput);

	// 変数を復元
	if (VariableManager)
	{
		// 全変数をクリア
		VariableManager->ClearAll();

		// セーブデータから変数を復元
		for (const auto& Pair : SaveGameObject->Variables)
		{
			VariableManager->SetVariableValue(Pair.Key, Pair.Value);
		}
		UE_LOG(LogKScript, Log, TEXT("変数を復元しました: %d個"), SaveGameObject->Variables.Num());
	}

	// 画像状態を復元
	UWorld* World = GetWorld();
	if (World)
	{
		UKScriptImageManager* ImageManager = World->GetSubsystem<UKScriptImageManager>();
		if (ImageManager)
		{
			// 背景を復元
			if (!SaveGameObject->BackgroundLayer.TexturePath.IsEmpty() && SaveGameObject->BackgroundLayer.bIsVisible)
			{
				ImageManager->SetBackground(SaveGameObject->BackgroundLayer.TexturePath);
			}

			// キャラクターを復元
			for (const auto& Pair : SaveGameObject->CharacterLayers)
			{
				const FString& CharaName = Pair.Key;
				const FKScriptSavedImageLayer& Layer = Pair.Value;

				if (Layer.bIsVisible && !Layer.TexturePath.IsEmpty())
				{
					ImageManager->ShowCharacter(CharaName, Layer.TexturePath, Layer.Position);
					ImageManager->SetLayerOpacity(CharaName, Layer.Opacity);
				}
			}
			UE_LOG(LogKScript, Log, TEXT("画像状態を復元しました: キャラクター%d個"), SaveGameObject->CharacterLayers.Num());
		}

		// 音声状態を復元
		UKScriptAudioManager* AudioManager = World->GetSubsystem<UKScriptAudioManager>();
		if (AudioManager && !SaveGameObject->CurrentBGMPath.IsEmpty())
		{
			AudioManager->PlayBGM(SaveGameObject->CurrentBGMPath, true, SaveGameObject->BGMVolume);
			UE_LOG(LogKScript, Log, TEXT("BGMを復元しました: %s"), *SaveGameObject->CurrentBGMPath);
		}

		// メッセージ状態を復元
		UKScriptUIManager* UIManager = World->GetSubsystem<UKScriptUIManager>();
		if (UIManager && !SaveGameObject->CurrentMessageText.IsEmpty())
		{
			UIManager->DisplayText(SaveGameObject->CurrentMessageText);
			UE_LOG(LogKScript, Log, TEXT("メッセージテキストを復元しました"));
		}
	}

	UE_LOG(LogKScript, Log, TEXT("セーブデータから状態を復元しました"));
	return true;
}
