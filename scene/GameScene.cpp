#include "GameScene.h"
#include "TextureManager.h"
#include "GlobalVariables.h"

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

	ImGui::Checkbox("DebugCamera", &isDebugCameraActive_);

	if (isDebugCameraActive_) {

		// デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {

		// ビュープロジェクション行列の更新と転送
		//viewProjection_.UpdateMatrix();
	}
}

void GameScene::FollowCameraUpdate() {

	// 追従カメラの更新
	followCamera_->Update();

	// 追従カメラのビュープロジェクションをコピー
	viewProjection_.matView = followCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;

	// ViewProjectionの転送
	viewProjection_.TransferMatrix();
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
	textureHandleLockOnMark_ = TextureManager::Load("Reticle.png");

	/* /////////////////////////
	           Model生成
	*/ /////////////////////////
	// Entity
	entityModel_.reset(Model::Create());

	// Player
	playerModel_.body_.reset(Model::CreateFromOBJ("PlayerBody", true));
	playerModel_.face_.reset(Model::CreateFromOBJ("PlayerFeace", true));
	playerModel_.core_.reset(Model::CreateFromOBJ("PlayerCore", true));
	playerModel_.leftArm_.reset(Model::CreateFromOBJ("PlayerLeftArm", true));
	playerModel_.rightArm_.reset(Model::CreateFromOBJ("PlayerRightArm", true));
	playerModel_.weapon_.reset(Model::CreateFromOBJ("PlayerWeapon", true));

	// Enemy
	enemyModel_.body_.reset(Model::CreateFromOBJ("EnemyBody", true));
	enemyModel_.weapon_.reset(Model::CreateFromOBJ("EnemyWeapon", true));

	// SkyDome
	skyDomeModel_.reset(Model::CreateFromOBJ("hosizora", true));

	// Ground
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
	// 自キャラのモデル
	std::vector<Model*> playerModels = {
		playerModel_.body_.get(),    playerModel_.face_.get(),     playerModel_.core_.get(),
		playerModel_.leftArm_.get(), playerModel_.rightArm_.get(), playerModel_.weapon_.get()};
	// 自キャラの初期化
	player_->Initialize(playerModels);

	/* /////////////////////////
	          Enemy
	*/ /////////////////////////
	// 敵キャラの生成
	enemies_.push_back(std::make_unique<Enemy>());
	// 敵キャラのモデル
	std::vector<Model*> enemyModels = {enemyModel_.body_.get(), enemyModel_.weapon_.get()};
	// 敵キャラの初期化
	// 途中
	for (const auto& enemy : enemies_) {

		enemy->Initialize(enemyModels);
	}

	/* /////////////////////////
	          LockOn
	*/ /////////////////////////
	// ロックオンの生成
	lockOnMark_.reset(Sprite::Create(textureHandleLockOnMark_, Vector2(0.0f, 0.0f)));
	lockOn_ = std::make_unique<LockOn>();
	lockOn_->Initialize(lockOnMark_.get(), textureHandleLockOnMark_);

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
				追従カメラ
	*/ /////////////////////////
	// 追従カメラの生成
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetTarget(&player_->GetWorldTransform());

	// 追従カメラのビュープロジェクションをセット
	player_->SetViewProjection(&followCamera_->GetViewProjection());

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

	for (const auto& enemy : enemies_) {
		// 敵キャラの更新
		enemy->Update();
	}

	// ロックオン更新
	lockOn_->Update(enemies_, viewProjection_);

	// SkyDomeの更新
	skyDome_->Update();

	// Groundの更新
	ground_->Update();

	// FollowCameraの更新
	FollowCameraUpdate();

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
	player_->Draw(viewProjection_);

	for (const auto& enemy : enemies_) {
		// 敵キャラの描画
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

	// ロックオンの描画
	lockOn_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
