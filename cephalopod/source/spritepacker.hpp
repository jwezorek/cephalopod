#pragma once

#include <vector>
#include <optional>
#include <algorithm>
#include <functional>
#include "..\include\cephalopod\types.hpp"

namespace ceph {

	using T = Rect<int>;

	namespace details
	{
		std::pair<Rect<int>, Rect<int>> splitVertically(const Rect<int>& self, int y)
		{
			auto top = Rect<int>(self.x, self.y, self.wd, y);
			auto bottom = Rect<int>(self.x, self.y + y, self.wd, self.hgt - y);
			return { top, bottom };
		}

		std::pair<Rect<int>, Rect<int>> splitHorizontally(const Rect<int>& self, int x)
		{
			auto left = Rect<int>(self.x, self.y, x, self.hgt);
			auto right = Rect<int>(self.x + x, self.y, self.wd - x, self.hgt);
			return { left, right };
		}

		struct PackingNode
		{
			T* sprite;
			Rect<int> rect;
			std::unique_ptr<PackingNode> children[2];

			PackingNode(T* s, const Rect<int>& r) : sprite(s), rect(r), children{ nullptr, nullptr }
			{}

			PackingNode(const Vec2<int>& sz) : PackingNode(nullptr, Rect<int>(0, 0, sz.x, sz.y))
			{}

			PackingNode(const PackingNode& node)
			{
				sprite = node.sprite;
				rect = node.rect;
				children[0] = (node.children[0]) ? std::make_unique<PackingNode>( *node.children[0] ) : nullptr;
				children[1] = (node.children[1]) ? std::make_unique<PackingNode>( *node.children[1] ) : nullptr;
			}

			bool isLeaf() const
			{
				return !children[0] && !children[1];
			}

			bool isEmptyLeaf() const
			{
				return isLeaf() && !sprite;
			}

			details::PackingNode* findEmptyLeaf(const Vec2<int>& sz)
			{
				if (isEmptyLeaf())
					return canContain(sz) ? this : nullptr;

				if (isLeaf())
					return nullptr;
				auto leaf = children[0]->findEmptyLeaf(sz);
				return (leaf) ? leaf : children[1]->findEmptyLeaf(sz);
			}

			bool canContain(const Vec2<int>& sz) const
			{
				return rect.wd >= sz.x && rect.hgt >= sz.y;
			}

			bool isCongruentWith(const Vec2<int>& sz) const
			{
				return rect.wd == sz.x && rect.hgt == sz.y;
			}

			bool shouldSplitVertically(const Vec2<int>& sz) const
			{
				if (rect.wd == sz.x)
					return true;
				else if (rect.hgt == sz.y)
					return false;
				auto vert_rects = splitVertically(rect, sz.y);
				auto horz_rects = splitHorizontally(rect, sz.x);
				return vert_rects.second.area() > horz_rects.second.area();
			}

			void splitNode(T& new_sprite)
			{
				if (!isLeaf())
					std::runtime_error("Attempted to split non-leaf");

				auto new_sprite_sz = new_sprite.getSize();
				if (!canContain(new_sprite_sz))
					throw std::runtime_error("Attempted to place an img in a node it doesn't fit");

				//if it fits exactly then we are done...
				if (isCongruentWith(new_sprite_sz)) {
					this->sprite = &new_sprite;
					return;
				}

				auto rects = (shouldSplitVertically(new_sprite_sz)) ?
					splitVertically(rect, new_sprite_sz.y) :
					splitHorizontally(rect, new_sprite_sz.x);

				children[0] = std::make_unique<PackingNode>(nullptr, rects.first);
				children[1] = std::make_unique<PackingNode>(nullptr, rects.second);

				children[0]->splitNode(new_sprite);
			}

			// TODO: should change the logic here so that we pick vert or horz based on a heuristic
			// regardless of whether we can grow without resizing in both directions, and then handle the 
			// case in which we need to resize in both directions correctly.

			bool shouldGrowVertically(const ceph::Vec2<int>& sprite_sz)
			{
				bool can_grow_vert = rect.wd >= sprite_sz.x;  // can grow vertically without having to resize horz
				bool can_grow_horz = rect.hgt >= sprite_sz.y; // can grow horz without having to resize vertically
				if (!can_grow_vert && !can_grow_horz)
					throw std::runtime_error("Unable to grow!"); //TODO: should grow horz & vert here...
				if (can_grow_vert && !can_grow_horz)
					return true;
				if (can_grow_horz && !can_grow_vert)
					return false;
				return (rect.hgt + sprite_sz.y < rect.wd + sprite_sz.x);
			}

			void growNode(T& new_sprite)
			{
				if (isEmptyLeaf())
					throw std::runtime_error("Attempted to grow an empty leaf");
				auto sprite_sz = new_sprite.getSize();
				auto new_child = std::make_unique<PackingNode>(*this);
				sprite = nullptr;
				if (shouldGrowVertically(sprite_sz)) {
					children[0] = std::move(new_child);
					children[1] = std::make_unique<PackingNode>(nullptr, Rect<int>(rect.x, rect.y2(), rect.wd, sprite_sz.y));
					rect.hgt += sprite_sz.y;
				} else {
					children[0] = std::move(new_child);
					children[1] = std::make_unique<PackingNode>(nullptr, Rect<int>(rect.x2(), rect.y, sprite_sz.x, rect.hgt));
					rect.wd += sprite_sz.x;
				}
				children[1]->splitNode(new_sprite);
			}

			void traverse(const std::function<void(PackingNode&)>& visit)
			{
				visit(*this);
				if (children[0])
					children[0]->traverse(visit);
				if (children[1])
					children[1]->traverse(visit);
			}
		};
	}

	void PackSprites(std::vector<T>& rects, std::optional<Vec2<int>> packing_sz = std::nullopt)
	{
		bool is_grow_mode = !packing_sz.has_value();
		std::vector<T*> ptr_rects(rects.size());
		std::transform(rects.begin(), rects.end(), ptr_rects.begin(), [](T& r){ return &r; });
		std::sort(ptr_rects.begin(), ptr_rects.end(), 
			[](T* r1, T* r2) { 
				return std::max(r1->wd, r1->hgt) > std::max(r2->wd, r2->hgt); 
			}
		);

		std::unique_ptr<details::PackingNode> root;
		for (auto p : ptr_rects) {
			auto& sprite = *p;
			if (!root) {
				root = (is_grow_mode) ?
					std::make_unique<details::PackingNode>(sprite.getSize()) :
					std::make_unique<details::PackingNode>(packing_sz.value());
				root->splitNode(sprite);
				continue;
			}

			auto leaf = root->findEmptyLeaf(sprite.getSize());
			if (leaf) {
				leaf->splitNode(sprite);
				continue;
			}

			if (is_grow_mode)
				root->growNode(sprite);
			else
				throw std::runtime_error("Can't pack images into this size");
		}

		root->traverse(
			[](details::PackingNode& node) { 
				if (node.sprite)
					node.sprite->setLocation(node.rect.getLocation());
			}
		);
	}
};