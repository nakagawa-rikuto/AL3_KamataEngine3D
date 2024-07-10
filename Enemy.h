#pragma once
#include "BaseCharacter.h"

class Enemy : public BaseCharacter {
public:
	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// 中心座標の取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetCenterPosition();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models"></param>
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

	enum ModelNum {
		kModelIndexBody,
		kModelIndexWeapon
	};

	WorldTransform worldTransformBody_;
	WorldTransform worldTransformWeapon_;
};
