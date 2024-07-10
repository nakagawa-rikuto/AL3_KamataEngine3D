#include "LockOn.h"

// 初期化
void LockOn::Initialize() {

	// テクスチャの取得
	textureHandle_ = TextureManager::Load("./Resource/Reticle.png");

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

	// ジョイスティック
	XINPUT_STATE joyState;

	// ロックオンボタンをトリガーしたら
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) {

		// ロックオン対象の検索
		
	}
}

// 描画
void LockOn::Draw() {

	if (target_ = nullptr) {

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
