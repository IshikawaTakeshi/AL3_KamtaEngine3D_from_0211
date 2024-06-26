#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "DirectXCommon.h"
#include "Sprite.h"
#include "Matrix4x4.h"

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene {
public:

	TitleScene();
	~TitleScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	//デスフラグのゲッター
	bool IsFinished() const { return finished_; }
private:

	DirectXCommon* dxCommon_ = nullptr;

private:

	//3Dテキストのモデル
	Model* titleTextModel_;
	//プレイヤーのモデル
	Model* playerModel_;
	//エネミーのモデル
	Model* enemyModel_;
	WorldTransform playerWorldTransform_;
	WorldTransform textWorldTransform_;
	WorldTransform enemyWorldTransform_;
	Matrix4x4 textMatrix_;
	Matrix4x4 playerMatrix_;
	Matrix4x4 enemyMatrix_;
	ViewProjection ViewProjection_;

	//終了フラグ
	bool finished_ = false;

};

