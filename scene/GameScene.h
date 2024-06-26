#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"

#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "MapChipBlocks.h"
#include "MapChipField.h"
#include "CameraController.h"
#include "DeathParticles.h"

#include <vector>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {
public: // メンバ関数


	//ゲームフェーズ
	enum class Phase {
		kPlay, //ゲームプレイ
		kDeath //デス演出
	};

	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// すべての当たり判定を行う処理
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// フェーズの切り替え処理
	/// </summary>
	void ChangePhase();

	//デスフラグのゲッター
	bool IsFinished() const { return finished_; }

private: // メンバ変数

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	ViewProjection viewProjection_;

	//ゲームの現在のフェーズ
	Phase phase_;

	//カメラ
	bool isDebugCameraActive_ = false;
	DebugCamera* debugCamera_ = nullptr;
	CameraController* cameraController_ = nullptr;

	//プレイヤー
	Player* player_ = nullptr;
	//エネミー
	std::list<Enemy*> enemies_;
	//天球
	Skydome* skydome_ = nullptr;
	//マップチップ用ブロック
	MapChipBlocks* mapChipBlocks_ = nullptr;
	
	//デス演出用パーティクル
	DeathParticles* deathParticles_ = nullptr;

	//マップチップフィールド
	MapChipField* mapChipField_ = nullptr;

	//終了フラグ
	bool finished_ = false;

};
