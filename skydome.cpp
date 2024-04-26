#include "skydome.h"

void skydome::Initialize(Model* model, ViewProjection* viewProjection) { 
	assert(model); 

	model_ = model;

	worldTransform_.Initialize();

	viewProjection_ = viewProjection;
}

void skydome::Update() { 
	worldTransform_.TransferMatrix(); 
}

void skydome::Draw() { 
	model_->Draw(worldTransform_, *viewProjection_); 
}
