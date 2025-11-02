// Copyright Epic Games, Inc. All Rights Reserved.

#include "KScriptParser.h"
#include "KScript.h"

UKScriptParser::UKScriptParser()
	: CurrentLineNumber(0)
	, CurrentColumnNumber(0)
{
}

bool UKScriptParser::Parse(const FString& ScriptText, TArray<FKScriptCommand>& OutCommands, TMap<FString, int32>& OutLabels)
{
	OutCommands.Empty();
	OutLabels.Empty();

	// 字句解析
	TArray<FKScriptToken> Tokens;
	if (!Tokenize(ScriptText, Tokens))
	{
		UE_LOG(LogKScript, Error, TEXT("字句解析に失敗しました"));
		return false;
	}
	UE_LOG(LogKScript, Log, TEXT("字句解析完了: トークン数 %d"), Tokens.Num());

	// 構文解析
	if (!ParseTokens(Tokens, OutCommands, OutLabels))
	{
		UE_LOG(LogKScript, Error, TEXT("構文解析に失敗しました"));
		return false;
	}

	return true;
}

bool UKScriptParser::Tokenize(const FString& ScriptText, TArray<FKScriptToken>& OutTokens)
{
	OutTokens.Empty();

	TArray<FString> Lines;
	ScriptText.ParseIntoArray(Lines, TEXT("\n"), false);

	CurrentLineNumber = 1;

	for (const FString& Line : Lines)
	{
		CurrentColumnNumber = 0;
		FString TrimmedLine = Line.TrimStartAndEnd();

		// 空行をスキップ
		if (TrimmedLine.IsEmpty())
		{
			CurrentLineNumber++;
			continue;
		}

		// コメント行（;で始まる）
		if (TrimmedLine.StartsWith(TEXT(";")))
		{
			FKScriptToken CommentToken;
			CommentToken.Type = EKScriptTokenType::Comment;
			CommentToken.Value = TrimmedLine;
			CommentToken.LineNumber = CurrentLineNumber;
			OutTokens.Add(CommentToken);
			CurrentLineNumber++;
			continue;
		}

		// ラベル行（*で始まる）
		if (TrimmedLine.StartsWith(TEXT("*")))
		{
			FKScriptToken LabelToken;
			LabelToken.Type = EKScriptTokenType::Label;
			LabelToken.Value = TrimmedLine.Mid(1).TrimStartAndEnd(); // *を除去
			LabelToken.LineNumber = CurrentLineNumber;
			OutTokens.Add(LabelToken);
			CurrentLineNumber++;
			continue;
		}

		// 行をトークン化（タグとテキストの混在に対応）
		FString RemainingLine = Line;
		int32 CurrentPosition = 0;

		while (CurrentPosition < RemainingLine.Len())
		{
			// タグの開始を探す
			int32 TagStartPos = RemainingLine.Find(TEXT("["), ESearchCase::IgnoreCase, ESearchDir::FromStart, CurrentPosition);

			// タグが見つからない場合、残りをテキストとして処理
			if (TagStartPos == INDEX_NONE)
			{
				FString TextContent = RemainingLine.Mid(CurrentPosition);
				if (!IsWhitespaceOnly(TextContent) && !TextContent.IsEmpty())
				{
					FKScriptToken TextToken;
					TextToken.Type = EKScriptTokenType::Text;
					TextToken.Value = TextContent;
					TextToken.LineNumber = CurrentLineNumber;
					OutTokens.Add(TextToken);
				}
				break;
			}

			// タグの前にテキストがある場合
			if (TagStartPos > CurrentPosition)
			{
				FString TextContent = RemainingLine.Mid(CurrentPosition, TagStartPos - CurrentPosition);
				if (!IsWhitespaceOnly(TextContent))
				{
					FKScriptToken TextToken;
					TextToken.Type = EKScriptTokenType::Text;
					TextToken.Value = TextContent;
					TextToken.LineNumber = CurrentLineNumber;
					OutTokens.Add(TextToken);
				}
			}

			// タグの終了を探す
			int32 TagEndPos = RemainingLine.Find(TEXT("]"), ESearchCase::IgnoreCase, ESearchDir::FromStart, TagStartPos);
			if (TagEndPos == INDEX_NONE)
			{
				UE_LOG(LogKScript, Warning, TEXT("閉じていないタグがあります (行番号: %d)"), CurrentLineNumber);
				break;
			}

			// タグをトークンとして追加
			FString TagContent = RemainingLine.Mid(TagStartPos + 1, TagEndPos - TagStartPos - 1);
			FKScriptToken TagToken;
			TagToken.Type = EKScriptTokenType::Tag;
			TagToken.Value = TagContent;
			TagToken.LineNumber = CurrentLineNumber;
			OutTokens.Add(TagToken);

			CurrentPosition = TagEndPos + 1;
		}

		CurrentLineNumber++;
	}

	// ファイル終端トークンを追加
	FKScriptToken EOFToken;
	EOFToken.Type = EKScriptTokenType::EndOfFile;
	EOFToken.LineNumber = CurrentLineNumber;
	OutTokens.Add(EOFToken);

	return true;
}

