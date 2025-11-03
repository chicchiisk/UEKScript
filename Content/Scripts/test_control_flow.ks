; ========================================
; KScript 制御フローテスト
; ジャンプ、条件分岐、サブルーチン、変数操作など
; ========================================

; 変数初期化テスト
[eval exp="f.player_name='太郎'"]
[eval exp="f.hp=100"]
[eval exp="f.level=1"]

変数を初期化しました。[l][r]
プレイヤー名: 太郎[r]
HP: 100[r]
レベル: 1[l][r]

; 条件分岐テスト（真の場合）
[if exp="f.hp >= 50"]
HPが50以上です。健康です！[l][r]
[else]
HPが50未満です。危険です！[l][r]
[endif]

; 条件分岐テスト（偽の場合）
[eval exp="f.hp=30"]
[if exp="f.hp >= 50"]
HPが50以上です。健康です！[l][r]
[else]
HPが50未満です。危険です！（この文が表示されるはず）[l][r]
[endif]

; ネストした条件分岐テスト
[eval exp="f.level=5"]
レベルチェック：[l][r]
[if exp="f.level >= 5"]
レベルは5以上です。[r]
[if exp="f.level >= 10"]
さらにレベル10以上です！[l][r]
[else]
でもレベル10未満です。[l][r]
[endif]
[else]
レベルは5未満です。[l][r]
[endif]

; サブルーチンテスト
サブルーチンを呼び出します。[l][r]
[call target="*subroutine1"]
サブルーチンから戻りました。[l][r]

; ジャンプテスト
ジャンプテストを開始します。[l][r]
[jump target="*after_skip"]

*skip_section
このテキストはスキップされるため表示されません。[l][r]

*after_skip
ジャンプ先に到達しました。[l][r]

; 変数の計算テスト
[eval exp="f.gold=100"]
所持金: 100ゴールド[l][r]
[eval exp="f.gold=f.gold+50"]
50ゴールド獲得しました！[l][r]
所持金: 150ゴールド（のはず）[l][r]

制御フローテストが完了しました。[l]
[jump target="*end"]

; サブルーチン定義
*subroutine1
これはサブルーチン1です。[l][r]
サブルーチン内でも変数が使えます。[l][r]
[eval exp="f.subroutine_called=1"]
[return]

*end
; テスト終了
