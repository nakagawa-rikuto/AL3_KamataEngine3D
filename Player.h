#pragma once
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"

#include <DebugText.h>
#include <algorithm>
#include <numbers>
#include <list>

#include "PlayerBullet.h"
#include "MyMath.h"
#include "imgui.h"

/// <summary>
/// 自キャラ
/// </summary>
class Player {
public:

	/// <summary>
	/// 旋回(回転)
	/// </summary>
	void Rotate();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="textureHandle"></param>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

private:

	/* ///////////////////////////////////////
	                 メンバ変数
	*/ ///////////////////////////////////////
	// 弾
	std::list<PlayerBullet*> bullets_;

	// モデル
	Model* model_ = nullptr;

	// ワールド変換データ
	WorldTransform worldTransform_;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	/* ///////////////////////////////////////
	                 メンバ関数
	*/ ///////////////////////////////////////

	/// <summary>
	/// 攻撃処理
	/// </summary>
	void Attack();
};
