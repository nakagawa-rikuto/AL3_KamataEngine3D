#pragma once
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"

#include <DebugText.h>
#include <algorithm>
#include <numbers>
#include <list>

#include "EnemyBullet.h"
#include "MyMath.h"
#include "imgui.h"

// 自機クラスの前方宣言
class Player;

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
	/// タイマーのカウント
	/// </summary>
	void CountTimer();

	/// <summary>
	/// 接近フェーズの初期化
	/// </summary>
	void PhaseInitialize();

	/// <summary>
	/// 接近フェーズ
	/// </summary>
	void ApproachPhase();

	/// <summary>
	/// 離脱フェーズ
	/// </summary>
	void LeasePhase();

	/// <summary>
	/// Setter
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; }

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();

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

	// Player
	Player* player_ = nullptr;

	// Bullets
	std::list<EnemyBullet*> bullets_;

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
