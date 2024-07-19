#pragma once
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"

#include "MyMath.h"
#include "imgui.h"

// 前方宣言
class LockOn;

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
	/// ロックオン状態の処理
	/// </summary>
	void LockOnMove();

	/// <summary>
	/// targetの座標を受け取る
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(const WorldTransform* target) { target_ = target; }

	/// <summary>
	/// 
	/// </summary>
	/// <param name="lockOn"></param>
	void SetLockOn(const LockOn* lockOn) { lockOn_ = lockOn; }

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
	// ロックオン
	const LockOn* lockOn_ = nullptr;
	bool isLockOn_ = false;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// 追従対象
	const WorldTransform* target_ = nullptr;
};