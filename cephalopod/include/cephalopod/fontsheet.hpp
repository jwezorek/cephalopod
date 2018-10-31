#include "spritesheet.hpp"
#include <vector>

namespace ceph
{
	class FontSheet : protected SpriteSheet
	{
	public:

		struct FontItem
		{
			std::string name;
			std::string filepath;
			int size;

			FontItem(const std::string& n, const std::string& fp, const int sz) :
				name(n),
				filepath(fp),
				size(sz)
			{}

			FontItem(const std::string& fp, const int sz = 12) :
				FontItem(fp, fp, sz)
			{ }
		};

		FontSheet(const std::vector<FontItem>& fonts);
	};
}