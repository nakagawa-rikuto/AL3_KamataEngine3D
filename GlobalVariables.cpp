#include "GlobalVariables.h"
#include "WinApp.h"
#include <format>
#include <fstream>
#include <imgui.h>

//
GlobalVariables* GlobalVariables::GetInstance() {

	static GlobalVariables instance;
	return &instance;
}

//
void GlobalVariables::CreateGroup(const std::string& groupName) {
	// 指定名のオブジェクトがなければ追加
	datas_[groupName];
}

/* ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                           ファイルの書き出し・読み込み
*/ ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ファイルの書き出し
void GlobalVariables::SaveFile(const std::string& groupName) {

	// グループ検索
	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);

	// 未登録チェック
	assert(itGroup != datas_.end());

	json root;

	// 代入
	root = json::object();

	// jsonオブジェクトの登録
	root[groupName] = json::object();

	// 各項目について(1)
	for (std::map<std::string, Item>::iterator itItem = itGroup->second.items.begin(); itItem != itGroup->second.items.end(); ++itItem) {

		// 項目名を取得
		const std::string& itemName = itItem->first;

		// 項目の参照を取得
		Item& item = itItem->second;

		// 各項目の処理(int32_t)
		if (std::holds_alternative<int32_t>(item.value)) { // holds_alternativeで型名の判別

			// int32_t型の値を登録
			root[groupName][itemName] = std::get<int32_t>(item.value);

			// 各項目の処理(float)
		} else if (std::holds_alternative<float>(item.value)) {

			// int32_t型の値を登録
			root[groupName][itemName] = std::get<float>(item.value);

			// 各項目の処理(Vector3)
		} else if (std::holds_alternative<Vector3>(item.value)) {

			Vector3 value = std::get<Vector3>(item.value);
			root[groupName][itemName] = json::array({value.x, value.y, value.z});

			// 各項目の処理(bool)
		} else if (std::holds_alternative<bool>(item.value)) {

			root[groupName][itemName] = std::get<bool>(item.value);
		}
	}

	// ディレクトリの作成
	std::filesystem::path dir(kDirectoryPath_);
	if (!std::filesystem::exists(kDirectoryPath_)) { // あっているかわからない
		std::filesystem::create_directories(kDirectoryPath_);
	}

	// 書き込むJSONファイルのフルパスを合成
	std::string filePath = kDirectoryPath_ + groupName + "Json";

	// 書き込み用ファイルストリーム
	std::ofstream ofs;

	// ファイルを書き込み用に開く
	ofs.open(filePath);

	// ファイルオープン失敗?
	if (ofs.fail()) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
		return;
	}

	// ファイルにjson文字列を書き込む(インデント幅4)
	ofs << std::setw(4) << root << std::endl;

	// ファイルを閉じる
	ofs.close();
}

// ディレクトリの全ファイル読み込み
void GlobalVariables::LoadFiles() {

	// グローバル変数の保存先ファイルパス
	const std::string kDirectoryPath;

	// ディレクトリがなければスキップ
	if (kDirectoryPath.empty()) {

		return;
	}

	std::filesystem::directory_iterator dir_it(kDirectoryPath);
	for (const std::filesystem::directory_entry& entry : dir_it) {
		// ファイルパスを取得
		const std::filesystem::path& filePath = entry.path();

		// ファイル拡張子を取得
		std::string extension = filePath.extension().string();

		// .jsonファイル以外はスキップ
		if (extension.compare(".json") != 0) {
			continue;
		}

		// ファイル読み込み
		LoadFiles(filePath.stem().string());
	}
}

