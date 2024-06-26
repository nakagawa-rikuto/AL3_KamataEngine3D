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

#include <memory>
#include <sstream>
#include <cassert>

#include "Player.h"
#include "Enemy.h"
#include "SkyDome.h"
#include "Ground.h"
#include "FollowCamera.h"
#include "MyMath.h"

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

	/* /////////////////////////////////////
	               デバッグカメラ
	*/ /////////////////////////////////////
	void DebugCameraUpdate();

	/* /////////////////////////////////////
	               FollowCamera
	*/ /////////////////////////////////////
	void FollowCameraUpdate();

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

	// Enemy
	std::unique_ptr<Enemy> enemy_;

	/* //////////////////////////////////////
	                 Field
	*/ //////////////////////////////////////
	// SkyDome
	std::unique_ptr<SkyDome> skyDome_;

	// Ground
	std::unique_ptr<Ground> ground_;

	/* //////////////////////////////////////
	                 カメラ
	*/ //////////////////////////////////////
	// デバッグガメラ
	std::unique_ptr<DebugCamera> debugCamera_;
	bool isDebugCameraActive_;

	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;

	/* //////////////////////////////////////
	                  モデル
	*/ //////////////////////////////////////
	// 3Dモデルデータ
	std::unique_ptr<Model> entityModel_;

	// Player
	std::unique_ptr<Model> playerBodyModel_;
	std::unique_ptr<Model> playerFaceModel_;
	std::unique_ptr<Model> playerCoreModel_;
	std::unique_ptr<Model> playerLeftArmModel_;
	std::unique_ptr<Model> playerRightArmModel_;

	// Enemy
	std::unique_ptr<Model> enemyBodyModel_;
	std::unique_ptr<Model> enemyWeaponModel_;

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
