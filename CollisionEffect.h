#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"

#include <numbers>
#include <array>
#include <DebugText.h>
#include <algorithm>
#include <optional>
#include <memory>

#include "MyMath.h"
#include "imgui.h"

class Collider;

/// <summary>
/// 衝突時のエフェクト
/// </summary>
class CollisionEffect {
public:

	/// <summary>
	/// Getter
	/// </summary>
	/// <returns></returns>
	bool IsFinished();

	/// <summary>
	/// エフェクトの開始
	/// </summary>
	void Start(Collider* collider);

public: /* 基礎的な関数 */
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

private: /* メンバ変数 */

	// 終了フラグ
	bool isFinish_ = true;

	// 色変更オブジェクト
	ObjectColor objectColor_;

	// 色の数値
	Vector4 color_;

	// モデル
	std::unique_ptr<Model> model_; 

	// ワールドトランスフォーム
	WorldTransform worldTransform_; 
};