// ファイルから読み込む
void GlobalVariables::LoadFiles(const std::string& groupName) {
	// 読み込むJSONファイルのフルパスを合成
	std::string filePath = kDirectoryPath_ + groupName + ".json";

	// 読み込み用ファイルストリーム
	std::ifstream ifs;

	// ファイルを読み込み用に開く
	ifs.open(filePath);

	// ファイルオープン失敗?
	if (ifs.fail()) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
		return;
	}

	json root;

	// json文字列からjsonのデータ構造に展開
	ifs >> root;

	// ファイルを閉じる
	ifs.close();

	// グループ検索
	json::iterator itGroup = root.find(groupName);

	// 未登録チェック
	assert(itGroup != root.end());

	// 各アイテムについて
	for (json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {

		// アイテム名を取得
		const std::string& itemName = itItem.key();

		// int32_t型の値を保持していれば
		if (itItem->is_number_integer()) {

			// int型の値を登録
			int32_t value = itItem->get<int32_t>();
			SetValue(groupName, itemName, value);

			// float型の値を保持していれば
		} else if (itItem->is_number_float()) {

			// float型の値を登録
			double value = itItem->get<double>();
			SetValue(groupName, itemName, static_cast<float>(value));

			// 要素数3の配列であれば
		} else if (itItem->is_array() && itItem->size() == 3) {

			// float型のjson配列登録
			Vector3 value = {itItem->at(0), itItem->at(1), itItem->at(2)};
			SetValue(groupName, itemName, value);

			// bool型の値を保持していれば
		} else if (itItem->is_number_integer()) {

			// bool型の値を保持していれば
			bool value = itItem->get<bool>();
			SetValue(groupName, itemName, value);
		}
	}
}

/* ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                                    項目のセット
*/ ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, int32_t value) {
	// グループの参照を取得
	Group& group = datas_[groupName];

	// 新しい項目データを設定
	Item newItem{};
	newItem.value = value;

	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}
void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, float value) {

	// グループの参照を取得
	Group& group = datas_[groupName];

	// 新しい項目データを設定
	Item newItem{};
	newItem.value = value;

	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}
void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, const Vector3 value) {

	// グループの参照を取得
	Group& group = datas_[groupName];

	// 新しい項目データを設定
	Item newItem{};
	newItem.value = value;

	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}
void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, bool value) {

	// グループの参照を取得
	Group& group = datas_[groupName];

	// 新しい項目データを設定
	Item newItem{};
	newItem.value = value;

	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}

/* ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                                    項目の追加
*/ ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, int32_t value) {

	json root;

	// グループ検索
	json::iterator itGroup = root.find(groupName);

	// 項目が未登録なら
	if (itGroup == root.end()) {

		SetValue(groupName, key, value);
	}
}
void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, float value) {
	json root;

	// グループ検索
	json::iterator itGroup = root.find(groupName);

	// 項目が未登録なら
	if (itGroup == root.end()) {

		SetValue(groupName, key, value);
	}
}
void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, const Vector3& value) {

	json root;

	// グループ検索
	json::iterator itGroup = root.find(groupName);

	// 項目が未登録なら
	if (itGroup == root.end()) {

		SetValue(groupName, key, value);
	}
}
void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, bool value) {

	json root;

	// グループ検索
	json::iterator itGroup = root.find(groupName);

	// 項目が未登録なら
	if (itGroup == root.end()) {

		SetValue(groupName, key, value);
	}
}

/* ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                                   　値の取得
*/ ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int32_t GlobalVariables::GetIntValue(const std::string& groupName, const std::string& key) const {

	// 指定グループが存在するか確認
	auto itGroup = datas_.find(groupName);
	assert(itGroup != datas_.end());

	// グループの参照を取得
	const Group& group = itGroup->second;

	// 指定したグループに指定のキーが存在するか確認
	auto itItem = group.items.find(key);
	assert(itItem != group.items.end());

	// 指定したキーの値がint32_t型か確認
	assert(std::holds_alternative<int32_t>(itItem->second.value));

	// 指定したグループから指定のキーの値を取得
	return std::get<int32_t>(itItem->second.value);
}
float GlobalVariables::GetFloatValue(const std::string& groupName, const std::string& key) const {

	// 指定グループが存在するか確認
	auto itGroup = datas_.find(groupName);
	assert(itGroup != datas_.end());

	// グループの参照を取得
	const Group& group = itGroup->second;

	// 指定したグループに指定のキーが存在するか確認
	auto itItem = group.items.find(key);
	assert(itItem != group.items.end());

	// 指定したキーの値がbool型か確認
	assert(std::holds_alternative<float>(itItem->second.value));

	// 指定したグループから指定のキーの値を取得
	return std::get<float>(itItem->second.value);
}
Vector3 GlobalVariables::GetVector3Value(const std::string& groupName, const std::string& key) const {

	 // 指定グループが存在するか確認
	auto itGroup = datas_.find(groupName);
	assert(itGroup != datas_.end());

	// グループの参照を取得
	const Group& group = itGroup->second;

	// 指定したグループに指定のキーが存在するか確認
	auto itItem = group.items.find(key);
	assert(itItem != group.items.end());

	// 指定したキーの値がVector3型か確認
	assert(std::holds_alternative<Vector3>(itItem->second.value));

	// 指定したグループから指定のキーの値を取得
	return std::get<Vector3>(itItem->second.value);
}
bool GlobalVariables::GetBoolValue(const std::string& groupName, const std::string& key) const {

	// 指定グループが存在するか確認
	 auto itGroup = datas_.find(groupName);
	 assert(itGroup != datas_.end());

	// グループの参照を取得
	 const Group& group = itGroup->second;

	// 指定したグループに指定のキーが存在するか確認
	 auto itItem = group.items.find(key);
	 assert(itItem != group.items.end());

	// 指定したキーの値がbool型か確認
	 assert(std::holds_alternative<bool>(itItem->second.value));

	// 指定したグループから指定のキーの値を取得
	 return std::get<bool>(itItem->second.value);
}

// 更新
void GlobalVariables::Update() {

	if (!ImGui::Begin("Global Variables", nullptr, ImGuiWindowFlags_MenuBar)) {
		ImGui::End();
		return;
	}

	if (!ImGui::BeginMenuBar())
		return;

	// 各グループについて(1)
	for (std::map<std::string, Group>::iterator itGroup = datas_.begin(); itGroup != datas_.end(); ++itGroup) {

		// グループ名を取得
		const std::string& groupName = itGroup->first;

		// グループの参照を取得
		Group& group = itGroup->second;

		// 各グループについて(2)
		if (!ImGui::BeginMenu(groupName.c_str()))
			continue;

		// 各項目について(1)
		for (std::map<std::string, Item>::iterator itItem = group.items.begin(); itItem != group.items.end(); ++itItem) {

			// 項目名を取得
			const std::string& itemName = itItem->first;

			// 項目の参照を取得
			Item& item = itItem->second;

			// 各項目の処理(int32_t)
			if (std::holds_alternative<int32_t>(item.value)) { // holds_alternativeで型名の判別

				int32_t* ptr = std::get_if<int32_t>(&item.value);
				ImGui::SliderInt(itemName.c_str(), ptr, 0, 100);

				// 各項目の処理(float)
			} else if (std::holds_alternative<float>(item.value)) {

				float* ptr = std::get_if<float>(&item.value);
				ImGui::SliderFloat(itemName.c_str(), ptr, 0, 100);

				// 各項目の処理(Vector3)
			} else if (std::holds_alternative<Vector3>(item.value)) {

				Vector3* ptr = std::get_if<Vector3>(&item.value);
				ImGui::SliderFloat3(itemName.c_str(), reinterpret_cast<float*>(ptr), -10.0f, 10.0f);

				// 各項目の処理(bool)
			} else if (std::holds_alternative<bool>(item.value)) {

				bool* ptr = std::get_if<bool>(&item.value);
				ImGui::Checkbox(itemName.c_str(), ptr);
			}
		}

		// 改行
		ImGui::Text("\n");

		if (ImGui::Button("Save")) {
			SaveFile(groupName);
			std::string message = std::format("{}.json saved.", groupName);
			MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		}

		ImGui::EndMenu();
	}

	ImGui::EndMenuBar();
	ImGui::End();
}
