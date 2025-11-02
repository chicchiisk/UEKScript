// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "KScriptEngine.h"
#include "KScriptParser.generated.h"

/**
 * トークンの種類
 */
UENUM()
enum class EKScriptTokenType : uint8
{
	Text,           // 通常のテキスト
	Tag,            // タグ（[tag]）
	Label,          // ラベル（*label）
	Comment,        // コメント（;comment）
	EndOfLine,      // 行末
	EndOfFile       // ファイル末尾
};

/**
 * トークン
 */
USTRUCT()
struct KSCRIPT_API FKScriptToken
{
	GENERATED_BODY()

	EKScriptTokenType Type = EKScriptTokenType::Text;
	FString Value;
	int32 LineNumber = 0;
	int32 ColumnNumber = 0;
};

/**
 * KScriptパーサー - スクリプトを解析して命令列に変換
 */
UCLASS()
class KSCRIPT_API UKScriptParser : public UObject
{
	GENERATED_BODY()

public:
	UKScriptParser();

	/**
	 * スクリプトテキストを解析する
	 * @param ScriptText スクリプトテキスト
	 * @param OutCommands 出力される命令列
	 * @param OutLabels 出力されるラベルマップ
	 * @return 解析成功時true
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript")
	bool Parse(const FString& ScriptText, TArray<FKScriptCommand>& OutCommands, TMap<FString, int32>& OutLabels);

protected:
	/**
	 * 字句解析 - テキストをトークン列に変換
	 * @param ScriptText スクリプトテキスト
	 * @param OutTokens 出力されるトークン列
	 * @return トークン化成功時true
	 */
	bool Tokenize(const FString& ScriptText, TArray<FKScriptToken>& OutTokens);

	/**
	 * 構文解析 - トークン列を命令列に変換
	 * @param Tokens トークン列
	 * @param OutCommands 出力される命令列
	 * @param OutLabels 出力されるラベルマップ
	 * @return 構文解析成功時true
	 */
	bool ParseTokens(const TArray<FKScriptToken>& Tokens, TArray<FKScriptCommand>& OutCommands, TMap<FString, int32>& OutLabels);

	/**
	 * タグをパースして命令に変換
	 * @param TagContent タグの内容（[]を除いた部分）
	 * @param LineNumber 行番号
	 * @param OutCommand 出力される命令
	 * @return パース成功時true
	 */
	bool ParseTag(const FString& TagContent, int32 LineNumber, FKScriptCommand& OutCommand);

	/**
	 * タグのパラメータを解析
	 * @param TagContent タグの内容
	 * @param OutTagName 出力されるタグ名
	 * @param OutParameters 出力されるパラメータマップ
	 * @return パース成功時true
	 */
	bool ParseTagParameters(const FString& TagContent, FString& OutTagName, TMap<FString, FString>& OutParameters);

	/**
	 * タグ名からコマンドタイプを取得
	 * @param TagName タグ名
	 * @return コマンドタイプ
	 */
	EKScriptCommandType GetCommandTypeFromTagName(const FString& TagName) const;

	/**
	 * 文字列が空白文字のみかチェック
	 */
	bool IsWhitespaceOnly(const FString& Str) const;

protected:
	int32 CurrentLineNumber;
	int32 CurrentColumnNumber;
};
