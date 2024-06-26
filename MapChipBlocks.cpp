#include "MapChipBlocks.h"
#include "WorldTransform.h"
#include "MapChipField.h"
#include "ViewProjection.h"

MapChipBlocks::~MapChipBlocks() {

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	delete modelBlock_;
}

void MapChipBlocks::GenerateBlocks(MapChipField* mapChipField) {
	//要素数
	const uint32_t kNumBlockVertical = mapChipField->GetNumBlockVirtical();
	const uint32_t kNumBlockHorizonal = mapChipField->GetNumBlockHorizontal();

	//要素数を変更する
	worldTransformBlocks_.resize(kNumBlockVertical);
	for (uint32_t i = 0; i < kNumBlockVertical; i++) {
		worldTransformBlocks_[i].resize(kNumBlockHorizonal);
	}

	//	ブロックの生成
	for (uint32_t row = 0; row < kNumBlockVertical; row++) {
		for (uint32_t column = 0; column < kNumBlockHorizonal; column++) {
			if (mapChipField->GetMapChipTypeByIndex(column, row) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[row][column] = worldTransform;
				worldTransformBlocks_[row][column]->translation_ = mapChipField->GetMapChipPositionByIndex(column, row);
			}
		}
	}
}

void MapChipBlocks::Initialize(MapChipField* mapChipField) {

	GenerateBlocks(mapChipField);
	modelBlock_ = new Model();
	modelBlock_ = Model::Create();

}

void MapChipBlocks::Update() {

	//ブロックの更新
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			worldTransformBlock->UpdateMatrix();
		}
	}
}

void MapChipBlocks::Draw(const ViewProjection& viewProjection) {
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)continue;
			modelBlock_->Draw(*worldTransformBlock, viewProjection);
		}
	}
}
