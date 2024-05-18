#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"
#include "cassert"
#include "imgui.h"


class MapChipField;

// 左右
enum class LRDirection {
	kRight,
	kLeft,
};

// 4つの角の座標計算
enum Corner {
	kRightBottom, // 右下
	kLeftBottom,  // 左下
	kRightTop,    // 右上
	kLeftTop,     // 左上

	kNumCorner // 要素数
};

// マップチップの当たり判定情報
struct CollisionMapInfo {
	bool kCeiling_ = false; // 天井衝突フラグ
	bool kLanding_ = false; // 着地フラグ
	bool kWall_ = false;    // 壁接触フラグ
	Vector3 moveVolume_;    // 移動量
};

/// <summary>
/// 自キャラ
/// </summary>
class Player {
private:

	/*//////////////////////////////////////////////////////////////////////
	                           メンバ変数
	*///////////////////////////////////////////////////////////////////////

	// マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;

	// ワールド変換データ
	WorldTransform worldTransform_;

	// ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;

	// モデル
	Model* model_ = nullptr;

	// 速度
	Vector3 velocity_ = {};

	// 左右
	LRDirection lrDirection_ = LRDirection::kRight;

	// 微小な余白
	static inline const float margin_ = 0.4f;

	/*///////////////////////////////////////////
	                 角度補間
	*////////////////////////////////////////////
	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;

	// 旋回タイマー
	float turnTimer_ = 0.0f;

	// 旋回時間<秒>
	static inline const float kTimeTurn_ = 0.3f;

	/*///////////////////////////////////////////
	                 左右移動
	*////////////////////////////////////////////
	// 慣性移動
	static inline const float KAcceleration_ = 0.01f;

	// 速度減衰
	static inline const float kAttenuation_ = 0.2f;

	// 最大速度制限
	static inline const float kLimitRunSpeed_ = 0.2f;

	// 壁に触れた時の減衰
	static inline const float kAttenuationWall_ = 0.7f;

	/*///////////////////////////////////////////
	                 ジャンプ
	*////////////////////////////////////////////
	// 接地状態フラグ
	bool onGround_ = true;

	// 重力加速度(下方向)
	static inline const float kGravityAcceleration_ = 0.02f;

	// 最大落下速度(下方向)
	static inline const float kLimitFallSpeed_ = 0.4f;

	// ジャンプ初速(上方向)
	static inline const float kJumpAcceleration_ = 0.5f;

	// 飛んでる時の横の減速率
	static inline const float kAttenuationLanding_ = 0.5f;

	/*///////////////////////////////////////////
	             衝突判定と応答の全容
	*////////////////////////////////////////////
	// キャラクターの当たり判定サイズ
	static inline const float kWidth_ = 1.6f;
	static inline const float kHeight_ = 1.6f;

	// 衝突判定を初期化
	CollisionMapInfo collisionMapInfo_;
	CollisionMapInfo preCollisionMapInfo_;

	// 
	static inline const float kBlank = 1.0f;


public:

	/// <summary>
	/// ImGui
	/// </summary>
	/// <param name="info"></param>
	/// <param name="preInfo"></param>
	void ImGuiDebug(CollisionMapInfo& info, CollisionMapInfo& preInfo);

	/// <summary>
	/// 移動入力
	/// </summary>
	void InputMove();

	/// <summary>
	/// 旋回処理
	/// </summary>
	void Turning();

	/// <summary>
	/// マップ衝突判定
	/// </summary>
	/// <param name="info"></param>
	void CollisionMap(CollisionMapInfo& info, CollisionMapInfo& preInfo);

	/// <summary>
	/// マップ衝突判定方向
	/// </summary>
	/// <param name="info"></param>
	void CollisionDistanceTop(CollisionMapInfo& info, CollisionMapInfo& preInfo);
	void CollisionDistanceBottom(CollisionMapInfo& info, CollisionMapInfo& preInfo);
	void CollisionDistanceRight(CollisionMapInfo& info, CollisionMapInfo& preInfo);
	void CollisionDistanceLeft(CollisionMapInfo& info, CollisionMapInfo& preInfo);

	/// <summary>
	/// 指定した角の座標計算
	/// </summary>
	/// <param name="center"></param>
	/// <param name="corner"></param>
	/// <returns></returns>
	Vector3 CornerPosition(const Vector3& center, Corner corner);

	/// <summary>
	/// 判定結果を反映して移動させる
	/// </summary>
	/// <param name="info"></param>
	void CollisionResultMove(CollisionMapInfo& info);

	/// <summary>
	/// 天井に接触している場合の処理
	/// </summary>
	/// <param name="info"></param>
	void CollisionCeiling(const CollisionMapInfo& info, CollisionMapInfo& preInfo);

	/// <summary>
	/// 壁に触れている場合の処理
	/// </summary>
	void CollisionWalling(const CollisionMapInfo& info, CollisionMapInfo& preInfo);

	/// <summary>
	/// 接地状態の切り替え処理
	/// </summary>
	void CollisionLanding(const CollisionMapInfo& info, CollisionMapInfo& preInfo);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="viewProjection"></param>
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& pos);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 速度加算
	/// </summary>
	/// <returns></returns>
	const Vector3& GetVelocity() const { return velocity_; }

	/// <summary>
	/// ゲッター
	/// </summary>
	/// <returns></returns>
	WorldTransform& GetWorldTransform() { return worldTransform_; }

	/// <summary>
	/// セッター
	/// </summary>
	/// <param name="mapChipField"></param>
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

};
