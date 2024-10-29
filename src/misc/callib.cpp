
#include "callib.h"

namespace {
	int   buf_int[2];
	float buf_float[2];
}

namespace callib {
	float FLT_PRECISE = DEFAULT_FLT_PRECISE;

	namespace style {
		float labelWidth		= 120.0f;
		Color color_mainText	(0.9f, 0.9f, 0.9f);
		Color color_subText		(0.7f, 0.7f, 0.7f);
	}

#pragma region string
	vector<string> Split(const string& src, char det)
	{
		vector<string> result;
		stringstream ss(src);
		string token;
		while (getline(ss, token, ' '))
			result.push_back(token);
		return result;
	}
#pragma endregion

#pragma region TimeWatch
	int TimeWatch::Precision = 3;

	std::ostream& operator<<(std::ostream& os, TimeWatch& obj)
	{
		os << "--- TimeWatch ";
		if (obj.unit == TimeWatch::SEC)
			os << "sec   ";
		else if (obj.unit == TimeWatch::MILLI_SEC)
			os << "milli ";
		else if (obj.unit == TimeWatch::MICRO_SEC)
			os << "micro ";
		else if (obj.unit == TimeWatch::NANO_SEC)
			os << "nano  ";
		os << "---\n";
		int maxWidth = 0;
		for (auto pair : obj.timeMap)
			if (pair.first.size() > maxWidth)maxWidth = (int)pair.first.size();
		os << std::fixed << std::setprecision(TimeWatch::Precision);
		for (auto pair : obj.timeMap) {
			os << pair.first;
			for (int i = maxWidth - (int)pair.first.size(); i > 0; i--)
				os << ' ';
			os << ": " << pair.second << '\n';
		}
		os << "Total: " << obj.Total();
		return os;
	}

	TimeWatch::TimeWatch()
	{
	}

	TimeWatch::~TimeWatch()
	{
	}

	void TimeWatch::Start(std::string id)
	{
		begTimeMap[id] = std::chrono::system_clock::now();
	}

	double TimeWatch::Stop(std::string id)
	{
		auto d = std::chrono::system_clock::now() - begTimeMap[id];
		if (!timeMap.count(id))
			timeMap[id] = 0;
		if (unit == SEC)
			timeMap[id] += std::chrono::duration<double, std::ratio<1, 1>>(d).count();
		else if (unit == MILLI_SEC)
			timeMap[id] += std::chrono::duration<double, std::milli>(d).count();
		else if (unit == MICRO_SEC)
			timeMap[id] += std::chrono::duration<double, std::micro>(d).count();
		else if (unit == NANO_SEC)
			timeMap[id] += std::chrono::duration<double, std::nano>(d).count();
		return timeMap[id];
	}

	void TimeWatch::Reset(std::string id)
	{
		timeMap[id] = 0;
	}

	void TimeWatch::Restart(std::string id)
	{
		Reset(id);
		Start(id);
	}

	void TimeWatch::Close(std::string id)
	{
		timeMap.erase(id);
		begTimeMap.erase(id);
	}

	double TimeWatch::Total()
	{
		double sum = 0;
		for (auto& pair : timeMap)
			sum += pair.second;
		return sum;
	}

	void TimeWatch::SetUnit(Unit unit)
	{
		double offset = unit / (double)this->unit;
		for (auto& pair : timeMap)
			pair.second *= offset;
		this->unit = unit;
	}
#pragma endregion

#pragma region Color
	Color::Color()
		:r(1.0), g(1.0), b(1.0), a(1.0) {}

	Color::Color(float r, float g, float b)
		:r(r), g(g), b(b), a(1.0) {}

	Color::Color(float r, float g, float b, float a)
		:r(r), g(g), b(b), a(a) {}

	Color::~Color() {}

	System::Drawing::Color Color::ToDrawingColor()
	{
		return System::Drawing::Color::FromArgb(a * 255, r * 255, g * 255, b * 255);
	}

