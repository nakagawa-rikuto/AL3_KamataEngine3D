#pragma once
#include "Audio.h"
#include "AxisIndicator.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include <cassert>
#include <memory>
#include <sstream>

#include "Enemy.h"

/// <summary>
/// ロックオン
/// </summary>
class LockOn {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="enemies"></param>
	/// <param name="viewProjection"></param>
	void Update(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	// ロックオンの対象
	const Enemy* target_ = nullptr;

	//
	const float kDegreeToRadian_ = 3.0f;

	// 最小距離
	float minDistance_ = 10.0f;

	// 最大距離
	float maxDistance_ = 30.0f;

	// 角度範囲
	float angleRange_ = 20.0f * kDegreeToRadian_;

	// ロックオンマーク用のスプライト
	std::unique_ptr<Sprite> lockOnMark_;

	// ロックオンマークのWorldTransform
	WorldTransform worldTransform_;

	// ロックオンマークのテクスチャ
	uint32_t textureHandle_ = 0u;

private: // メンバ関数

	/// <summary>
	/// 検索
	/// </summary>
	/// <param name="enemies"></param>
	/// <param name="viewProjection"></param>
	void Search(
		const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection);
};
