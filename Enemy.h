#pragma once
#include "BaseCharacter.h"

class Enemy : public BaseCharacter {
public:

	// デフォルトコンストラクタ
	Enemy();

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// 中心座標の取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetCenterPosition() const override;

	/// <summary>
	/// numberの取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetNumber() const;

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

	// シリアルナンバー
	uint32_t number_ = 0;

	// 次のシリアルナンバー
	static uint32_t nextNumber_;

	WorldTransform worldTransformBody_;
	WorldTransform worldTransformWeapon_;
};
