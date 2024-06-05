#include "GameScene.h"
#include "TextureManager.h"

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete model_; 
	delete skyDomeModel_;
	delete skyDome_;
	delete player_;
	delete enemy_;
	delete railCamera_;
	delete debugCamera_;
}

void GameScene::CheckAllCollisions() {

	// 判定対象AとBの座標
	Vector3 posA, posB;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定

	// 自キャラの座標
	posA = player_->GetWorldPosition();

	// 自キャラと敵弾すべての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {

		// 敵弾の座標
		posB = bullet->GetWorldPosition();

		// 距離を求める
		Vector3 distancePos = {posB.x - posA.x, posB.y - posA.y, posB.z - posA.z};

		// 球と球の交差判定
		if (distancePos.x * distancePos.x + distancePos.y * distancePos.y + distancePos.z * distancePos.z <= 1.0f) {

			// 自キャラの衝突時個コールバックを呼び出す
			player_->OnCollision();

			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定

	// 敵キャラの座標
	posA = enemy_->GetWorldPosition();

	// 敵キャラと敵弾すべての当たり判定
	for (PlayerBullet* bullet : playerBullets) {

		// 自弾の座標
		posB = bullet->GetWorldPosition();

		// 距離を求める
		Vector3 distancePos = {posB.x - posA.x, posB.y - posA.y, posB.z - posA.z};

		// 球と球の交差判定
		if (distancePos.x * distancePos.x + distancePos.y * distancePos.y + distancePos.z * distancePos.z <= 1.0f) {

			// 自キャラの衝突時個コールバックを呼び出す
			enemy_->OnCollision();

			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵弾の当たり判定

	for (EnemyBullet* enemyBullet : enemyBullets) {

		// 敵キャラと敵弾すべての当たり判定
		for (PlayerBullet* playerBullet : playerBullets) {

			// 自弾の座標
			posA = playerBullet->GetWorldPosition();

			// 敵弾の座標
			posB = enemyBullet->GetWorldPosition();

			// 距離を求める
			Vector3 distancePos = {posB.x - posA.x, posB.y - posA.y, posB.z - posA.z};

			// 球と球の交差判定
			if (distancePos.x * distancePos.x + distancePos.y * distancePos.y + distancePos.z * distancePos.z <= 1.0f) {

				// 自キャラの衝突時個コールバックを呼び出す
				playerBullet->OnCollision();

				// 敵弾の衝突時コールバックを呼び出す
				enemyBullet->OnCollision();
			}
		}
	}
	

#pragma endregion
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
	skyDomeModel_ = Model::CreateFromOBJ("SkyDome", true);

	/* //////////////////////////
	           初期化
	*/ /////////////////////////
	viewProjection_.Initialize();

	// レールカメラ
	railCamera_ = new RailCamera();
	railCamera_->Initialize(railCameraPos_, railCameraRotation_);

	player_ = new Player();
	Vector3 playerPosition(0.0f, 0.0f, 30.0f);
	player_->SetParent(&railCamera_->GetWorldTransform());
	player_->Initialize(model_, textureHandle_, playerPosition);

	enemy_ = new Enemy();
	enemy_->Initialize(model_, textureHandle_);
	enemy_->SetPlayer(player_);

	skyDome_ = new SkyDome();
	skyDome_->Initialize(skyDomeModel_, &viewProjection_);

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
	skyDome_->Update();

	CheckAllCollisions();

#ifdef _DEBUG

	if (input_->TriggerKey(DIK_C)) {
		
		isDebugCameraActive_ = true;
	}
#endif // DEBUG

	if (input_->TriggerKey(DIK_R)) {

		isRailCameraActive_ = true;
	}

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

	/* ///////////////
	   レールカメラの処理
	*/ ///////////////
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
	skyDome_->Draw();

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
