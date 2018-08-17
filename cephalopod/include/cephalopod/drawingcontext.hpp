#pragma once

#include "mat3x3.hpp"

namespace ceph
{
	class Graphics;

	struct DrawingContext
	{
		Graphics& graphics;
		Mat3x3 transformation;
		float alpha;

		DrawingContext(Graphics& g, const Mat3x3& t = Mat3x3(), float a = 1.0f) :
			graphics(g),
			transformation(t),
			alpha(a)
		{}

		DrawingContext(const DrawingContext& dc ) :
			graphics(dc.graphics),
			transformation(dc.transformation),
			alpha(dc.alpha)
		{}
	};
}