	Color Color::FromHSV(float h, float s, float v, float a)
	{
		h = std::fmod(h, 360.0f);
		if (h < 0) h += 360.0f;  // 保證 h 在 0-360 的範圍內

		float c = v * s; // 色度
		float x = c * (1.0f - std::abs(std::fmod(h / 60.0f, 2) - 1.0f)); // 中間值
		float m = v - c; // 亮度的偏移量

		float r, g, b;

		if (h >= 0 && h < 60) {
			r = c;
			g = x;
			b = 0;
		}
		else if (h >= 60 && h < 120) {
			r = x;
			g = c;
			b = 0;
		}
		else if (h >= 120 && h < 180) {
			r = 0;
			g = c;
			b = x;
		}
		else if (h >= 180 && h < 240) {
			r = 0;
			g = x;
			b = c;
		}
		else if (h >= 240 && h < 300) {
			r = x;
			g = 0;
			b = c;
		}
		else {
			r = c;
			g = 0;
			b = x;
		}

		// 調整最小值並確保 RGB 值在 0-1 的範圍
		r = (r + m);
		g = (g + m);
		b = (b + m);

		return Color(r, g, b, a);
	}

#ifdef IMGUI_API
	Color::Color(const ImVec4& imVec4)
		:r(imVec4.x), g(imVec4.y), b(imVec4.z), a(imVec4.w) {}
	Color::Color(const ImColor& imColor)
		:r(imColor.Value.x), g(imColor.Value.y), b(imColor.Value.z), a(imColor.Value.w) {}
	Color::Color(const ImU32& imU32)
		: Color((ImColor)imU32) {}
	Color::operator ImVec4() const { return ImVec4(r, g, b, a); }
	Color::operator ImColor() const { return ImColor(r, g, b, a); }
	Color::operator ImU32() const { return (*this).operator ImColor(); }
#endif
#pragma endregion

#ifdef IMGUI_API
	namespace ImEx {
		using namespace std;
		using namespace ImGui;
		using namespace callib;

		void SubText(const char* text) {
			TextColored(style::color_subText, text);
		}

		void BeginFrontLabel(const char* label, const ImVec4& color) {
			TextColored(color, label);
			SameLine(style::labelWidth - GetCursorPosX());
			PushItemWidth(-1);
		}
		void EndFrontLabel() {
			PopItemWidth();
		}

		template <>
		void OnGui(const char* label, int& obj) {
			BeginFrontLabel(label, style::color_mainText);
			DragInt(label, &obj);
			EndFrontLabel();
		}
		template <>
		void OnGui(const char* label, float& obj) {
			BeginFrontLabel(label, style::color_mainText);
			DragFloat(label, &obj, obj * 0.01f);
			EndFrontLabel();
		}
		template <>
		void OnGui(const char* label, Vector2& obj) {
			BeginFrontLabel(label, style::color_mainText);
			buf_float[0] = obj.x;
			buf_float[1] = obj.y;
			DragFloat2(label, buf_float);
			obj.x = buf_float[0];
			obj.y = buf_float[1];
			EndFrontLabel();
		}
		template <>
		void OnGui(const char* label, Vector2Int& obj) {
			BeginFrontLabel(label, style::color_mainText);
			buf_int[0] = obj.x;
			buf_int[1] = obj.y;
			DragInt2(label, buf_int);
			obj.x = buf_int[0];
			obj.y = buf_int[1];
			EndFrontLabel();
		}
		template <>
		void OnGui(const char* label, Triangle2D& obj) {
			OnGui("P1", obj.p1);
			OnGui("P2", obj.p2);
			OnGui("P3", obj.p3);
		}
		template <>
		void OnGui(const char* label, Rect& obj) {
			auto position = obj.get_position();
			auto size = obj.get_size();
			OnGui("position", position);
			OnGui("size", size);
			obj.set_position(position);
			obj.set_size(size);
		}
		template <>
		void OnGui(const char* label, ImWindowDrawer2D& obj) {
			ImEx::SubText("Camera");
			OnGui("Position", obj.coordOffset);
			OnGui("Scale"   , obj.coordScale);
			if (obj.coordScale < FLT_PRECISE)
				obj.coordScale = FLT_PRECISE;
		}
	}

#pragma region ImWindowDrawer2D
	ImWindowDrawer2D::ImWindowDrawer2D() {};

	void ImWindowDrawer2D::AssignCurrentWindow() {
		drawList	   = ImGui::GetWindowDrawList();
		windowSize	   = ImGui::GetWindowSize();
		worldViewBound = ScreenToWorldPosition(Rect({ 0,0 }, windowSize));
	}

	Vector2 ImWindowDrawer2D::WorldToScreenPosition(const Vector2& position) const {
		Vector2 ret = position;
		ret *= coordScale;
		ret += coordOffset;
		return ret;
	}
	Vector2 ImWindowDrawer2D::ScreenToWorldPosition(const Vector2& position) const {
		Vector2 ret = position;
		ret -= coordOffset;
		ret /= coordScale;
		return ret;
	}

	Line2D ImWindowDrawer2D::WorldToScreenPosition(const Line2D& position) const {
		return {
			WorldToScreenPosition(position.p1),
			WorldToScreenPosition(position.p2),
		};
	}
	Line2D ImWindowDrawer2D::ScreenToWorldPosition(const Line2D& position) const {
		return {
			ScreenToWorldPosition(position.p1),
			ScreenToWorldPosition(position.p2),
		};
	}

