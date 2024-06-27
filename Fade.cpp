#define NOMINMAX
#include "Fade.h"
#include "TextureManager.h"
#include "WinApp.h"
#include <algorithm>

void Fade::Initialize() {

	//position初期化
	pos_ = { 0,0 };
	//テクスチャ初期化
	textureHandle_ = TextureManager::Load("white1x1.png");

	sprite_ = Sprite::Create(textureHandle_, pos_);
	sprite_->SetSize({ WinApp::kWindowWidth, WinApp::kWindowHeight });
	sprite_->SetColor(Vector4(0, 0, 0, 1));
}

void Fade::Update() {

	//フェード状態による分岐
	switch (status_) {
	case Fade::Status::None:
		//何もしない
		break;
	case Fade::Status::FadeIn:
#pragma region フェードイン中の更新処理

		//1フレーム分の秒数をカウントアップ
		counter_ += 1.0f / 60.0f;

		//フェード持続時間に達したら打ち止め
		counter_ = std::min(duration_, counter_);

		//0.0fから1.0fの間で、経過時間がフェード持続時間に近づくほどアルファ値を大きくする。
		sprite_->SetColor(Vector4(0, 0, 0, std::clamp(1 - counter_ / duration_, 0.0f, 1.0f)));


#pragma endregion
		break;
	case Fade::Status::FadeOut:
#pragma region フェードアウト中の更新処理

		//1フレーム分の秒数をカウントアップ
		counter_ += 1.0f / 60.0f;

		//フェード持続時間に達したら打ち止め
		counter_ = std::min(duration_, counter_);

		//0.0fから1.0fの間で、経過時間がフェード持続時間に近づくほどアルファ値を大きくする。
		sprite_->SetColor(Vector4(0, 0, 0, std::clamp(counter_ / duration_, 0.0f, 1.0f)));

#pragma endregion
		break;
	}
}

void Fade::Draw() {

	if (status_ == Status::None) {
		return;
	}

#pragma region 前景スプライト描画

	/// ここに前景スプライトの描画処理を追加できる
	sprite_->Draw();


#pragma endregion

}

void Fade::Start(Status status, float duration) {
	status_ = status;
	duration_ = duration;
	counter_ = 0.0f;
}

void Fade::Stop() {

	status_ = Status::None;
}

bool Fade::IsFinished() const {
	
	//フェード状態による分岐
	switch (status_) {
	case Fade::Status::FadeIn:
	case Fade::Status::FadeOut:
		return (counter_ >= duration_);
	}

	return true;
}
