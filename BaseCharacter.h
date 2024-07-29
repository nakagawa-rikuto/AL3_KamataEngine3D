#pragma once
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"

#include <numbers>
#include <vector>

#include "Collider.h"
#include "MyMath.h"
#include "imgui.h"

/// <summary>
/// キャラクターの基礎
/// </summary>
class BaseCharacter : public Collider{

protected:
	// モデルデータ配列
	std::vector<Model*> models_;

	// ワールド変換データ
	WorldTransform worldTransform_;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models"></param>
	virtual void Initialize(const std::vector<Model*>& models);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	virtual void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 衝突時に呼ばれる関数
	/// </summary>
	virtual void OnCollision(Collider* other) override;

	/// <summary>
	/// ワールド変換データを取得
	/// </summary>
	/// <returns></returns>
	virtual const WorldTransform& GetWorldTransform() const;

	/// <summary>
	/// 中心座標を取得
	/// </summary>
	/// <returns></returns>
	virtual Vector3 GetCenterPosition() const override;
};
