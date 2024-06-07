#include "GameScene.h"
#include "TextureManager.h"
#include <fstream>

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
	for (EnemyBullet* bullet : enemyBullets_) {
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
              EnemyBullet
*/ ///////////////////////////////////

/// <summary>
/// 敵弾の追加
/// </summary>
/// <param name="enemyBullet"></param>
void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {

	// リストに登録
	enemyBullets_.push_back(enemyBullet);
}

/// <summary>
/// 敵弾の更新
/// </summary>
void GameScene::EnemyBulletUpdate() {
	// ですフラグの立った弾を削除
	// remove_if()はあくまでリストから要素を消すだけ
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// 弾の更新　
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}
}

/// <summary>
/// 敵弾の描画
/// </summary>
/// <param name="viewProjection"></param>
void GameScene::EnemyBulletDraw(ViewProjection& viewProjection) {
	// 弾の描画
	for (EnemyBullet* bullet : enemyBullets_) {

		bullet->Draw(viewProjection);
	}
}

void GameScene::EnemyInitialize(Enemy* enemy, Vector3 position) {

	enemy->Initialize(model_, textureHandle_, position);
	enemy->SetPlayer(player_);
	enemy->SetGameScene(this);
}

/// <summary>
/// 敵発生データの読み込み
/// </summary>
void GameScene::LoadEnemyPopData() {

	// ファイルを開く
	std::ifstream file;
	file.open("./Resources/enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands_ << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

/// <summary>
/// Enemyが重なっていて描画している
/// それが悪いところ
/// </summary>

void GameScene::UpdateEnemyPopCommands() {

	// 待機処理
	if (isEnemyWait_) {

		enemyWaitTimer_--;
		if (enemyWaitTimer_ <= 0) {

			// 待機完了
			isEnemyWait_ = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands_, line)) {

		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;

		// ,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメントアウト
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPの実行
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			Enemy* enemy = new Enemy();
			EnemyInitialize(enemy, Vector3(x, y, z));
			enemy_.push_back(enemy);
			//// 敵を発生させる
			//EnemyInitialize(enemy, Vector3(x, y, z));

			// WAITコマンドの実行
		} else if (word.find("WAIT") == 0) {

			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			isEnemyWait_ = true;
			enemyWaitTimer_ = waitTime;

			// コマンドループを抜ける
			break;
		}
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

	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemyBullets_;

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
	enemyTextureHandle_ = TextureManager::Load("./Resources/uvChecker.png");

	/* //////////////////////////
	      3Dモデルの読み込み
	*/ /////////////////////////
	model_ = Model::Create();
	skyDomeModel_ = Model::CreateFromOBJ("SkyDome", true);

	/* //////////////////////////
	        viewProjection
	*/ /////////////////////////
	viewProjection_.Initialize();

	/* //////////////////////////
	         レールカメラ
	*/ /////////////////////////
	railCamera_ = new RailCamera();
	railCamera_->Initialize(railCameraPos_, railCameraRotation_);
	isRailCameraActive_ = true;

	/* //////////////////////////
	            Player
	*/ /////////////////////////
	player_ = new Player();
	Vector3 playerPosition(0.0f, 0.0f, 30.0f);
	player_->SetParent(&railCamera_->GetWorldTransform());
	player_->Initialize(model_, textureHandle_, playerPosition);
	player_->SetGameScene(this);

	/* //////////////////////////
	            Enemy
	*/ /////////////////////////

	enemy_.push_back(new Enemy());

	for (Enemy* enemy : enemy_) {

		enemy->Initialize(model_, textureHandle_, {0.0f, 0.0f, 0.0f});
		enemy->SetPlayer(player_);
		enemy->SetGameScene(this);
	}

	// ファイル読み込み
	LoadEnemyPopData();

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

	/* //////////////////////////
	          SkyDome
	*/ /////////////////////////
	skyDome_->Update();

	/* //////////////////////////
	      Player・PlayerBullet
	*/ /////////////////////////
	player_->Update();

	PlayerBulletUpdate();

	/* //////////////////////////
	      Enemy・EnemyBullet
	*/ /////////////////////////

	for (Enemy* enemy : enemy_) {
		enemy->Update();
	}

	EnemyBulletUpdate();

	// スクリプト実行
	UpdateEnemyPopCommands();

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
	EnemyBulletDraw(viewProjection_);

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
