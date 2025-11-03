// Copyright Epic Games, Inc. All Rights Reserved.

#include "KScriptImageManager.h"
#include "KScript.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Engine/Texture2D.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "UObject/ConstructorHelpers.h"

UKScriptImageManager::UKScriptImageManager()
{
}

void UKScriptImageManager::Initialize(UImage* InBackgroundWidget, UCanvasPanel* InCharacterContainer)
{
	BackgroundWidget = InBackgroundWidget;
	CharacterContainer = InCharacterContainer;

	BackgroundLayer.LayerName = TEXT("Background");
	BackgroundLayer.ImageWidget = InBackgroundWidget;

	UE_LOG(LogKScript, Log, TEXT("KScriptImageManagerを初期化しました"));
}

UTexture2D* UKScriptImageManager::LoadTexture(const FString& TexturePath)
{
	if (TexturePath.IsEmpty())
	{
		UE_LOG(LogKScript, Warning, TEXT("テクスチャパスが空です"));
		return nullptr;
	}

	// アセットパスからテクスチャをロード
	UTexture2D* Texture = LoadObject<UTexture2D>(nullptr, *TexturePath);

	if (!Texture)
	{
		UE_LOG(LogKScript, Warning, TEXT("テクスチャのロードに失敗しました: %s"), *TexturePath);
		return nullptr;
	}

	UE_LOG(LogKScript, Log, TEXT("テクスチャをロードしました: %s"), *TexturePath);
	return Texture;
}

bool UKScriptImageManager::SetBackground(const FString& TexturePath)
{
	if (!BackgroundWidget)
	{
		UE_LOG(LogKScript, Error, TEXT("背景ウィジェットが設定されていません"));
		return false;
	}

	UTexture2D* Texture = LoadTexture(TexturePath);
	if (!Texture)
	{
		return false;
	}

	BackgroundLayer.Texture = Texture;
	BackgroundWidget->SetBrushFromTexture(Texture);
	BackgroundWidget->SetVisibility(ESlateVisibility::Visible);

	UE_LOG(LogKScript, Log, TEXT("背景画像を設定しました: %s"), *TexturePath);
	return true;
}

bool UKScriptImageManager::ShowCharacter(const FString& CharaName, const FString& TexturePath, const FVector2D& Position)
{
	if (!CharacterContainer)
	{
		UE_LOG(LogKScript, Error, TEXT("キャラクターコンテナが設定されていません"));
		return false;
	}

	UTexture2D* Texture = LoadTexture(TexturePath);
	if (!Texture)
	{
		return false;
	}

	// 既存のキャラクターがいる場合は更新
	if (CharacterLayers.Contains(CharaName))
	{
		FKScriptImageLayer& Layer = CharacterLayers[CharaName];
		Layer.Texture = Texture;
		if (Layer.ImageWidget)
		{
			Layer.ImageWidget->SetBrushFromTexture(Texture);
			Layer.ImageWidget->SetVisibility(ESlateVisibility::Visible);
			Layer.Position = Position;
			Layer.bVisible = true;

			// 位置を更新
			if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Layer.ImageWidget->Slot))
			{
				CanvasSlot->SetPosition(Position);
			}
		}
	}
	else
	{
		// 新しいキャラクターを作成
		// UserWidgetのWidgetTreeを使用して適切に構築
		UUserWidget* OwningWidget = CharacterContainer->GetTypedOuter<UUserWidget>();
		if (!OwningWidget || !OwningWidget->WidgetTree)
		{
			UE_LOG(LogKScript, Error, TEXT("UserWidgetまたはWidgetTreeが見つかりません"));
			return false;
		}

		UImage* CharaImage = OwningWidget->WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
		if (!CharaImage)
		{
			UE_LOG(LogKScript, Error, TEXT("キャラクター画像ウィジェットの作成に失敗しました"));
			return false;
		}

		CharaImage->SetBrushFromTexture(Texture);

		// コンテナに追加
		UCanvasPanelSlot* CanvasSlot = CharacterContainer->AddChildToCanvas(CharaImage);
		if (CanvasSlot)
		{
			CanvasSlot->SetPosition(Position);
			CanvasSlot->SetAutoSize(true);
		}

		// レイヤー情報を保存
		FKScriptImageLayer NewLayer;
		NewLayer.LayerName = CharaName;
		NewLayer.Texture = Texture;
		NewLayer.ImageWidget = CharaImage;
		NewLayer.Position = Position;
		NewLayer.bVisible = true;
		CharacterLayers.Add(CharaName, NewLayer);
	}

	UE_LOG(LogKScript, Log, TEXT("キャラクター画像を表示しました: %s at (%f, %f)"), *CharaName, Position.X, Position.Y);
	return true;
}

bool UKScriptImageManager::HideCharacter(const FString& CharaName)
{
	if (!CharacterLayers.Contains(CharaName))
	{
		UE_LOG(LogKScript, Warning, TEXT("キャラクター '%s' が見つかりません"), *CharaName);
		return false;
	}

	FKScriptImageLayer& Layer = CharacterLayers[CharaName];
	if (Layer.ImageWidget)
	{
		Layer.ImageWidget->SetVisibility(ESlateVisibility::Hidden);
		Layer.bVisible = false;
	}

	UE_LOG(LogKScript, Log, TEXT("キャラクター画像を非表示にしました: %s"), *CharaName);
	return true;
}

void UKScriptImageManager::HideAllCharacters()
{
	for (auto& Pair : CharacterLayers)
	{
		FKScriptImageLayer& Layer = Pair.Value;
		if (Layer.ImageWidget)
		{
			Layer.ImageWidget->SetVisibility(ESlateVisibility::Hidden);
			Layer.bVisible = false;
		}
	}

	UE_LOG(LogKScript, Log, TEXT("すべてのキャラクター画像を非表示にしました"));
}

void UKScriptImageManager::SetLayerOpacity(const FString& LayerName, float Opacity)
{
	Opacity = FMath::Clamp(Opacity, 0.0f, 1.0f);

	if (LayerName == TEXT("Background") && BackgroundWidget)
	{
		BackgroundWidget->SetRenderOpacity(Opacity);
		BackgroundLayer.Opacity = Opacity;
	}
	else if (CharacterLayers.Contains(LayerName))
	{
		FKScriptImageLayer& Layer = CharacterLayers[LayerName];
		if (Layer.ImageWidget)
		{
			Layer.ImageWidget->SetRenderOpacity(Opacity);
			Layer.Opacity = Opacity;
		}
	}

	UE_LOG(LogKScript, Log, TEXT("レイヤー '%s' の不透明度を %f に設定しました"), *LayerName, Opacity);
}
