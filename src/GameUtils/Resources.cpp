#pragma once
#include "Resources.h"

namespace {
	auto resources_start = Start::Create([]() {
		RefResources::TankBodys = (List<Image^>^)System::Array::CreateInstance(Image::typeid, 2);
		RefResources::TankBodys[0] = Image::FromFile("./resources/TankBody1.png");
		RefResources::TankBodys[1] = Image::FromFile("./resources/TankBody2.png");

		RefResources::ForestBackground = (List<Image^>^)System::Array::CreateInstance(Image::typeid, 1);
		RefResources::ForestBackground[0] = Image::FromFile("./resources/bg/forest/0.png");

		RefResources::NormalMonster  = Image::FromFile("./resources/monster/monster2.png");
		RefResources::ShooterMonster = Image::FromFile("./resources/monster/monster1.png");
		});
}