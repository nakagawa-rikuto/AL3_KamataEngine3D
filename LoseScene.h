#pragma once
#include "Audio.h"
#include "AxisIndicator.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "imgui.h"

#include <DebugText.h>

#include "Fade.h"
#include "MyMath.h"


/// <summary>
/// 負けシーン
/// </summary>
class LoseScene {

public: /* メンバ関数 */
	/// <summary>
	/// コンストクラタ
	/// </summary>
	LoseScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~LoseScene();

	/// <summary>
	/// デスフラグのGetter
	/// </summary>
	/// <returns></returns>
	bool IsFinished();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialise();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: /* メンバ変数 */
	// メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// シーンのフェーズ
	enum class Phase {
		kFadeIn,  // フェードイン
		kMain,    // メイン部
		kFadeOut, // フェードアウト
	};


	// 現在のフェーズ
	Phase phase_ = Phase::kFadeIn;

	// 終了フラグ
	bool finished_ = false;

	float speed_ = 0.03f;

	Fade* fade_ = nullptr;

	// モデル
	Model* modelText_ = nullptr;

	// スプライト
	Sprite* spriteText_ = nullptr;

	// worldTransform
	WorldTransform text_;
	WorldTransform player_;

	// viewProjection
	ViewProjection viewProjection_;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
};
