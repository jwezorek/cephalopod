#include "sceneimpl.hpp"
#include "drawingcontext.hpp"
#include "../include/cephalopod/texture.hpp"
#include "textureimpl.hpp"
#include "util.hpp"

ceph::SceneImpl::SceneImpl() : dropped_actors_()
{
	coordinate_mapping_mode_ = ceph::CoordinateMapping::StretchToFit;
}

void ceph::SceneImpl::drawBackground(const DrawingContext & dc)
{
	dc.target.draw(bkgd_, dc.transform * bkgd_mat_);
}

void ceph::SceneImpl::setBackground(const std::shared_ptr<Texture>& tex, ceph::CoordinateMapping mapping)
{
	auto& game = ceph::Game::getInstance();
	bkgd_tex_ = tex;
	bkgd_tex_->impl_->sfml_impl_.setSmooth(true);
	bkgd_ = sf::Sprite(tex->impl_->sfml_impl_);

	sf::Transform matrix;
	switch (mapping) {
	case ceph::CoordinateMapping::StretchToFit:
		matrix = ceph::getStretchToFitMatrix(tex->getSize(), game.getLogicalSize());
		break;
	case ceph::CoordinateMapping::UseBlackBars:
		matrix = ceph::getUseBlackBarsMatrix(tex->getSize(), game.getLogicalSize());
		break;
	case ceph::CoordinateMapping::PreserveHeight:
		matrix = ceph::getCenterBkgdMatrix(tex->getSize(), game.getLogicalSize(), true);
		break;
	case ceph::CoordinateMapping::PreserveWidth:
		matrix = ceph::getCenterBkgdMatrix(tex->getSize(), game.getLogicalSize(), false);
		break;
	}
	auto offset = game.getLogicalRect().getLocation();
	bkgd_mat_ = sf::Transform().translate(
		offset.x, offset.y
	).combine(
		matrix
	);
}
