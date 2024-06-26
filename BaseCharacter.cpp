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
/// ワールド変換データを取得
/// </summary>
/// <returns></returns>
const WorldTransform& BaseCharacter::GetWorldTransform() {return worldTransform_; }
