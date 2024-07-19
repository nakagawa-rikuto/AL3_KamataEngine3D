#pragma once

#include <iostream>
#include <variant>
#include <string>
#include <map>
#include <json.hpp>

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
	/// ファイルの書き出し
	/// </summary>
	/// <param name="groupName"></param>
	void SaveFile(const std::string& groupName);

	/// <summary>
	/// ディレクトリの全ファイル読み込み
	/// </summary>
	void LoadFiles();

	/// <summary>
	/// ファイルから読み込む
	/// </summary>
	/// <param name="groupName"></param>
	void LoadFiles(const std::string& groupName);

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
	/// 値のセット(bool)
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <param name="value"></param>
	void SetValue(const std::string& groupName, const std::string& key, bool value);

	/// <summary>
	/// 項目の追加(int)
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <param name="value"></param>
	void AddItem(const std::string& groupName, const std::string& key, int32_t value);

	/// <summary>
	/// 項目の追加(float)
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <param name="value"></param>
	void AddItem(const std::string& groupName, const std::string& key, float value);

	/// <summary>
	/// 項目の追加(Vector3)
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <param name="value"></param>
	void AddItem(const std::string& groupName, const std::string& key, const Vector3& value);

	/// <summary>
	/// 項目の追加(bool)
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <param name="value"></param>
	void AddItem(const std::string& groupName, const std::string& key, bool value);

	/// <summary>
	/// 値の取得(int)
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <returns></returns>
	int32_t GetIntValue(const std::string& groupName, const std::string& key) const;

	/// <summary>
	/// 値の取得(float)
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <returns></returns>
	float GetFloatValue(const std::string& groupName, const std::string& key) const;

	/// <summary>
	/// 値の取得(Vector3)
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <returns></returns>
	Vector3 GetVector3Value(const std::string& groupName, const std::string& key) const;

	/// <summary>
	/// 値の取得(bool)
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <returns></returns>
	bool GetBoolValue(const std::string& groupName, const std::string& key) const;

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
		std::variant<int32_t, float, Vector3, bool> value;
	};

	// グループ
	struct Group {
		std::map<std::string, Item> items;
	};

	// 全データ
	std::map<std::string, Group> datas_;

	// using
	using json = nlohmann::json;
	//using Item = std::variant<int32_t, float, Vector3>;
	//using Group = std::map<std::string, Item>;

	// グローバル変数の保存先ファイルパス
	const std::string kDirectoryPath_ = "Resources/GlobalVariables/";
};
