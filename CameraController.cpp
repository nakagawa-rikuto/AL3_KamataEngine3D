#include "CameraController.h"
#include "Player.h"
#include <algorithm>

void CameraController::Reset() {
	
	/// 瞬間合わせ ///
	// 追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	// 追従対象とオフセットからカメラの座標を計算
	viewProjection_->translation_ = targetWorldTransform.translation_ + targetOffset_;
}

void CameraController::Initialize(ViewProjection* viewProjection) { 
	viewProjection_ = viewProjection;
}

void CameraController::Update() {

	// 追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	// TargetのVelocityを取得
	targetVelocity_ = target_->GetVelocity();

	/* 29ページから */
	// 追従対象とオフセットからカメラの目標座標を計算 
	goalPos_ = targetWorldTransform.translation_ + targetOffset_ + targetVelocity_ * kVelocityBias_;

	// 座標補間によりゆったり追従
	viewProjection_->translation_ = Leap(viewProjection_->translation_, goalPos_, kInterpolationRate_);

	// 追従対象が画面外に出ないように補正
	viewProjection_->translation_.x = std::clamp(
		viewProjection_->translation_.x, targetWorldTransform.translation_.x + targetMargin_.right, targetWorldTransform.translation_.x + targetMargin_.left);
	viewProjection_->translation_.y = std::clamp(
		viewProjection_->translation_.y, targetWorldTransform.translation_.y + targetMargin_.top, targetWorldTransform.translation_.y + targetMargin_.bottom);

	// 移動範囲の制限
	viewProjection_->translation_.x = std::clamp(viewProjection_->translation_.x, movableArea_.left, movableArea_.right);
	viewProjection_->translation_.y = std::clamp(viewProjection_->translation_.y, movableArea_.bottom, movableArea_.top);
	

	// 行列を更新する
	viewProjection_->UpdateMatrix();
}


