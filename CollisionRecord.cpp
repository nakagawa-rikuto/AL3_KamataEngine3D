#include "CollisionRecord.h"

void CollisionRecord::AddHistory(uint32_t number) {

	// 履歴に登録
	history_.push_back(number);
}

void CollisionRecord::Clear() {

	// 履歴を抹消
	history_.clear();
}
