#pragma once

namespace ceph {

	class Action;
	class Scene;

	class ActionPlayer : public Slot<ActionPlayer>
	{
	private:
		std::vector<std::shared_ptr<Action>> actions_;
		void update(float dt);

	public:
		void run(Scene& scene);
		bool hasActions() const;
		void applyAction(const std::shared_ptr<ceph::Action>& action);
		void applyActions(std::initializer_list<std::shared_ptr<Action>> actions);
		void removeAction(const std::shared_ptr<ceph::Action>& removee);
	};

}