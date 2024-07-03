#include "GlobalVariables.h"
#include "WinApp.h"
#include <fstream>
#include <format>
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

//
void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, int32_t value) {
	// グループの参照を取得
	Group& group = datas_[groupName];

	// 新しい項目データを設定
	Item newItem{};
	newItem.value = value;

	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}

//
void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, float value) {

	// グループの参照を取得
	Group& group = datas_[groupName];

	// 新しい項目データを設定
	Item newItem{};
	newItem.value = value;

	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}

//
void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, const Vector3 value) {

	// グループの参照を取得
	Group& group = datas_[groupName];

	// 新しい項目データを設定
	Item newItem{};
	newItem.value = value;

	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}

//
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

//
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
