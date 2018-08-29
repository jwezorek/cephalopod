#pragma once
#include <deque>
#include <list>
#include <memory>
#include <functional>
#include <initializer_list>

#include "actor.hpp"
#include "events.hpp"
#include "texture.hpp"
#include "signals.hpp"
#include "drawingcontext.hpp"

namespace ceph {

	class GameImpl;

	typedef std::function<void(Actor&, float)> UpdateHandlerFunc;
	typedef std::function<void(Actor&, bool, KeyCode, unsigned char)> KeyEventHandlerFunc;

	enum class BackgroundMode
	{
		StretchToFit,
		PreserveWidth,
		PreserveHeight,
		Tile
	};

	class Scene : public Slot<Scene>, public std::enable_shared_from_this<Scene>
	{
		friend class GameImpl;
		friend class Actor;
		friend class ActionPlayer;

	private:

		ceph::Rect<float> bkgd_rect_;
		std::shared_ptr<Texture> bkgd_;
		BackgroundMode bkgd_mode_;
		std::deque<std::shared_ptr<Actor>> stage_;
		std::list<std::shared_ptr<Actor>> dropped_actors_;
		ColorRGB bkgd_color_;

	protected:
		Signal<float> updateActionsEvent;
		void drawBackground(DrawingContext& dc);
		void endGameLoopIteration();
		void update(float dt);
		Scene();

	public:
		virtual void initialize() = 0;

		void setBackground(const std::shared_ptr<Texture>& tex, BackgroundMode mapping = BackgroundMode::StretchToFit);

		void setBackgroundColor(const ColorRGB& tex);
		ColorRGB getBackgroundColor() const;

		void addActor(const std::shared_ptr<Actor>& child, bool add_on_top = true);
		void addActors(std::initializer_list<std::shared_ptr<Actor>> children, bool add_on_top = true);
		void removeActor(const std::shared_ptr<Actor>& child);

		template<typename T>
		void addActor(const std::shared_ptr<T>& child) { addActor(std::static_pointer_cast<Actor>(child)); }

		void draw(DrawingContext& rt);

		Signal<float> updateEvent;

		virtual ~Scene();

		template<typename T> static std::shared_ptr<T> create() {
			auto scene = std::make_shared<T>();
			scene->initialize();
			return scene;
		}
	};

}