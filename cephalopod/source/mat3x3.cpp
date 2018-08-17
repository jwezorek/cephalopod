#include "..\include\cephalopod\mat3x3.hpp"

namespace {
	const int _11 = 0;
	const int _21 = 1;
	const int _31 = 2;
	const int _12 = 3;
	const int _22 = 4;
	const int _32 = 5;
	const int _13 = 6;
	const int _23 = 7;
	const int _33 = 8;

	float det2x2( 
			float a , float b,
			float c , float d ) {
		return a*d - b*c;
	}
}

ceph::Mat3x3::Mat3x3() :
	ceph::Mat3x3(
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	)
{
}

ceph::Mat3x3::Mat3x3(float a11, float a12, float a13, float a21, float a22, float a23, float a31, float a32, float a33) :
	m_{
		a11, a21, a31,  // column 1
		a12, a22, a32,  // column 2
		a13, a23, a33   // column 3
	}
{}

float ceph::Mat3x3::getDeterminant() const
{
	return m_[_11] * (m_[_22]*m_[_33] - m_[_23]*m_[_32]) -
		   m_[_12] * (m_[_21]*m_[_33] - m_[_23]*m_[_31]) +
		   m_[_13] * (m_[_21]*m_[_32] - m_[_22]*m_[_31]);
}

const float * ceph::Mat3x3::get() const
{
	return &(m_[0]);
}

std::optional<ceph::Mat3x3> ceph::Mat3x3::getInverse() const
{
	auto det = getDeterminant();
	if (det == 0)
		return std::nullopt;
	return 1.0f/det * ceph::Mat3x3(
		 det2x2(m_[_22], m_[_23], m_[_32], m_[_33]), -det2x2(m_[_12], m_[_13], m_[_32], m_[_33]) , det2x2(m_[_12], m_[_13], m_[_22], m_[_23]),
		-det2x2(m_[_21], m_[_23], m_[_31], m_[_33]),  det2x2(m_[_11], m_[_13], m_[_31], m_[_33]), -det2x2(m_[_11], m_[_13], m_[_21], m_[_23]) ,
		 det2x2(m_[_21], m_[_22], m_[_31], m_[_32]), -det2x2(m_[_11], m_[_12], m_[_31], m_[_32]),  det2x2(m_[_11], m_[_12], m_[_21], m_[_22])
	);
}

ceph::Mat3x3& ceph::Mat3x3::transpose()
{
	std::swap(m_[_21], m_[_12]);
	std::swap(m_[_31], m_[_13]);
	std::swap(m_[_32], m_[_23]);
	return *this;
}


ceph::Vec2<float> ceph::Mat3x3::apply(float x, float y) const
{
	return Vec2<float>(
		x*m_[_11] + y * m_[_12] + m_[_13], 
		x*m_[_21] + y * m_[_22] + m_[_23]
	);
}

ceph::Vec2<float> ceph::Mat3x3::apply(const Vec2<float>& point) const
{
	return apply(point.x, point.y);
}

ceph::Rect<float> ceph::Mat3x3::apply(const Rect<float>& r) const
{
	ceph::Vec2<float> vertices[4] = {
		r.getLocation(),
		ceph::Vec2<float>(r.x2(), r.y),
		ceph::Vec2<float>(r.x2(), r.y2()),
		ceph::Vec2<float>(r.x, r.y2())
	};

	float left = std::numeric_limits<float>::max();
	float bottom = std::numeric_limits<float>::max();
	float right = std::numeric_limits<float>::min();
	float top = std::numeric_limits<float>::min();

	for (auto& v : vertices) {
		v = this->apply(v);

		left = (v.x < left) ? v.x : left;
		bottom = (v.y < bottom) ? v.y : bottom;
		right = (v.x > right) ? v.x : right;
		top = (v.y > top) ? v.y : top;
	}

	return ceph::Rect<float>(left, bottom, right - left, top - bottom);
}

