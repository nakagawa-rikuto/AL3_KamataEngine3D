#pragma once
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"

#include <numbers>
#include <optional>

#include "BaseCharacter.h"
#include "Collider.h"
#include "MyMath.h"
#include "imgui.h"

// 前方宣言
class LockOn;

/// <summary>
/// 自キャラ
/// </summary>
class Player : public BaseCharacter {
public:

	/* ///////////////////////////////////////////////
	                 セッター・ゲッター
	*/ ///////////////////////////////////////////////
	/// <summary>
	/// ViewProjectionを取得
	/// </summary>
	/// <param name="viewProjection"></param>
	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection; }

	/// <summary>
	/// LockOnを取得
	/// </summary>
	/// <param name="lockOn"></param>
	void SetLockOn(LockOn* lockOn) { lockOn_ = lockOn; }

	/// <summary>
	/// PlayerのWorldTransformを取得
	/// </summary>
	/// <returns></returns>
	const WorldTransform& GetWorldTransform() const override { return worldTransform_; }

	/// <summary>
	/// 中心座標を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetCenterPosition() const override;

	/* ///////////////////////////////////////////////
	                    衝突判定
	*/ ///////////////////////////////////////////////
	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision() override;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize(const std::vector<Model*>& models) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	/// /// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection) override;

private: /* メンバ変数 */

	// PlayerModel
	enum ModelNum { kModelIndexBody, kModelIndexFace, kModelIndexCore, kModelIndexL_Arm, kModelIndexR_Arm, kModelIndexWeapon };

	// 振る舞い(ビヘイビア)
	enum class Behavior {
		kRoot,   // 通常状態
		kAttack, // 攻撃中
		kJump,   // ジャンプ中
	};

	Input* input_ = nullptr;
	LockOn* lockOn_ = nullptr;

	// 振る舞い
	Behavior behavior_ = Behavior::kRoot;

	// 次の振る舞いリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt; // std::nullpotは無効状態であることを表す値

	// 浮遊ギミックの媒体変数
	float floatingParameter_ = 0.0f;

	// 腕ギミックの媒体変数
	float armParameter_ = 0.0f;

	// Attackの際に使用する変数
	// 定数
	const float maxVelocity_ = 3.0f;  // 最大速度
	const float acceleration_ = 0.1f; // 加速度
	const float deceleration_ = 0.1f; // 減速度
	const float startAngleWeapon_ = -30.0f;
	const float endAngleWeapon_ = 100.0f;
	const float moveSpeed_ = 0.5f;

	// 状態
	float weaponAngle_ = startAngleWeapon_; // 初期角度

	// 速度
	Vector3 velocity_ = {};

	// ワールド変換データ
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformFace_;
	WorldTransform worldTransformCore_;
	WorldTransform worldTransformLeftArm_;
	WorldTransform worldTransformRightArm_;
	WorldTransform worldTransformWeapon_;

	// ビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

private: /* メンバ関数 */

	/* ///////////////////////////////////////////////
	                    初期化
	*/ ///////////////////////////////////////////////
	/// <summary>
	/// 浮遊魏キックの初期化
	/// </summary>
	void InitializeFloatingGimmick();

	/// <summary>
	/// 腕のギミックの初期化
	/// </summary>
	void InitializeArmGimmick();

	/// <summary>
	/// 通常行動の初期化
	/// </summary>
	void BehaviorRootInitialize();

	/// <summary>
	/// 攻撃行動の初期化
	/// </summary>
	void BehaviorAttackInitialize();

	/// <summary>
	/// ジャンプ行動の初期化
	/// </summary>
	void BehaviorJumpInitialize();

	/* ///////////////////////////////////////////////
	                    更新
	*/ ///////////////////////////////////////////////
	/// <summary>
	/// 浮遊ギミックの更新
	/// </summary>
	void UpdateFloatingGimmick();

	/// <summary>
	/// 腕ギミックの更新
	/// </summary>
	void UpdateArmGimmick();

	/// <summary>
	/// 通常行動の更新
	/// </summary>
	void BehaviorRootUpdate();

	/// <summary>
	/// 攻撃行動の更新
	/// </summary>
	void BehaviorAttackUpdate();

	/// <summary>
	/// ジャンプ行動の更新
	/// </summary>
	void BehaviorJumpUpdate();

	/// <summary>
	/// 調整項目の適用
	/// </summary>
	void ApplyGlobalVariables();
};
