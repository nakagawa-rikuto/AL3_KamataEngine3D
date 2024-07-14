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
	/// ワールド座標の取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();

	/// <summary>
	/// ロックオン対象の座標の取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetTargetPosition() const;

	/// <summary>
	/// ロックオン中かどうかを取得
	/// </summary>
	/// <returns></returns>
	bool ExistTarget() const;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Sprite* lockOnMark, uint32_t textureHandle);

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
	const float kDegreeToRadian_ = pi() * 2.0f;

	// 最小距離
	float minDistance_ = 1.0f;

	// 最大距離
	float maxDistance_ = 100.0f;

	// 角度範囲
	float angleRange_ = 20.0f * kDegreeToRadian_;

	// ロックオンマーク用のスプライト
	Sprite* lockOnMark_;

	// ロックオンマークのWorldTransform
	WorldTransform worldTransform_;

	// ロックオンマークのViewProjection
	ViewProjection viewProjection_;

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

	/// <summary>
	/// スクリーン座標への変換
	/// </summary>
	/// <param name="position"></param>
	/// <returns></returns>
	Vector3 TransformScreen(Vector3 position, const ViewProjection& viewProjection);

	/// <summary>
	/// 範囲外判定
	/// </summary>
	/// <param name="viewProjection"></param>
	/// <returns></returns>
	bool CheckOutRange(const ViewProjection& viewProjection);
};