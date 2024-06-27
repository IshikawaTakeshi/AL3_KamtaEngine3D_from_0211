#pragma once
#include "Sprite.h"
#include <d3d12.h>

class Fade {
public:

	//フェードの状態
	enum class Status {
		None, //フェードなし
		FadeIn, //フェードイン中
		FadeOut //フェードアウト中
	};

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
	/// フェード開始処理
	/// </summary>
	void Start(Status status, float duration);

	/// <summary>
	/// フェード終了処理
	/// </summary>
	void Stop();

	/// <summary>
	/// フェード終了判定
	/// </summary>
	bool IsFinished() const;

private:

	//フェードの持続時間
	float duration_ = 1.0f;
	//経過時間カウンター
	float counter_ = 0.0f;

	//現在のフェードの状態
	Status status_ = Status::None;

	//座標
	Vector2 pos_;
	//テクスチャ
	uint32_t textureHandle_;
	//スプライト
	Sprite* sprite_;

};

