#pragma once
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"

#include <DebugText.h>
#include <algorithm>
#include <list>
#include <numbers>

#include "MyMath.h"
#include "PlayerBullet.h"

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

public: /* Getter */
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
	/// ワールドトランスフォームの取得
	/// </summary>
	/// <returns></returns>
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	/// <summary>
	/// スケールの取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetScale() { return worldTransform_.scale_; }

public: /* Setter */
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

public: /* 衝突判定 */
	/// <summary>
	/// 衝突判定
	/// </summary>
	void OnCollision();

public: /* 基本の関数 */
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="textureHandle"></param>
	void Initialize(Model* model, uint32_t textureHandle, Vector3 position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(ViewProjection& viewProjection);

private:
	/* ///////////////////////////////////////
	                 メンバ変数
	*/ ///////////////////////////////////////

	// GameScene
	GameScene* gameScene_ = nullptr;

	// モデル
	Model* model_ = nullptr;
	Model* bulletModel_ = nullptr;

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

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 旋回(回転)
	/// </summary>
	void Rotate();

	/// <summary>
	/// Reticle
	/// </summary>
	/// <param name="viewProjection"></param>
	void Reticle();

	/// <summary>
	/// 
	/// </summary>
	Vector3 GetForwardVector(const Vector3& rotation);
};
