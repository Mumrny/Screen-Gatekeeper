#include <DxLib.h>
#include <cmath>
#include "Geometry.h"


Position2f
Position2f::operator-(void) {
	return Position2f(-x, -y);
}

void
Position2f::operator+=(const Position2f & pos) {
	x += pos.x;
	y += pos.y;
}

void
Position2f::operator-=(const Position2f & pos) {
	x -= pos.x;
	y -= pos.y;
}

void
Position2f::operator*=(const float & scale) {
	x *= scale;
	y *= scale;
}

void
Position2f::operator/=(const float & scale) {
	x /= scale;
	y /= scale;
}

float
Position2f::Length(void) const {
	return hypot(x, y);
}

Position2f
Position2f::Normalized(void) {
	float ren = Length();
	return Position2f(x / ren, y / ren);
}

float
Dot(const Position2f & lval, const Position2f & rval) {
	return lval.x * rval.x + lval.y * rval.y;
}

float
Cross(const Position2f & lval, const Position2f & rval) {
	return lval.x * rval.y - lval.y * rval.x;
}

Position2f
operator+(const Position2f & lval, const Position2f & rval) {
	return Position2f(lval.x + rval.x, lval.y + rval.y);
}

Position2f
operator-(const Position2f & lval, const Position2f & rval) {
	return Position2f(lval.x - rval.x, lval.y - rval.y);
}

Position2f
operator*(const Position2f & val, const float & scale) {
	return Position2f(val.x * scale, val.y * scale);
}

bool
IsCrossing(const Position2f & as, const Position2f & ae, const Position2f & bs, const Position2f & be) {
	Vector2f asae(ae - as);
	Vector2f asbs(bs - as);
	Vector2f asbe(be - as);
	Vector2f bsbe(be - bs);
	Vector2f bsae(ae - bs);
	return (Cross(asae, asbs) * Cross(asae, asbe) < 0
			&& Cross(bsbe, -asbs) * Cross(bsbe, bsae) < 0);
}

const Rect&
GetOverlap(const Rect & ra, const Rect & rb) {
	float left = max(ra.Left(), rb.Left());
	float right = min(ra.Right(), rb.Right());
	float top = max(ra.Top(), rb.Top());
	float bottom = min(ra.Bottom(), rb.Bottom());

	return Rect(Position2f((left + right) / 2, (top + bottom) / 2), Size(right - left, bottom - top));
}

void
Segment::Draw(unsigned int color) {
	DrawLine(spos.x, spos.y, epos.x, epos.y, color);
}

float
Rect::Left(void) const {
	return pos.x - size.width / 2;
}

float
Rect::Right(void) const {
	return pos.x + size.width / 2;
}

float
Rect::Top(void) const {
	return pos.y - size.height / 2;
}

float
Rect::Bottom(void) const {
	return pos.y + size.height / 2;
}

void
Rect::Draw(const unsigned int & color, const Position2f & offset, const int & param)const {
	SetDrawBlendMode(DX_BLENDMODE_MULA, param);
	DrawBox(Left() - offset.x, Top() - offset.y, Right() - offset.x, Bottom() - offset.y, color, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
