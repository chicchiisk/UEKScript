; ====================================
; Phase3機能テストスクリプト
; セーブ/ロード、トランジション機能のテスト
; ====================================

*start

; タイトル表示
Phase3機能のテストを開始します[l][r]

; ====================================
; テスト1: 基本的なトランジション
; ====================================

*test_transition

; フェードトランジションのテスト[l][cm]
[trans method="fade" time="1000"]
[bg storage="bg_room.png"]
フェードイン効果で背景が表示されました[l][cm]

; キャラクター表示（スライド）[l]
[trans method="slide_left" time="800"]
[chara_show name="hero" storage="chara_hero.png" left="100" top="100"]
左からスライドしてキャラクターが登場しました[l][cm]

; ====================================
; テスト2: 変数とセーブ機能
; ====================================

*test_save

; 変数を設定[l]
[eval exp="f.player_name='太郎'"]
[eval exp="f.hp=100"]
[eval exp="f.level=5"]
[eval exp="f.score=1500"]

変数を設定しました：[r]
名前: [emb exp="f.player_name"][r]
HP: [emb exp="f.hp"][r]
レベル: [emb exp="f.level"][r]
スコア: [emb exp="f.score"][l][cm]

; セーブポイント[l]
ここでセーブします...[l]
[save slot="test_save01"]
セーブが完了しました！[l][cm]

; 変数を変更[l]
[eval exp="f.hp=50"]
[eval exp="f.level=10"]
[eval exp="f.score=3000"]

変数を変更しました：[r]
HP: [emb exp="f.hp"] (100→50に変更)[r]
レベル: [emb exp="f.level"] (5→10に変更)[r]
スコア: [emb exp="f.score"] (1500→3000に変更)[l][cm]

; ====================================
; テスト3: ロード機能
; ====================================

*test_load

セーブデータをロードしてみます...[l]
[load slot="test_save01"]

; この部分はロード後に実行される
ロードが完了しました！[r]
変数は元の値に戻っているはずです：[r]
HP: [emb exp="f.hp"] (50→100に復元)[r]
レベル: [emb exp="f.level"] (10→5に復元)[r]
スコア: [emb exp="f.score"] (3000→1500に復元)[l][cm]

; ====================================
; テスト4: 複数のトランジション
; ====================================

*test_multi_transition

様々なトランジション効果を試します[l][cm]

; 右からスライド[l]
[trans method="slide_right" time="600"]
[chara_show name="partner" storage="chara_partner.png" left="500" top="100"]
右からスライド[l][cm]

; 上からスライド[l]
[trans method="slide_up" time="600"]
[bg storage="bg_sky.png"]
上からスライド（背景変更）[l][cm]

; 下からスライド[l]
[trans method="slide_down" time="600"]
[bg storage="bg_ground.png"]
下からスライド（背景変更）[l][cm]

; クロスフェード[l]
[trans method="crossfade" time="1200"]
[bg storage="bg_sunset.png"]
クロスフェードで背景がスムーズに切り替わりました[l][cm]

; ====================================
; テスト5: 条件分岐とセーブ/ロード
; ====================================

*test_conditional_save

[eval exp="f.choice=0"]

重要な選択肢です：[r]
1. 戦う[r]
2. 逃げる[l][cm]

; オートセーブ[l]
[save slot="auto_save"]

; 選択によって分岐（テストでは自動的に1を選択）[l]
[eval exp="f.choice=1"]

[if exp="f.choice==1"]
あなたは戦うことを選びました！[l]
[eval exp="f.courage=100"]
勇気が100になりました[l][cm]
[else]
あなたは逃げることを選びました[l]
[eval exp="f.courage=0"]
勇気が0になりました[l][cm]
[endif]

現在の勇気: [emb exp="f.courage"][l][cm]

; ====================================
; テスト6: マルチメディアとの組み合わせ
; ====================================

*test_multimedia

; BGMを再生[l]
[playbgm storage="bgm_battle.ogg" volume="0.8"]
BGMが再生されました[l][cm]

; トランジション付きで背景変更[l]
[trans method="fade" time="1500"]
[bg storage="bg_battle.png"]
戦闘画面にフェードイン[l][cm]

; SEを再生[l]
[playse storage="se_attack.wav"]
攻撃SE！[l][cm]

; キャラクターを非表示（フェードアウト）[l]
[trans method="fade" time="800"]
[chara_hide name="hero"]
キャラクターがフェードアウト[l][cm]

; BGMを停止（フェードアウト）[l]
[stopbgm time="2000"]
BGMがフェードアウトしていきます...[l][cm]

; ====================================
; テスト7: サブルーチンとセーブ/ロード
; ====================================

*test_subroutine

サブルーチンのテスト[l][cm]

[eval exp="f.count=0"]
カウント: [emb exp="f.count"][l]

; サブルーチン呼び出し[l]
[call target="*increment_count"]

カウント増加後: [emb exp="f.count"][l][cm]

; ここでセーブ[l]
[save slot="subroutine_test"]
サブルーチン後のセーブ完了[l][cm]

[jump target="*test_complete"]

; サブルーチン定義[l]
*increment_count
[eval exp="f.count=f.count+1"]
カウントを増やしました[l]
[return]

; ====================================
; 完了
; ====================================

*test_complete

[cm]
すべてのテストが完了しました！[l][r]
[r]
実装された機能：[r]
✓ セーブ/ロード機能[r]
✓ トランジションエフェクト（フェード、スライド）[r]
✓ 変数との連携[r]
✓ マルチメディアとの組み合わせ[r]
✓ 条件分岐・サブルーチンとの統合[l][cm]

Phase3機能のテストを終了します[l]

*end
