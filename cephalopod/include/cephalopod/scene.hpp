#pragma once
#include <vector>
#include <memory>
#include <functional>
#include "actor.hpp"
#include "events.hpp"
#include "texture.hpp"
#include "signals.hpp"

namespace ceph {

	class SceneImpl;

	typedef std::function<void(Actor&, float)> UpdateHandlerFunc;
	typedef std::function<void(Actor&, bool, KeyCode, unsigned char)> KeyEventHandlerFunc;

	class Scene : public Slot<Scene>
	{
	private:
		std::unique_ptr<SceneImpl> impl_;

	public:
		Scene();

		void setBackground(const std::shared_ptr<Texture>& tex );
		void setBackground(const std::shared_ptr<Texture>& tex, CoordinateMapping mapping);
		void addActor(const std::shared_ptr<Actor>& child);
		void draw(DrawingContext&& rt);

		Signal<float> updateEvent;

		virtual ~Scene();
	};

}