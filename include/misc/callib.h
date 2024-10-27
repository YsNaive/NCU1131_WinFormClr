#pragma once
#define EXCLUDE_IMGUI
#ifndef EXCLUDE_IMGUI
#include "imgui.h";
#endif // !EXCLUDE_IMGUI


#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>
#include <limits>
#include <chrono>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <set>
#include <map>

#include "callib_config.h"

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif


namespace callib {
	using namespace std;

#pragma region forward decleartion

	class TimeWatch;
	class Color;

	template<class T>
	class T_Shape2D;
	template<class T>
	class T_Vector2;
	template<class T>
	class T_Line2D;
	template<class T>
	class T_Triangle2D;
	template<class T>
	class T_Rect;
	template<class T>
	class T_Path2D;
	template<class T>
	class T_Polygon2D;

#ifdef IMGUI_API
	namespace ImEx {
		void SubText(const char* text);

		void BeginFrontLabel(const char* label, const ImVec4& color = ImVec4(1, 1, 1, 1));
		void EndFrontLabel();

		template<class T>
		inline void OnGui(T& obj);
		template<class T>
		void OnGui(const string& label, T& obj);
	}

	class ImWindowDrawer2D;
#endif // IMGUI_API

#pragma endregion

#pragma region Global var
	extern CALLIB_FLT_BASE FLT_PRECISE;

	namespace style {
		extern float labelWidth;
		extern Color color_mainText;
		extern Color color_subText;
	}
#pragma endregion

#pragma region Math

#define float_equal(lhs, rhs) (((lhs-rhs) <= FLT_PRECISE) && ((lhs-rhs) >= -FLT_PRECISE))
#define float_abEqu(lhs, rhs) ((lhs+FLT_PRECISE) >= rhs)
#define float_beEqu(lhs, rhs) ((lhs-FLT_PRECISE) <= rhs)
#define float_above(lhs, rhs) ((lhs-FLT_PRECISE) >= rhs)
#define float_below(lhs, rhs) ((lhs+FLT_PRECISE) <= rhs)

	template<class T>
	constexpr T clamp(const T& min, const T& max, const T& value) {
		return
			(value < min) ? min :
			(value > max) ? max : value;
	}

	template<class T>
	inline constexpr T sgn(const T value) {
		return (value > 0) ? T(1) : T(-1);
	}

#pragma endregion

#pragma region string

	template<class... Args>
	string FormatString(const char* format, const Args&... args) {
		auto scale = snprintf(nullptr, 0, format, args...);
		auto buffer = new char[scale + 1];
		sprintf(buffer, format, args...);
		auto ret = string(buffer);
		delete[] buffer;
		return ret;
	}

	vector<string> Split(const string& src, char det);

#pragma endregion

	class TimeWatch {
	public:
		static int Precision;
		enum Unit {
			NANO_SEC = 1000000000,
			MICRO_SEC = 1000000,
			MILLI_SEC = 1000,
			SEC = 1,
		};
		TimeWatch();
		~TimeWatch();
		void Start(std::string id = "");
		double Stop(std::string id = "");
		void Reset(std::string id = "");
		void Restart(std::string id = "");
		void Close(std::string id = "");
		double Total();
		void SetUnit(Unit unit);
		friend std::ostream& operator<< (std::ostream& os, TimeWatch& obj);
		std::map<std::string, double> timeMap;
	private:
		Unit unit = SEC;
		std::map<std::string, std::chrono::system_clock::time_point> begTimeMap;
	};

	class Color
	{
	public:
		float r, g, b, a;

		Color();
		Color(float r, float g, float b);
		Color(float r, float g, float b, float a);
		~Color();

		System::Drawing::Color ToDrawingColor();

		static Color FromHSV(float h, float s, float v, float a = 1.0f);

#ifdef IMGUI_API
		Color(const ImVec4& imVec4);
		Color(const ImColor& imColor);
		Color(const ImU32& imU32);
		operator ImVec4() const;
		operator ImColor() const;
		operator ImU32() const;
#endif // __CALLIB_USE_IMGUI__
	};

	template<class T>
	class T_Shape2D {
		using Vector2 = T_Vector2<T>;
		using Rect = T_Rect<T>;
	public:
		virtual void add_offset(const Vector2& offset) = 0;
		virtual Rect get_boundingBox() const = 0;
	};

	template<class T>
	class T_Vector2 : public T_Shape2D<T> {
		using Rect = T_Rect<T>;
		using Vector2 = T_Vector2;
	public:
		T_Vector2() :x(T()), y(T()) {}
		T_Vector2(const T x, const T y) :x(x), y(y) {}

		T x, y;

		/* temp add in this project */
		static Vector2 FromDegree(float deg) {
			float rad = deg * 0.01745329251;
			return { (float)sin(rad), -(float)cos(rad) };
		}

		Vector2	get_abs() const {
			return { abs(x), abs(y) };
		}
		T    get_min() const {
			return min(x, y);
		}
		T    get_max() const {
			return max(x, y);
		}

