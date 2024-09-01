#define NOMINMAX
#include "GameScene.h"
#include "TextureManager.h"

#include <algorithm>
#include <chrono> // std::chrono::steady_clock, std::chrono::duration_cast, std::chrono::milliseconds
#include <fstream>
#include <random> // std::random_device, std::mt19937, std::uniform_real_distribution

GameScene::GameScene() {}

/// <summary>
/// デストラクタ
/// </summary>
GameScene::~GameScene() {
	delete model_;
	delete skyDomeModel_;
	delete skyDome_;
	delete player_;
	delete railCamera_;
	delete debugCamera_;
	for (Enemy* enemy : enemy_) {
		delete enemy;
	}
	for (PlayerBullet* bullet : playerBullets_) {
		delete bullet;
	}
}

/* ///////////////////////////////////
              PlayerBullet
*/ ///////////////////////////////////
/// <summary>
/// 自弾の追加
/// </summary>
/// <param name="playerBullet"></param>
void GameScene::AddPlayerBullet(PlayerBullet* playerBullet) {

	// リストに登録
	playerBullets_.push_back(playerBullet);
}

/// <summary>
/// 自弾の更新
/// </summary>
void GameScene::PlayerBulletUpdate() {

	// ですフラグの立った弾を削除
	// remove_if()はあくまでリストから要素を消すだけ
	playerBullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// 弾の更新　
	for (PlayerBullet* bullet : playerBullets_) {
		bullet->Update();
	}
}

/// <summary>
/// 自弾の描画
/// </summary>
/// <param name="viewProjection"></param>
void GameScene::PlayerBulletDraw(ViewProjection& viewProjection) {

	// 弾の描画
	for (PlayerBullet* bullet : playerBullets_) {

		bullet->Draw(viewProjection);
	}
}

/* ///////////////////////////////////
                 Enemy
*/ ///////////////////////////////////
void GameScene::EnemyInitialize(Enemy* enemy, Vector3 position) {

	enemy->Initialize(model_, enemyTextureHandle_, position);
	enemy->SetPlayer(player_);
	enemy->SetGameScene(this);
}

void GameScene::EnemyPop() {

	if (enemy_.size() >= kMaxEnemy_) {
		// すでに20体の敵が存在する場合、新しい敵を生成しない
		return;
	}

	// ランダムデバイスの初期化
	std::random_device rd;
	std::mt19937 gen(rd());

	// 2つの範囲を設定 (-∞, -200) と (200, ∞)
	std::uniform_real_distribution<float> dis1(-300.0f, -200.0f);
	std::uniform_real_distribution<float> dis2(200.0f, 300.0f);

	// どちらの範囲から生成するかをランダムに選択
	auto random_value = [&](std::uniform_real_distribution<float>& dis1, std::uniform_real_distribution<float>& dis2) {
		return (std::uniform_int_distribution<int>(0, 1)(gen) == 0) ? dis1(gen) : dis2(gen);
	};

	// 一度に出現させる敵の数
	int32_t numberOfEnemies = 5; // ここで数を調整可能

	// 生成可能な敵の数を計算
	int32_t availableSlots = kMaxEnemy_ - static_cast<int32_t>(enemy_.size());
	int32_t spawnCount = std::min(numberOfEnemies, availableSlots);

	for (int i = 0; i < spawnCount; ++i) {
		float x = random_value(dis1, dis2);
		float y = random_value(dis1, dis2);
		float z = random_value(dis1, dis2);

		Vector3 newPosition = {x, y, z};
		Vector3 playerPosition = player_->GetWorldPosition();
		Vector3 distanceVec = newPosition - playerPosition;

		// 新しい Enemy オブジェクトを生成し、初期化
		Enemy* enemy = new Enemy();
		EnemyInitialize(enemy, Vector3(x, y, z));
		enemy_.push_back(enemy);
	}
}

