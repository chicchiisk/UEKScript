// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/Texture2D.h"
#include "KScriptImageManager.generated.h"

class UImage;

/**
 * 画像レイヤー情報
 */
USTRUCT(BlueprintType)
struct KSCRIPT_API FKScriptImageLayer
{
	GENERATED_BODY()

	UPROPERTY()
	FString LayerName;

	UPROPERTY()
	TObjectPtr<UTexture2D> Texture;

	UPROPERTY()
	TObjectPtr<UImage> ImageWidget;

	UPROPERTY()
	FVector2D Position = FVector2D::ZeroVector;

	UPROPERTY()
	float Opacity = 1.0f;

	UPROPERTY()
	bool bVisible = true;
};

/**
 * 画像表示システム - 背景とキャラクターの画像を管理
 * GameInstanceSubsystemとして実装
 */
UCLASS()
class KSCRIPT_API UKScriptImageManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// USubsystem interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End of USubsystem interface

	/**
	 * UIウィジェットを設定
	 * @param InBackgroundWidget 背景用のImageウィジェット
	 * @param InCharacterContainer キャラクター用のコンテナウィジェット
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript|Image")
	void SetupWidgets(UImage* InBackgroundWidget, class UCanvasPanel* InCharacterContainer);

	/**
	 * 背景画像を設定
	 * @param TexturePath テクスチャのアセットパス
	 * @return 成功時true
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript|Image")
	bool SetBackground(const FString& TexturePath);

	/**
	 * キャラクター画像を表示
	 * @param CharaName キャラクター名
	 * @param TexturePath テクスチャのアセットパス
	 * @param Position 表示位置
	 * @return 成功時true
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript|Image")
	bool ShowCharacter(const FString& CharaName, const FString& TexturePath, const FVector2D& Position);

	/**
	 * キャラクター画像を非表示
	 * @param CharaName キャラクター名
	 * @return 成功時true
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript|Image")
	bool HideCharacter(const FString& CharaName);

	/**
	 * すべてのキャラクターを非表示
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript|Image")
	void HideAllCharacters();

	/**
	 * 画像レイヤーの不透明度を設定
	 * @param LayerName レイヤー名
	 * @param Opacity 不透明度（0.0〜1.0）
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript|Image")
	void SetLayerOpacity(const FString& LayerName, float Opacity);

	/**
	 * テクスチャをロード
	 * @param TexturePath テクスチャのアセットパス
	 * @return ロードしたテクスチャ
	 */
	UTexture2D* LoadTexture(const FString& TexturePath);

protected:
	// 背景レイヤー
	UPROPERTY()
	FKScriptImageLayer BackgroundLayer;

	// キャラクターレイヤー（複数）
	UPROPERTY()
	TMap<FString, FKScriptImageLayer> CharacterLayers;

	// 背景用のImageウィジェット
	UPROPERTY()
	TObjectPtr<UImage> BackgroundWidget;

	// キャラクター用のコンテナウィジェット
	UPROPERTY()
	TObjectPtr<class UCanvasPanel> CharacterContainer;
};
