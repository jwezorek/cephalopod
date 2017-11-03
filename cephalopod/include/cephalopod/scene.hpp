#pragma once
#include <vector>
#include <memory>
#include <functional>
#include "actor.hpp"
#include "events.hpp"
#include "texture.hpp"

namespace ceph {

	class SceneImpl;

	typedef std::function<void(Actor&, float)> UpdateHandlerFunc;
	typedef std::function<void(Actor&, bool, KeyCode, unsigned char)> KeyEventHandlerFunc;

	class Scene
	{
	private:
		std::unique_ptr<SceneImpl> impl_;

	public:
		Scene();

		void handleKeyEvent(bool isPressed, KeyCode key, unsigned char modifiers);
		void setBackground(const std::shared_ptr<Texture>& tex );
		void setBackground(const std::shared_ptr<Texture>& tex, CoordinateMapping mapping);
		void scheduleUpdate(Actor&, const UpdateHandlerFunc& func);
		void unscheduleUpdate(Actor&);
		void registerKeyEventHandler(Actor&, const KeyEventHandlerFunc& func);
		void unregisterKeyEvenHandler(Actor&);
		void addActor(const std::shared_ptr<Actor>& child);
		void update(float elapsed);
		void draw(DrawingContext&& rt);

		virtual ~Scene();
	};

}