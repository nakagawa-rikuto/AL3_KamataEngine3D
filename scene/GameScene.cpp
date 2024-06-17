#include "GameScene.h"
#include "TextureManager.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

/// <summary>
/// デバッグカメラの更新
/// </summary>
void GameScene::DebugCameraUpdate() {

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
}

/// <summary>
/// 初期化
/// </summary>
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
	playerModel_.reset(Model::CreateFromOBJ("Player", true));
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
	player_->Initialeze(playerModel_.get(), &viewProjection_);

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

	/* /////////////////////////
	          デバッグカメラ
	*/ /////////////////////////
	debugCamera_ = std::make_unique<DebugCamera>(1280, 720);

	/* //////////////////////////
	         軸方向表示の使用
	*/ /////////////////////////
	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

/// <summary>
/// 更新
/// </summary>
void GameScene::Update() {

	// 　自キャラの更新
	player_->Update();

	// SkyDomeの更新
	skyDome_->Update();

	// Groundの更新
	ground_->Update();

	// デバッグカメラの更新
	DebugCameraUpdate();
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
