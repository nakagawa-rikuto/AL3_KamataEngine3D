#include "Fade.h"

#include <array>
#include <DebugText.h>
#include <algorithm>
#include <numbers>

/// <summary>
/// フェード開始関数
/// </summary>
void Fade::Start(Status status, float duration) {

	status_ = status;
	duration_ = duration;
	counter_ = 0.0f;
}

/// <summary>
/// フェード停止
/// </summary>
void Fade::Stop() { status_ = Status::None; }

/// <summary>
/// フェード終了判定
/// </summary>
bool Fade::IsFinished() const { 

	/* /////////////////
	  フェード状態による分岐
	*/ /////////////////
	switch (status_) {
	case Fade::Status::FadeIn:
	case Fade::Status::FadeOut:
		if (counter_ >= duration_) {
			return true;
		} else {
			return false;
		}
	}
	return true;
}

Fade::Fade() {}

Fade::~Fade() { delete sprite_; }

/* //////////////////////////////////////////
                    初期化
*/ //////////////////////////////////////////
void Fade::Initialize() {

    sprite_ = new Sprite;
	sprite_->Initialize();
	sprite_->SetSize(Vector2(1280.0f, 720.0f));
	sprite_->SetColor(Vector4(0, 0, 0, 1));
}

/* //////////////////////////////////////////
                    更新
*/ //////////////////////////////////////////
void Fade::Update() {

	/* ///////////////////
	   フェード状態による分岐
	*/ ///////////////////
	switch (status_) {
	case Status::None:
		break;
	case Status::FadeIn:
		/// フェードイン中の更新処理
		// １フレーム分の秒数をカウントアップ
		counter_ += 1.0f / 60.0f;

		// フェード継続時間に達したら打ち止め
		if (counter_ >= duration_) {
			counter_ = duration_;
		}

		// 1.0から0.0の間で、経過時間がフェード継続時間に近づくほどアルファ値を小さくする
		sprite_->SetColor(Vector4(0, 0, 0, std::clamp(1.0f - (counter_ / duration_), 0.0f, 1.0f)));
		break;
	case Status::FadeOut:
		/// フェードアウト中の更新処理
		// １フレーム分の秒数をカウントアップ
		counter_ += 1.0f / 60.0f;

		// フェード継続時間に達したら打ち止め
		if (counter_ >= duration_) {
			counter_ = duration_;
		}

		// 0.0から1.0の間で、経過時間がフェード継続時間に近づくほどアルファ値を大きくする
		sprite_->SetColor(Vector4(0, 0, 0, std::clamp(counter_ / duration_, 0.0f, 1.0f)));
		break;
	}
}

/* //////////////////////////////////////////
                    描画
*/ //////////////////////////////////////////
void Fade::Draw() {

	if (status_ == Status::None) {
		return;
	}

	sprite_->Draw();
}