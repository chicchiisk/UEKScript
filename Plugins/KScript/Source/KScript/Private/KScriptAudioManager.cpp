// Copyright Epic Games, Inc. All Rights Reserved.

#include "KScriptAudioManager.h"
#include "KScript.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

void UKScriptAudioManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UE_LOG(LogKScript, Log, TEXT("KScriptAudioManagerサブシステムを初期化しました"));
}

void UKScriptAudioManager::Deinitialize()
{
	// BGMを停止
	if (BGMAudioComponent && BGMAudioComponent->IsPlaying())
	{
		BGMAudioComponent->Stop();
	}
	
	// AudioComponent参照をクリア
	BGMAudioComponent = nullptr;

	UE_LOG(LogKScript, Log, TEXT("KScriptAudioManagerサブシステムを終了しました"));

	Super::Deinitialize();
}

USoundBase* UKScriptAudioManager::LoadSound(const FString& SoundPath)
{
	if (SoundPath.IsEmpty())
	{
		UE_LOG(LogKScript, Warning, TEXT("サウンドパスが空です"));
		return nullptr;
	}

	// アセットパスからサウンドをロード
	USoundBase* Sound = LoadObject<USoundBase>(nullptr, *SoundPath);

	if (!Sound)
	{
		UE_LOG(LogKScript, Warning, TEXT("サウンドのロードに失敗しました: %s"), *SoundPath);
		return nullptr;
	}

	UE_LOG(LogKScript, Log, TEXT("サウンドをロードしました: %s"), *SoundPath);
	return Sound;
}

bool UKScriptAudioManager::PlayBGM(const FString& SoundPath, bool bLoop, float Volume)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogKScript, Error, TEXT("Worldが無効です"));
		return false;
	}

	USoundBase* Sound = LoadSound(SoundPath);
	if (!Sound)
	{
		return false;
	}

	// 既存のBGMを停止
	if (BGMAudioComponent && BGMAudioComponent->IsPlaying())
	{
		BGMAudioComponent->Stop();
	}

	// 新しいBGMを再生
	BGMAudioComponent = UGameplayStatics::SpawnSound2D(World, Sound, Volume);
	if (!BGMAudioComponent)
	{
		UE_LOG(LogKScript, Error, TEXT("BGMオーディオコンポーネントの作成に失敗しました"));
		return false;
	}

	BGMAudioComponent->bIsUISound = true;
	BGMAudioComponent->bAutoDestroy = false;

	CurrentBGM = Sound;

	UE_LOG(LogKScript, Log, TEXT("BGMを再生しました: %s (Loop: %d, Volume: %f)"), *SoundPath, bLoop, Volume);
	return true;
}

void UKScriptAudioManager::StopBGM(float FadeOutDuration)
{
	if (!BGMAudioComponent || !BGMAudioComponent->IsPlaying())
	{
		return;
	}

	if (FadeOutDuration > 0.0f)
	{
		BGMAudioComponent->FadeOut(FadeOutDuration, 0.0f);
	}
	else
	{
		BGMAudioComponent->Stop();
	}

	CurrentBGM = nullptr;
	UE_LOG(LogKScript, Log, TEXT("BGMを停止しました (FadeOut: %f秒)"), FadeOutDuration);
}

bool UKScriptAudioManager::PlaySE(const FString& SoundPath, float Volume)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogKScript, Error, TEXT("Worldが無効です"));
		return false;
	}

	USoundBase* Sound = LoadSound(SoundPath);
	if (!Sound)
	{
		return false;
	}

	// SEを再生（ワンショット、自動破棄）
	UGameplayStatics::PlaySound2D(World, Sound, Volume);

	UE_LOG(LogKScript, Log, TEXT("SEを再生しました: %s (Volume: %f)"), *SoundPath, Volume);
	return true;
}

void UKScriptAudioManager::SetBGMVolume(float Volume)
{
	Volume = FMath::Clamp(Volume, 0.0f, 1.0f);

	if (BGMAudioComponent)
	{
		BGMAudioComponent->SetVolumeMultiplier(Volume);
		UE_LOG(LogKScript, Log, TEXT("BGMボリュームを %f に設定しました"), Volume);
	}
}

bool UKScriptAudioManager::IsBGMPlaying() const
{
	return BGMAudioComponent && BGMAudioComponent->IsPlaying();
}
