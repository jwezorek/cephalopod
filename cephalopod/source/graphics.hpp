#pragma once

#include <vector>
#include "..\include\cephalopod\texture.hpp"
#include "..\include\cephalopod\mat3x3.hpp"
#include "..\include\cephalopod\types.hpp"
#include "glad.h"
#include <GLFW/glfw3.h>

namespace ceph
{
	class Shader;

	struct CoordinateMappingInfo
	{
		ceph::Mat3x3 matrix;
		ceph::Rect<float> viewport;

		CoordinateMappingInfo(const ceph::Mat3x3& m = ceph::Mat3x3::Identity, ceph::Rect<float>& vp = ceph::Rect<float>(0, 0, 0, 0));
	};

	class Graphics
	{
	private:
		std::shared_ptr<Texture> current_texture_;
		GLuint program_id_;
		GLuint vao_ = 0;
		GLuint vbo_ = 0;
		GLFWwindow* window_;
		ceph::Mat3x3 coord_sys_mat_;
		CoordinateMappingInfo coord_mapping_;
		ceph::Vec2<float> log_sz_;
		std::vector<std::unique_ptr<Shader>> shaders_;
		
		void CreateShaders();
		void CreateQuadVerts();

	public:
		Graphics(GLFWwindow* window = nullptr, ceph::CoordinateSystem system = ceph::CoordinateSystem::CenterOriginAscendingY, ceph::CoordinateMapping mapping = ceph::CoordinateMapping::StretchToFit, const ceph::Vec2<float>& log_size = ceph::Vec2<float>(0.0f, 0.0f));
		void setWindow(GLFWwindow* window);
		void setCoordinateSystem(ceph::CoordinateSystem system, ceph::CoordinateMapping mapping, const ceph::Vec2<float>& log_size);
		void BeginFrame();
		void EndFrame();
		void SetCurrentTexture(const std::shared_ptr<Texture>& tex);
		std::shared_ptr<Texture> GetCurrentTexture() const;
		void Clear(ColorRGB color, bool just_viewport = false);
		void Blit(const Mat3x3& mat, const ceph::Rect<int>& src_rect, float alpha);
		void Blit(const Mat3x3& mat, const ceph::Rect<int>& src_rect, const NormalizedColorRGBA& color);
		void Blit(const Mat3x3& mat, const ceph::Rect<float>& dest_rect, const ceph::Rect<int>& src_rect, float alpha );
		void PaintRectangle(const Mat3x3& mat, const ceph::Rect<float>& dest_rect, const ColorRGB& color, float alpha );
		ceph::Mat3x3 getViewMatrix() const;

		~Graphics();
	};
};
