#include "DeathParticles.h"

/* //////////////////////////////////////////////////
                    初期化
*/ //////////////////////////////////////////////////
void DeathParticles::Initialize(Model* model, ViewProjection* viewProjection, Vector3 position) {

	model_ = model;

	viewProjection_ = viewProjection;

	// ワールドトランスフォームの初期化
	for (auto& worldTransform : worldTransform_) {

		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}

	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};

}

/* //////////////////////////////////////////////////
                    更新
*/ //////////////////////////////////////////////////
void DeathParticles::Update() {

	// 終了なら何もしない
	if (isFinished_) {
		return;
	}

	// カウンターを１フレーム分の秒数進める
	counter_ += 1.0f / 60.0f;

	//存続時間の上限に達したら
	if (counter_ >= kDuration_) {

		counter_ = kDuration_;

		// 終了扱いにする
		isFinished_ = true;
	}

	// アルファ値の範囲を設定
	color_.w = std::clamp(color_.w, 0.0f, 1.0f);

	// 色変更オブジェクトに色の数値を設定する
	objectColor_.SetColor(color_);

	// 色変更オブジェクトをVRAMに転送
	objectColor_.TransferMatrix();

	for (uint32_t i = 0; i < kNumParticles; ++i) {

		// 基本となる速度ベクトル
		Vector3 velocity = {kSpeed_, 0, 0};

		// 回転角を計算する
		float angle = kAngleUnit * i;

		// ｚ軸周り回転行列
		Matrix4x4 matrixRotation = MakeRotateZMatrix(angle);

		// 基本となるベクトルを回転させて速度ベクトルを得る
		velocity = Transform(velocity, matrixRotation);

		// 移動処理
		worldTransform_[i].translation_ += velocity;
	}

	// ワールドトランスフォームの更新
	for (auto& worldTransform : worldTransform_) {

		worldTransform.UpdateMatrix();
	}
}

/* //////////////////////////////////////////////////
                     描画
*/ //////////////////////////////////////////////////
void DeathParticles::Draw() {

	// 終了なら何もしない
	if (isFinished_) {
		return;
	}

	for (auto& worldTransform : worldTransform_) {

		model_->Draw(worldTransform, *viewProjection_, &objectColor_);
	}
}
