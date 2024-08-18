#pragma once

#include <functional>

/// <summary>
/// 次元発動
/// </summary>
class TimedCall {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TimedCall(std::function<void()> callback, uint32_t time);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 完了フラグを返す
	/// </summary>
	/// <returns></returns>
	bool IsFinished() { return isFinish_; }

private:

	// コールバック用の関数
	std::function<void()> callback_;

	// 残り時間
	uint32_t time_;

	// 完了フラグ
	bool isFinish_ = false;
};
