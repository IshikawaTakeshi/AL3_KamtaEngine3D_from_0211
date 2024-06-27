#include "TitleScene.h"
#include "input/Input.h"
#include "MyMath/MatrixMath.h"

//TitleScene::TitleScene() {}

TitleScene::~TitleScene() {

}

void TitleScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();

	textWorldTransform_.Initialize();
	playerWorldTransform_.Initialize();
	enemyWorldTransform_.Initialize();
	ViewProjection_.Initialize();

	textWorldTransform_.translation_.x -= 25.0f;
	playerWorldTransform_.translation_.x = 7.0f;
	enemyWorldTransform_.translation_.x = 14.0f;

	titleTextModel_ = Model::CreateFromOBJ("titleText",true);
	playerModel_ = Model::CreateFromOBJ("Player02_03", true);
	enemyModel_ = Model::CreateFromOBJ("enemyPieroFace", true);

	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, fadeTime_);
}

void TitleScene::Update() {

	textWorldTransform_.UpdateMatrix();
	playerWorldTransform_.UpdateMatrix();
	enemyWorldTransform_.UpdateMatrix();
	ViewProjection_.UpdateMatrix();

	fade_->Update();

	//スペースキーを押すと終了する
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}
}

void TitleScene::Draw() {

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

	//タイトルテキストの描画
	titleTextModel_->Draw(textWorldTransform_, ViewProjection_);
	//プレイヤーのモデルの描画
	playerModel_->Draw(playerWorldTransform_, ViewProjection_);
	enemyModel_->Draw(enemyWorldTransform_, ViewProjection_);
	
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// ここに前景スプライトの描画処理を追加できる
	fade_->Draw();


	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion


}

void TitleScene::ChangePhase() {
	switch (phase_) {
	case TitleScene::Phase::kFadeIn:

		break;
	case TitleScene::Phase::kMain:
		break;
	case TitleScene::Phase::kFadeOut:
		break;
	}
}
