#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include <vector>
#include "skydome.h"
#include "Player.h"
#include "CameraController.h"
#include "MapChipField.h"

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
	/// 表示ブロックの生成
	/// </summary>
	void GenerateBlocks();

private: 
	// メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	 
	// クラス
	skydome* skyDome_ = nullptr; // SkyDome
	Player* player_ = nullptr; // Player
	MapChipField* mapChipField_ = nullptr; // マップチップフィールド
	CameraController* cameraController_ = nullptr; // CameraController

	// Playerの座標
	Vector3 playerPosition_;

	// カメラの移動範囲
	Vector4 cameraLimitMove_ = {0.0f, 100.0f, 0.0f, 100.0f};

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//ViewProjection
	ViewProjection viewProjection_;

	// 3Dモデル
	Model* modelSkyDome_ = nullptr;
	Model* modelPlayer_ = nullptr;

	// 3Dモデルデータ
	Model* model_ = nullptr;

	/// *************************************
	/// デバッグカメラの切り替え
	/// *************************************

	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	/// *************************************
	/// デバッグカメラの生清と解放
	/// *************************************

	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	/// *************************************
	/// 二次元Std::vector
	/// *************************************
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;
};
