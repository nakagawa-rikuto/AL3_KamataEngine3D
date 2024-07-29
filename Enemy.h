#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"

class Player;

/// <summary>
/// 敵
/// </summary>
class Enemy {
private:
	/*//////////////////////////////////////////////////////////////////////
	                           メンバ変数
	*///////////////////////////////////////////////////////////////////////

	// WorldTransform
	WorldTransform worldTransform_;

	// Model
	Model* model_;

	// ViewProjection
	ViewProjection* viewProjection_;

	/*///////////////////////////////////////////
	                簡単な移動
	*////////////////////////////////////////////
	// 歩行の速さ
	static inline const float kWalkSpeed_ = 0.05f;

	// 速度
	Vector3 velocity_ = {};

	/*///////////////////////////////////////////
	        　　     衝突判定
	*////////////////////////////////////////////
	// キャラクターの当たり判定サイズ
	static inline const float kWidth_ = 1.6f;
	static inline const float kHeight_ = 1.6f;

	/*///////////////////////////////////////////
	        　　     アニメーション
	*////////////////////////////////////////////
	// 最初の角度[度]
	static inline const float kWalkMotionAngleStart_ = 0.0f;

	// 最後の角度[度]
	static inline const float kWalkMotionAngleEnd_ = 50.0f;

	// アニメーションの周期となる時間[秒]
	static inline const float kWalkMotionTime_ = 1.0f;

	// 経過時間
	float walkTimer_ = 0.0f;

	float param_;

	float radian_;

private: /* メンバ関数 */

	

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="viewProjection"></param>
	/// <param name="pos"></param>
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& pos);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突判定
	/// </summary>
	void OnCollision(const Player* player);

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();

	/// <summary>
	/// AABBを取得
	/// </summary>
	AABB GetAABB();
};