ceph::Mat3x3& ceph::Mat3x3::combine(const Mat3x3& mat)
{
	auto a = get();
	auto b = mat.get();

	*this = ceph::Mat3x3(
		a[_11]*b[_11] + a[_12]*b[_21] + a[_13]*b[_31], a[_11]*b[_12] + a[_12]*b[_22] + a[_13]*b[_32], a[_11]*b[_13] + a[_12]*b[_23] + a[_13]*b[_33],
		a[_21]*b[_11] + a[_22]*b[_21] + a[_23]*b[_31], a[_21]*b[_12] + a[_22]*b[_22] + a[_23]*b[_32], a[_21]*b[_13] + a[_22]*b[_23] + a[_23]*b[_33],
		a[_31]*b[_11] + a[_32]*b[_21] + a[_33]*b[_31], a[_31]*b[_12] + a[_32]*b[_22] + a[_33]*b[_32], a[_31]*b[_13] + a[_32]*b[_23] + a[_33]*b[_33]
	);

	return *this;
}

ceph::Mat3x3& ceph::Mat3x3::translate(float x, float y)
{
	return combine(
		ceph::Mat3x3(
			1, 0, x,
			0, 1, y,
			0, 0, 1
		)
	);
}

ceph::Mat3x3& ceph::Mat3x3::translate(const Vec2<float>& vec)
{
	return translate(vec.x, vec.y);
}

ceph::Mat3x3& ceph::Mat3x3::rotate(float angle)
{
	float cos = std::cos(angle);
	float sin = std::sin(angle);
	return combine(
		ceph::Mat3x3(
			cos, -sin, 0,
			sin, cos,  0,
			0,   0,    1
		)
	);
}

ceph::Mat3x3& ceph::Mat3x3::rotate(float angle, float centerX, float centerY)
{
	float cos = std::cos(angle);
	float sin = std::sin(angle);

	return combine(
		ceph::Mat3x3(
			cos, -sin, centerX * (1 - cos) + centerY * sin,
			sin, cos, centerY * (1 - cos) - centerX * sin,
			0, 0, 1
		)
	);
}

ceph::Mat3x3& ceph::Mat3x3::rotate(float angle, const Vec2<float>& center)
{
	return rotate(angle, center.x, center.y);
}

ceph::Mat3x3& ceph::Mat3x3::scale(float x, float y)
{
	return combine(
		ceph::Mat3x3{
			x, 0, 0,
			0, y, 0,
			0, 0, 1
		}
	);
}

ceph::Mat3x3& ceph::Mat3x3::scale(float scaleX, float scaleY, float centerX, float centerY)
{
	return combine(
		ceph::Mat3x3(
			scaleX, 0, centerX * (1 - scaleX),
			0, scaleY, centerY * (1 - scaleY),
			0, 0, 1
		)
	);
}

ceph::Mat3x3& ceph::Mat3x3::scale(const Vec2<float>& vec)
{
	return scale(vec.x, vec.y);
}

ceph::Mat3x3& ceph::Mat3x3::scale(const Vec2<float>& vec, const Vec2<float>& center)
{
	return scale(vec.x, vec.y, center.x, center.y);
}

ceph::Mat3x3& ceph::operator*=(ceph::Mat3x3& left, const ceph::Mat3x3& right)
{
	return left.combine(right);
}

ceph::Vec2<float> ceph::operator*(const ceph::Mat3x3& mat, const ceph::Vec2<float>& vec)
{
	return mat.apply(vec);
}

ceph::Mat3x3 ceph::operator*(const ceph::Mat3x3& left, const Mat3x3& right)
{
	return ceph::Mat3x3(left).combine(right);
}

ceph::Mat3x3 ceph::operator*(float k, const Mat3x3& mat)
{
	auto m = mat.get();
	return ceph::Mat3x3(k*m[0], k*m[1], k*m[2], k*m[3], k*m[4], k*m[5], k*m[6], k*m[7], k*m[8]);
}

bool ceph::operator==(const ceph::Mat3x3& left, const ceph::Mat3x3& right)
{
	auto a = left.get();
	auto b = right.get();
	return 
		a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && 
		a[3] == b[3] && a[4] == b[4] && a[5] == b[5] && 
		a[6] == b[6] && a[7] == b[7] && a[8] == b[8];
}

bool ceph::operator!=(const ceph::Mat3x3& left, const ceph::Mat3x3& right)
{
	return !(left == right);
}

const ceph::Mat3x3 ceph::Mat3x3::Identity;