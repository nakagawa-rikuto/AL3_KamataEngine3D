#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"
#include "cassert"

/// <summary>
/// 自キャラ
/// </summary>
class Player {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="viewProjection"></param>
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
	/// 速度加算
	/// </summary>
	/// <returns></returns>
	const Vector3& GetVelocity() const { return velocity_; }

	WorldTransform& GetWorldTransform() { return worldTransform_; }

private:

	// 左右
	enum class LRDirection {
		kRight,
		kLeft,
	};

	// 速度
	Vector3 velocity_ = {};

	// 左右
	LRDirection lrDirection_ = LRDirection::kRight;

	/*///////////////////////////////////////////
	                 角度補間
	*////////////////////////////////////////////
	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;

	// 旋回タイマー
	float turnTimer_ = 0.0f;

	// 旋回時間<秒>
	static inline const float kTimeTurn_ = 0.3f;

	/*///////////////////////////////////////////
	                 左右移動
	*////////////////////////////////////////////
	// 慣性移動
	static inline const float KAcceleration_ = 0.02f;

	// 速度減衰
	static inline const float kAttenuation_ = 0.1f;

	// 最大速度制限
	static inline const float kLimitRunSpeed_ = 4.0f;

	/*///////////////////////////////////////////
	                 ジャンプ
	*////////////////////////////////////////////
	// 接地状態フラグ
	bool onGround_ = true;

	// 重力加速度(下方向)
	static inline const float kGravityAcceleration_ = 0.05f;

	// 最大落下速度(下方向)
	static inline const float kLimitFallSpeed_ = 1.0f;

	// ジャンプ初速(上方向)
	static inline const float kJumpAcceleration_ = 1.0f;

	// 
	static inline const float kAttenuationLanding_ = 0.05f;

	// ワールド変換データ
	WorldTransform worldTransform_;

	// ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;

	// モデル
	Model* model_ = nullptr;
};
