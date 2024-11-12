#pragma once
#include "Resources.h"

namespace {
	auto resources_start = Start::Create([]() {
		RefResources::TankBodys = (List<Image^>^)System::Array::CreateInstance(Image::typeid, 2);
		RefResources::TankBodys[0] = Image::FromFile("./resources/TankBody1.png");
		RefResources::TankBodys[1] = Image::FromFile("./resources/TankBody2.png");
		});
}