/// <summary>
/// Enemyの更新
/// </summary>
void GameScene::EnemyUpdate() {
	// 死んでいる敵をリストから削除
	enemy_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

	// Enemyの更新
	for (Enemy* enemy : enemy_) {
		enemy->Update();
	}

	// 敵を出現させる間隔 (ミリ秒単位)
	enemyWaitTimer_--;

	if (enemyWaitTimer_ < 0.0f) {
		EnemyPop();
		enemyWaitTimer_ = 60;
	}
}

/* ///////////////////////////////////
           フェーズの切り替え
*/ ///////////////////////////////////
void GameScene::Clear() {
	Vector3 clearScale = player_->GetScale();
	if (clearScale.x < 0.05f || clearScale.y < 0.05f || clearScale.z < 0.05f) {
		phase_ = Phase::kClear;
	}
}

void GameScene::Lose() {
	Vector3 loseScale = player_->GetScale();
	if (loseScale.x > 10.0f || loseScale.y > 10.0f || loseScale.z > 10.0f) {
		phase_ = Phase::kLose;
	}
}

void GameScene::ChangPhase() {

	switch (phase_) {
	case GameScene::Phase::kPlay:

		Clear();
		Lose();

		break;
	}
}

/* ///////////////////////////////////
             GameScene
*/ ///////////////////////////////////

/// <summary>
/// 当たり判定
/// </summary>
void GameScene::CheckAllCollisions() {

	// 判定対象AとBの座標
	Vector3 posA, posB;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = playerBullets_;

#pragma region 自弾と敵キャラの当たり判定
	for (Enemy* enemy : enemy_) {

		// 敵キャラの座標
		posA = enemy->GetWorldPosition();

		// 敵キャラと敵弾すべての当たり判定
		for (PlayerBullet* bullet : playerBullets) {

			// 自弾の座標
			posB = bullet->GetWorldPosition();

			// 距離を求める
			Vector3 distancePos = {posB.x - posA.x, posB.y - posA.y, posB.z - posA.z};

			// 球と球の交差判定
			if (distancePos.x * distancePos.x + distancePos.y * distancePos.y + distancePos.z * distancePos.z <= 1.0f) {

				// 自キャラの衝突時個コールバックを呼び出す
				enemy->OnCollision();

				// 敵弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
			}
		}
	}

#pragma endregion
#pragma region 敵と自分の当たり判定

	for (Enemy* enemy : enemy_) {

		// 敵キャラの座標
		posA = enemy->GetWorldPosition();

		// 敵キャラとの当たり判定

		// 自分の座標
		posB = player_->GetWorldPosition();

		// 距離を求める
		Vector3 distancePos = {posB.x - posA.x, posB.y - posA.y, posB.z - posA.z};

		//// 球と球の交差判定
		if (distancePos.x * distancePos.x + distancePos.y * distancePos.y + distancePos.z * distancePos.z <= 1.0f) {

			// 自キャラの衝突時個コールバックを呼び出す
			enemy->OnCollision2();

			// 敵弾の衝突時コールバックを呼び出す
			player_->OnCollision();
		}
	}

#pragma endregion
}

