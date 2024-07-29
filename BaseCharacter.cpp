#include "BaseCharacter.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="models"></param>
void BaseCharacter::Initialize(const std::vector<Model*>& models) {

	models_ = models;

	// ワールド変換データの初期化
	worldTransform_.Initialize();
}

/// <summary>
/// 更新
/// </summary>
void BaseCharacter::Update() { 
	
	worldTransform_.UpdateMatrix(); 
}

/// <summary>
/// 描画
/// </summary>
/// <param name="viewProjection"></param>
void BaseCharacter::Draw(const ViewProjection& viewProjection) {

	for (Model* model : models_) {

		model->Draw(worldTransform_, viewProjection);
	}
}

/// <summary>
/// 衝突時に呼ばれる関数
/// </summary>
void BaseCharacter::OnCollision(Collider* other) {}

/// <summary>
/// ワールド変換データを取得
/// </summary>
/// <returns></returns>
const WorldTransform& BaseCharacter::GetWorldTransform() const{ return worldTransform_; }

/// <summary>
/// 中心座標を取得
/// </summary>
/// <returns></returns>
Vector3 BaseCharacter::GetCenterPosition() const { 

	// ワールド座標を入れる変数
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
