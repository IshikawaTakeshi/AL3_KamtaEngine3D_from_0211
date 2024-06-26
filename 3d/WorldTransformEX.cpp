#include "WorldTransform.h"
#include "math/MyMath/MatrixMath.h"

void WorldTransform::UpdateMatrix() {

	//アフィン行列の更新
	matWorld_ = MatrixMath::MakeAffineMatrix(scale_, rotation_, translation_);
	//行列を定数バッファに転送
	TransferMatrix();
}
