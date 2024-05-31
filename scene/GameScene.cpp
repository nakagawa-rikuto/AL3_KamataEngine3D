#include "GameScene.h"
#include "TextureManager.h"

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete model_; 
	delete player_;
	delete enemy_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	/* //////////////////////////
	      テクスチャの読み込み
	*/ /////////////////////////
	// テクスチャの読み込み
	textureHandle_ = TextureManager::Load("./Resources/mario.png");
	enemyTextureHandle_ = TextureManager::Load("./Resources/uvChecker.png");

	/* //////////////////////////
	      3Dモデルの読み込み
	*/ /////////////////////////
	model_ = Model::Create();

	/* //////////////////////////
	           初期化
	*/ /////////////////////////
	// Playerの生成
	viewProjection_.Initialize();

	player_ = new Player();
	player_->Initialize(model_, textureHandle_);

	enemy_ = new Enemy();
	enemy_->Initialize(model_, textureHandle_);
	enemy_->SetPlayer(player_);

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	/* //////////////////////////
	         軸方向表示の使用
	*/ /////////////////////////
	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {

	/* //////////////////////////
	            更新
	*/ /////////////////////////
	player_->Update();
	enemy_->Update();

#ifdef _DEBUG

	if (input_->TriggerKey(DIK_C)) {
		
		isDebugCameraActive_ = true;
	}
#endif // DEBUG

	/* /////////////
	デバッグカメラの処理
	*/ /////////////
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
	
	player_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);

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
