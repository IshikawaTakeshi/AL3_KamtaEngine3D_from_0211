#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include "Math/MyMath/MatrixMath.h"
#include <cassert>

GameScene::GameScene() {}

//解放処理
GameScene::~GameScene() {

	delete skydome_;
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	delete deathParticles_;

	delete player_;

	delete mapChipBlocks_;

	delete debugCamera_;
	delete cameraController_;
	delete mapChipField_;
}


void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//ゲームプレイフェーズから開始
	phase_ = Phase::kPlay;

	//カメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	

	//マップチップフィールド生成
	mapChipField_ = new MapChipField();
	mapChipField_->LoadMapChipCsv("Resources/AL3_MapChip.csv");

	
	//プレイヤー生成
	player_ = new Player();
	player_->Initialize();
	player_->SetPos(mapChipField_->GetMapChipPositionByIndex(1, 18));
	player_->SetMapChipField(mapChipField_);

	//エネミー生成
	for (int32_t i = 0; i < 2; ++i) {
		Enemy* newEnemy = new Enemy();
		newEnemy->Initialize();
		newEnemy->SetPos(mapChipField_->GetMapChipPositionByIndex(12 + i, 17 + i));
		enemies_.push_back(newEnemy);
	}

	//ブロックの生成
	mapChipBlocks_ = new MapChipBlocks();
	mapChipBlocks_->Initialize(mapChipField_);

	//天球の生成
	skydome_ = new Skydome();
	skydome_->Initialize();

	//デス演出用パーティクルの生成
	deathParticles_ = new DeathParticles();
	deathParticles_->Initialize(player_->GetWorldPos());

	//カメラコントローラーの生成
	cameraController_ = new CameraController();
	cameraController_->Initialize();
	cameraController_->SetTarget(player_);
	cameraController_->Reset();

	//軸方向表示の生成	
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	viewProjection_.Initialize();
}

void GameScene::Update() {

	

	switch (phase_) {
#pragma region ゲームプレイフェーズの処理
	case GameScene::Phase::kPlay:

		//天球の更新処理
		skydome_->Update();

		//エネミーの更新処理	
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		//プレイヤーの更新処理
		player_->Update();

		//カメラコントローラの更新処理
		cameraController_->Update();

#ifdef _DEBUG

		if (input_->TriggerKey(DIK_C)) {
			isDebugCameraActive_ = true;
		}
#endif // _DEBUG

		//デバッグカメラの処理
		if (isDebugCameraActive_ == true) {
			debugCamera_->Update();
			viewProjection_.matView = debugCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
			viewProjection_.TransferMatrix();
		} else {
			//ビュープロジェクション行列の更新と転送
			viewProjection_.UpdateMatrix();
		}

		//ブロックの更新処理
		mapChipBlocks_->Update();
		
		//すべての衝突判定
		CheckAllCollisions();

		ChangePhase();

		break;
#pragma endregion

#pragma region デス演出フェーズの処理
	case GameScene::Phase::kDeath:


		//天球の更新処理
		skydome_->Update();

		//エネミーの更新処理	
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		//デス演出用パーティクルの更新
		if (deathParticles_) {
			deathParticles_->Update();
		}

#ifdef _DEBUG

		if (input_->TriggerKey(DIK_C)) {
			isDebugCameraActive_ = true;
		}
#endif // _DEBUG

		//デバッグカメラの処理
		if (isDebugCameraActive_ == true) {
			debugCamera_->Update();
			viewProjection_.matView = debugCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
			viewProjection_.TransferMatrix();
		} else {
			//ビュープロジェクション行列の更新と転送
			viewProjection_.UpdateMatrix();
		}

		//ブロックの更新処理
		mapChipBlocks_->Update();

		//デスした時のパーティクルの演出が終了したらゲームシーンの終了フラグを立てる
		if (deathParticles_ && deathParticles_->GetIsFinished() == true) {
			finished_ = true;
		}

		break;
#pragma endregion
	}
	
	

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	switch (phase_) {
#pragma region ゲームプレイフェーズの処理
	case GameScene::Phase::kPlay:

		//天球描画
		skydome_->Draw(cameraController_->GetViewProjection());

		//プレイヤー描画
		player_->Draw(cameraController_->GetViewProjection());

		//エネミーの描画
		for (Enemy* enemy : enemies_) {
			enemy->Draw(cameraController_->GetViewProjection());
		}

		//ブロック描画
		mapChipBlocks_->Draw(cameraController_->GetViewProjection());

		break;
#pragma endregion;

#pragma region デス演出フェーズの処理
	case GameScene::Phase::kDeath:

		//天球描画
		skydome_->Draw(cameraController_->GetViewProjection());

		//エネミーの描画
		for (Enemy* enemy : enemies_) {
			enemy->Draw(cameraController_->GetViewProjection());
		}

		//デス演出用パーティクルの更新
		if (deathParticles_) {
			deathParticles_->Draw(cameraController_->GetViewProjection());
		}

		//ブロック描画
		mapChipBlocks_->Draw(cameraController_->GetViewProjection());

		break;
#pragma endregion;
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() {

#pragma region PlayerとEnemy

	//判定対象1,2の座標
	AABB aabb1, aabb2;

	//自キャラの座標
	aabb1 = player_->GetAABB();

	//自キャラと敵のすべての当たり判定
	for (Enemy* enemy : enemies_) {
		//敵の座標
		aabb2 = enemy->GetAABB();

		//AABB同士の交差判定
		if (AABB::IsCollision(aabb1, aabb2)) {
			
			//自キャラの衝突コールバックを呼び出す
			player_->OnCollision(enemy);

			//エネミーの衝突コールバック
			enemy->OnCollision(player_);
		}
	}

#pragma endregion

#pragma region PlayerとItem

#pragma endregion

#pragma region PlayerBulletとEnemy

#pragma endregion

}

void GameScene::ChangePhase() {

	switch (phase_) {
	case GameScene::Phase::kPlay:

		if (player_->GetIsAlive() == false) {
			
			//デス演出フェーズに切り替える
			phase_ = Phase::kDeath;
			//自キャラの座標を取得する
			const Vector3& deathParticlesPosition = player_->GetWorldPos();

			deathParticles_->Initialize(deathParticlesPosition);

			
		}

		break;
	case GameScene::Phase::kDeath:



		break;
	}
}
