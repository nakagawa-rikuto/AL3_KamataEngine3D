#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	/* /////////////////////////
	      テクスチャ読み込み
	*/ /////////////////////////
	textureHandle_ = TextureManager::Load("mario.png");

	/* /////////////////////////
	           Model生成
	*/ /////////////////////////
	entityModel_.reset(Model::Create());
	skyDomeModel_.reset(Model::CreateFromOBJ("hosizora", true));
	groundModel_.reset(Model::CreateFromOBJ("Ground", true));

	/* /////////////////////////
	       viewProjection
	*/ /////////////////////////
	viewProjection_.Initialize();

	/* /////////////////////////
	          Player
	*/ /////////////////////////
	// 自キャラの生成
	player_ = std::make_unique<Player>();

	// 自キャラの初期化
	player_->Initialeze(entityModel_.get(), textureHandle_, &viewProjection_);

	/* /////////////////////////
	         SkyDome
	*/ /////////////////////////
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Initialize(skyDomeModel_.get(), &viewProjection_);

	/* /////////////////////////
	          Ground
	*/ /////////////////////////
	ground_ = std::make_unique<Ground>();
	ground_->Initialize(groundModel_.get(), &viewProjection_);
}

void GameScene::Update() {

	// 　自キャラの更新
	player_->Update();

	// SkyDomeの更新
	skyDome_->Update();

	// Groundの更新
	ground_->Update();
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

	// SkyDomeの描画
	skyDome_->Draw();

	// Groundの描画
	ground_->Draw();

	// 自キャラの描画
	player_->Draw();

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
