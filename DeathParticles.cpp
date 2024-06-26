#include "DeathParticles.h"
#include "math/MyMath/MatrixMath.h"

#include <algorithm>

void DeathParticles::Initialize(Vector3 position) {

	//モデル初期化
	for (size_t i = 0; i < models_.size(); ++i) {
		models_[i] = Model::CreateSphere();
	};

	//ワールドトランスフォームの初期化
	for (size_t i = 0; i < worldTransforms_.size(); ++i) {
		worldTransforms_[i].Initialize();
		worldTransforms_[i].translation_ = position;
	}

	//カラー初期化
	objectColor_.Initialize();
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	
}

void DeathParticles::Update() {

	//終了なら何もしない
	if (isFinished_ == true) {
		return;
	}

	for (size_t i = 0; i < worldTransforms_.size(); ++i) {

#pragma region 移動処理
		//基本となる速度ベクトル
		Vector3 velocity = { kSpeed, 0, 0 };
		//回転角の計算
		float angle = kAngleUnit * i;
		//Z軸まわり回転行列の更新
		Matrix4x4 matrixRotation = MatrixMath::MakeRotateZMatrix(angle);
		//基本ベクトルを回転させて速度ベクトルを得る
		velocity = MatrixMath::Transform(velocity, matrixRotation);
		//移動処理
		worldTransforms_[i].translation_ += velocity;
#pragma endregion
		
#pragma region 一定時間で消す処理

		//カウンターを1フレーム分の秒数進める
		counter_ += 1.0f / 60.0f;

		//存続時間の上限に達した時
		if (counter_ >= kDuration) {
			counter_ = kDuration;
			//終了扱いにする
			isFinished_ = true;
		}

#pragma endregion

#pragma region フェードアウト処理

		color_.w = std::clamp(1.0f - counter_ / kDuration, 0.0f, 1.0f);
		objectColor_.SetColor(color_);
		objectColor_.TransferMatrix();
#pragma endregion

#pragma region ワールド行列の更新処理
		worldTransforms_[i].UpdateMatrix();
#pragma endregion
	}
}

void DeathParticles::Draw(const ViewProjection& viewProjection) {

	// サイズ不一致時の処理
	if (models_.size() != worldTransforms_.size()) {	
		return;
	}

	//終了なら何もしない
	if (isFinished_ == true) {
		return;
	}
	
	for (size_t i = 0; i < models_.size(); ++i) {
		models_[i]->Draw(worldTransforms_[i], viewProjection, &objectColor_);
	}
}
