#pragma once

#include <iostream>
#include <variant>
#include <string>
#include <map>

#include  "MyMath.h"
/// <summary>
/// グローバル変数
/// </summary>
class GlobalVariables {
public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static GlobalVariables* GetInstance();

	/// <summary>
	/// グループの作成
	/// </summary>
	/// <param name="groupName"></param>
	void CreateGroup(const std::string& groupName);

	/// <summary>
	/// 値のセット(int)
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <param name="value"></param>
	void SetValue(const std::string& groupName, const std::string& key, int32_t value);

	/// <summary>
	/// 値のセット(float)
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <param name="value"></param>
	void SetValue(const std::string& groupName, const std::string& key, float value);

	/// <summary>
	/// 値のセット(Vector3)
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <param name="value"></param>
	void SetValue(const std::string& groupName, const std::string& key, const Vector3 value);

	/// <summary>
	/// 毎フレームの処理
	/// </summary>
	void Update();

private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GlobalVariables() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GlobalVariables() = default;

	/// <summary>
	/// コピーコンストラクト
	/// </summary>
	/// <param name=""></param>
	GlobalVariables(const GlobalVariables&) = delete;

	/// <summary>
	/// コピー代入演算子
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const GlobalVariables& operator=(const GlobalVariables&) = delete;

	// 項目
	struct Item {
		// 項目の値
		std::variant<int32_t, float, Vector3> value;
	};

	// グループ
	struct Group {
		std::map<std::string, Item> items;
	};

	// 全データ
	std::map<std::string, Group> datas_;
};