	Triangle2D ImWindowDrawer2D::WorldToScreenPosition(const Triangle2D& position) const {
		return {
			WorldToScreenPosition(position.p1),
			WorldToScreenPosition(position.p2),
			WorldToScreenPosition(position.p3),
		};
	}
	Triangle2D ImWindowDrawer2D::ScreenToWorldPosition(const Triangle2D& position) const {
		return {
			ScreenToWorldPosition(position.p1),
			ScreenToWorldPosition(position.p2),
			ScreenToWorldPosition(position.p3),
		};
	}

	Rect ImWindowDrawer2D::WorldToScreenPosition(const Rect& position) const {
		return {
			WorldToScreenPosition(position.get_position()),
			position.get_size() * coordScale,
		};
	}
	Rect ImWindowDrawer2D::ScreenToWorldPosition(const Rect& position) const {
		return {
			ScreenToWorldPosition(position.get_position()),
			position.get_size() / coordScale,
		};
	}

	void ImWindowDrawer2D::PrintText(const Vector2& position, const string& text, const Color& color, const Color& bgColor) {
		Vector2 textCenter = WorldToScreenPosition(position);
		Rect textBound = { {0,0}, ImGui::CalcTextSize(text.c_str()) };
		textBound.set_center(textCenter);
		if (Graph2D::is_collide(textBound, Rect({ 0,0 }, windowSize))) {
			Rect fillRect = textBound;
			fillRect.x -= 4;
			fillRect.y -= 4;
			fillRect.width += 8;
			fillRect.height += 8;
			drawList->AddRectFilled(fillRect.get_min(), fillRect.get_max(), bgColor, fillRect.height * 0.2f);
			drawList->AddText(textBound.get_min(), color, text.c_str());
		}
	}

	void ImWindowDrawer2D::DrawPoint(const Vector2& obj, const Color& color, float radius) {
		if (Graph2D::is_collide(obj, worldViewBound)) {
			drawList->AddCircleFilled(WorldToScreenPosition(obj), radius, color);
		}
	}

	void ImWindowDrawer2D::DrawLine(const Line2D& obj, const Color& color, float thickness) {
		auto solvedObj = WorldToScreenPosition(obj);
		drawList->AddLine(solvedObj.p1, solvedObj.p2, color, thickness);
	}

	void ImWindowDrawer2D::DrawRect(const Rect& obj, const Color& color, float thickness) {
		if (Graph2D::is_collide(obj, worldViewBound)) {
			auto solvedObj = WorldToScreenPosition(obj);
			drawList->AddRect(solvedObj.get_min(), solvedObj.get_max(), color, thickness);
		}
	}
	void ImWindowDrawer2D::DrawRectFilled(const Rect& obj, const Color& color) {
		if (Graph2D::is_collide(obj, worldViewBound)) {
			auto solvedObj = WorldToScreenPosition(obj);
			drawList->AddRectFilled(solvedObj.get_min(), solvedObj.get_max(), color);
		}
	}
	void  ImWindowDrawer2D::DrawTriangle(const Triangle2D& obj, const Color& color, float thickness) {
		if (Graph2D::is_collide(obj.get_boundingBox(), worldViewBound)) {
			auto solvedObj = WorldToScreenPosition(obj);
			drawList->AddTriangle(solvedObj.p1, solvedObj.p2, solvedObj.p3, color, thickness);
		}
	}
	void  ImWindowDrawer2D::DrawTriangleFilled(const Triangle2D& obj, const Color& color) {
		if (Graph2D::is_collide(obj.get_boundingBox(), worldViewBound)) {
			auto solvedObj = WorldToScreenPosition(obj);
			drawList->AddTriangleFilled(solvedObj.p1, solvedObj.p2, solvedObj.p3, color);
		}
	}
	void  ImWindowDrawer2D::DrawPoly(const Polygon2D& obj, const Color& color, float thickness) {
		int imax = obj.vertices.size();
		if (imax > 2) {
			Vector2 p1 = obj.vertices[imax - 1];
			Vector2 p2 = obj.vertices[0];
			for (int i = 1; i < imax; i++) {
				drawList->AddLine(WorldToScreenPosition(p1), WorldToScreenPosition(p2), color, thickness);
				p1 = p2;
				p2 = obj.vertices[i];
			}
			drawList->AddLine(WorldToScreenPosition(p1), WorldToScreenPosition(p2), color, thickness);
		}
	}
	void  ImWindowDrawer2D::DrawPolyFilled(const Polygon2D& obj, const Color& color) {
		for (auto& tri : TriangulationSolver2D(obj).solve())
			DrawTriangleFilled(tri, color);
	}

	const Vector2& ImWindowDrawer2D::get_windowSize() const {
		return windowSize;
	}
	const Rect& ImWindowDrawer2D::get_worldViewBound() const {
		return worldViewBound;
	}
#pragma endregion


#endif // IMGUI_API
}
