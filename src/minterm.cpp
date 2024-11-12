#include "App.h"

namespace {
	auto midterm_start = 
	Start::Create([]() {
		cout << "init\n";
	});

	auto midterm_update = 
	Update::Create([]() {

	});
	
	auto midterm_render = 
	Render::Create([]() {

	});
}