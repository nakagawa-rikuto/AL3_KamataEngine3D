#include "WorldTransform.h"

/// *************************************
/// ワールド行列更新関数
/// *************************************

// 行列を計算・転送する
void WorldTransform::UpdateMatrix() { 

	// スケール、回転、平行移動を合成して行列を計算する
	matWorld_ = math_.MakeAffineMatrix(scale_, rotation_, translation_); 

	// 定数バッファに転送する
	TransferMatrix();
}