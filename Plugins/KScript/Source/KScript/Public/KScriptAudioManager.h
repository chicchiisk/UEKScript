// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Sound/SoundBase.h"
#include "KScriptAudioManager.generated.h"

class UAudioComponent;

/**
 * 音声再生システム - BGMとSEの再生を管理
 * GameInstanceSubsystemとして実装
 */
UCLASS()
class KSCRIPT_API UKScriptAudioManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// USubsystem interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End of USubsystem interface

	/**
	 * BGMを再生
	 * @param SoundPath サウンドアセットのパス
	 * @param bLoop ループ再生するか
	 * @param Volume ボリューム（0.0〜1.0）
	 * @return 成功時true
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript|Audio")
	bool PlayBGM(const FString& SoundPath, bool bLoop = true, float Volume = 1.0f);

	/**
	 * BGMを停止
	 * @param FadeOutDuration フェードアウト時間（秒）
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript|Audio")
	void StopBGM(float FadeOutDuration = 0.0f);

	/**
	 * SEを再生
	 * @param SoundPath サウンドアセットのパス
	 * @param Volume ボリューム（0.0〜1.0）
	 * @return 成功時true
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript|Audio")
	bool PlaySE(const FString& SoundPath, float Volume = 1.0f);

	/**
	 * BGMのボリュームを設定
	 * @param Volume ボリューム（0.0〜1.0）
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript|Audio")
	void SetBGMVolume(float Volume);

	/**
	 * BGMが再生中かどうか
	 * @return 再生中の場合true
	 */
	UFUNCTION(BlueprintPure, Category = "KScript|Audio")
	bool IsBGMPlaying() const;

	/**
	 * サウンドアセットをロード
	 * @param SoundPath サウンドアセットのパス
	 * @return ロードしたサウンド
	 */
	USoundBase* LoadSound(const FString& SoundPath);

protected:
	// BGM再生用のオーディオコンポーネント
	UPROPERTY()
	TObjectPtr<UAudioComponent> BGMAudioComponent;

	// 現在再生中のBGM
	UPROPERTY()
	TObjectPtr<USoundBase> CurrentBGM;
};
