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

/// <summary>
/// マップチップデータ構造
/// </summary>
struct MapChipData {
	std::vector<std::vector<MapChipType>> data_;
};

/// <summary>
/// インデックス
/// </summary>
struct IndexSet {
	uint32_t xIndex;
	uint32_t yIndex;
};

struct Rect {
	float left;    // 左端
	float right;   // 右端
	float bottom;  // 下端
	float top;     // 上端
};

class MapChipField {

private:

	/// ***********************
	/// 定数の定義
	/// ***********************
	MapChipData mapChipData_;

	// ブロックのサイズ
	static inline const float kBlockWidth_ = 2.0f;
	static inline const float kBlockHeight_ = 2.0f;

	// ブロックの個数
	static inline const uint32_t kNumBlockVirtical_ = 20;
	static inline const uint32_t kNumBlockHorizontal_ = 100;

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
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	/// <summary>
	/// マップチップ座標の取得
	/// </summary>
	/// <param name="xIndex"></param>
	/// <param name="yIndex"></param>
	/// <returns></returns>
	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	/// <summary>
	/// 座標からマップチップ番号を計算
	/// </summary>
	/// <param name="position"></param>
	/// <returns></returns>
	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);

	/// <summary>
	/// ブロックの範囲取得関数
	/// </summary>
	/// <param name="xIndex"></param>
	/// <param name="yIndex"></param>
	/// <returns></returns>
	Rect GetRectByIndex(uint32_t xIndex, uint32_t yIndex);

	/// <summary>
	/// ブロックの個数の取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetNumBlockVirtical() { return kNumBlockVirtical_; }
	uint32_t GetNumBlockHorizontal() { return kNumBlockHorizontal_; }


};
