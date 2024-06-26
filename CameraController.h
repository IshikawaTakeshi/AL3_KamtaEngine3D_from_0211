#pragma once

#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

class Player;
class CameraController {
public:

	//矩形構造体
	struct Rect {
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};

	
	CameraController() = default;
	~CameraController() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// リセット処理
	/// </summary>
	void Reset();

	/// <summary>
	/// targetのsetter
	/// </summary>
	void SetTarget(Player* target) { target_ = target; }

	/// <summary>
	/// ViewProjectionの取得
	/// </summary>
	const ViewProjection& GetViewProjection() { return *viewProjection_; }

	void SetMovableArea(Rect area) { movableArea_ = area; }


private:

	//座標補間割合
	static inline const float kInterpolationRate = 0.05f;
	//速度掛け率
	static inline const float kVelocityBias = 31.3f;
	//追従対象の各方向へのカメラ移動範囲
	static inline const Rect movableAreaMargin_ = { -20.0f,20.0f,-20.0f,20.0f };


	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;
	Player* target_ = nullptr;
	//追従対象とカメラの座標の差(オフセット)
	Vector3 targetOffset_ = { 0, 0, -30.0f };
	//カメラ移動範囲
	Rect movableArea_ = { 0,100.0f,0,100.0f };
	//カメラの目標座標
	Vector3 targetPos_;
};

