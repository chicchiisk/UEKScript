; ========================================
; KScript 総合テスト
; すべての機能を組み合わせた実践的なテストシナリオ
; ========================================

; ゲーム初期化
[eval exp="f.chapter=1"]
[eval exp="f.player_name='勇者'"]
[eval exp="f.hp=100"]
[eval exp="f.max_hp=100"]
[eval exp="f.gold=500"]
[eval exp="f.visited_town=0"]

; オープニング
[bg storage="bg_title.png"]
[playbgm storage="bgm_title.ogg" loop="true" volume="80"]

=== KScript RPG デモ ===[l][r]
総合機能テストシナリオ[p]

; 第1章開始
[cm]
[bg storage="bg_forest.png"]
[playbgm storage="bgm_field.ogg" loop="true" volume="70"]

*chapter1
第1章: 冒険の始まり[l][r]

あなたは森の中を歩いていました。[l][r]
[chara_show name="hero" storage="chara_hero_normal.png" left="200" top="50"]

[if exp="f.chapter == 1"]
初めての冒険です。[l][r]
[else]
この場所には見覚えがあります。[l][r]
[endif]

突然、モンスターが現れた！[l][r]
[playse storage="se_encounter.ogg" volume="90"]

; 戦闘シーン
[call target="*battle"]

; 戦闘後
戦闘に勝利しました！[l][r]
[eval exp="f.gold=f.gold+100"]
100ゴールドを獲得しました。[l][r]
現在の所持金: 所持金ゴールド[l][r]

; 町へ移動
[cm]
[chara_hide name="hero"]
森を抜けて町に到着しました。[l][r]
[bg storage="bg_town.png"]
[playbgm storage="bgm_town.ogg" loop="true" volume="75"]
[eval exp="f.visited_town=1"]

*town
町の人々で賑わっています。[l][r]

; 町でのイベント
[if exp="f.visited_town == 1"]
初めて訪れる町です。[l][r]
[chara_show name="npc" storage="chara_npc_merchant.png" left="300" top="50"]
商人: ようこそ！何か買っていきませんか？[l][r]
[call target="*shop"]
[chara_hide name="npc"]
[else]
見慣れた町の風景です。[l][r]
[endif]

; 宿屋で回復
[cm]
宿屋に泊まりますか？[l][r]

[if exp="f.gold >= 50"]
50ゴールドで宿に泊まりました。[l][r]
[eval exp="f.gold=f.gold-50"]
[eval exp="f.hp=f.max_hp"]
HPが全回復しました！[l][r]
[playse storage="se_heal.ogg" volume="85"]
[else]
お金が足りません...[l][r]
[endif]

; ステータス確認
[call target="*show_status"]

; 第2章へ
[cm]
町を出発し、次の目的地へ向かいます。[l][r]
[bg storage="bg_mountain.png"]
[playbgm storage="bgm_dungeon.ogg" loop="true" volume="80"]

第2章: 試練の山[l][r]
険しい山道を進んでいきます...[l][r]

; エンディング
[call target="*ending"]
[jump target="*finish"]

; ========================================
; サブルーチン定義
; ========================================

*battle
[bg storage="bg_battle.png"]
[playbgm storage="bgm_battle.ogg" loop="true" volume="90"]
[cm]
--- 戦闘開始 ---[l][r]
[chara_show name="hero" storage="chara_hero_battle.png" left="100" top="50"]
[chara_show name="monster" storage="chara_monster.png" left="500" top="50"]

勇者の攻撃！[l][r]
[playse storage="se_attack.ogg" volume="95"]
[eval exp="f.monster_hp=50"]
[eval exp="f.monster_hp=f.monster_hp-30"]

モンスターに30のダメージ！[l][r]

モンスターの反撃！[l][r]
[playse storage="se_damage.ogg" volume="85"]
[eval exp="f.hp=f.hp-15"]
勇者は15のダメージを受けた！[l][r]

[if exp="f.hp > 0"]
最後の一撃！[l][r]
[playse storage="se_attack.ogg" volume="100"]
[else]
危機的状況です！[l][r]
[endif]

[chara_hide name="monster"]
モンスターを倒した！[l][r]
[playse storage="se_victory.ogg" volume="90"]
[return]

*shop
商人: この薬草は100ゴールドだよ。[l][r]

[if exp="f.gold >= 100"]
薬草を購入しました。[l][r]
[eval exp="f.gold=f.gold-100"]
[eval exp="f.items_herb=1"]
[playse storage="se_buy.ogg" volume="80"]
[else]
お金が足りません。[l][r]
[endif]
[return]

*show_status
[cm]
=== ステータス ===[r]
名前: 勇者[r]
HP: HP / MaxHP[r]
所持金: 所持金ゴールド[r]
章: Chapter章[l][r]
[return]

*ending
[cm]
[bg storage="bg_ending.png"]
[playbgm storage="bgm_ending.ogg" loop="true" volume="70"]
[stopbgm fadeout="3000"]

--- エンディング ---[l][r]
こうして勇者の冒険は続いていく...[l][r]
[p]

スタッフロール[r]
企画・開発: KScriptチーム[r]
音楽: (音楽担当)[r]
グラフィック: (グラフィック担当)[l][r]
[p]

THE END[l][r]
[return]

*finish
[cm]
総合テストが完了しました。[l][r]
お疲れ様でした！[l]

; ========================================
; テスト終了
;
; このスクリプトは以下の機能をテストします：
; - テキスト表示 (通常、改行、ページ区切り)
; - クリック待ち [l] / ページ区切り [p]
; - メッセージクリア [cm]
; - 変数の定義と操作 [eval]
; - 条件分岐 [if] [else] [endif]
; - ラベルジャンプ [jump]
; - サブルーチン [call] [return]
; - 背景表示 [bg]
; - キャラクター表示/非表示 [chara_show] [chara_hide]
; - BGM再生/停止 [playbgm] [stopbgm]
; - SE再生 [playse]
; ========================================
