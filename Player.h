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
#include "MyMath.h"
#include "imgui.h"

/// <summary>
/// 自キャラ
/// </summary>
class Player : public BaseCharacter {
public:
	/// <summary>
	/// 移動
	/// </summary>
	void Move();

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
	/// ダッシュ行動の初期化
	/// </summary>
	void BehaviorDashInitialize();

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
	/// ダッシュ行動の更新
	/// </summary>
	void BehaviorDashUpdate();

	/* ///////////////////////////////////////////////
	                 セッター・ゲッター
	*/ ///////////////////////////////////////////////
	/// <summary>
	/// ViewProjectionを取得
	/// </summary>
	/// <param name="viewProjection"></param>
	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection; }

	/// <summary>
	/// PlayerのWorldTransformを渡す
	/// </summary>
	/// <returns></returns>
	WorldTransform& GetWorldTransform() { return worldTransform_; }

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="viewProjection"></param>
	void Initialize(const std::vector<Model*>& models) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection) override;

private:
	// PlayerModel
	enum ModelNum { kModelIndexBody, kModelIndexFace, kModelIndexCore, kModelIndexL_Arm, kModelIndexR_Arm, kModelIndexWeapon };

	// 振る舞い
	enum class Behavior {
		kRoot,   // 通常状態
		kAttack, // 攻撃中
		kDash,
	};

	// 振る舞い
	Behavior behavior_ = Behavior::kRoot;
	float changeTimer_ = 300.0f;

	// 次の振る舞いリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt; // std::nullpotは無効状態であることを表す値

	// 浮遊ギミックの媒体変数
	float floatingParameter_ = 0.0f;

	// 腕ギミックの媒体変数
	float armParameter_ = 0.0f;

	Vector3 velocity_;

	// ワールド変換データ
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformFace_;
	WorldTransform worldTransformCore_;
	WorldTransform worldTransformLeftArm_;
	WorldTransform worldTransformRightArm_;
	WorldTransform worldTransformWeapon_;

	// ビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	// ダッシュ用ワーク
	struct WorkDash {
		// ダッシュ用の媒体変数
		uint32_t dashParameter_ = 0;
	};

	WorkDash workDash_;

	const float destinationAngleY_ = 1.0f;
};