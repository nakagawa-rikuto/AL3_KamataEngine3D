#include "RailCamera.h"

void RailCamera::Rotate() {


}

// 初期化
void RailCamera::Initialize(Vector3& worldPosition, Vector3 rotation) {

	// ワールドトランスフォームの初期化設定
	worldTransform_.Initialize();
	worldTransform_.rotation_ = rotation;
	worldTransform_.translation_ = worldPosition;

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

// 更新
void RailCamera::Update() {

	//worldTransform_.translation_.z += 0.05f;
	// worldTransform_.rotation_.y += 0.02f;

	// カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	// 行列の再計算
	worldTransform_.UpdateMatrix();
}