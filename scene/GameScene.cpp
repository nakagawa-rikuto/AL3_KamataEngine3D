#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

void GameScene::GenerateBlocks() {
	//  要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 要素数を変更する
	// 列数を設定(縦方向のブロック数)
	worldTransformBlocks_.resize(numBlockVirtical);
	// for (uint32_t i = 0; i < numBlockVirtical; ++i) {

	//	// 1列の要素数を設定(横方向のブロック数)
	//	worldTransformBlocks_[i].resize(numBlockHorizontal);
	//}

	// ブロックの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {

			// 1列の要素数を設定(横方向のブロック数)
			worldTransformBlocks_[i].resize(numBlockHorizontal);

			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::CheckAllCollisions() {
#pragma region 自キャラと敵キャラの当たり判定

	// 判定対象1と2の座標
	AABB aabb1, aabb2;

	// 自キャラの座標
	aabb1 = player_->GetAABB();

	// 自キャラと敵弾すべての当たり判定
	for (Enemy* enemy : enemies_) {

		// 敵弾の座標
		aabb2 = enemy->GetAABB();

		// AABB同士の交差判定
		if (IsCollision(aabb1, aabb2)) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision(enemy);

			// 敵弾の衝突時コールバックを呼び出す
			enemy->OnCollision(player_);
		}
	}

#pragma endregion

#pragma region 自キャラとアイテムの当たり判定
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
#pragma endregion
}

// フェーズの切り替え
void GameScene::ChangPhase() {

	switch (phase_) {
	case Phase::kPlay:

		if (player_->IsDead()) {

			// 死亡演出フェーズに切り替え
			phase_ = Phase::kDeath;

			// 自キャラの座標を取得
			const Vector3& deathParticlesPosition = player_->GetWorldPosition();

			// パーティクルの生成と初期化
			deathParticles_ = new DeathParticles;
			deathParticles_->Initialize(modelParticle_, &viewProjection_, deathParticlesPosition);
		}

		break;
	case Phase::kDeath:

		// パーティクルが終わったらフェーズアウトに移行を角
		if (deathParticles_ && deathParticles_->IsFinished()) {

			fade_->Start(Fade::Status::FadeOut, 1.0f);
			phase_ = Phase::kFadeOut;
		}

		break;
	}
}

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete modelSkyDome_;
	delete modelPlayer_;
	delete modelEnemy_;
	delete modelParticle_;
	delete player_;
	for (Enemy* enemies : enemies_) {

		delete enemies;
	}
	delete skyDome_;
	delete cameraController_;
	delete mapChipField_;
	delete deathParticles_;
	delete debugCamera_;

	/// *************************************
	/// 解放
	/// *************************************
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
}

// Getter
bool GameScene::IsFinished() { return finished_; }

void GameScene::Initialize() {

	/// *************************************
	/// 初期化
	/// *************************************

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	/// *************************************
	/// 読み込み・生成
	/// *************************************

	// テクスチャの読み込み
	textureHandle_ = TextureManager::Load("./Resources/cube/cube.jpg");

	// 3Dモデルの生成
	model_ = Model::Create();
	modelSkyDome_ = Model::CreateFromOBJ("skydome", true);
	modelPlayer_ = Model::CreateFromOBJ("Player", true);
	modelEnemy_ = Model::CreateFromOBJ("Enemy", true);
	modelParticle_ = Model::CreateFromOBJ("Particle", true);

	/// *************************************
	/// Initialize
	/// *************************************

	// viewProjectionの初期化
	viewProjection_.Initialize();

	// SkyDome
	skyDome_ = new skydome();
	skyDome_->Initialize(modelSkyDome_, &viewProjection_);

	// MapChipField
	mapChipField_ = new MapChipField();
	mapChipField_->LoadMapChipCsv("./Resources/ALMap.csv");

	// Player
	player_ = new Player();
	playerPosition_ = mapChipField_->GetMapChipPositionByIndex(2, 18);
	player_->Initialize(modelPlayer_, &viewProjection_, playerPosition_);
	player_->SetMapChipField(mapChipField_);

	// Enemy
	for (int32_t i = 0; i < 3; ++i) {

		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(10 + i * 2, 18);
		newEnemy->Initialize(modelEnemy_, &viewProjection_, enemyPosition);

		enemies_.push_back(newEnemy);
	}

	// CameraController
	cameraController_ = new CameraController();
	cameraController_->Initialize(&viewProjection_);
	cameraController_->SetTarget(player_);
	cameraController_->SetMovableArea(cameraLimitMove_);
	cameraController_->Reset();

	// 表示ブロックの生成
	GenerateBlocks();

	/// *************************************
	/// デバッグカメラの生清と解放
	/// *************************************
	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	/* //////////////////////////////////////
	                フェード
	*/ //////////////////////////////////////
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);

	/* //////////////////////////////////////
	                フェーズ
	*/ //////////////////////////////////////
	// ゲームプレイフェーズから開始
	phase_ = Phase::kFadeIn;
}

void GameScene::Update() {

	/// *************************************
	/// 更新
	/// *************************************

	// フェーズの変更
	ChangPhase();

	switch (phase_) {
	case Phase::kFadeIn:

		fade_->Update();
		if (fade_->IsFinished()) {

			phase_ = Phase::kPlay;
		}

		skyDome_->Update(); // 天球の更新
		player_->Update();  // 自キャラの更新
		for (Enemy* enemies : enemies_) { // 敵の更新
			enemies->Update();
		}
		cameraController_->Update(); // カメラコントローラの更新
		debugCamera_->Update();      // カメラの更新

		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;
				worldTransformBlock->UpdateMatrix();
			}
		}

		break;
	case Phase::kPlay:

		skyDome_->Update();               // 天球の更新
		player_->Update();                // 自キャラの更新
		for (Enemy* enemies : enemies_) { // 敵の更新
			enemies->Update();
		}
		cameraController_->Update(); // カメラコントローラの更新
		debugCamera_->Update();      // カメラの更新

		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;
				worldTransformBlock->UpdateMatrix();
			}
		}

		CheckAllCollisions(); // 全ての当たり判定を行う

		break;
	case Phase::kDeath:

		skyDome_->Update();               // 天球の更新
		for (Enemy* enemies : enemies_) { // 敵の更新
			enemies->Update();
		}
		if (deathParticles_) { // パーティクル

			deathParticles_->Update();
		}
		debugCamera_->Update(); // カメラの更新

		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;
				worldTransformBlock->UpdateMatrix();
			}
		}

		break;
	case Phase::kFadeOut:

		fade_->Update();
		if (fade_->IsFinished()) {

			finished_ = true;
		}

		break;
	}

#ifdef _DEBUG
    if (input_->TriggerKey(DIK_C)) {
	    isDebugCameraActive_ = true;
    }
#endif // DEBUG

	/// *************************************
	/// デバッグカメラの処理
	/// *************************************
	// カメラの処理
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {

		// ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}
}

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

	/// *************************************
	/// 描画
	/// *************************************
	skyDome_->Draw();
	player_->Draw();
	for (Enemy* enemies : enemies_) {
		enemies->Draw();
	}
	// ブロックの描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}

			model_->Draw(*worldTransformBlock, viewProjection_);
		}
	}

	if (deathParticles_) {

		deathParticles_->Draw();
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
	 
	fade_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
