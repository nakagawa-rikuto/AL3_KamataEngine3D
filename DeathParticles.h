#pragma once
#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"
#include "imgui.h"

#include <array>
#include <DebugText.h>
#include <algorithm>
#include <numbers>

// 前方宣言
class Player;

/// <summary>
/// デス演出用パーティクル
/// </summary>
class DeathParticles {
public: 

	/// <summary>
	/// デスフラグのGetter
	/// </summary>
	/// <returns></returns>
	bool IsFinished();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, ViewProjection* viewProjection, Vector3 position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: /* メンバ変数 */

	// パーティクルの個数
	static inline const uint32_t kNumParticles = 8;

	// 終了フラグ
	bool isFinished_ = false;

	// 経過時間カウント
	float counter_ = 0.0f;

	/* //////////////
	    フェードアウト
	*/ //////////////
	// 色変更オブジェクト
	ObjectColor objectColor_;

	// 色の数値
	Vector4 color_;

	/* //////////////
	    移動についての定数
	*/ //////////////
	// 存在時間（消滅までの時間）＜秒＞
	static inline const float kDuration_ = 1.0f;

	// 移動の速さ
	static inline const float kSpeed_ = 0.1f;

	// 分割した１個分の角度
	static inline const float kAngleUnit = 2.0f * pi() / float(kNumParticles);

	// ワールドトランスフォーム
	std::array<WorldTransform, kNumParticles> worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;
};
