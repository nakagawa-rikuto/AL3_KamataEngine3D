#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "AxisIndicator.h"

#include <memory>
#include <sstream>
#include <cassert>

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
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	// ロックオンマーク用のスプライト
	std::unique_ptr<Sprite> lockOnMark_;

	// ロックオンマークのWorldTransform
	WorldTransform worldTransform_;

	// ロックオンマークのテクスチャ
	uint32_t textureHandle_ = 0u;
};
