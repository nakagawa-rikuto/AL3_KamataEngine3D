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
#include "TextureManager.h"

#include <memory>
#include <sstream>
#include <cassert>

#include "CollisionManager.h"
#include "Player.h"
#include "Enemy.h"
#include "SkyDome.h"
#include "CollisionEffect.h"
#include "Ground.h"
#include "FollowCamera.h"
#include "LockOn.h"
#include "MyMath.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数

	/// <summary>
	/// PlayerModel
	/// </summary>
	struct PlayerModel {
		std::unique_ptr<Model> body_;
		std::unique_ptr<Model> face_;
		std::unique_ptr<Model> core_;
		std::unique_ptr<Model> leftArm_;
		std::unique_ptr<Model> rightArm_;
	};

	/// <summary>
	/// EnemyModel
	/// </summary>
	struct EnemyModel {
		std::unique_ptr<Model> body_;
		std::unique_ptr<Model> weapon_;
	};

	/// <summary>
	/// PlayerWeapon
	/// </summary>
	struct PlayerWeapon {
		std::unique_ptr<Model> hammer_;
	};

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

	/* /////////////////////////////////////
	               衝突判定と応答
	*/ /////////////////////////////////////
	void CheckAllCollisions();

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
	              衝突マネージャ
	*/ //////////////////////////////////////
	// 衝突マネージャ
	std::unique_ptr<CollisionManager> collisionManeger_; 
	
	/* //////////////////////////////////////
	                 Entity
	*/ //////////////////////////////////////
	// Player
	std::unique_ptr<Player> player_;

	// Enemy
	std::list<std::unique_ptr<Enemy>> enemies_;

	/* //////////////////////////////////////
	                 Effect
	*/ //////////////////////////////////////
	// Effect
	 std::unique_ptr<CollisionEffect> effect_;

	/* //////////////////////////////////////
	                 Object
	*/ //////////////////////////////////////
	// LockOn
	std::unique_ptr<Sprite> lockOnMark_;
	std::unique_ptr<LockOn> lockOn_;

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
	PlayerModel playerModel_;

	// PlayerWeapon
	PlayerWeapon PlayerWeaponModel_;

	// Enemy
	EnemyModel enemyModel_;

	// SkyDome
	std::unique_ptr<Model> skyDomeModel_;

	// Ground
	std::unique_ptr<Model> groundModel_;

	/* //////////////////////////////////////
	             テクスチャハンドル
	*/ //////////////////////////////////////
	uint32_t textureHandle_ = 0;
	uint32_t textureHandleLockOnMark_ = 0u;

	/* //////////////////////////////////////
	           ビュープロジェクション
	*/ //////////////////////////////////////
	ViewProjection viewProjection_;
};
