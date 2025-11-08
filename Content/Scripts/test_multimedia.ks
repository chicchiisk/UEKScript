; ========================================
; KScript マルチメディアテスト
; 背景表示、キャラクター表示、BGM・SE再生など
; ========================================

マルチメディア機能のテストを開始します。[l][r]

; 背景表示テスト
背景を表示します。[l][r]
[bg storage="bg_room.png"]
背景「部屋」を表示しました。[l][r]

; 背景切り替えテスト
背景を切り替えます。[l][r]
[bg storage="bg_city.png"]
背景「街」を表示しました。[l][r]

; キャラクター表示テスト
[cm]
キャラクターを表示します。[l][r]
[chara_show name="taro" storage="chara_taro_normal.png" left="100" top="50"]
太郎が登場しました！[l][r]

; 複数キャラクター表示
[chara_show name="hanako" storage="chara_hanako_smile.png" left="400" top="50"]
花子も登場しました！[l][r]

; キャラクター切り替え（表情変更）
[chara_show name="taro" storage="chara_taro_smile.png" left="100" top="50"]
太郎の表情が変わりました。[l][r]

; キャラクター非表示テスト
[chara_hide name="hanako"]
花子が退場しました。[l][r]

; BGM再生テスト
[cm]
BGMを再生します。[l][r]
[playbgm storage="bgm_main.ogg" loop="true" volume="80"]
メインテーマを再生中...[l][r]

; BGM切り替えテスト
BGMを切り替えます。[l][r]
[playbgm storage="bgm_battle.ogg" loop="true" volume="90"]
バトルBGMを再生中...[l][r]

; BGM停止テスト（フェードアウト付き）
BGMをフェードアウトで停止します。[l][r]
[stopbgm fadeout="2000"]
2秒かけてフェードアウト中...[l][r]

; SE再生テスト
効果音を再生します。[l][r]
[playse storage="se_click.ogg" volume="100"]
クリック音を再生しました。[l][r]

[playse storage="se_door.ogg" volume="80"]
ドアの音を再生しました。[l][r]

; 複合テスト（背景＋キャラ＋BGM）
[cm]
[bg storage="bg_forest.png"]
[chara_show name="taro" storage="chara_taro_normal.png" left="200" top="50"]
[playbgm storage="bgm_field.ogg" loop="true" volume="70"]
森の中を歩いています...[l][r]

; クリーンアップ
[chara_hide name="taro"]
[stopbgm]

[cm]
マルチメディアテストが完了しました。[l][r]

; 注意書き
※実際のテストには、以下のアセットファイルが必要です：[r]
- 背景画像: bg_room.png, bg_city.png, bg_forest.png[r]
- キャラクター画像: chara_taro_*.png, chara_hanako_*.png[r]
- BGM: bgm_main.ogg, bgm_battle.ogg, bgm_field.ogg[r]
- SE: se_click.ogg, se_door.ogg[l]