		T	 get_length() const {
			return sqrt(x * x + y * y);
		}
		void set_length(T length) {
			auto len = get_length();
			if (!float_equal(len, 0))
				operator*= (length / len);
		}

		T dot(const Vector2& other) const {
			return (x * other.x) + (y * other.y);
		}
		T cross(const Vector2& other) const {
			return x * other.y - y * other.x;
		}
		const Vector2 lerp(const Vector2& other, CALLIB_FLT_BASE t) const {
			return (operator* ((1.0f - t))) + (other * t);
		}
		const Vector2 normalize() const {
			auto len = get_length();
			if (float_equal(len, T(0)))
				return { T(0),T(0) };
			T_Vector2 ret(*this);
			ret /= len;
			return ret;
		}

		void add_offset(const Vector2& offset) override {
			operator+= (offset);
		}
		Rect get_boundingBox() const override {
			return { *this, {0,0} };
		}

		string to_string() const {
			return (stringstream() << *this).str();
		}
		string to_string(int precise) const {
			return (stringstream() << std::fixed << std::setprecision(precise) << *this).str();
		}
		size_t get_hashCode() const {
			auto hasher = std::hash<T>();
			auto v1 = hasher(x);
			auto v2 = hasher(y);
			return v1 ^ (v2 << 1);
		}

		constexpr bool operator== (const Vector2& other) const {
			return
				float_equal(x, other.x) &&
				float_equal(y, other.y);
		}
		constexpr bool operator!= (const Vector2& other) const {
			return
				!float_equal(x, other.x) ||
				!float_equal(y, other.y);
		}
		// compare hashCode
		constexpr bool operator< (const Vector2& other) const {
			return get_hashCode() < other.get_hashCode();
		}

		constexpr Vector2 operator-() const {
			return { -x, -y };
		}
		void operator+= (const Vector2& other) {
			x += other.x;
			y += other.y;
		}
		void operator-= (const Vector2& other) {
			x -= other.x;
			y -= other.y;
		}
		void operator*= (const Vector2& other) {
			x *= other.x;
			y *= other.y;
		}
		void operator/= (const Vector2& other) {
			x /= other.x;
			y /= other.y;
		}
		constexpr Vector2 operator+ (const Vector2& other) const {
			auto ret = *this;
			ret += other;
			return ret;
		}
		constexpr Vector2 operator- (const Vector2& other) const {
			auto ret = *this;
			ret -= other;
			return ret;
		}
		constexpr Vector2 operator* (const Vector2& other) const {
			auto ret = *this;
			ret *= other;
			return ret;
		}
		constexpr Vector2 operator/ (const Vector2& other) const {
			auto ret = *this;
			ret /= other;
			return ret;
		}

		void operator*= (const T other) {
			x *= other;
			y *= other;
		}
		void operator/= (const T other) {
			x /= other;
			y /= other;
		}
		constexpr Vector2 operator* (const T other) const {
			auto ret = *this;
			ret *= other;
			return ret;
		}
		constexpr Vector2 operator/ (const T other) const {
			auto ret = *this;
			ret /= other;
			return ret;
		}

		friend ostream& operator<< (ostream& os, const Vector2& obj) {
			return os << '(' << obj.x << ", " << obj.y << ')';
		}

		class Hasher {
		public:
			size_t operator()(const T_Vector2& value) const {
				return value.get_hashCode();
			}
		};

		template<class T2>
		inline constexpr operator T_Vector2<T2>() const {
			return { T2(x), T2(y) };
		}

#ifdef IMGUI_API
		Vector2(const ImVec2& imVec2)
			: x(imVec2.x), y(imVec2.y) {}
		operator ImVec2() const {
			return { (float)x,(float)y };
		}
#endif // IMGUI_API
	};

	template<class T>
	class T_Line2D : public T_Shape2D<T> {
		using Vector2 = T_Vector2<T>;
		using Rect = T_Rect<T>;
		using Line = T_Line2D;
	public:
		T_Line2D() : p1(Vector2()), p2(Vector2()) {}
		T_Line2D(const Vector2& p1, const Vector2& p2) : p1(p1), p2(p2) {}
		T_Line2D(T x1, T y1, T x2, T y2) : p1({ x1, y1 }), p2({ x2, y2 }) {}

		Vector2 p1, p2;

		const bool is_vertical() const {
			return float_equal(p1.x, p2.x);
		}

		const CALLIB_FLT_BASE get_slope() const {
			if (is_vertical()) {
				return std::numeric_limits<CALLIB_FLT_BASE>::max();
			}
			return (p2.y - p1.y) / (p2.x - p1.x);
		}
		const T get_length() const {
			return (p1 - p2).get_length();
		}

