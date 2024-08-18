#pragma once
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"

#include <DebugText.h>
#include <functional>
#include <algorithm>
#include <list>
#include <numbers>

#include "EnemyBullet.h"
#include "TimedCall.h"
#include "MyMath.h"
#include "imgui.h"

/// <summary>
/// 敵
/// </summary>
class Enemy {

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy();

	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

	/// <summary>
	/// 弾を発射し、タイマーをリセットするコールバック関数
	/// </summary>
	void FireReset();

	/// <summary>
	/// 接近フェーズの初期化
	/// </summary>
	void PhaseInitialize();

public: /* 基本的な関数 */
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="textureHandle"></param>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

private:
	// 行動フェーズ
	enum class Phase {
		APPROACH, // 接近する
		LEACE,    // 離脱する
	};

	/*
	   メンバ変数
	*/

	// Bullets
	std::list<EnemyBullet*> bullets_;

	// 時限発動のリスト
	std::list<TimedCall*> timedCalls_;

	// 発射間隔
	static const int kFireInterval = 60;

	// 発射タイマー
	int32_t fireTimer = 0;

	// フェーズ
	Phase phase_;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
};
