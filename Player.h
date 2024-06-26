#pragma once
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"

#include <numbers>

#include "MyMath.h"
#include "imgui.h"

/// <summary>
/// 自キャラ
/// </summary>
class Player {
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
	/// 浮遊ギミック
	/// </summary>
	void UpdateFloatingGimmick();

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
	void Initialize(Model* modelBody, Model* modelFace, Model* modelCore, Model* modelLeftArm, Model* modelRightArm, ViewProjection* viewProjection);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	// 浮遊ギミックの媒体変数
	float floatingParameter_ = 0.0f;

	// ワールド変換データ
	WorldTransform worldTransform_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformFace_;
	WorldTransform worldTransformCore_;
	WorldTransform worldTransformLeftArm_;
	WorldTransform worldTransformRightArm_;

	//ビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	//ViewProjection* viewProjection_ = nullptr;

	// モデル
	Model* faceModel_ = nullptr;
	Model* bodyModel_ = nullptr;
	Model* leftArmModel_ = nullptr;
	Model* rightArmModel_ = nullptr;
	Model* coreModel_ = nullptr;
};
