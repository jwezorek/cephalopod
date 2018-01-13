#pragma once
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>

namespace ceph {
	template<class... Args> class Signal;
	template<class T> class Slot;

	namespace details {
		template<class... Args>
		class SubscriberBase {
		public:
			virtual void call(Args... args) = 0;
			virtual void* getInstance() = 0;
			virtual ~SubscriberBase() { }
		};

		template<class T, class... Args>
		class Subscriber : public details::SubscriberBase<Args...> {
		private:
			void* t;
			std::function<void(Args...)> func;
		public:
			Subscriber(T* t, void(T::*f)(Args...)) :
				t(t),
				func(
					[t, f](Args...args) {(t->*f)(args...); }
				) 
			{ }

			Subscriber(T* t, const std::function<void(Args...)>& f) : t(t), func(f) 
			{ }

			void call(Args... args) override
			{
				func(args...);
			}

			void* getInstance() override
			{
				return static_cast<void*>(t);
			}

			~Subscriber() final {  }
		};

		class SlotBase {
		public:
			virtual ~SlotBase() = default;
		private:
			template<class... Args> friend class Signal;
		protected:
			virtual void removeEvent(void* ev) = 0;
		};

		class SignalBase {
			template<class Receiver> friend class Slot;
		protected:
			virtual void removeSubscriber(void* t) = 0;
		public:
			virtual ~SignalBase() {
			}
		};
	}

	template<class... Args>
	class Signal : public details::SignalBase {
		template<class Receiver> friend class Slot;

	private:

		int firing_count_ = 0;

		class FiringLock
		{
		private:
			Signal* parent_;
		public:
			FiringLock(Signal* parent) : parent_(parent) { (parent_->firing_count_)++; }
			~FiringLock() 
			{
				if (--parent_->firing_count_ == 0) {
					while (!parent_->deferred_ops_.empty()) {
						auto do_signal_op = parent_->deferred_ops_.back();
						parent_->deferred_ops_.pop_back();
						do_signal_op();
					}
				}
			}
		};

		std::vector<std::unique_ptr<details::SubscriberBase<Args...>>> subscribers;

		template<class T>
		void addSubscriber(details::Subscriber<T, Args...>* subscriber) {
			subscribers.push_back(std::unique_ptr<details::SubscriberBase<Args...>>(subscriber));
		}

		void removeSubscriber(void* t) override {
			auto to_remove = std::remove_if(
				subscribers.begin(),
				subscribers.end(),
				[t](auto& elt) { return elt->getInstance() == t; }
			);
			subscribers.erase(to_remove, subscribers.end());
		}

		std::vector<std::function<void()>> deferred_ops_;

	public:
		// Signals are move only.
		Signal() = default;
		Signal(const Signal&) = delete;
		Signal& operator=(const Signal&) = delete;
		Signal(Signal&& a) = default;
		Signal& operator=(Signal&& a) = default;

		void fire(Args... args) {
			FiringLock lock(this);
			for (auto& s : subscribers)
				s->call(args...);
		}

		bool isFiring() const
		{
			return firing_count_ > 0;
		}

		template<class T>
		void connect(Slot<T>& t, void(T::*f)(Args... args)) {
			if (!isFiring()) {
				t.connect(*this, f);
			} else {
				deferred_ops_.push_back(
					[&]() {
						this->connect(t, f);
					}
				);
			}
		}

		template<class T>
		void connect(Slot<T>& t, const std::function<void(Args... args)>& f) {
			if (!isFiring()) {
				t.connect(*this, f);
			} else {
				deferred_ops_.push_back(
					[&]() {
						this->connect(t, f);
					}
				);
			}
		}

		template<class T>
		void disconnect(Slot<T>& t) {
			if (! isFiring()) {
				t.disconnect(*this);
			} else {
				deferred_ops_.push_back(
					[&]() {
						this->disconnect(t);
					}
				);
			}
		}

		~Signal() {
			for (auto& s : subscribers)
				static_cast<details::SlotBase*>(s->getInstance())->removeEvent(this);
		}
	};

	template<class Receiver>
	class Slot : public details::SlotBase {
	private:
		std::vector<details::SignalBase*> events;

		void removeEvent(void* ev) final {
			auto to_remove = std::remove_if(
				events.begin(),
				events.end(),
					[ev](auto& elt) {
					return elt == static_cast<details::SignalBase*>(ev);
				}
			);
			events.erase(to_remove, events.end());
		}

	public:
		/*
		template<class... Args>
		void connect(Signal<Args...>& ev, void (Receiver::*listenerMethod)(Args... args)) {
			ev.addSubscriber(new details::Subscriber<Receiver, Args...>(static_cast<Receiver*>(this), listenerMethod));
			events.push_back(&ev);
		}
		*/
		template<class U, class... Args>
		void connect(Signal<Args...>& ev, void (U::*listenerMethod)(Args... args)) {
			ev.addSubscriber(new details::Subscriber<U, Args...>(static_cast<U*>(this), listenerMethod));
			events.push_back(&ev);
		}

		template<class... Args>
		void connect(Signal<Args...>& ev, const std::function<void(Args... args)>& f) {
			ev.addSubscriber(new details::Subscriber<Receiver, Args...>(static_cast<Receiver*>(this), f));
			events.push_back(&ev);
		}

		template<class... Args>
		void disconnect(Signal<Args...>& ev)
		{
			ev.removeSubscriber(this);
			removeEvent(&ev);
		}

		~Slot() {
			for (auto& e : events)
				e->removeSubscriber(static_cast<void*>(this));
		}
	};
}