#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "AxisIndicator.h"

#include <cassert>

#include "Player.h"
#include "Enemy.h"
#include "SkyDome.h"
#include "RailCamera.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// クラス
	/// </summary>

	// Player
	Player* player_ = nullptr;

	// Enemy
	Enemy* enemy_ = nullptr;

	// SkyDome
	SkyDome* skyDome_ = nullptr;

	// レールカメラ
	RailCamera* railCamera_ = nullptr;
	Vector3 railCameraPos_ = {0.0f, 0.0f, -50.0f};
	Vector3 railCameraRotation_ = {0.0f, 0.0f, 0.0f};

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	// デバッグカメラ切り替え変数
	bool isDebugCameraActive_ = false;

	// レールカメラ切り替え変数
	bool isRailCameraActive_ = false;

	// 3Dモデル
	Model* model_ = nullptr;
	Model* skyDomeModel_ = nullptr;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	// テクスチャ
	uint32_t textureHandle_ = 0; 
	uint32_t enemyTextureHandle_ = 0;
	
};
