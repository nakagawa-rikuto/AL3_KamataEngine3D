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
#include <sstream>

#include "Fade.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "Enemy.h"
#include "EnemyBullet.h"
#include "SkyDome.h"
#include "RailCamera.h"
#include "MyMath.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数

	/* ///////////////////////////////////
	         Player・PlayerBullet
	*/ ///////////////////////////////////

	/// <summary>
	/// 自弾を追加する
	/// </summary>
	/// <param name="playerBullet"></param>
	void AddPlayerBullet(PlayerBullet* playerBullet);

	/// <summary>
	/// 自弾の更新
	/// </summary>
	void PlayerBulletUpdate();

	/// <summary>
	/// 自弾の描画
	/// </summary>
	void PlayerBulletDraw(ViewProjection& viewProjection);

	/* ///////////////////////////////////
	           Enemy・EnemyBullet
	*/ ///////////////////////////////////

	/// <summary>
	/// 敵発生処理
	/// </summary>
	void EnemyInitialize(Enemy* enemy, Vector3 position);

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	//void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	//void UpdateEnemyPopCommands();

	/// <summary>
	/// 敵の発生場所
	/// </summary>
	void EnemyPop();

	/// <summary>
	/// 敵の更新
	/// </summary>
	/// <param name="enemy"></param>
	void EnemyUpdate();

	/* ///////////////////////////////////
	              Scene移動
	*/ ///////////////////////////////////

	/// <summary>
	/// クリア
	/// </summary>
	void Clear();

	/// <summary>
	/// 負け
	/// </summary>
	void Lose();

	/// <summary>
	/// フェーズ切り替え
	/// </summary>
	void ChangPhase();

	/// <summary>
	/// 終了フラグのGetter
	/// </summary>
	/// <returns></returns>
	bool isFinished() { return isFinished_; }

	/* ///////////////////////////////////
	              GameScene
	*/ ///////////////////////////////////

	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
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

	/* ///////////////////////////////////
	         Player・PlayerBullet
	*/ ///////////////////////////////////
	Player* player_ = nullptr;

	std::list<PlayerBullet*> playerBullets_;

	/* ///////////////////////////////////
	           Enemy・EnemyBullet
	*/ ///////////////////////////////////
	std::list<Enemy*> enemy_;

	// 敵発生コマンド
	std::stringstream enemyPopCommands_;

	// 敵の待機フラグ
	bool isEnemyWait_ = false;

	// 敵の待機タイマー
	int32_t enemyWaitTimer_ = 60;

	const int32_t kMaxEnemy_ = 50;

	/* ///////////////////////////////////
	                シーン移動
	*/ ///////////////////////////////////

	enum class Phase {
		kFeadIn,
		kPlay,
		kLose,
		kClear,
		kFeadOut,
	};

	// ゲームの現在のフェーズ（変数）
	Phase phase_;
	Fade* fade_ = nullptr;

	bool isFinished_ = false;

	/* ///////////////////////////////////
	                SkyDome
	*/ ///////////////////////////////////
	SkyDome* skyDome_ = nullptr;

	/* ///////////////////////////////////
	               RailCamera
	*/ ///////////////////////////////////
	RailCamera* railCamera_ = nullptr;

	// Position
	Vector3 railCameraPos_ = {0.0f, 0.0f, -20.0f};

	// Rotation
	Vector3 railCameraRotation_ = {0.0f, 0.0f, 0.0f};

	// レールカメラ切り替え変数
	bool isRailCameraActive_ = false;

	/* ///////////////////////////////////
	              DebugCamera
	*/ ///////////////////////////////////
	DebugCamera* debugCamera_ = nullptr;

	// デバッグカメラ切り替え変数
	bool isDebugCameraActive_ = false;

	/* ///////////////////////////////////
	                Model
	*/ ///////////////////////////////////
	Model* model_ = nullptr;
	Model* skyDomeModel_ = nullptr;

	/* ///////////////////////////////////
	              ViewProjection
	*/ ///////////////////////////////////
	ViewProjection viewProjection_;

	/* ///////////////////////////////////
	              WorldTransform
	*/ ///////////////////////////////////
	WorldTransform worldTransform_;

	/* ///////////////////////////////////
	               TexTure
	*/ ///////////////////////////////////
	// Player
	uint32_t textureHandle_ = 0; 
	// Enemy
	uint32_t enemyTextureHandle_ = 0;
	
};