/// <summary>
/// 初期化
/// </summary>
void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	/* //////////////////////////
	      テクスチャの読み込み
	*/ /////////////////////////
	// テクスチャの読み込み
	textureHandle_ = TextureManager::Load("./Resources/mario.png");
	enemyTextureHandle_ = TextureManager::Load("./Resources/cube/cube.jpg");

	// レティクルのテクスチャ
	TextureManager::Load("./Resources/Reticle.png");

	/* //////////////////////////
	      3Dモデルの読み込み
	*/ /////////////////////////
	model_ = Model::CreateFromOBJ("Player", true);
	skyDomeModel_ = Model::CreateFromOBJ("SkyDome", true);

	/* //////////////////////////
	        viewProjection
	*/ /////////////////////////
	viewProjection_.Initialize();

	/* //////////////////////////
	            Player
	*/ /////////////////////////
	player_ = new Player();
	Vector3 playerPosition(0.0f, 10.0f, 0.0f);
	player_->Initialize(model_, textureHandle_, playerPosition);
	player_->SetGameScene(this);

	/* //////////////////////////
	         レールカメラ
	*/ /////////////////////////
	railCamera_ = new RailCamera();
	railCamera_->SetParent(&player_->GetWorldTransform());
	railCamera_->Initialize(railCameraPos_, railCameraRotation_);
	isRailCameraActive_ = true;

	/* //////////////////////////
	            Enemy
	*/ /////////////////////////

	enemy_.push_back(new Enemy());

	for (Enemy* enemy : enemy_) {

		enemy->Initialize(model_, enemyTextureHandle_, {20.0f, 20.0f, 100.0f});
		enemy->SetPlayer(player_);
		enemy->SetGameScene(this);
	}

	/* //////////////////////////
	           SkyDome
	*/ /////////////////////////
	skyDome_ = new SkyDome();
	skyDome_->Initialize(skyDomeModel_, &viewProjection_);

	/* //////////////////////////
	      デバッグカメラの生成
	*/ /////////////////////////
	debugCamera_ = new DebugCamera(1280, 720);

	/* //////////////////////////
	            フェード
	*/ //////////////////////////

	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);

	// ゲームプレイフェーズから開始
	phase_ = Phase::kFeadIn;
}

