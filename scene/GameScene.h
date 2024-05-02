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

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	skydome* skydome_ = nullptr;
	Player* player_ = nullptr;
	// マップチップフィールド
	MapChipField* mapChipField_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	uint32_t playerTextureHandle_ = 0;

	//VeiwProjection
	ViewProjection viewProjection_;

	// 3Dモデル
	Model* modelSkydome_ = nullptr;

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
	/// 二次元std::vector
	/// *************************************
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