		inline T get_xMin() const {
			return min(p1.x, p2.x);
		}
		inline T get_xMax() const {
			return max(p1.x, p2.x);
		}
		inline T get_yMin() const {
			return min(p1.y, p2.y);
		}
		inline T get_yMax() const {
			return max(p1.y, p2.y);
		}
		Rect get_boundingBox() const override
		{
			Rect ret;
			if (p1.x < p2.x) {
				ret.x = p1.x;
				ret.width = p2.x - p1.x;
			}
			else {
				ret.x = p2.x;
				ret.width = p1.x - p2.x;
			}
			if (p1.y < p2.y) {
				ret.y = p1.y;
				ret.height = p2.y - p1.y;
			}
			else {
				ret.y = p2.y;
				ret.height = p1.y - p2.y;
			}
			return ret;
		}
		void add_offset(const Vector2& offset) override {
			p1 += offset;
			p2 += offset;
		}

		string to_string() const {
			return (stringstream() << *this).str();
		}
		string to_string(int precise) const {
			return (stringstream() << std::fixed << std::setprecision(precise) << *this).str();
		}
		size_t get_hashCode() const {
			auto hasher = std::hash<T>();
			auto v1 = hasher(p1.x);
			auto v2 = hasher(p1.y);
			auto v3 = hasher(p2.x);
			auto v4 = hasher(p2.y);
			return v1 ^ (v2 << 1) ^ (v3 << 2) ^ (v4 << 3);
		}

		bool operator==(const Line& other) const {
			return
				(p1 == other.p1) &&
				(p2 == other.p2);
		}
		bool operator!=(const Line& other) const {
			return
				!(p1 == other.p1) ||
				!(p2 == other.p2);
		}

		friend ostream& operator<<(ostream& os, const Line& obj)
		{
			os << obj.p1 << "-" << obj.p2;
			return os;
		}

		class Hasher {
		public:
			size_t operator()(const Line& value) const {
				return value.get_hashCode();
			}
		};
	};

	template<class T>
	class T_Triangle2D : public T_Shape2D<T> {
		using Vector2 = T_Vector2<T>;
		using Rect = T_Rect<T>;
		using Line = T_Line2D<T>;
		using Triangle = T_Triangle2D;
	public:
		T_Triangle2D() : p1(Vector2()), p2(Vector2()), p3(Vector2()) {}
		T_Triangle2D(const Vector2& p1, const Vector2& p2, const Vector2& p3) : p1(p1), p2(p2), p3(p3) {}

		Vector2 p1, p2, p3;

		string to_string() const {
			return (stringstream() << *this).str();
		}
		string to_string(int precise) const {
			return (stringstream() << std::fixed << std::setprecision(precise) << *this).str();
		}
		size_t get_hashCode() const {
			auto v1 = p1.get_hashCode();
			auto v2 = p2.get_hashCode();
			auto v3 = p3.get_hashCode();
			return v1 ^ (v2 << 1) ^ (v3 << 2);
		}

		const T get_xMin() const
		{
			return min(min(p1.x, p2.x), p3.x);
		}
		const T get_xMax() const
		{
			return std::max(std::max(p1.x, p2.x), p3.x);
		}
		const T get_yMin() const
		{
			return min(min(p1.y, p2.y), p3.y);
		}
		const T get_yMax() const
		{
			return std::max(std::max(p1.y, p2.y), p3.y);
		}
		const T	get_area() const {
			return 0.5f * abs((p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y)));
		}

		const Vector2 get_center() const {
			return (p1 + p2 + p3) / 3.0;
		}
		void		  set_center(const Vector2& pos) {
			auto offset = pos - get_center();
			p1 += offset;
			p2 += offset;
			p3 += offset;
		}
		Rect get_boundingBox() const override {
			auto xmin = get_xMin(), ymin = get_yMin();
			return Rect(xmin, ymin, get_xMax() - xmin, get_yMax() - ymin);
		}
		void add_offset(const Vector2& offset) override {
			p1 += offset;
			p2 += offset;
			p3 += offset;
		}
		vector<Line> get_lines() {
			return {
				{p1,p2},
				{p2,p3},
				{p3,p1},
			};
		}

