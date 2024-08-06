#include "Hammer.h"
#include "CollisionTypeIdDef.h"
#include "CollisionEffect.h"
#include "Enemy.h"

// 　回転の取得
const Vector3& Hammer::GetRotate() const { return worldTransform_.rotation_; }

// 回転のセット
void Hammer::SetRotate(const Vector3& rotation) { worldTransform_.rotation_ = rotation; }

// 中心座標の取得
Vector3 Hammer::GetCenterPosition() const { 

	// ローカル座標でのオフセット
	const Vector3 offset = {0.0f, 12.5f, 4.0f};

	// ワールド座標に変換
	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);

	return worldPos;
}

// エフェクトのセット
void Hammer::SetEffect(CollisionEffect* effect) { effect_ = effect; }

// 衝突判定
void Hammer::OnCollision(Collider* other) {

	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();

	// 衝突相手が敵なら
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemy)) {
		Enemy* enemy = static_cast<Enemy*>(other);

		// 敵の位置にエフェクトを発生
		// エフェクトを発生させる
		effect_->Start(enemy);
	}
}

// WorldTransformの初期化
void Hammer::WorldTransformInitialize() { 
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f}; 
}

// 初期化
void Hammer::Initialize(Model* model, const WorldTransform* worldTransform) {

	model_ = model;

	worldTransform_.SetParent(worldTransform);

	// 種別IDの設定
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kPlayerWeapon));
}

// 更新
void Hammer::Update() { 

	worldTransform_.UpdateMatrix(); 
}

// 描画
void Hammer::Draw(const ViewProjection& viewProjection) { 

	model_->Draw(worldTransform_, viewProjection); 
}
