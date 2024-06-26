#define NOMINMAX
#include "Player.h"
#include "Math/MyMath/MyMath.h"
#include "Math/MyMath/Easing.h"
#include "MapChipField.h"
#include "DebugText.h"
#include <algorithm>
#include <numbers>
#include <cassert>


void Player::Initialize() {
	//worldTransform初期化
	worldTransform_.Initialize();
	//初期回転
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	//モデル初期化
	model_ = Model::CreateFromOBJ("Player02_03", true);
}



void Player::Update() {

	Move();

#pragma region 2.衝突判定

	//移動量に速度の値をコピー
	collisionMapInfo.move = velocity_;

	//マップ衝突チェック
	
	IsCollisionTop(collisionMapInfo);
	IsCollisionBottom(collisionMapInfo);
	IsCollisionLeft(collisionMapInfo);
	IsCollisionRight(collisionMapInfo);

#pragma endregion

#pragma region 3.判定結果を反映した移動

	//3.判定結果を反映した移動
	MovementByCollision(collisionMapInfo);

	//4.天井に接触している場合の処理
	ProcessWhenTouchCeiling(collisionMapInfo);

	//5.壁に接触している場合の処理
	ProcessWhenTouchWall(collisionMapInfo);

#pragma endregion

	//6.接地状態切り替え
	SwitchGroundState(collisionMapInfo);

#pragma region 7.旋回制御
	//======================== 旋回制御 =======================//

	if (turnTimer_ > 0.0f) {

		//タイマーのカウントダウン
		turnTimer_ -= 1.0f / 60.0f;

		//左右の自キャラ角度テーブル
		float destinationRotationYTable[] = {
			std::numbers::pi_v<float> / 2.0f,
			std::numbers::pi_v<float> *3.0f / 2.0f
		};
		//状態に応じた角度を取得する
		float destinationRotationY =
			destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		//自キャラの角度を設定する
		worldTransform_.rotation_.y = Easing::Liner(
			destinationRotationY,
			turnFirstRotationY_,
			Easing::EaseInOut(turnTimer_)
		);
	}
#pragma endregion

#pragma region 8.行列の更新
	worldTransform_.UpdateMatrix();
#pragma endregion
}

void Player::Draw(const ViewProjection& viewProjection) {

	//モデル描画
	model_->Draw(worldTransform_, viewProjection);
}

void Player::Move() {
#pragma region 移動入力
	//接地状態
	if (onGround_) {

		//======================= 左右移動操作 ========================//
		if (Input::GetInstance()->PushKey(DIK_A) ||
			Input::GetInstance()->PushKey(DIK_D) ||
			Input::GetInstance()->PushKey(DIK_LEFT) ||
			Input::GetInstance()->PushKey(DIK_RIGHT)) {

			if (Input::GetInstance()->PushKey(DIK_D) ||
				Input::GetInstance()->PushKey(DIK_RIGHT)) {

				//左移動中の右入力
				if (velocity_.x < 0.0f) {
					//ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}

				//加速
				acceleration_.x += kAcceleration;

				//左右状態切り替え
				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = 0.7f;
				}

			} else if (Input::GetInstance()->PushKey(DIK_A) ||
				Input::GetInstance()->PushKey(DIK_LEFT)) {

				//右移動中の左入力
				if (velocity_.x > 0.0f) {
					//ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}

				//加速
				acceleration_.x -= kAcceleration;

				//左右状態切り替え
				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = 0.7f;
				}
			}
			//加速・減速
			velocity_ = MyMath::Add(velocity_, acceleration_);

			//最大速度制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);

			if (acceleration_.x >= 0.01f || acceleration_.x <= -0.01f) {
				acceleration_.x = 0;
			}



		} else {
			//非入力時に移動減衰をかける
			velocity_.x *= (1.0f - kAttenuation);
		}

		//======================= ジャンプ入力 ====================//
		if (Input::GetInstance()->PushKey(DIK_UP) ||
			Input::GetInstance()->PushKey(DIK_SPACE)) {
			//ジャンプ初速
			velocity_.y += kJumpAcceleration;

		}

		//空中にいる場合
	} else {
		//======================= 落下速度計算 ====================//

		//落下速度
		velocity_.y += -kGravityAcceleration;
		//落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);

	}
#pragma endregion
}

