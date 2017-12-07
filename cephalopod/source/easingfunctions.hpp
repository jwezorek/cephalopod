#pragma once

namespace ceph
{
	namespace ease {

		class Back {
		public:
			static float in(float t);
			static float out(float t);
			static float inOut(float t);
		};

		class Bounce {
		public:
			static float in(float t);
			static float out(float t);
			static float inOut(float t);
		};

		class Circ {
		public:
			static float in(float t);
			static float out(float t);
			static float inOut(float t);
		};

		class Cubic {
		public:
			static float in(float t);
			static float out(float t);
			static float inOut(float t);
		};

		class Elastic {
		public:
			static float in(float t);
			static float out(float t);
			static float inOut(float t);
		};

		class Expo {
		public:
			static float in(float t);
			static float out(float t);
			static float inOut(float t);
		};

		class Quad {
		public:
			static float in(float t);
			static float out(float t);
			static float inOut(float t);
		};

		class Quart {
		public:
			static float in(float t);
			static float out(float t);
			static float inOut(float t);
		};

		class Quint {
		public:
			static float in(float t);
			static float out(float t);
			static float inOut(float t);
		};

		class Sine {
		public:
			static float in(float t);
			static float out(float t);
			static float inOut(float t);
		};
	}
}