		friend ostream& operator<<(ostream& os, const Triangle& obj) {
			os << obj.p1 << obj.p2 << obj.p3;
			return os;
		}
	};

	template<class T>
	class T_Rect : public T_Shape2D<T> {
		using Vector2 = T_Vector2<T>;
		using Line = T_Line2D<T>;
		using Rect = T_Rect;
	public:
		T_Rect()
			: Rect(0, 0, 0, 0) {}
		T_Rect(T x, T y, T w, T h)
			: x(x), y(y), width(w), height(h) {}
		T_Rect(const Vector2& position, const Vector2& scale)
			: Rect(position.x, position.y, scale.x, scale.y) {}

		T x, y, width, height;

		string to_string() const {
			return (stringstream() << *this).str();
		}
		string to_string(int precise) const {
			return (stringstream() << std::fixed << std::setprecision(precise) << *this).str();
		}
		size_t get_hashCode() const {
			auto hasher = std::hash<T>();
			auto v1 = hasher(x);
			auto v2 = hasher(y);
			auto v3 = hasher(width);
			auto v4 = hasher(height);
			return v1 ^ (v2 << 1) ^ (v3 << 2) ^ (v4 << 3);
		}

		const Vector2 get_position() const {
			return Vector2(x, y);
		}
		void		  set_position(const Vector2& pos) {
			x = pos.x;
			y = pos.x;
		}
		const Vector2 get_size() const {
			return { width,height };
		}
		void		  set_size(const Vector2& scale) {
			width = scale.x;
			height = scale.y;
		}
		const Vector2 get_center() const {
			return { x + (width / (T)2), y + (height / (T)2) };
		}
		void		  set_center(const Vector2& pos) {
			x = pos.x - (width / (T)2);
			y = pos.y - (height / (T)2);
		}
		const T		  get_area() const {
			return width * height;
		}
		void		  set_area(T value) {
			auto wr = get_widthRatio();
			width = value;
			height = 1;
			set_widthRatio(wr);
		}
		const T		  get_widthRatio() const {
			if (float_equal(height, 0.0f))
				return 1.0f;
			return width / height;
		}
		void		  set_widthRatio(T value) {
			T a = get_area();
			width = sqrt(a * value);
			height = a / width;
		}
		const T		  get_heightRatio() const {
			if (float_equal(width, 0.0f))
				return 1.0f;
			return height / width;
		}
		void		  set_heightRatio(T value) {
			T a = get_area();
			height = sqrt(a * value);
			width = a / height;
		}
		const T		  get_perimeter() const {
			return width + width + height + height;
		}
		const T		  get_halfPerimeter() const {
			return width + height;
		}

		const T		  get_xMin() const {
			if (width < 0)
				return x + width;
			return x;
		}
		void		  set_xMin(T value) {
			width += x - value;
			x = value;
		}
		const T		  get_xMax() const {
			if (width > 0)
				return x + width;
			return x;
		}
		void		  set_xMax(T value) {
			width = value - x;
		}
		const T		  get_yMin() const {
			if (height < 0)
				return y + height;
			return y;
		}
		void		  set_yMin(T value) {
			height += y - value;
			y = value;
		}
		const T		  get_yMax() const {
			if (height > 0)
				return y + height;
			return y;
		}
		void		  set_yMax(T value) {
			height = value - y;
		}
		const Vector2 get_min() const {
			return Vector2(get_xMin(), get_yMin());
		}
		void		  set_min(const Vector2& value) {
			set_xMin(value.x);
			set_yMin(value.y);
		}
		const Vector2 get_max() const {
			return Vector2(get_xMax(), get_yMax());
		}
		void		  set_max(const Vector2& value) {
			set_xMax(value.x);
			set_yMax(value.y);
		}

		vector<Line>  get_lines() const {
			auto xmin = get_xMin();
			auto xmax = get_xMax();
			auto ymin = get_yMin();
			auto ymax = get_yMax();
			auto p1 = Vector2(xmin, ymin);
			auto p2 = Vector2(xmax, ymin);
			auto p3 = Vector2(xmax, ymax);
			auto p4 = Vector2(xmin, ymax);
			return {
				Line(p1, p2),
				Line(p2, p3),
				Line(p3, p4),
				Line(p4, p1),
			};
		}

		Rect get_boundingBox() const override {
			return *this;
		}
		void add_offset(const Vector2& offset) override {
			x += offset.x;
			y += offset.y;
		}

		Rect operator& (const Rect& other) {
			Rect result;
			result.width  = std::max(0.0f, std::min(this->get_xMax(), other.get_xMax()) - std::max(this->x, other.x));
			result.height = std::max(0.0f, std::min(this->get_yMax(), other.get_yMax()) - std::max(this->y, other.y));
			result.x = std::max(this->x, other.x);
			result.y = std::max(this->y, other.y);
			return result;
		}

		bool operator== (const Rect& other) {
			return
				float_equal(x, other.x) &&
				float_equal(y, other.y) &&
				float_equal(width, other.width) &&
				float_equal(height, other.height);
		}
		bool operator!= (const Rect& other) {
			return
				!float_equal(x, other.x) ||
				!float_equal(y, other.y) ||
				!float_equal(width, other.width) ||
				!float_equal(height, other.height);
		}

		friend ostream& operator<<(ostream& os, const Rect& obj) {
			os << '(' << obj.x << ", " << obj.y << ", " << obj.width << ", " << obj.height << ')';
			return os;
		}

		class Hasher {
		public:
			size_t operator()(const Rect& value) const {
				return value.get_hashCode();
			}
		};
	};

	template<class T>
	class T_Path2D : public T_Shape2D<T> {
		using Vector2 = T_Vector2<T>;
		using Line = T_Line2D<T>;
		using Rect = T_Rect<T>;
		using Path = T_Path2D;
	public:
		vector<Vector2> wayPoints;

		Rect get_boundingBox() const override {
			T xmin(0); T xmax(0); T ymin(0); T ymax(0);
			int count = (int)wayPoints.size();
			if (count > 0) {
				xmin = wayPoints[0].x;
				xmax = wayPoints[0].x;
				ymin = wayPoints[0].y;
				ymax = wayPoints[0].y;
				for (int i = 1; i < count; i++) {
					if (xmin > wayPoints[i].x) xmin = wayPoints[i].x;
					if (xmax < wayPoints[i].x) xmax = wayPoints[i].x;
					if (ymin > wayPoints[i].y) ymin = wayPoints[i].y;
					if (ymax < wayPoints[i].y) ymax = wayPoints[i].y;
				}
			}
			return Rect(xmin, ymin, xmax - xmin, ymax - ymin);
		}
		void add_offset(const Vector2& offset) override {
			for (int i = 0, imax = (int)wayPoints.size(); i < imax; i++) {
				wayPoints[i] += offset;
			}
		}

		inline Vector2& operator[](int index) { return wayPoints[index]; }
	};

	template<class T>
	class T_Polygon2D : public T_Shape2D<T> {
		using Vector2 = T_Vector2<T>;
		using Line = T_Line2D<T>;
		using Rect = T_Rect<T>;
		using Path = T_Path2D<T>;
		using Polygon = T_Polygon2D;
	public:
		static bool is_clockWise(const vector<Vector2>& polygon) {
			int count = (int)polygon.size();
			if (count == 0)
				return true;
			Vector2 lastPoint;
			lastPoint = polygon[count - 1];
			T sum = T(0);
			for (auto& point : polygon) {
				sum += lastPoint.x * point.y;
				sum -= point.x * lastPoint.y;
				lastPoint = point;
			}
			return sum <= 0;
		}

		T_Polygon2D() {}
		T_Polygon2D(const vector<Vector2>& points) {
			vertices = points;
		}

		vector<Vector2> vertices;

		Rect get_boundingBox() const override {
			T xmin(0); T xmax(0); T ymin(0); T ymax(0);
			int count = (int)vertices.size();
			if (count > 0) {
				xmin = vertices[0].x;
				xmax = vertices[0].x;
				ymin = vertices[0].y;
				ymax = vertices[0].y;
				for (int i = 1; i < count; i++) {
					if (xmin > vertices[i].x) xmin = vertices[i].x;
					if (xmax < vertices[i].x) xmax = vertices[i].x;
					if (ymin > vertices[i].y) ymin = vertices[i].y;
					if (ymax < vertices[i].y) ymax = vertices[i].y;
				}
			}
			return Rect(xmin, ymin, xmax - xmin, ymax - ymin);
		}
		void add_offset(const Vector2& offset) override {
			for (auto& p : vertices)
				p.add_offset(offset);
		}

		bool is_clockWise() { return Polygon::is_clockWise(vertices); }

		void		 get_lines(vector<Line>& dst) {
			int imax = vertices.size();
			Vector2 from = vertices[imax - 1];
			for (int i = 0; i < imax; i++) {
				Vector2 to = vertices[i];
				dst.push_back(Line(from, to));
				from = to;
			}
		}
		vector<Line> get_lines() {
			vector<Line> ret;
			get_lines(ret);
			return ret;
		}
	};

	namespace Graph2D {

#pragma region collide
		// point
		template<class T>
		bool is_collide(const T_Vector2<T>& lhs, const T_Vector2<T>& rhs) {
			return (lhs == rhs);
		}
		template<class T>
		bool is_collide(const T_Vector2<T>& lhs, const T_Line2D<T>& rhs) {
			auto v1 = rhs.get_length();
			auto v2 = (lhs - rhs.p1).get_length() + (lhs - rhs.p2).get_length();
			return float_equal(v1, v2);
		}
		template<class T>
		bool is_collide(const T_Vector2<T>& lhs, const T_Triangle2D<T>& rhs) {
			T_Vector2<T> v1 = rhs.p1 - lhs;
			T_Vector2<T> v2 = rhs.p2 - lhs;
			T_Vector2<T> v3 = rhs.p3 - lhs;
			float c1 = v1.cross(v2);
			float c2 = v2.cross(v3);
			float c3 = v3.cross(v1);
			return
				(c1 > 0 && c2 > 0 && c3 > 0) ||
				(c1 < 0 && c2 < 0 && c3 < 0);
		}
		template<class T>
		bool is_collide(const T_Vector2<T>& lhs, const T_Rect<T>& rhs) {
			return
				float_abEqu(lhs.x, rhs.get_xMin()) &&
				float_beEqu(lhs.x, rhs.get_xMax()) &&
				float_abEqu(lhs.y, rhs.get_yMin()) &&
				float_beEqu(lhs.y, rhs.get_yMax());
		}
		template<class T>
		bool is_collide(const T_Vector2<T>& lhs, const T_Path2D<T>& rhs);
		template<class T>
		bool is_collide(const T_Vector2<T>& lhs, const T_Polygon2D<T>& rhs) {
			int imax = rhs.vertices.size();
			bool inside = false;
			float x = lhs.x, y = lhs.y;
			for (int i = 0, j = imax - 1; i < imax; j = i, i++) {
				auto& p1 = rhs.vertices[i];
				auto& p2 = rhs.vertices[j];
				if (float_above(p1.y, y) != float_above(p2.y, y) && float_below(x, (p2.x - p1.x) * (y - p1.y) / (p2.y - p1.y) + p1.x)) {
					inside = !inside;
				}
			}
			return inside;
		}

		// line
		template<class T>
		bool is_collide(const T_Line2D<T>& lhs, const T_Vector2<T>& rhs) { return is_collide(rhs, lhs); };
		template<class T>
		bool is_collide(const T_Line2D<T>& lhs, const T_Line2D<T>& rhs) {
			auto orientation = [](const T_Vector2<T>& a, const T_Vector2<T>& b, const T_Vector2<T>& c) -> int {
				T val = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);
				if (std::abs(val) < static_cast<T>(1e-9)) return 0;
				return (val > 0) ? 1 : 2;
			};

			auto onSegment = [](const T_Vector2<T>& p, const T_Vector2<T>& q, const T_Vector2<T>& r) -> bool {
				float minX1 = (p.x < r.x) ? p.x : r.x;
				float minX2 = (p.x > r.x) ? p.x : r.x;
				float minY1 = (p.y < r.y) ? p.y : r.y;
				float minY2 = (p.y > r.y) ? p.y : r.y;
				return (q.x >= minX1 && q.x <= minX2 && q.y >= minY1 && q.y <= minY2);
			};

			T_Vector2<T> p1 = lhs.p1, q1 = lhs.p2;
			T_Vector2<T> p2 = rhs.p1, q2 = rhs.p2;

			int o1 = orientation(p1, q1, p2);
			int o2 = orientation(p1, q1, q2);
			int o3 = orientation(p2, q2, p1);
			int o4 = orientation(p2, q2, q1);

			//交叉情況
			if (o1 != o2 && o3 != o4) return true;

			//重合情況
			if (o1 == 0 && onSegment(p1, p2, q1)) return true;
			if (o2 == 0 && onSegment(p1, q2, q1)) return true;
			if (o3 == 0 && onSegment(p2, p1, q2)) return true;
			if (o4 == 0 && onSegment(p2, q1, q2)) return true;
			return false;
		}
		template<class T>
		bool is_collide(const T_Line2D<T>& lhs, const T_Triangle2D<T>& rhs);
		template<class T>
		bool is_collide(const T_Line2D<T>& lhs, const T_Rect<T>& rhs) {
			T_Line2D<T> line1(T_Vector2<T>(rhs.x, rhs.y), T_Vector2<T>(rhs.x + rhs.width, rhs.y));
			T_Line2D<T> line2(T_Vector2<T>(rhs.x + rhs.width, rhs.y), T_Vector2<T>(rhs.x + rhs.width, rhs.y + rhs.height));
			T_Line2D<T> line3(T_Vector2<T>(rhs.x + rhs.width, rhs.y + rhs.height), T_Vector2<T>(rhs.x, rhs.y + rhs.height));
			T_Line2D<T> line4(T_Vector2<T>(rhs.x, rhs.y), T_Vector2<T>(rhs.x, rhs.y + rhs.height));

			if (is_collide(lhs, line1) || is_collide(lhs, line2) || is_collide(lhs, line3) || is_collide(lhs, line4))
				return true;
			else if (is_collide(lhs.p1, rhs))
				return true;
			return false;

		}
		template<class T>
		bool is_collide(const T_Line2D<T>& lhs, const T_Path2D<T>& rhs);
		template<class T>
		bool is_collide(const T_Line2D<T>& lhs, const T_Polygon2D<T>& rhs) {
			int pointNum = rhs.vertices.size();

			for (int i = 0; i < pointNum - 1; ++i) {
				T_Line2D<T> edge(rhs.vertices[i], rhs.vertices[i + 1]);
				if (is_collide(lhs, edge))
					return true;
			}
			T_Line2D<T> closingEdge(rhs.vertices[pointNum - 1], rhs.vertices[0]);
			if (is_collide(lhs, closingEdge))
				return true;
			if (is_collide(lhs.p1, rhs) || is_collide(lhs.p2, rhs))
				return true;
			return false;
		}

		// triangle
		template<class T>
		bool is_collide(const T_Triangle2D<T>& lhs, const T_Vector2<T>& rhs) { return is_collide(rhs, lhs); };
		template<class T>
		bool is_collide(const T_Triangle2D<T>& lhs, const T_Line2D<T>& rhs) { return is_collide(rhs, lhs); };
		template<class T>
		bool is_collide(const T_Triangle2D<T>& lhs, const T_Triangle2D<T>& rhs);
		template<class T>
		bool is_collide(const T_Triangle2D<T>& lhs, const T_Rect<T>& rhs);
		template<class T>
		bool is_collide(const T_Triangle2D<T>& lhs, const T_Path2D<T>& rhs);
		template<class T>
		bool is_collide(const T_Triangle2D<T>& lhs, const T_Polygon2D<T>& rhs);

		// rect
		template<class T>
		bool is_collide(const T_Rect<T>& lhs, const T_Vector2<T>& rhs) { return is_collide(rhs, lhs); };
		template<class T>
		bool is_collide(const T_Rect<T>& lhs, const T_Line2D<T>& rhs) { return is_collide(rhs, lhs); };
		template<class T>
		bool is_collide(const T_Rect<T>& lhs, const T_Triangle2D<T>& rhs) { return is_collide(rhs, lhs); };
		template<class T>
		bool is_collide(const T_Rect<T>& lhs, const T_Rect<T>& rhs) {
			return !(
				float_below(lhs.get_xMax(), rhs.get_xMin()) ||
				float_above(lhs.get_xMin(), rhs.get_xMax()) ||
				float_below(lhs.get_yMax(), rhs.get_yMin()) ||
				float_above(lhs.get_yMin(), rhs.get_yMax()));
		}
		template<class T>
		bool is_collide(const T_Rect<T>& lhs, const T_Path2D<T>& rhs);
		template<class T>
		bool is_collide(const T_Rect<T>& lhs, const T_Polygon2D<T>& rhs);

		// path
		template<class T>
		bool is_collide(const T_Path2D<T>& lhs, const T_Vector2<T>& rhs) { return is_collide(rhs, lhs); };
		template<class T>
		bool is_collide(const T_Path2D<T>& lhs, const T_Line2D<T>& rhs) { return is_collide(rhs, lhs); };
		template<class T>
		bool is_collide(const T_Path2D<T>& lhs, const T_Triangle2D<T>& rhs) { return is_collide(rhs, lhs); };
		template<class T>
		bool is_collide(const T_Path2D<T>& lhs, const T_Rect<T>& rhs) { return is_collide(rhs, lhs); };
		template<class T>
		bool is_collide(const T_Path2D<T>& lhs, const T_Path2D<T>& rhs);
		template<class T>
		bool is_collide(const T_Path2D<T>& lhs, const T_Polygon2D<T>& rhs);

		// polygon
		template<class T>
		bool is_collide(const T_Polygon2D<T>& lhs, const T_Vector2<T>& rhs) { return is_collide(rhs, lhs); };
		template<class T>
		bool is_collide(const T_Polygon2D<T>& lhs, const T_Line2D<T>& rhs) { return is_collide(rhs, lhs); };
		template<class T>
		bool is_collide(const T_Polygon2D<T>& lhs, const T_Triangle2D<T>& rhs) { return is_collide(rhs, lhs); };
		template<class T>
		bool is_collide(const T_Polygon2D<T>& lhs, const T_Rect<T>& rhs) { return is_collide(rhs, lhs); };
		template<class T>
		bool is_collide(const T_Polygon2D<T>& lhs, const T_Path2D<T>& rhs) { return is_collide(rhs, lhs); };
		template<class T>
		bool is_collide(const T_Polygon2D<T>& lhs, const T_Polygon2D<T>& rhs);

		template<class T>
		T_Vector2<T> get_closest(const T_Vector2<T>& lhs, const T_Line2D<T>& rhs) {
			auto delta = rhs.p2 - rhs.p1;
			auto mgn2 = delta.x * delta.x + delta.y * delta.y;
			auto t = (lhs - rhs.p1).dot(delta) / mgn2;
			if (t <= 0)return rhs.p1;
			if (t >= 1)return rhs.p2;
			return { rhs.p1.x + t * (rhs.p2.x - rhs.p1.x),rhs.p1.y + t * (rhs.p2.y - rhs.p1.y) };
		}

		template<class T>
		float get_distance(const T_Vector2<T>& lhs, const T_Line2D<T>& rhs) {
			return (lhs - get_closest(lhs, rhs)).get_length();
		}

