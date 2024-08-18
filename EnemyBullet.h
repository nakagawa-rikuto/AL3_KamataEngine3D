#pragma once
#include "Input.h"
#include "Model.h"
#include "MyMath.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"
#include "imgui.h"

#include <DebugText.h>
#include <algorithm>
#include <numbers>

class Player;

/// <summary>
/// 敵の弾
/// </summary>
class EnemyBullet {
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyBullet();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="position"></param>
	/// <param name="velocity"></param>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	///
	/// </summary>
	/// <returns></returns>
	bool IsDead() const { return isDead_; }

	void SetPlayer(Player* player) { player_ = player; }

private:
	/* /////////////////////////
	           メンバ変数
	*/ /////////////////////////

	Player* player_;

	// 速度
	Vector3 velocity_;

	// 補間割合
	float t = 1.0f;

	// 寿命
	static const int32_t kLifeTime_ = 60 * 5;

	// デスタイマー
	int32_t deathTimer_ = kLifeTime_;

	// デスフラグ
	bool isDead_ = false;

	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデルのポインタ
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
};
