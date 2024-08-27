#pragma once
#include "BaseCharacter.h"

class Enemy : public BaseCharacter {
public:
	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

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
	enum ModelNum { kModelIndexBody, kModelIndexWeapon };

	// ワールドたランスフォーム
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformWeapon_;

private:

	// 浮遊ギミックの媒体変数
	float floatingParameter_ = 0.0f;

	// 腕ギミックの媒体変数
	float armParameter_ = 0.0f;

	/// <summary>
	/// アニメーション
	/// </summary>
	void Animation();
};
