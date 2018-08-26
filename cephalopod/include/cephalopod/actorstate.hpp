#pragma once

#include <string>
#include <optional>
#include "../include/cephalopod/Mat3x3.hpp"
#include "../include/cephalopod/types.hpp"

namespace ceph {

	class SpriteSheet;

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
		std::shared_ptr<const SpriteSheet> sprite_sheet_;
		std::string sprite_frame_;
		Vec2<int> frame_sz_;

	public:
		ActorState();

		void setSpriteSheet(const std::shared_ptr<const SpriteSheet>& sheet);
		std::shared_ptr<const SpriteSheet> getSpriteSheet() const;

		void moveBy(const Vec2<float>& vec);
		void rotateBy(float theta);
		void changeAlphaBy(float alpha);
		void changeScaleBy(const Vec2<float>& scale);

		float getAlpha() const;
		Vec2<float> getPosition() const;
		Vec2<float> getScale() const;
		Vec2<float> getAnchorPt() const;
		Vec2<float> getAnchorPcnt() const;
		float getRotation() const;
		void setAnchorPcnt(const Vec2<float>& pt);
		void setAnchorPcnt(float x, float y);

		std::string getSpriteFrame() const;
		void setSpriteFrame(const std::string& sprite_frame);
		Vec2<int> getSize() const;
		Rect<int> getRect() const;

		Mat3x3 getTransformationMatrix() const;
	};
}