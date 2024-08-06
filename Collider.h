#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "MyMath.h"
#include "imgui.h"

/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider {
public:
	/* ////////////////////////////////////////////////////
	                        Getter
	*/ ////////////////////////////////////////////////////
	/// <summary>
	/// 半径の取得
	/// </summary>
	/// <returns></returns>
	float GetRadius() const;

	/// <summary>
	/// 種別IDを取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetTypeID() const;

	/* ////////////////////////////////////////////////////
	                        Setter
	*/ ////////////////////////////////////////////////////
	/// <summary>
	/// 半径を設定
	/// </summary>
	/// <param name="radius"></param>
	void SetRadius(float& radius);

	/// <summary>
	/// 種別IDを取得
	/// </summary>
	/// <param name="typeID"></param>
	void SetTypeID(uint32_t typeID);

public: /* 基本的な関数 */
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// ワールドトランスフォームの更新
	/// </summary>
	void UpdateWorldTransform();

	// 描画
	void Draw(Model* model, const ViewProjection& viewProjection);

public: /* 仮想関数 */
	virtual Vector3 GetCenterPosition() const = 0;

	/// <summary>
	/// 衝突時に呼ばれる関数
	/// </summary>
	virtual void OnCollision(Collider* other);

private: /* メンバ変数 */
	// 衝突半径
	float radius_ = 2.0f;

	// 種別ID
	uint32_t typeID_ = 0u;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;

private: /* ポリモーフィズム */
	     // virtual ~Collider() = default;
};
