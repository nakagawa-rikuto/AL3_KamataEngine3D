#pragma once
#include "BaseCharacter.h"

class Enemy : public BaseCharacter {
public:

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
