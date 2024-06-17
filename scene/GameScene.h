#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include <memory>

#include "Player.h"
#include "SkyDome.h"
#include "Ground.h"

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
	/// ゲームシーン用
	/// </summary>

	/* //////////////////////////////////////
	                 Entity
	*/ //////////////////////////////////////
	// Player
	std::unique_ptr<Player> player_;

	/* //////////////////////////////////////
	                 Field
	*/ //////////////////////////////////////
	// SkyDome
	std::unique_ptr<SkyDome> skyDome_;

	// Ground
	std::unique_ptr<Ground> ground_;
	/* //////////////////////////////////////
	                  モデル
	*/ //////////////////////////////////////
	// 3Dモデルデータ
	std::unique_ptr<Model> entityModel_;

	// SkyDome
	std::unique_ptr<Model> skyDomeModel_;

	// Ground
	std::unique_ptr<Model> groundModel_;

	/* //////////////////////////////////////
	             テクスチャハンドル
	*/ //////////////////////////////////////
	uint32_t textureHandle_ = 0;

	/* //////////////////////////////////////
	           ビュープロジェクション
	*/ //////////////////////////////////////
	ViewProjection viewProjection_;
};
