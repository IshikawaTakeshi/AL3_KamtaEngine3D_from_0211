#include "CameraController.h"
#include "Player.h"
#include "MyMath/MyMath.h"
#include "MyMath/Easing.h"
#include <algorithm>
#include <numbers>
#include <cassert>

void CameraController::Initialize() {
	viewProjection_ = new ViewProjection();
	viewProjection_->Initialize();
}

void CameraController::Update() {

	//追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	
	//追従対象とオフセットと追従対象の速度からカメラの目標座標を計算
	targetPos_ = MyMath::Add(MyMath::Add(
		targetWorldTransform.translation_, targetOffset_), MyMath::Multiply(kVelocityBias, target_->GetVelocity()));

	
	//座標補間によりゆったり追従
	viewProjection_->translation_ = Easing::Liner(
		viewProjection_->translation_, targetPos_, kInterpolationRate);

	//追従対象が画面外に出ないように補正
	viewProjection_->translation_.x = std::clamp(
		viewProjection_->translation_.x,
		targetPos_.x + movableAreaMargin_.left,
		targetPos_.x + movableAreaMargin_.right
	);

	viewProjection_->translation_.y = std::clamp(
		viewProjection_->translation_.y,
		targetPos_.y + movableAreaMargin_.bottom,
		targetPos_.y + movableAreaMargin_.top
	);
	
	//移動範囲制限
	viewProjection_->translation_.x = std::clamp(
		viewProjection_->translation_.x,
		movableArea_.left,
		movableArea_.right
	);

	viewProjection_->translation_.y = std::clamp(
		viewProjection_->translation_.y,
		movableArea_.bottom,
		movableArea_.top
	);

	//行列の更新
	viewProjection_->UpdateMatrix();
}

void CameraController::Reset() {

	//追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	//追従対象とオフセットからカメラの座標を計算
	viewProjection_->translation_ = MyMath::Add(targetWorldTransform.translation_, targetOffset_);

}
