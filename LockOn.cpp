#include "LockOn.h"

Vector3 LockOn::GetWorldPosition() { 

	// ワールド座標を格納するための変数
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

// 初期化
void LockOn::Initialize() {

	// テクスチャの取得
	textureHandle_ = TextureManager::Load("Reticle.png");

	// WorldTransformの初期化
	worldTransform_.Initialize();

	// スプライトの生成
	lockOnMark_ = std::make_unique<Sprite>();
	lockOnMark_->Create(
		textureHandle_, 
		Vector2(worldTransform_.translation_.x, worldTransform_.translation_.y), 
		Vector4(1.0f, 1.0f, 1.0f, 1.0f), 
		Vector2(0.5f, 0.5f));
}

// 更新
void LockOn::Update(
	const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection) {

	// ロックオン状態なら
	if (isLockOn_) {

		/* C.ロックオン解放処理 */
		//if (Input::GetInstance()->TriggerKey(DIK_Q)) {

		//	// ロックオンを外す
		//	target_ = nullptr;
		//}
		//// 範囲外判定
		//else if () {
		//
		//	// ロックオンを外す
		//	target_ = nullptr;
		//}
	} else {

		/* A.ロックオン対象の検索 */
		// ロックオンボタンをトリガーしたら
		if (Input::GetInstance()->TriggerKey(DIK_Q)) {

			// 
			isLockOn_ = true;

			// ロックオン対象の検索
			Search(enemies, viewProjection);
		}
	}

	// ロックオン状態なら
	if (isLockOn_) {

		/* B.ロックオンマークの座標計算 */
		// ロックオン継続
		if (target_) {

			// 敵のロックオン座標の取得
			Vector3 positionWorld = target_->GetCenterPosition();

			// ワールド座標からスクリーン座標に変換
			Vector3 positionScreen = TransformScreen(positionWorld);

			// Vector2に格納
			Vector2 positionScreenV2 = Vector2(positionScreen.x, positionScreen.y);

			// スプライトの座標を設定
			lockOnMark_->SetPosition(positionScreenV2);
		}
	}
}

// 描画
void LockOn::Draw() {

	if (isLockOn_) {

		lockOnMark_->Draw();
	}
}

// 検索
void LockOn::Search(
	const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection) {

	/* ////////////////////////
		ロックオン対象の絞り込み
	*/ ////////////////////////
	// 目標
	std::list<std::pair<float, const Enemy*>> targets;

	// すべての敵に対して順にロックオン判定
	for (const std::unique_ptr<Enemy>& enemy : enemies) {

		// 敵のロックオン座標の取得
		Vector3 positionWorld = enemy->GetCenterPosition();

		// ワールド->ビュー座標に変換
		Vector3 positionView = Transform(positionWorld, viewProjection.matView);

		// 距離条件のチェック
		if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {

			// カメラ前方都の角度を計算
			float arcTangent = std::atan2(
				std::sqrt(positionView.x * positionView.x + positionView.y * positionView.y), 
				positionView.z);

			// 角度条件チェック(コーンに収まっているか)
			if (arcTangent <= angleRange_) {

				targets.emplace_back(std::make_pair(positionView.z, enemy.get()));
			}
		}

		// ロックオン対象のリセット
		target_ = nullptr;
		if (!targets.empty()) {

			// 距離で昇順にソート
			targets.sort([](auto& pair1, auto& pair2) { return pair1.first < pair2.first; });

			// ソートの結果一番近い敵をロックオンの対象とする
			target_ = targets.front().second;
		}
	}
}

Vector3 LockOn::TransformScreen(Vector3 position) { 

	Vector3 positionWorld = position;

	// ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	// ビュー行列とプロジェクション行列、ビューポート行列を合成
	Matrix4x4 matWVP = Multiply(Multiply(viewProjection_.matView, viewProjection_.matProjection), matViewport);

	// ワールド
	positionWorld = Transform(positionWorld, matWVP);

	return positionWorld;
}
