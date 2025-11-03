// Copyright Epic Games, Inc. All Rights Reserved.

#include "KScriptViewer.h"
#include "KScriptAsset.h"
#include "Widgets/Text/SMultiLineEditableText.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/SWindow.h"
#include "Framework/Application/SlateApplication.h"
#include "EditorStyleSet.h"
#include "Styling/AppStyle.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformProcess.h"
#include "Misc/MessageDialog.h"
#include "Misc/Paths.h"

#define LOCTEXT_NAMESPACE "KScriptViewer"

void SKScriptViewer::Construct(const FArguments& InArgs)
{
	KScriptAsset = InArgs._KScriptAsset;

	// スクリプトテキストを保持するためのSharedPtr
	ScriptTextPtr = MakeShareable(new FString());
	if (KScriptAsset.IsValid())
	{
		*ScriptTextPtr = KScriptAsset->GetScriptText();
	}

	// UIレイアウトを構築
	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
		.Padding(FMargin(8.0f))
		[
			SNew(SVerticalBox)

			// ツールバー
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0.0f, 0.0f, 0.0f, 8.0f)
			[
				SNew(SHorizontalBox)

				// VSCodeで開くボタン
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(0.0f, 0.0f, 8.0f, 0.0f)
				[
					SNew(SButton)
					.Text(LOCTEXT("OpenInVSCode", "Open in VSCode"))
					.ToolTipText(LOCTEXT("OpenInVSCodeTooltip", "VSCodeでこのスクリプトを開きます"))
					.OnClicked(this, &SKScriptViewer::OnOpenInVSCodeClicked)
				]

				// 保存ボタン
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(0.0f, 0.0f, 8.0f, 0.0f)
				[
					SNew(SButton)
					.Text(LOCTEXT("Save", "Save"))
					.ToolTipText(LOCTEXT("SaveTooltip", "変更をアセットに保存します"))
					.OnClicked(this, &SKScriptViewer::OnSaveClicked)
				]

				// 更新ボタン
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SButton)
					.Text(LOCTEXT("Refresh", "Refresh"))
					.ToolTipText(LOCTEXT("RefreshTooltip", "アセットから最新のテキストを読み込みます"))
					.OnClicked(this, &SKScriptViewer::OnRefreshClicked)
				]

				// アセット名表示
				+ SHorizontalBox::Slot()
				.FillWidth(1.0f)
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Center)
				.Padding(8.0f, 0.0f, 0.0f, 0.0f)
				[
					SNew(STextBlock)
					.Text(FText::FromString(KScriptAsset.IsValid() ? KScriptAsset->GetName() : TEXT("")))
					.Font(FAppStyle::GetFontStyle("BoldFont"))
				]
			]

			// テキストエディタ
			+ SVerticalBox::Slot()
			.FillHeight(1.0f)
			[
				SNew(SBorder)
				.BorderImage(FAppStyle::GetBrush("ToolPanel.DarkGroupBorder"))
				.Padding(FMargin(4.0f))
				[
					SNew(SScrollBox)
					.Orientation(Orient_Vertical)

					+ SScrollBox::Slot()
					[
						SNew(SMultiLineEditableText)
						.Text(this, &SKScriptViewer::GetScriptText)
						.OnTextChanged(this, &SKScriptViewer::OnTextChanged)
						.Font(FCoreStyle::GetDefaultFontStyle("Mono", 10))
						.IsReadOnly(false)
						.AutoWrapText(false)
						.Margin(4.0f)
					]
				]
			]
		]
	];
}

void SKScriptViewer::OpenViewer(UKScriptAsset* InAsset)
{
	if (!InAsset)
	{
		return;
	}

	// ウィンドウを作成
	TSharedRef<SWindow> ViewerWindow = SNew(SWindow)
		.Title(FText::Format(LOCTEXT("KScriptViewerTitle", "KScript Viewer - {0}"), FText::FromString(InAsset->GetName())))
		.ClientSize(FVector2D(800.0f, 600.0f))
		.SupportsMaximize(true)
		.SupportsMinimize(true);

	// ビューアーウィジェットを作成
	TSharedRef<SKScriptViewer> ViewerWidget = SNew(SKScriptViewer)
		.KScriptAsset(InAsset);

	// ウィンドウにウィジェットを設定
	ViewerWindow->SetContent(ViewerWidget);

	// ウィンドウを表示
	FSlateApplication::Get().AddWindow(ViewerWindow);
}

FReply SKScriptViewer::OnOpenInVSCodeClicked()
{
	if (!KScriptAsset.IsValid())
	{
		return FReply::Handled();
	}

	// 一時ファイルを作成
	FString TempFilePath = FPaths::CreateTempFilename(*FPaths::ProjectSavedDir(), TEXT("KScript_"), TEXT(".ks"));

	// スクリプトテキストを一時ファイルに保存
	if (FFileHelper::SaveStringToFile(*ScriptTextPtr, *TempFilePath))
	{
		// VSCodeのパスを探す
		FString VSCodePath;

#if PLATFORM_WINDOWS
		// Windowsの場合、一般的なインストールパスを確認
		TArray<FString> PossiblePaths = {
			TEXT("C:/Program Files/Microsoft VS Code/Code.exe"),
			TEXT("C:/Program Files (x86)/Microsoft VS Code/Code.exe"),
			FPaths::Combine(FPlatformMisc::GetEnvironmentVariable(TEXT("LOCALAPPDATA")), TEXT("Programs/Microsoft VS Code/Code.exe"))
		};

		for (const FString& Path : PossiblePaths)
		{
			if (FPaths::FileExists(Path))
			{
				VSCodePath = Path;
				break;
			}
		}

		// 見つからない場合は、コマンド名で試す
		if (VSCodePath.IsEmpty())
		{
			VSCodePath = TEXT("code");
		}
#elif PLATFORM_MAC
		VSCodePath = TEXT("/Applications/Visual Studio Code.app/Contents/Resources/app/bin/code");
		if (!FPaths::FileExists(VSCodePath))
		{
			VSCodePath = TEXT("code");
		}
#elif PLATFORM_LINUX
		VSCodePath = TEXT("code");
#endif

		// VSCodeでファイルを開く
		FString Args = FString::Printf(TEXT("\"%s\""), *TempFilePath);
		FPlatformProcess::CreateProc(*VSCodePath, *Args, true, false, false, nullptr, 0, nullptr, nullptr);
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("FailedToSaveTempFile", "一時ファイルの作成に失敗しました。"));
	}

	return FReply::Handled();
}

FReply SKScriptViewer::OnSaveClicked()
{
	if (!KScriptAsset.IsValid())
	{
		return FReply::Handled();
	}

	// アセットにテキストを保存
	KScriptAsset->SetScriptText(*ScriptTextPtr);
	KScriptAsset->MarkPackageDirty();

	// 保存成功のメッセージ
	FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("SaveSuccess", "スクリプトをアセットに保存しました。"));

	return FReply::Handled();
}

FReply SKScriptViewer::OnRefreshClicked()
{
	if (!KScriptAsset.IsValid())
	{
		return FReply::Handled();
	}

	// アセットから最新のテキストを読み込み
	*ScriptTextPtr = KScriptAsset->GetScriptText();

	return FReply::Handled();
}

void SKScriptViewer::OnTextChanged(const FText& NewText)
{
	// テキストが変更されたら、SharedPtrを更新
	*ScriptTextPtr = NewText.ToString();
}

FText SKScriptViewer::GetScriptText() const
{
	return FText::FromString(*ScriptTextPtr);
}

#undef LOCTEXT_NAMESPACE
