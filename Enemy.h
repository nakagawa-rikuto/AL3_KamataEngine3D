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

#include "MyMath.h"
#include "imgui.h"

// 自機クラスの前方宣言
class Player;

// GameSceneの前方宣言
class GameScene;

/// <summary>
/// 敵
/// </summary>
class Enemy {

public:

	/* ///////////////////////////////
				フェーズ
	*/ ///////////////////////////////
	/// <summary>
	/// フェーズの初期化
	/// </summary>
	void PhaseInitialize();

	/// <summary>
	/// 接近フェーズ
	/// </summary>
	void ApproachPhase();

	/// <summary>
	/// 衝突フェーズ
	/// </summary>
	void CollisionPhase();


	/* ///////////////////////////////
	               当たり判定
	*/ ///////////////////////////////
	/// <summary>
	/// 衝突判定
	/// </summary>
	void OnCollision();

	/// <summary>
	/// 衝突判定2
	/// </summary>
	void OnCollision2();

	/// <summary>
	/// 死亡情報
	/// </summary>
	/// <returns></returns>
	bool IsDead() const { return isDead_; }

	/* ///////////////////////////////
	              Setter
	*/ ///////////////////////////////
	/// <summary>
	/// Playerをセット
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; }

	/// <summary>
	/// GameSceneをセット
	/// </summary>
	/// <param name="gameScene"></param>
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	/* ///////////////////////////////
	              Getter
	*/ ///////////////////////////////
	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

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
	void Draw(ViewProjection& viewProjection);

private:

	// 行動フェーズ
	enum class Phase {
		APPROACH, // 接近する
		Collsiion,    // 離脱する
	};


	/*
	   メンバ変数
	*/
	// Player
	Player* player_ = nullptr;

	// GameScene
	GameScene* gameScene_ = nullptr;

	// デスフラグ
	bool isDead_ = false;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// フェーズ
	Phase phase_;

	// 速度
	const float speed_ = 0.3f;

	float isTimer_;
};