#pragma endregion

	} // Graph2D

	template<class T>
	class T_TriangulationSolver2D {
		using Vector2 = T_Vector2<T>;
		using Triangle = T_Triangle2D<T>;
		using Path = T_Path2D<T>;
		using Polygon = T_Polygon2D<T>;
		using Solver = T_TriangulationSolver2D;
	public:
		T_TriangulationSolver2D(const Polygon& target) :src(target.vertices) {}
		T_TriangulationSolver2D(const Path& target) :src(target.wayPoints) {}
		T_TriangulationSolver2D(const vector<Vector2>& target) :src(target) {}

		const vector<Vector2>& src;
		int n = 0;				   // point count
		short* earTable = nullptr; // -1=unknow 0=false 1=true 
		int* prev = nullptr; // loop linked list
		int* next = nullptr;
		Triangle triangleBuffer;
		int is_ear(int i) {
			Vector2 v1 = src[i] - src[prev[i]];
			Vector2 v2 = src[next[i]] - src[i];
			if (float_below(v1.cross(v2), 0))
				return 0;
			triangleBuffer.p1 = src[prev[i]];
			triangleBuffer.p2 = src[i];
			triangleBuffer.p3 = src[next[i]];
			if (Line2D(triangleBuffer.p1, triangleBuffer.p2).get_slope() == Line2D(triangleBuffer.p1, triangleBuffer.p3).get_slope())
				return 0;
			for (int j = next[next[i]], jend = prev[i]; j != jend; j = next[j]) {
				if (Graph2D::is_collide(src[j], triangleBuffer))
					return 0;
			}
			return 1;
		}

		vector<Triangle> solve() {
			vector<Triangle> ret;
			n = src.size();
			earTable = new short[n];
			prev = new   int[n];
			next = new   int[n];
			bool isCW = Polygon::is_clockWise(src);
			int* arr1 = isCW ? next : prev;
			int* arr2 = isCW ? prev : next;
			for (int i = 0; i < n; i++) {
				earTable[i] = -1;
				arr1[i] = i - 1;
				arr2[i] = i + 1;
			}
			arr1[0] = n - 1;
			arr2[n - 1] = 0;

			int i = 0;
			for (int j = 0, jmax = n - 3; j < jmax; j++) {
				int imax = n;
				while (true)
				{
					if (earTable[i] == -1)
						earTable[i] = is_ear(i);
					if (earTable[i] == 1)
						break;
					i = next[i];
					if (--imax < 0)
						break;
				}
				ret.push_back(triangleBuffer);
				earTable[prev[i]] = -1;
				earTable[next[i]] = -1;

				next[prev[i]] = next[i];
				prev[next[i]] = prev[i];
				i = next[i];
			}
			ret.push_back(Triangle(src[prev[i]], src[i], src[next[i]]));

			delete earTable;
			delete prev;
			delete next;

			return ret;
		}
	};

	typedef T_Shape2D	  <CALLIB_FLT_BASE> Shape;
	typedef T_Vector2     <CALLIB_FLT_BASE> Vector2;
	typedef T_Vector2     <CALLIB_INT_BASE> Vector2Int;
	typedef T_Line2D      <CALLIB_FLT_BASE> Line2D;
	typedef T_Triangle2D  <CALLIB_FLT_BASE> Triangle2D;
	typedef T_Rect        <CALLIB_FLT_BASE> Rect;
	typedef T_Rect        <CALLIB_INT_BASE> RectInt;
	typedef T_Path2D	  <CALLIB_FLT_BASE> Path2D;
	typedef T_Polygon2D   <CALLIB_FLT_BASE> Polygon2D;

	typedef T_TriangulationSolver2D<CALLIB_FLT_BASE> TriangulationSolver2D;


