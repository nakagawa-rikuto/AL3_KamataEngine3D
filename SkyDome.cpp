#include "skydome.h"

void SkyDome::Initialize(Model* model, ViewProjection* viewProjection) {
	assert(model);

	model_ = model;

	worldTransform_.Initialize();

	viewProjection_ = viewProjection;
}

void SkyDome::Update() { worldTransform_.TransferMatrix(); }

void SkyDome::Draw() { model_->Draw(worldTransform_, *viewProjection_); }
