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
	//for (uint32_t i = 0; i < numBlockVirtical; ++i) {

	//	// 1列の要素数を設定(横方向のブロック数)
	//	worldTransformBlocks_[i].resize(numBlockHorizontal);
	//}

	// ブロックの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {

			// 1列の要素数を設定(横方向のブロック数)
			worldTransformBlocks_[i].resize(numBlockHorizontal);

			if (mapChipField_->GetMapChipTypeByIndex(j,i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}


GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete modelSkyDome_;
	delete modelPlayer_;
	delete modelEnemy_;
	delete player_;
	delete enemy_;
	delete skyDome_;
	delete cameraController_;
	delete mapChipField_;
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
	enemy_ = new Enemy();
	enemyPosition_ = mapChipField_->GetMapChipPositionByIndex(10, 18);
	enemy_->Initialize(modelEnemy_, &viewProjection_, enemyPosition_);

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
}

void GameScene::Update() {

	/// *************************************
	/// 更新
	/// *************************************

	skyDome_->Update();
	player_->Update();
	enemy_->Update();
	cameraController_->Update();

	//  ブロックの更新
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			worldTransformBlock->UpdateMatrix();
		}
	}

	/// *************************************
	/// デバッグカメラの生清と解放
	/// *************************************
	// デバッグカメラの更新
	debugCamera_->Update();

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
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
	enemy_->Draw();

	// ブロックの描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
				
			model_->Draw(*worldTransformBlock, viewProjection_);
		}
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