#ifdef IMGUI_API
	namespace ImEx {

		template<class T>
		inline void OnGui(T& obj) { OnGui("unnamed", obj); }
		template<class T>
		void OnGui(const char* label, T& obj) {
			ImGui::Text("No GUI Implement");
		}
		template <>
		void OnGui(const char* label, int& obj);
		template <>
		void OnGui(const char* label, float& obj);
		template <>
		void OnGui(const char* label, Vector2& obj);
		template <>
		void OnGui(const char* label, Vector2Int& obj);
		template <>
		void OnGui(const char* label, Triangle2D& obj);
		template <>
		void OnGui(const char* label, Rect& obj);
		template <>
		void OnGui(const char* label, ImWindowDrawer2D& obj);

	}

	class ImWindowDrawer2D {
	public:
		ImWindowDrawer2D();

		ImDrawList* drawList = nullptr;
		Vector2 coordOffset = Vector2(0, 0);
		float   coordScale = 1;

		void AssignCurrentWindow();

		Vector2 WorldToScreenPosition(const Vector2& position) const;
		Vector2 ScreenToWorldPosition(const Vector2& position) const;
		Line2D WorldToScreenPosition(const Line2D& position) const;
		Line2D ScreenToWorldPosition(const Line2D& position) const;
		Triangle2D WorldToScreenPosition(const Triangle2D& position) const;
		Triangle2D ScreenToWorldPosition(const Triangle2D& position) const;
		Rect WorldToScreenPosition(const Rect& position) const;
		Rect ScreenToWorldPosition(const Rect& position) const;

		void PrintText(const Vector2& position, const string& text, const Color& color = { 1,1,1 }, const Color& bgColor = { 0,0,0,0 });
		void DrawPoint(const Vector2& obj, const Color& color = { 1,1,1 }, float radius = 3);
		void DrawLine(const Line2D& obj, const Color& color = { 1,1,1 }, float thickness = 1);
		void DrawRect(const Rect& obj, const Color& color = { 1,1,1 }, float thickness = 1);
		void DrawRectFilled(const Rect& obj, const Color& color = { 1,1,1 });
		void DrawTriangle(const Triangle2D& obj, const Color& color = { 1,1,1 }, float thickness = 1);
		void DrawTriangleFilled(const Triangle2D& obj, const Color& color = { 1,1,1 });
		void DrawPoly(const Polygon2D& obj, const Color& color = { 1,1,1 }, float thickness = 1);
		void DrawPolyFilled(const Polygon2D& obj, const Color& color = { 1,1,1 });

		const Vector2& get_windowSize() const;
		const Rect& get_worldViewBound() const;

	private:
		Vector2 windowSize;
		Rect worldViewBound;
	};

#endif // IMGUI_API

}
