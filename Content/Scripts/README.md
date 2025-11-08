# KScript テストスクリプト

このディレクトリには、KScriptエンジンの各機能をテストするためのスクリプトファイルが含まれています。

## テストスクリプト一覧

### 1. test_basic.ks
**基本機能テスト**

テストする機能：
- テキスト表示
- クリック待ち `[l]`
- 改行 `[r]`
- ページ区切り待ち `[p]`
- メッセージクリア `[cm]`

推奨用途：最初に実行して基本的なテキスト表示機能を確認

### 2. test_control_flow.ks
**制御フローテスト**

テストする機能：
- 変数定義と操作 `[eval exp="..."]`
- 条件分岐 `[if]` `[else]` `[endif]`
- ネストした条件分岐
- ラベルジャンプ `[jump target="*label"]`
- サブルーチン呼び出し `[call target="*subroutine"]` / `[return]`
- 変数を使った計算

推奨用途：ロジックや分岐処理が正しく動作するか確認

### 3. test_multimedia.ks
**マルチメディアテスト**

テストする機能：
- 背景表示 `[bg storage="..."]`
- キャラクター表示 `[chara_show name="..." storage="..."]`
- キャラクター非表示 `[chara_hide name="..."]`
- BGM再生 `[playbgm storage="..." loop="true"]`
- BGM停止 `[stopbgm fadeout="..."]`
- SE再生 `[playse storage="..." volume="..."]`

推奨用途：画像・音声アセットの読み込みと表示/再生機能を確認

**注意**: このスクリプトを実行するには、以下のアセットファイルが必要です：
- 背景画像: `bg_room.png`, `bg_city.png`, `bg_forest.png`
- キャラクター画像: `chara_taro_normal.png`, `chara_hanako_smile.png` など
- BGM: `bgm_main.ogg`, `bgm_battle.ogg`, `bgm_field.ogg`
- SE: `se_click.ogg`, `se_door.ogg`

### 4. test_comprehensive.ks
**総合テスト**

テストする機能：
- すべての基本機能
- 複雑な制御フロー
- マルチメディア機能
- 実践的なRPGシナリオ形式

推奨用途：すべての機能が統合された状態で正しく動作するか確認

このスクリプトは小さなRPGデモとして実装されており、以下のシーンを含みます：
- オープニング
- 森での戦闘
- 町でのイベント（商店、宿屋）
- ステータス表示
- エンディング

## テストの実行方法

### 方法1: コンテンツブラウザから

1. UEエディタでこれらの`.ks`ファイルをコンテンツブラウザにドラッグ&ドロップ
2. インポートされたKScriptアセットをダブルクリック
3. KScriptViewerウィンドウで内容を確認
4. BlueprintまたはC++から`LoadAndStartScriptFromAsset`を呼び出して実行

### 方法2: ファイルパスから直接

```cpp
// C++
UKScriptSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UKScriptSubsystem>();
FString FilePath = FPaths::ProjectContentDir() + TEXT("Scripts/test_basic.ks");
Subsystem->LoadAndStartScript(FilePath);
```

### 方法3: Blueprint

1. `Get Game Instance` → `Get Subsystem` (KScriptSubsystem)
2. `Load And Start Script` ノードを使用
3. ファイルパスまたはアセット参照を指定

## 期待される結果

各テストスクリプトを実行すると：
- ログウィンドウに実行されたコマンドが表示される
- `[l]`や`[p]`でスクリプトが一時停止し、入力待ちになる
- `OnInput()`を呼び出すと実行が継続される
- 変数操作、条件分岐、ジャンプが正しく機能する
- マルチメディアコマンドがマネージャーを通じて処理される

## トラブルシューティング

### スクリプトが実行されない
- KScriptSubsystemが初期化されているか確認
- ファイルパスが正しいか確認
- ログウィンドウでエラーメッセージを確認

### マルチメディアが動作しない
- UIManagerが設定されているか確認
- アセットファイルが正しいパスに配置されているか確認
- ImageManager/AudioManagerが初期化されているか確認

### 変数が正しく動作しない
- VariableManagerが設定されているか確認
- 変数名の記法が正しいか確認（例: `f.変数名`）

## カスタマイズ

これらのテストスクリプトは、プロジェクトのニーズに合わせて自由にカスタマイズできます：
- 新しいコマンドを追加した場合は、対応するテストケースを追加
- 実際のゲームアセットのパスに合わせてファイル名を変更
- より複雑なシナリオを追加してストレステスト

## 今後の拡張

Phase3以降で以下の機能が追加される予定です：
- アニメーション・トランジション
- セーブ/ロード機能
- 選択肢とフラグ管理
- マクロとテンプレート

これらの機能が追加されたら、対応するテストスクリプトも追加されます。
