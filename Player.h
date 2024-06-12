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

// GameSceneの前方宣言
class GameScene;

/// <summary>
/// 自キャラ
/// </summary>
class Player {
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 旋回(回転)
	/// </summary>
	void Rotate();

	/// <summary>
	/// 3Dレティクルの配置
	/// </summary>
	void Update3DReticle(ViewProjection& viewProjection);

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();

	/// <summary>
	/// 3Dレティクルのワールド座標を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorld3DReticlePosition();

	/// <summary>
	/// 親となるワールどトランスフォームをセット
	/// </summary>
	/// <param name="parent"></param>
	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

	/// <summary>
	/// GameSceneをセット
	/// </summary>
	/// <param name="gameScene"></param>
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	/// <summary>
	/// 衝突判定
	/// </summary>
	void OnCollision();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="textureHandle"></param>
	void Initialize(Model* model, uint32_t textureHandle, Vector3 position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(ViewProjection& viewProjection);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI();

private:

	/* ///////////////////////////////////////
	                 メンバ変数
	*/ ///////////////////////////////////////

	// GameScene
	GameScene* gameScene_ = nullptr;

	// モデル
	Model* model_ = nullptr;

	// 2Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;

	// ワールド変換データ
	WorldTransform worldTransform_;

	// 3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// レティクル用テクスチャ取得
	uint32_t textureReticle_ = 0u;

	/* ///////////////////////////////////////
	                 メンバ関数
	*/ ///////////////////////////////////////

	/// <summary>
	/// 攻撃処理
	/// </summary>
	void Attack();
};
