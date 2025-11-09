; ====================================
; トランジションエフェクトテスト
; 様々なトランジション効果の確認
; ====================================

*start

トランジションエフェクトのテストを開始します[l][cm]

; ====================================
; フェードイン/アウト
; ====================================

*fade_test

【フェードテスト】[l][cm]

; 1秒かけてフェードイン
[trans method="fade" time="1000"]
[bg storage="bg_forest.png"]
背景が1秒かけてフェードインします[l][cm]

; 0.5秒かけてフェードイン
[trans method="fade" time="500"]
[chara_show name="hero" storage="chara_hero.png" left="200" top="100"]
キャラクターが0.5秒でフェードイン[l][cm]

; ====================================
; スライドトランジション
; ====================================

*slide_test

【スライドテスト】[l][cm]

; 左からスライド
[trans method="slide_left" time="800"]
[chara_show name="enemy" storage="chara_enemy.png" left="600" top="100"]
敵が左からスライドイン！[l][cm]

; 右からスライド
[trans method="slide_right" time="800"]
[chara_show name="friend" storage="chara_friend.png" left="100" top="300"]
仲間が右からスライドイン[l][cm]

; 上からスライド
[trans method="slide_up" time="600"]
[bg storage="bg_sky.png"]
空の背景が上からスライド[l][cm]

; 下からスライド
[trans method="slide_down" time="600"]
[bg storage="bg_ground.png"]
地面の背景が下からスライド[l][cm]

; ====================================
; クロスフェード
; ====================================

*crossfade_test

【クロスフェードテスト】[l][cm]

; 通常のフェード
[trans method="fade" time="1000"]
[bg storage="bg_day.png"]
昼の背景（通常フェード）[l][cm]

; クロスフェード
[trans method="crossfade" time="1500"]
[bg storage="bg_night.png"]
夜の背景（クロスフェード）[r]
前の背景がフェードアウトしながら新しい背景がフェードインします[l][cm]

; ====================================
; 速度の違い
; ====================================

*speed_test

【トランジション速度テスト】[l][cm]

; 超高速（200ms）
[trans method="fade" time="200"]
[bg storage="bg_fast.png"]
超高速フェード（0.2秒）[l][cm]

; 高速（500ms）
[trans method="fade" time="500"]
[bg storage="bg_normal.png"]
通常フェード（0.5秒）[l][cm]

; ゆっくり（2000ms）
[trans method="fade" time="2000"]
[bg storage="bg_slow.png"]
ゆっくりフェード（2秒）[r]
じっくりと背景が変わります[l][cm]

; ====================================
; トランジションなし（即座に切り替え）
; ====================================

*no_transition_test

【トランジションなしテスト】[l][cm]

; トランジション設定なし（即座に変更）
[bg storage="bg_instant.png"]
即座に背景が切り替わります（トランジションなし）[l][cm]

; 再度トランジションを設定
[trans method="fade" time="800"]
[bg storage="bg_final.png"]
再びフェード効果が適用されます[l][cm]

; ====================================
; 完了
; ====================================

*complete

トランジションテストが完了しました！[l][r]
[r]
テストした効果：[r]
- フェード（fade）[r]
- クロスフェード（crossfade）[r]
- スライド左（slide_left）[r]
- スライド右（slide_right）[r]
- スライド上（slide_up）[r]
- スライド下（slide_down）[r]
- 速度の違い（200ms〜2000ms）[l][cm]

テスト終了[l]

*end
