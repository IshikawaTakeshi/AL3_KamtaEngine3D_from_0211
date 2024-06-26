#pragma once

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Vector4.h"
#include "ObjectColor.h"
#include <numbers>

#include <array>

/// <summary>
/// デス演出用パーティクル
/// </summary>
class DeathParticles {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Vector3 position);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();


	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// ワールド座標の取得
	/// </summary>
	const Vector3& GetWorldPos() { return centerPos_; }

	/// <summary>
	/// 終了フラグの取得
	/// </summary>
	bool GetIsFinished() { return isFinished_; }
private:

	//パーティクルの個数
	static inline const uint32_t kNumParticles = 8;
	//存続時間<秒>
	static inline const float kDuration = 10.0f;
	//移動の速さ
	static inline const float kSpeed = 0.1f;
	//分割した1個分の角度
	static inline const float kAngleUnit = (2.0f * std::numbers::pi_v<float>) / kNumParticles;

	//ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;
	//モデル
	std::array<Model*, kNumParticles> models_ = { nullptr };
	//ワールドトランスフォーム
	std::array<WorldTransform, kNumParticles> worldTransforms_;
	//パーティクルの中心座標
	Vector3 centerPos_;
	//色変更オブジェクト
	ObjectColor objectColor_;
	//色の数値
	Vector4 color_;
	
	//終了フラグ
	bool isFinished_ = false;
	//経過時間カウント
	float counter_ = 0.0f;

};

