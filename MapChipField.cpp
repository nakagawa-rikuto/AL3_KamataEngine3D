#include "MapChipField.h"
#include <assert.h>
#include <fstream>
#include <map>
#include <sstream>


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
} // namespace

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
	for (uint32_t i = 0; i < kNumBlockVirtical_; ++i) {

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

MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) {

	/* 範囲外を指定されたら空白を返す */
	if (xIndex < 0 || kNumBlockHorizontal_ - 1 < xIndex) {
		return MapChipType::kBlank;
	}
	if (yIndex < 0 || kNumBlockVirtical_ - 1 < yIndex) {
		return MapChipType::kBlank;
	}

	return mapChipData_.data_[yIndex][xIndex];
}

Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) { 
	return Vector3(kBlockWidth_ * xIndex, kBlockHeight_ * (kNumBlockVirtical_ - 1 - yIndex), 0); 
}

IndexSet MapChipField::GetMapChipIndexSetByPosition(const Vector3& position) { 
	IndexSet indexSet = {}; 
	indexSet.xIndex = static_cast<uint32_t>((position.x + kBlockWidth_ / 2.0f) / kBlockWidth_);
	indexSet.yIndex = kNumBlockVirtical_ - 1 - static_cast<uint32_t>((position.y + kBlockHeight_ / 2.0f) / kBlockHeight_);
	return indexSet;
}

Rect MapChipField::GetRectByIndex(uint32_t xIndex, uint32_t yIndex) {

	// 指定ブロックの中心座標を取得
	Vector3 center = GetMapChipPositionByIndex(xIndex, yIndex);

	Rect rect;
	rect.left = center.x - kBlockWidth_ / 2.0f;
	rect.right = center.x + kBlockWidth_ / 2.0f;
	rect.bottom = center.y - kBlockHeight_ / 2.0f;
	rect.top = center.y + kBlockHeight_ / 2.0f;

	return rect;
}
