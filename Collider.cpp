#include "Collider.h"

// 半径の取得
float Collider::GetRadius() const { return radius_; }

// 半径を設定
void Collider::SetRadius(float& radius) {

	radius_ = radius; 
}