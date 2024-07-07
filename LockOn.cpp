#include "LockOn.h"

void LockOn::Initialize() {

	// テクスチャの取得


	// WorldTransformの初期化
	worldTransform_.Initialize();

	// スプライトの生成
	lockOnMark_ = Sprite::Create(
		textureHandle_, Vector2()) }
