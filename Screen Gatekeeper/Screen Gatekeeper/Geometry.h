#pragma once


struct Position2 {
	int x, y;
};

using Vector2 = Position2;

struct Position2f {
	float x, y;
	Position2f() {}
	Position2f(const float& inx, const float& iny) :x(inx), y(iny) {}
	Position2f operator-(void);
	void operator+=(const Position2f& pos);
	void operator-=(const Position2f& pos);
	void operator*=(const float& scale);
	void operator/=(const float& scale);
	float Length(void) const;
	Position2f Normalized(void);
};

float Dot(const Position2f& lval, const Position2f& rval);
float Cross(const Position2f& lval, const Position2f& rval);

Position2f operator+(const Position2f& lval, const Position2f& rval);
Position2f operator-(const Position2f& lval, const Position2f& rval);
Position2f operator*(const Position2f& val, const float& scale);

using Vector2f = Position2f;

struct Size {
	int width, height;
	Size() {}
	Size(const int& inw, const int& inh) :width(inw), height(inh) {}
};

struct Circle {
	Position2f pos;
	float radius;
	Circle() :pos(0, 0), radius(0) {}
	Circle(const Position2f& inpos,  const float& inr) :pos(inpos), radius(inr) {}
};

struct Segment {
	Position2f spos, epos;		// 始点座標, 終点座標
	Segment() :spos(0, 0), epos(0, 0) {}
	Segment(const Position2f& inspos, const Position2f& inepos) :spos(inspos), epos(inepos) {}
	Segment(const float& insposx, const float& insposy, const float& ineposx, const float& ineposy) :spos(insposx, insposy), epos(ineposx, ineposy) {}
	void Draw(unsigned int color);
};

bool IsCrossing(const Position2f& as, const Position2f& ae, const Position2f& bs, const Position2f& be);

struct Rect {
	Position2f pos;
	Size size;
	Rect() :pos(0, 0), size(0, 0) {}
	Rect(const Position2f& inpos, const Size& insize) :pos(inpos), size(insize) {}
	float Left(void)const;
	float Right(void)const;
	float Top(void)const;
	float Bottom(void)const;
	void Draw(const unsigned int& color, const Position2f& offset, const int & param)const;
};

const Rect& GetOverlap(const Rect& ra, const Rect& rb);
