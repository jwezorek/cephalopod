#pragma once

#include <string>
#include <optional>
#include "../include/cephalopod/Mat3x3.hpp"
#include "../include/cephalopod/types.hpp"

namespace ceph {

	class ActorState
	{
	private:
		Mat3x3 local_to_global_transform_;
		mutable std::optional<Mat3x3> transform_;
		Vec2<float> position_; 
		Vec2<float> scale_;
		Vec2<float> origin_pcnt_;
		float rotation_;
		float alpha_;
		std::string sprite_frame_;
		Vec2<int> frame_sz_;

	public:
		ActorState();

		void translate(const ceph::Vec2<float>& v);
		void translate(float x, float y);
		void rotate(float theta);

		void setAlpha(float alpha);
		float getAlpha() const;

		Vec2<float> getGlobalPosition() const;
		void setGlobalPosition(const Vec2<float>& pt);

		void setPosition(const ceph::Vec2<float>& v);
		void setPosition(float x, float y);
		Vec2<float> getPosition() const;

		Vec2<float> getScale() const;
		void setScale(const Vec2<float>& scale);

		Vec2<float> getAnchorPt() const;
		Vec2<float> getAnchorPcnt() const;
		void setAnchorPcnt(const Vec2<float>& pt);
		void setAnchorPcnt(float x, float y);

		void setRotation(float theta);
		float getRotation() const;

		std::string getSpriteFrame() const;
		void setSpriteFrame(const std::string& sprite_frame, const Vec2<int> frame_sz);

		Vec2<int> getFrameSize() const;

		Mat3x3 getTransformationMatrix() const;
	};
}