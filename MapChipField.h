#pragma once
#include "stdint.h"
#include "Vector3.h"
#include <vector>
#include <string>

/// <summary>
/// マップチップ種別の定義
/// </summary>
enum class MapChipType {
	kBlank, // 空白
	kBlock, // ブロック
};

struct MapChipData {

	/// <summary>
	/// マップチップデータ構造
	/// </summary>
	std::vector<std::vector<MapChipType>> data_;
};

class MapChipField {
public:

	/// <summary>
	/// リセット
	/// </summary>
	void ResetMapChipData();

	/// <summary>
	/// 読み込み
	/// </summary>
	/// <param name="filePath"></param>
	void LoadMapChipCsv(const std::string& filePath);

	/// <summary>
	/// マップチップ種別の取得
	/// </summary>
	/// <param name="xIndex"></param>
	/// <param name="yIndex"></param>
	/// <returns></returns>
	MapChipType GetMapVhipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	/// <summary>
	/// マップチップ座標の取得
	/// </summary>
	/// <param name="xIndex"></param>
	/// <param name="yIndex"></param>
	/// <returns></returns>
	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	/// <summary>
	/// ブロックの個数の取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetNumBlockVirtical() { return kNumBlockVirtical_; }
	uint32_t GetNumBlockHorizontal() { return kNumBlockHorizontal_; }

private:

	/// ***********************
	/// 定数の定義
	/// ***********************
	MapChipData mapChipData_;

	// ブロックのサイズ
	static inline const float kBlockWidth_ = 1.0f;
	static inline const float kBlockHeight_ = 1.0f;

	// ブロックの個数
	static inline const uint32_t kNumBlockVirtical_ = 20;
	static inline const uint32_t kNumBlockHorizontal_ = 100;
};
