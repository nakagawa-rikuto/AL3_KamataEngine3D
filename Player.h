#pragma once
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"

#include <numbers>

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

	/// <summary>
	/// 浮遊魏キックの初期化
	/// </summary>
	void InitializeFloatingGimmick();

	/// <summary>
	/// 腕のギミックの初期化
	/// </summary>
	void InitializeArmGimmick();

	/// <summary>
	/// 浮遊ギミック
	/// </summary>
	void UpdateFloatingGimmick();

	/// <summary>
	/// 腕のギミック
	/// </summary>
	void UpdateArmGimmick();

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
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection) override;

private:

	enum ModelNum { 
		kModelIndexBody,
		kModelIndexFace,
		kModelIndexCore,
		kModelIndexL_Arm,
		kModelIndexR_Arm
	};

	// 浮遊ギミックの媒体変数
	float floatingParameter_ = 0.0f;

	// 腕ギミックの媒体変数
	float armParameter_ = 0.0f;

	// ワールド変換データ
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformFace_;
	WorldTransform worldTransformCore_;
	WorldTransform worldTransformLeftArm_;
	WorldTransform worldTransformRightArm_;

	//ビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
};
