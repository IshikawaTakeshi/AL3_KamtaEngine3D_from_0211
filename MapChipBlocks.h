#pragma once
#include <vector>

class Model;
class WorldTransform;
class MapChipField;
class ViewProjection;
class MapChipBlocks {
public:

	MapChipBlocks() = default;
	~MapChipBlocks();

	/// <summary>
	/// ブロックの生成処理
	/// </summary>
	void GenerateBlocks(MapChipField* mapChipField);

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(MapChipField* mapChipField);

	/// <summary>
	/// 更新処理処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

private:

	//ブロック
	Model* modelBlock_ = nullptr;
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;
};