void Player::IsCollisionTop(CollisionMapInfo& info) {

	//上昇しているかどうか
	if (info.move.y <= 0) {
		return;
	}

	//移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); i++) {
		positionsNew[i] = CornerPosition(
			worldTransform_.translation_ + info.move,
			static_cast<Corner>(i));
	}

	MapChipType mapchipType;
	//真上の当たり判定を行う
	bool hit = false;
	//左上の角の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapchipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapchipType == MapChipType::kBlock) {
		hit = true;
	}

	//右上の角の当たり判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapchipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapchipType == MapChipType::kBlock) {
		hit = true;
	}

	//ブロックに衝突した場合
	if (hit == true) {

		// 上端中央の座標を計算
		Vector3 topCenterPosition;
		topCenterPosition.x = (positionsNew[kLeftTop].x + positionsNew[kRightTop].x) / 2;
		topCenterPosition.y = positionsNew[kLeftTop].y; // 左右で高さは同じ
		topCenterPosition.z = (positionsNew[kLeftTop].z + positionsNew[kRightTop].z) / 2;

		//移動量を調整してめり込みを防止
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(topCenterPosition);

		//めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - kHeight / 2 + kBlank);
		//天井に当たったことを記録する
		info.touchCeiling = true;
	}
}

void Player::IsCollisionBottom(CollisionMapInfo& info) {

	//上昇しているかどうか
	if (info.move.y >= 0) {
		return;
	}

	//移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); i++) {
		positionsNew[i] = CornerPosition(
			worldTransform_.translation_ + info.move,
			static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	//真下の当たり判定を行う
	bool hit = false;
	//左下の角の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom] + Vector3(0, -kBottomBlank, 0));
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	//右下の角の当たり判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom] + Vector3(0, -kBottomBlank, 0));
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	//ブロックに衝突した場合
	if (hit == true) {

		// 下端中央の座標を計算
		Vector3 bottomCenterPosition;
		bottomCenterPosition.x = (positionsNew[kLeftBottom].x + positionsNew[kRightBottom].x) / 2;
		bottomCenterPosition.y = positionsNew[kLeftBottom].y; // 左右で高さは同じ
		bottomCenterPosition.z = (positionsNew[kLeftBottom].z + positionsNew[kRightBottom].z) / 2;

		//移動量を調整してめり込みを防止
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(bottomCenterPosition);


		//めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.y = std::min(0.0f, rect.top - worldTransform_.translation_.y + kHeight / 2 + kBlank);
		//地面に当たったことを記録する
		info.landing = true;
	}
}

void Player::IsCollisionRight(CollisionMapInfo& info) {

	//右に移動しているかどうか
	if (info.move.x <= 0) {
		return;
	}

	//移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); i++) {
		positionsNew[i] = CornerPosition(
			worldTransform_.translation_ + info.move,
			static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	//当たり判定
	bool hit = false;
	//右上の角の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	//右下の角の当たり判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	//ブロックに衝突した場合
	if (hit == true) {

		// 右端中央の座標を計算
		Vector3 rightCenterPosition;
		rightCenterPosition.x = positionsNew[kRightTop].x; //上下で座標は同じ
		rightCenterPosition.y = (positionsNew[kRightTop].y + positionsNew[kRightBottom].y) / 2;
		rightCenterPosition.z = (positionsNew[kRightTop].z + positionsNew[kRightBottom].z) / 2;

		//移動量を調整してめり込みを防止
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(rightCenterPosition);

		//めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.x = std::max(0.0f, rect.left - worldTransform_.translation_.x - kWidth / 2 + kBlank);
		//地面に当たったことを記録する
		info.touchWall = true;
	}
}

void Player::IsCollisionLeft(CollisionMapInfo& info) {

	//左に移動しているかどうか
	if (info.move.x >= 0) {
		return;
	}

	//移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); i++) {
		positionsNew[i] = CornerPosition(
			worldTransform_.translation_ + info.move,
			static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	//当たり判定
	bool hit = false;
	//左上の角の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	//左下の角の当たり判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	//ブロックに衝突した場合
	if (hit == true) {

		// 左端中央の座標を計算
		Vector3 leftCenterPosition;
		leftCenterPosition.x = positionsNew[kLeftTop].x; //上下で座標は同じ
		leftCenterPosition.y = (positionsNew[kLeftTop].y + positionsNew[kLeftBottom].y) / 2;
		leftCenterPosition.z = (positionsNew[kLeftTop].z + positionsNew[kLeftBottom].z) / 2;

		//移動量を調整してめり込みを防止
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(leftCenterPosition);

		//めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.x = std::min(0.0f, rect.right - worldTransform_.translation_.x + kWidth / 2 + kBlank);
		//地面に当たったことを記録する
		info.touchWall = true;
	}
}


void Player::MovementByCollision(const CollisionMapInfo& info) {
	worldTransform_.translation_ = worldTransform_.translation_ + info.move;
}

void Player::ProcessWhenTouchCeiling(CollisionMapInfo& info) {

	//天井に接触している場合
	if (info.touchCeiling == true) {
		DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
		velocity_.y = 0;
		info.touchCeiling = false;
	}
}

void Player::ProcessWhenTouchWall(CollisionMapInfo& info) {

	//壁に接触している場合
	if (info.touchWall == true) {
		DebugText::GetInstance()->ConsolePrintf("hit Wall\n");
		velocity_.x *= (1.0f - kAttenuationWall);
		info.touchWall = false;
	}
}

void Player::SwitchGroundState(CollisionMapInfo& info) {
	if (onGround_ == true) {

		//ジャンプ開始
		if (velocity_.y >= 0.0f) {
			//空中状態に切り替える
			onGround_ = false;
		} else {

			//移動後の4つの角の座標
			std::array<Vector3, kNumCorner> positionsNew;

			for (uint32_t i = 0; i < positionsNew.size(); i++) {
				positionsNew[i] = CornerPosition(
					MyMath::Add(worldTransform_.translation_, info.move),
					static_cast<Corner>(i));
			}

			MapChipType mapChipType;
			//真下の当たり判定をとる
			bool hit = false;

			//左下の角の判定
			MapChipField::IndexSet indexSet;
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(MyMath::Add(positionsNew[kLeftBottom], Vector3(0, -kBlank, 0)));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}
			//右下の角の判定
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(MyMath::Add(positionsNew[kRightBottom], Vector3(0, -kBlank, 0)));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}

			//落下開始
			if (!hit) {
				//空中状態へ切り替える
				onGround_ = false;
			}
		}

	} else {

		if (info.landing == true) {

			//着地状態に切り替える
			onGround_ = true;

			//着地時にX速度を減衰
			velocity_.x *= (1.0f - kAttenuationLanding);
			//Y速度を0にする
			velocity_.y = 0.0f;
			info.landing = false;
		}
	}
}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {

	Vector3 offsetTable[kNumCorner] = {
		{ +kWidth / 2.0f, -kHeight / 2.0f, 0 },
		{ -kWidth / 2.0f, -kHeight / 2.0f, 0 },
		{ +kWidth / 2.0f, +kHeight / 2.0f, 0 },
		{ -kWidth / 2.0f, +kHeight / 2.0f, 0 }
	};

	return MyMath::Add(center, offsetTable[static_cast<uint32_t>(corner)]);
}

void Player::OnCollision(const Enemy* enemy) {
	(void)enemy;

	//ジャンプさせる
	velocity_.y += 1.0f;
}

AABB Player::GetAABB() {
	
	Vector3 worldPos = GetWorldPos();

	AABB aabb;
	aabb.min = { worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f };
	aabb.max = { worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f };

	return aabb;
}
