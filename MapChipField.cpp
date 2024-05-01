#include "MapChipField.h"
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <assert.h>

/// <summary>
/// 無名名前空間
/// </summary>
/* 無名ネームスペースで囲むと内部リンゲージを持つようになり、
グローバル変数の名前の衝突を避けることができる。*/
namespace {

/// <summary>
/// マップチップテーブル
/// </summary>
std::map<std::string, MapChipType> mapChipTable = {
    {"0", MapChipType::kBlank},
    {"1", MapChipType::kBlock},
};
}

void MapChipField::ResetMapChipData() {
    // マップチップデータのリセット
	mapChipData_.data_.clear();
	mapChipData_.data_.resize(kNumBlockVirtical_);
	for (std::vector<MapChipType>& mapChipDataLine : mapChipData_.data_) {
		mapChipDataLine.resize(kNumBlockHorizontal_);
	}
}

void MapChipField::LoadMapChipCsv(const std::string& filePath) {

	/// ***********************
	/// 読み込み
	/// ***********************

	// マップチップデータをリセット
	ResetMapChipData();

	// ファイルを開く
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

	// マップチップCSV
	std::stringstream mapChipCsv;

	// ファイルの内容を文字列ストリームにコピー
	mapChipCsv << file.rdbuf();

	// ファイルを閉じる
	file.close();

	/// ***********************
	/// 解析と記録
	/// ***********************
	// CSVからマップチップデータを読み込む
	for (uint32_t i = 0; i < kNumBlockVirtical_; i++) {

		std::string line;
		std::getline(mapChipCsv, line);

		// 一行の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		for (uint32_t j = 0; j < kNumBlockHorizontal_; ++j) {

			std::string word;
			std::getline(line_stream, word, ',');

			if (mapChipTable.contains(word)) {
				mapChipData_.data_[i][j] = mapChipTable[word];
			}
		}
	}
}

MapChipType MapChipField::GetMapVhipTypeByIndex(uint32_t xIndex, uint32_t yIndex) { 
	if (xIndex < 0 || kNumBlockHorizontal_ - 1 < xIndex) {
		return MapChipType::kBlank;
	}
	if (yIndex < 0 || kNumBlockVirtical_ - 1 < yIndex) {
		return MapChipType::kBlank;
	}

	//	ブレークポイントがここを通るとき例外スルーが起きるため改善
	return mapChipData_.data_[yIndex][xIndex];
}

Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) { 
	return Vector3(kBlockWidth_ * xIndex, kBlockHeight_ * (kNumBlockVirtical_ - 1 - yIndex), 0); 
}
