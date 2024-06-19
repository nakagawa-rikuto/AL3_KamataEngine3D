#pragma once
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"

#include "MyMath.h"
#include "imgui.h"

/// <summary>
/// 追従カメラ
/// </summary>
class FollowCamera {

public:

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// 旋回処理
	/// </summary>
	void Rotate();

	/// <summary>
	/// targetの座標を受け取る
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(const WorldTransform* target) { target_ = target; }

	/// <summary>
	/// viewProjectionを渡す
	/// </summary>
	/// <returns></returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private:
	// ビュープロジェクション
	ViewProjection viewProjection_;

	// 追従対象
	const WorldTransform* target_ = nullptr;
};