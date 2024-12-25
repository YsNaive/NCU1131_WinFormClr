#pragma once
#include "Core.h"

ref class RefResources {
public:
    static Image^ ResizeImage(Image^ sourceImage, int targetWidth, int targetHeight) {
        Drawing::Bitmap^ resizedBitmap = gcnew Drawing::Bitmap(targetWidth, targetHeight);

        Graphics^ graphics = Graphics::FromImage(resizedBitmap);
        graphics->InterpolationMode = System::Drawing::Drawing2D::InterpolationMode::NearestNeighbor;

        graphics->DrawImage(sourceImage, 0, 0, targetWidth, targetHeight);
        delete graphics;
        return resizedBitmap;
    }

	static List<Image^>^ TankBodys;
	static Image^ TankGun;
	static List<Image^>^ ForestBackground;

	static Image^ NormalMonster;
	static Image^ ShooterMonster;
};