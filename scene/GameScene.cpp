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

	/* /////////////////////////
	           Model生成
	*/ /////////////////////////
	// Entity
	entityModel_.reset(Model::Create());

	// Player
	playerBodyModel_.reset(Model::CreateFromOBJ("PlayerBody", true));
	playerFaceModel_.reset(Model::CreateFromOBJ("PlayerFeace", true));
	playerCoreModel_.reset(Model::CreateFromOBJ("PlayerCore", true));
	playerLeftArmModel_.reset(Model::CreateFromOBJ("PlayerLeftArm", true));
	playerRightArmModel_.reset(Model::CreateFromOBJ("PlayerRightArm", true));

	// Enemy
	enemyBodyModel_.reset(Model::CreateFromOBJ("EnemyBody", true));
	enemyWeaponModel_.reset(Model::CreateFromOBJ("EnemyWeapon", true));

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
		playerBodyModel_.get(), playerFaceModel_.get(), playerCoreModel_.get(), playerLeftArmModel_.get(), playerRightArmModel_.get()};
	// 自キャラの初期化
	player_->Initialize(playerModels);

	/* /////////////////////////
	          Enemy
	*/ /////////////////////////
	// 敵キャラの生成
	enemy_ = std::make_unique<Enemy>();
	// 敵キャラのモデル
	std::vector<Model*> enemyModels = {enemyBodyModel_.get(), enemyWeaponModel_.get()};
	// 敵キャラの初期化
	enemy_->Initialize(enemyModels);

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

	// 敵キャラの更新
	enemy_->Update();

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

	// 敵キャラの描画
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