bool UKScriptParser::ParseTokens(const TArray<FKScriptToken>& Tokens, TArray<FKScriptCommand>& OutCommands, TMap<FString, int32>& OutLabels)
{
	for (const FKScriptToken& Token : Tokens)
	{
		switch (Token.Type)
		{
		case EKScriptTokenType::Label:
		{
			// ラベルを登録（次のコマンドのインデックスを指す）
			OutLabels.Add(Token.Value, OutCommands.Num());
			UE_LOG(LogKScript, Verbose, TEXT("ラベル登録: '%s' (コマンドインデックス: %d)"), *Token.Value, OutCommands.Num());
			break;
		}

		case EKScriptTokenType::Tag:
		{
			FKScriptCommand Command;
			if (ParseTag(Token.Value, Token.LineNumber, Command))
			{
				OutCommands.Add(Command);
			}
			break;
		}

		case EKScriptTokenType::Text:
		{
			// テキストを表示コマンドとして追加
			FKScriptCommand TextCommand;
			TextCommand.Type = EKScriptCommandType::Text;
			TextCommand.Text = Token.Value;
			TextCommand.LineNumber = Token.LineNumber;
			OutCommands.Add(TextCommand);
			break;
		}

		case EKScriptTokenType::Comment:
			// コメントは無視
			break;

		case EKScriptTokenType::EndOfFile:
			// ファイル終端
			break;

		default:
			break;
		}
	}

	return true;
}

bool UKScriptParser::ParseTag(const FString& TagContent, int32 LineNumber, FKScriptCommand& OutCommand)
{
	FString TagName;
	TMap<FString, FString> Parameters;

	if (!ParseTagParameters(TagContent, TagName, Parameters))
	{
		UE_LOG(LogKScript, Warning, TEXT("タグパラメータの解析に失敗しました (行番号: %d)"), LineNumber);
		return false;
	}

	OutCommand.Type = GetCommandTypeFromTagName(TagName);
	OutCommand.Parameters = Parameters;
	OutCommand.LineNumber = LineNumber;

	// 特定のタグに対する追加処理
	if (TagName.Equals(TEXT("jump"), ESearchCase::IgnoreCase))
	{
		// jumpタグの場合、targetパラメータからラベル名を取得
		const FString* Target = Parameters.Find(TEXT("target"));
		if (Target)
		{
			OutCommand.Text = *Target;
		}
	}

	UE_LOG(LogKScript, Verbose, TEXT("タグ解析完了: '%s' (行番号: %d)"), *TagName, LineNumber);

	return true;
}

