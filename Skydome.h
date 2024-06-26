#pragma once
#include "Model.h"
#include "WorldTransform.h"

#include <stdint.h>

/// <summary>
/// 天球
/// </summary>
class Skydome {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	Skydome() = default;
	~Skydome();

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//ビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

};

