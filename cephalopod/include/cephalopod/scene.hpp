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
	class GameImpl;

	typedef std::function<void(Actor&, float)> UpdateHandlerFunc;
	typedef std::function<void(Actor&, bool, KeyCode, unsigned char)> KeyEventHandlerFunc;

	class Scene : public Slot<Scene>, std::enable_shared_from_this<Scene>
	{
		friend class GameImpl;
		friend class Actor;

	private:
		std::unique_ptr<SceneImpl> impl_;
		std::vector<std::shared_ptr<Actor>> stage_;

	protected:
		Signal<float> updateActionsEvent;

		Scene();

	public:
		virtual void initialize() = 0;

		void setBackground(const std::shared_ptr<Texture>& tex );
		void setBackground(const std::shared_ptr<Texture>& tex, CoordinateMapping mapping);

		void addActor(const std::shared_ptr<Actor>& child);
		void removeActor(const std::shared_ptr<Actor>& child);

		template<typename T>
		void addActor(const std::shared_ptr<T>& child) { addActor(std::static_pointer_cast<Actor>(child)); }

		void draw(DrawingContext&& rt);

		Signal<float> updateEvent;

		virtual ~Scene();

		template<typename T> static std::shared_ptr<T> create() {
			auto scene = std::make_shared<T>();
			scene->initialize();
			return scene;
		}
	};

}