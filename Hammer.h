#pragma once
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"

#include <numbers>
#include <optional>

#include "Collider.h"
#include "MyMath.h"
#include "imgui.h"

class CollisionEffect;

/// <summary>
/// 武器
/// </summary>
class Hammer final : public Collider {
public:

	/// <summary>
	/// 回転の取得
	/// </summary>
	/// <returns></returns>
	const Vector3& GetRotate() const;

	/// <summary>
	/// 回転のセット
	/// </summary>
	/// <param name="rotation"></param>
	void SetRotate(const Vector3& rotation); 

	/// <summary>
	/// 中心座標の取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetCenterPosition() const override;

	/// <summary>
	/// エフェクトのSetter
	/// </summary>
	void SetEffect(CollisionEffect* effect);

	/// <summary>
	/// 衝突判定
	/// </summary>
	void OnCollision(Collider* other) override;

	/// <summary>
	/// WorldTransformの初期化
	/// </summary>
	void WorldTransformInitialize();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="worldTransform"></param>
	void Initialize(Model* model, const WorldTransform *worldTransform);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection &viewProjection);

private:

	// エフェクト
	CollisionEffect* effect_ = nullptr;

	Model* model_ = nullptr;        // モデル
	WorldTransform worldTransform_; // WorldTransform
};