bool UKScriptParser::ParseTagParameters(const FString& TagContent, FString& OutTagName, TMap<FString, FString>& OutParameters)
{
	OutParameters.Empty();

	// タグ内容をトリム
	FString TrimmedContent = TagContent.TrimStartAndEnd();

	// 最初の空白までがタグ名
	int32 FirstSpacePos = INDEX_NONE;
	if (TrimmedContent.FindChar(TEXT(' '), FirstSpacePos))
	{
		OutTagName = TrimmedContent.Left(FirstSpacePos);
		FString ParamsString = TrimmedContent.Mid(FirstSpacePos + 1).TrimStartAndEnd();

		// パラメータを解析（key=value形式）
		TArray<FString> ParamPairs;
		ParamsString.ParseIntoArray(ParamPairs, TEXT(" "), true);

		for (const FString& Pair : ParamPairs)
		{
			int32 EqualPos = INDEX_NONE;
			if (Pair.FindChar(TEXT('='), EqualPos))
			{
				FString Key = Pair.Left(EqualPos).TrimStartAndEnd();
				FString Value = Pair.Mid(EqualPos + 1).TrimStartAndEnd();

				// クォートを除去
				if (Value.StartsWith(TEXT("\"")) && Value.EndsWith(TEXT("\"")))
				{
					Value = Value.Mid(1, Value.Len() - 2);
				}

				OutParameters.Add(Key, Value);
			}
		}
	}
	else
	{
		// パラメータなしのタグ
		OutTagName = TrimmedContent;
	}

	return !OutTagName.IsEmpty();
}

EKScriptCommandType UKScriptParser::GetCommandTypeFromTagName(const FString& TagName) const
{
	if (TagName.Equals(TEXT("l"), ESearchCase::IgnoreCase))
	{
		return EKScriptCommandType::WaitClick;
	}
	else if (TagName.Equals(TEXT("p"), ESearchCase::IgnoreCase))
	{
		return EKScriptCommandType::WaitPageBreak;
	}
	else if (TagName.Equals(TEXT("r"), ESearchCase::IgnoreCase))
	{
		return EKScriptCommandType::LineBreak;
	}
	else if (TagName.Equals(TEXT("cm"), ESearchCase::IgnoreCase))
	{
		return EKScriptCommandType::ClearMessage;
	}
	else if (TagName.Equals(TEXT("jump"), ESearchCase::IgnoreCase))
	{
		return EKScriptCommandType::Jump;
	}
	else if (TagName.Equals(TEXT("call"), ESearchCase::IgnoreCase))
	{
		return EKScriptCommandType::Call;
	}
	else if (TagName.Equals(TEXT("return"), ESearchCase::IgnoreCase))
	{
		return EKScriptCommandType::Return;
	}
	else if (TagName.Equals(TEXT("if"), ESearchCase::IgnoreCase))
	{
		return EKScriptCommandType::If;
	}
	else if (TagName.Equals(TEXT("else"), ESearchCase::IgnoreCase))
	{
		return EKScriptCommandType::Else;
	}
	else if (TagName.Equals(TEXT("endif"), ESearchCase::IgnoreCase))
	{
		return EKScriptCommandType::EndIf;
	}
	else if (TagName.Equals(TEXT("eval"), ESearchCase::IgnoreCase))
	{
		return EKScriptCommandType::Eval;
	}
	else if (TagName.Equals(TEXT("bg"), ESearchCase::IgnoreCase))
	{
		return EKScriptCommandType::Bg;
	}
	else if (TagName.Equals(TEXT("chara_show"), ESearchCase::IgnoreCase))
	{
		return EKScriptCommandType::CharaShow;
	}
	else if (TagName.Equals(TEXT("chara_hide"), ESearchCase::IgnoreCase))
	{
		return EKScriptCommandType::CharaHide;
	}
	else if (TagName.Equals(TEXT("playbgm"), ESearchCase::IgnoreCase))
	{
		return EKScriptCommandType::PlayBgm;
	}
	else if (TagName.Equals(TEXT("stopbgm"), ESearchCase::IgnoreCase))
	{
		return EKScriptCommandType::StopBgm;
	}
	else if (TagName.Equals(TEXT("playse"), ESearchCase::IgnoreCase))
	{
		return EKScriptCommandType::PlaySe;
	}

	return EKScriptCommandType::Unknown;
}

bool UKScriptParser::IsWhitespaceOnly(const FString& Str) const
{
	for (TCHAR Char : Str)
	{
		if (!FChar::IsWhitespace(Char))
		{
			return false;
		}
	}
	return true;
}
