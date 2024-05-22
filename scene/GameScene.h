#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "Player.h"

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

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// クラス
	/// </summary>

	// Player
	Player* player_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	// 3Dモデル
	Model* model_ = nullptr;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// テクスチャ
	uint32_t textureHandle_ = 0; 
	
};