/// <summary>
/// 更新
/// </summary>
void GameScene::Update() {

	// フェーズの変更
	ChangPhase();

	switch (phase_) {
	case GameScene::Phase::kFeadIn:
#pragma region KFeadIn
		fade_->Update();
		if (fade_->IsFinished()) {
			phase_ = Phase::kPlay;
		}

		/* //////////////////////////
		      SkyDome
		*/ /////////////////////////
		skyDome_->Update();

		/* //////////////////////////
		      Player・PlayerBullet
		*/ /////////////////////////
		player_->Update(viewProjection_);

		PlayerBulletUpdate();

		/* //////////////////////////
		      Enemy・EnemyBullet
		*/ /////////////////////////
		EnemyUpdate();

		/* //////////////////////////
		     DebugCamera
	*/ /////////////////////////
#ifdef _DEBUG

		if (input_->TriggerKey(DIK_C)) {

			isDebugCameraActive_ = true;
		}
#endif // DEBUG

		if (isDebugCameraActive_) {

			// デバッグカメラの更新
			debugCamera_->Update();
			viewProjection_.matView = debugCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

			// ビュープロジェクション行列の転送
			viewProjection_.TransferMatrix();
		} else {

			// ビュープロジェクション行列の更新と転送
			viewProjection_.UpdateMatrix();
		}

		/* //////////////////////////
		         RailCamera
		*/ /////////////////////////
		if (isRailCameraActive_) {

			// レールカメラの更新
			railCamera_->Update();
			viewProjection_.matView = railCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;

			// ビュープロジェクション行列の転送
			viewProjection_.TransferMatrix();
		} else {

			// ビュープロジェクション行列の更新と転送
			viewProjection_.UpdateMatrix();
		}
#pragma endregion
		break;
	case GameScene::Phase::kPlay:
#pragma region KPlay
		/* //////////////////////////
		      SkyDome
		*/ /////////////////////////
		skyDome_->Update();

		/* //////////////////////////
		      Player・PlayerBullet
		*/ /////////////////////////
		player_->Update(viewProjection_);

		PlayerBulletUpdate();

		/* //////////////////////////
		      Enemy・EnemyBullet
		*/ /////////////////////////

		EnemyUpdate();

		/* //////////////////////////
		         Collision
		*/ /////////////////////////
		CheckAllCollisions();

		/* //////////////////////////
		         DebugCamera
		*/ /////////////////////////
#ifdef _DEBUG

		if (input_->TriggerKey(DIK_C)) {

			isDebugCameraActive_ = true;
		}
#endif // DEBUG

		if (isDebugCameraActive_) {

			// デバッグカメラの更新
			debugCamera_->Update();
			viewProjection_.matView = debugCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

			// ビュープロジェクション行列の転送
			viewProjection_.TransferMatrix();
		} else {

			// ビュープロジェクション行列の更新と転送
			viewProjection_.UpdateMatrix();
		}

		/* //////////////////////////
		         RailCamera
		*/ /////////////////////////
		if (isRailCameraActive_) {

			// レールカメラの更新
			railCamera_->Update();
			viewProjection_.matView = railCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;

			// ビュープロジェクション行列の転送
			viewProjection_.TransferMatrix();
		} else {

			// ビュープロジェクション行列の更新と転送
			viewProjection_.UpdateMatrix();
		}
#pragma endregion
		break;
	case GameScene::Phase::kLose:
#pragma region KLose

		/* //////////////////////////
		      SkyDome
		*/ /////////////////////////
		skyDome_->Update();

		/* //////////////////////////
		         DebugCamera
		*/ /////////////////////////
#ifdef _DEBUG

		if (input_->TriggerKey(DIK_C)) {

			isDebugCameraActive_ = true;
		}
#endif // DEBUG

		if (isDebugCameraActive_) {

			// デバッグカメラの更新
			debugCamera_->Update();
			viewProjection_.matView = debugCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

			// ビュープロジェクション行列の転送
			viewProjection_.TransferMatrix();
		} else {

			// ビュープロジェクション行列の更新と転送
			viewProjection_.UpdateMatrix();
		}

		/* //////////////////////////
		         RailCamera
		*/ /////////////////////////
		if (isRailCameraActive_) {

			// レールカメラの更新
			railCamera_->Update();
			viewProjection_.matView = railCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;

			// ビュープロジェクション行列の転送
			viewProjection_.TransferMatrix();
		} else {

			// ビュープロジェクション行列の更新と転送
			viewProjection_.UpdateMatrix();
		}
#pragma endregion
		fade_->Update();
		if (fade_->IsFinished()) {

			isFinished_ = true;
		}
		break;
	case GameScene::Phase::kClear:
#pragma region KClear
		/* //////////////////////////
		      SkyDome
		*/ /////////////////////////
		skyDome_->Update();

		/* //////////////////////////
		         DebugCamera
		*/ /////////////////////////
#ifdef _DEBUG

		if (input_->TriggerKey(DIK_C)) {

			isDebugCameraActive_ = true;
		}
#endif // DEBUG

		if (isDebugCameraActive_) {

			// デバッグカメラの更新
			debugCamera_->Update();
			viewProjection_.matView = debugCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

			// ビュープロジェクション行列の転送
			viewProjection_.TransferMatrix();
		} else {

			// ビュープロジェクション行列の更新と転送
			viewProjection_.UpdateMatrix();
		}

		/* //////////////////////////
		         RailCamera
		*/ /////////////////////////
		if (isRailCameraActive_) {

			// レールカメラの更新
			railCamera_->Update();
			viewProjection_.matView = railCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;

			// ビュープロジェクション行列の転送
			viewProjection_.TransferMatrix();
		} else {

			// ビュープロジェクション行列の更新と転送
			viewProjection_.UpdateMatrix();
		}
#pragma endregion
		fade_->Update();
		if (fade_->IsFinished()) {

			isFinished_ = true;
		}
		break;
	}
}

/// <summary>
/// 描画
/// </summary>
void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	/* //////////////////////////
	         SkyDome
	*/ /////////////////////////
	skyDome_->Draw();

	/* //////////////////////////
	      Player・PlayerBullet
	*/ /////////////////////////
	player_->Draw(viewProjection_);
	PlayerBulletDraw(viewProjection_);

	/* //////////////////////////
	      Enemy・EnemyBullet
	*/ /////////////////////////
	for (Enemy* enemy : enemy_) {
		enemy->Draw(viewProjection_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
