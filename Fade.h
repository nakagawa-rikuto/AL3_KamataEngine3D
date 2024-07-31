#pragma once
#include "DirectXCommon.h"
#include "Sprite.h"

/// <summary>
/// フェード
/// </summary>
class Fade {
public: /* メンバ関数 */

	enum class Status {
		None,    // フェードなし
		FadeIn,  // フェードイン中
		FadeOut, // フェードアウト中
	};

	/// <summary>
	/// フェード開始
	/// </summary>
	void Start(Status status, float duration);

	/// <summary>
	/// フェード停止
	/// </summary>
	void Stop();

	/// <summary>
	/// フェード終了判定
	/// </summary>
	/// <returns></returns>
	bool IsFinished() const;

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

	

private: /* メンバ変数 */
	// 現在のフェードの状態
	Status status_ = Status::None;

	// フェードの接続時間
	float duration_ = 0.0f;

	// 経過時間カウンター
	float counter_ = 0.0f;

	Sprite* sprite_ = nullptr;
};
