#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "imgui.h"

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene {

public: /* メンバ関数 */
	/// <summary>
	/// コンストクラタ
	/// </summary>
	TitleScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleScene();

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

	// 終了フラグ
	bool finished_ = false;

	

	// モデル
	Model* modelText_ = nullptr;
	Model* modelPlayer_ = nullptr;

	// worldTransform
	WorldTransform text_;
	WorldTransform player_;

	// viewProjection
	ViewProjection viewProjection_;
};
