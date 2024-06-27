#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "DirectXCommon.h"
#include "Fade.h"
#include "Matrix4x4.h"

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene {
public:

	//タイトルシーンのフェーズ
	enum class Phase {
		kFadeIn, //フェードイン
		kMain, //メイン部
		kFadeOut, //フェードアウト
	};

	TitleScene() = default;
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

	/// <summary>
	/// フェーズの切り替え処理
	/// </summary>
	void ChangePhase();

	//デスフラグのゲッター
	bool IsFinished() const { return finished_; }


private:

	DirectXCommon* dxCommon_ = nullptr;

private:

	//現在のフェーズ
	Phase phase_ = Phase::kFadeIn;

	//フェード
	Fade* fade_ = nullptr;
	//フェード時間
	static inline const float fadeTime_ = 1.0f;
	//3Dテキストのモデル
	Model* titleTextModel_ = nullptr;
	//プレイヤーのモデル
	Model* playerModel_ = nullptr;
	//エネミーのモデル
	Model* enemyModel_ = nullptr;
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

