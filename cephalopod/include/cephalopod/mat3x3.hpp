#pragma once
#include "types.hpp"
#include <optional>

namespace ceph
{
	class Mat3x3
	{
	public:
		Mat3x3();
		Mat3x3(
			float a11, float a12, float a13, 
			float a21, float a22, float a23, 
			float a31, float a32, float a33
		);

		const float* get() const;
		float getDeterminant() const;
		std::optional<Mat3x3> getInverse() const;
		Mat3x3& transpose();
		Vec2<float> apply(float x, float y) const;
		Vec2<float> apply(const Vec2<float>& point) const;
		Rect<float> apply(const Rect<float>& rectangle) const;
		Mat3x3& combine(const Mat3x3& transform);
		Mat3x3& translate(float x, float y);
		Mat3x3& translate(const Vec2<float>& offset);
		Mat3x3& rotate(float angle);
		Mat3x3& rotate(float angle, float centerX, float centerY);
		Mat3x3& rotate(float angle, const Vec2<float>& center);
		Mat3x3& scale(float scaleX, float scaleY);
		Mat3x3& scale(float scaleX, float scaleY, float centerX, float centerY);
		Mat3x3& scale(const Vec2<float>& factors);
		Mat3x3& scale(const Vec2<float>& factors, const Vec2<float>& center);
		static const Mat3x3 Identity;

	private:
		float m_[9]; 
	};

	ceph::Mat3x3& operator *=(ceph::Mat3x3& left, const ceph::Mat3x3& right);
	ceph::Vec2<float> operator *(const ceph::Mat3x3& left, const ceph::Vec2<float>& right);
	ceph::Mat3x3 operator *(const ceph::Mat3x3& left, const Mat3x3& right);
	ceph::Mat3x3 operator *(float k, const Mat3x3& mat);
	bool operator ==(const ceph::Mat3x3& left, const ceph::Mat3x3& right);
	bool operator !=(const ceph::Mat3x3& left, const ceph::Mat3x3& right);
}