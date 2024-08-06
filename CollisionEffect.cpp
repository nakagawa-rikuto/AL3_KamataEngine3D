#include "CollisionEffect.h"
#include "Collider.h"

/* ////////////////////////////////////////////////////////////////
                           Getter
*/ ////////////////////////////////////////////////////////////////
// 終了フラグ
bool CollisionEffect::IsFinished() { return isFinish_; }

/* ////////////////////////////////////////////////////////////////
                           エフェクトの終始
*/ ////////////////////////////////////////////////////////////////
// エフェクトの開始
void CollisionEffect::Start(Collider* collider) {

    /* 
    ・うまくエネミーの位置にエフェクトを出せない
    ・エフェクトのスケールを変えても大きさが変わらない
    */
    worldTransform_.translation_ = collider->GetCenterPosition();
	worldTransform_.scale_ = {5.0f, 5.0f, 5.0f};
	color_ = {1, 1, 1, 1};
    isFinish_ = false;
}

/* ////////////////////////////////////////////////////////////////
                           Setter
*/ ////////////////////////////////////////////////////////////////


/* ////////////////////////////////////////////////////////////////
                            初期化
*/ ////////////////////////////////////////////////////////////////
void CollisionEffect::Initialize() {

    // モデルの読み込み
    model_.reset(Model::CreateFromOBJ("CollisionModel", true)); 

    // ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.scale_ = {5.0f, 5.0f, 5.0f};

    // オブジェクトカラーの初期化
	objectColor_.Initialize();

    // カラーの設定
	color_ = {1, 1, 1, 1};
}

/* ////////////////////////////////////////////////////////////////
                            更新
*/ ////////////////////////////////////////////////////////////////
void CollisionEffect::Update() { 

    ImGui::DragFloat3("EffectWorldTransform", &worldTransform_.translation_.x, 0.1f);
	ImGui::DragFloat3("EffectWorldTransform, scale", &worldTransform_.scale_.x, 0.1f);
    ImGui::Checkbox("isFinish", &isFinish_);

    // 終了なら何もしない
	if (isFinish_) {

		return;
	}

    // 透明度が0以下になると終了
    if (color_.w <= 0.0f) {

        isFinish_ = true;
    }

    worldTransform_.scale_.x += 1.0f;
	worldTransform_.scale_.y += 1.0f;
	worldTransform_.scale_.z += 1.0f;

    color_.w -= 0.01f;

	// 色変更オブジェクトに色の数値を設定する
	objectColor_.SetColor(color_);

	// 色変更オブジェクトをVRAMに転送
	objectColor_.TransferMatrix();
	worldTransform_.UpdateMatrix();
}

/* ////////////////////////////////////////////////////////////////
                            描画
*/ ////////////////////////////////////////////////////////////////
void CollisionEffect::Draw(const ViewProjection& viewProjection) {

    // 終了なら何もしない
	if (isFinish_) {

		return;
	}

    model_->Draw(worldTransform_, viewProjection, &objectColor_);
}
