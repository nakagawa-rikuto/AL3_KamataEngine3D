#pragma once
#include "Vector4.h"
#include "ViewProjection.h"
#include "MyMath.h"

// 前方宣言
class Player;

/// <summary>
/// カメラコントローラ
/// </summary>
class CameraController {
public:

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(ViewProjection* viewProjection);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// セッター
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(Player* target) { target_ = target; }

	/// <summary>
	/// セッター
	/// </summary>
	/// <param name="area"></param>
	void SetMovableArea(Vector4 area) { 
		movableArea_.left = area.x; 
		movableArea_.right = area.y;
		movableArea_.bottom = area.z;
		movableArea_.top = area.w;
	}


private:

	// 矩形
	struct Rect {
		float left = 0.0f;    // 左端
 		float right = 1.0f;   // 右端
		float bottom = 0.0f;  // 下端
		float top = 1.0f;     // 上端
	};

	// ビュープロジェクション
	ViewProjection* viewProjection_;

	/* ////////////////////////////////////////
	              追従対象のポインタ
	*/ ////////////////////////////////////////
	Player* target_;

	/* ////////////////////////////////////////
	                瞬間合わせ
	*/ ////////////////////////////////////////
	// 追従対象とカメラの座標の差(オフセット)
	Vector3 targetOffset_ = {0, 0, -30.0f};

	/* ////////////////////////////////////////
	                カメラ移動範囲
	*/ ////////////////////////////////////////
	// カメラ移動範囲
	Rect movableArea_;

	/* ////////////////////////////////////////
	                目標座標
	*/ ////////////////////////////////////////
	// カメラの目標座標
	Vector3 goalPos_;

	// 座標補間割合
	static inline const float kInterpolationRate_ = 0.1f;

	/* ////////////////////////////////////////
	                速度加算
	*/ ////////////////////////////////////////
	// 速度掛け率
	static inline const float kVelocityBias_ = 15.0f;

	Vector3 targetVelocity_;

	/* ////////////////////////////////////////
	                追従対象を画面内に収める
	*/ ////////////////////////////////////////
	// 追従対象の各方向へのカメラ移動範囲
	static inline const Rect targetMargin_ = {10.0f, -10.0f, 10.0f, -10.0f};
};
