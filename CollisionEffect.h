#pragma once
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"

#include <numbers>
#include <optional>

#include "MyMath.h"
#include "imgui.h"

/// <summary>
/// 衝突時のエフェクト
/// </summary>
class CollisionEffect {
public: /* 基本の関数 */

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
	void Draw();

private:

	// モデル
	std::unique_ptr<Model> model_;

	// ワールドトランスフォーム
	WorldTransform worldTrasform_;
};
