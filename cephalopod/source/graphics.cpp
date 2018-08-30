#include "graphics.hpp"
#include "glad.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <fstream>
#include <streambuf>
#include <unordered_map>
#include <functional>
#include "..\include\cephalopod\mat3x3.hpp"
#include "..\include\cephalopod\texture.hpp"

ceph::CoordinateMappingInfo::CoordinateMappingInfo(const ceph::Mat3x3& m, ceph::Rect<float>& vp) :
	matrix(m), viewport(vp)
{
}

namespace
{
	struct Vertex
	{
		float XY[2];
	};



	const unsigned int VERT_SHADER_XY = 0;

	std::string vert_shader_code =
		R"(
		#version 400

		layout(location = 0) in vec2 vert;

		uniform mat3 mat;
		uniform vec2 uv[4];

		out vec2 fragTexCoord;

		void main() {
			fragTexCoord = uv[gl_VertexID];
			
			vec3 v = mat * vec3( vert, 1.0 );

			gl_Position = vec4( v.x, v.y, 0.0, 1.0 );
		}
	)";

	std::string frag_shader_code =
		R"(
		#version 400

		uniform sampler2D tex;
		uniform vec4 tintColor;
		in vec2 fragTexCoord;
		out vec4 finalColor;

		void main() {
			finalColor = texture(tex, fragTexCoord) * tintColor ;
		}
	)";

	ceph::Vec2<float> getWindowSize(GLFWwindow* window)
	{
		int wd, hgt;
		glfwGetWindowSize(window, &wd, &hgt);
		return ceph::Vec2<float>(static_cast<float>(wd), static_cast<float>(hgt));
	}

	ceph::Mat3x3 CreateLowerLeftOriginAscendingYMatrix()
	{
		return ceph::Mat3x3().translate(-1.0f, -1.0f);
	}

	ceph::Mat3x3 CreateUpperLeftOriginDescendingYMatrix()
	{
		return ceph::Mat3x3().scale(1.0f, -1.0f).translate(-1.0f, -1.0f);
	}

	ceph::Mat3x3 CreateCoordinateSystemMatrix(ceph::CoordinateSystem system)
	{
		static std::unordered_map<ceph::CoordinateSystem, std::function<ceph::Mat3x3()>> creation_funcs{
			{ ceph::CoordinateSystem::CenterOriginAscendingY,  []() {return ceph::Mat3x3::Identity; } },
			{ ceph::CoordinateSystem::LowerLeftOriginAscendingY, CreateLowerLeftOriginAscendingYMatrix },
			{ ceph::CoordinateSystem::UpperLeftOriginDescendingY, CreateUpperLeftOriginDescendingYMatrix }
		};

		return creation_funcs[system]();
	}

	ceph::Mat3x3 CreateStretchToFitMatrix(float logical_wd, float logical_hgt)
	{
		return ceph::Mat3x3().scale(
			2.0f / logical_wd,
			2.0f / logical_hgt
		);
	}
	
	ceph::CoordinateMappingInfo CreateStretchToFit(float scr_wd, float scr_hgt, float logical_wd, float logical_hgt)
	{
		return CreateStretchToFitMatrix(logical_wd, logical_hgt);
	}

	ceph::CoordinateMappingInfo CreatePreserveHeight(float scr_wd, float scr_hgt, float logical_wd, float logical_hgt)
	{
		float scr_to_log_ratio = logical_hgt / scr_hgt;
		logical_wd = scr_wd * scr_to_log_ratio;
		return CreateStretchToFitMatrix(logical_wd, logical_hgt);
	}

	ceph::CoordinateMappingInfo CreatePreserveWidth(float scr_wd, float scr_hgt, float logical_wd, float logical_hgt)
	{
		float scr_to_log_ratio = logical_wd / scr_wd;
		logical_hgt = scr_hgt * scr_to_log_ratio;
		return CreateStretchToFitMatrix(logical_wd, logical_hgt);
	}

	ceph::CoordinateMappingInfo CreateUseBlackBars(float scr_wd, float scr_hgt, float logical_wd, float logical_hgt)
	{
		bool vert_bars = logical_wd / logical_hgt < scr_wd / scr_hgt;
		float log_to_scr = (vert_bars) ? scr_hgt / logical_hgt : scr_wd / logical_wd;

		float scr_vp_wd = logical_wd * log_to_scr;
		float scr_vp_hgt = logical_hgt * log_to_scr;

		return ceph::CoordinateMappingInfo(
			CreateStretchToFitMatrix(logical_wd, logical_hgt),
			ceph::Rect<float>(
			(scr_wd - scr_vp_wd) / 2.0f,
				(scr_hgt - scr_vp_hgt) / 2.0f,
				scr_vp_wd,
				scr_vp_hgt
				)
		);
	}

	ceph::CoordinateMappingInfo CreateCoordinateMapping(ceph::CoordinateMapping mapping, float scr_wd, float scr_hgt, float logical_wd, float logical_hgt)
	{
		static std::unordered_map<ceph::CoordinateMapping, std::function<ceph::CoordinateMappingInfo(float, float, float, float)>> creation_funcs{
			{ ceph::CoordinateMapping::UseBlackBars, CreateUseBlackBars },
			{ ceph::CoordinateMapping::StretchToFit, CreateStretchToFit },
			{ ceph::CoordinateMapping::PreserveHeight, CreatePreserveHeight },
			{ ceph::CoordinateMapping::PreserveWidth, CreatePreserveWidth }
		};

		return creation_funcs[mapping](scr_wd, scr_hgt, logical_wd, logical_hgt);
	}

}

namespace ceph
{
	class Shader
	{
	private:
		GLint id_;

	public:
		Shader(const std::string& src, GLenum shaderType)
		{
			//create the shader object
			id_ = glCreateShader(shaderType);
			if (id_ == 0)
				throw std::runtime_error("glCreateShader failed");

			//set the source code
			const char* code = src.c_str();
			glShaderSource(id_, 1, (const GLchar**)&code, NULL);

			//compile
			glCompileShader(id_);

			//throw exception if compile error occurred
			GLint status;
			glGetShaderiv(id_, GL_COMPILE_STATUS, &status);
			if (status == GL_FALSE) {
				std::string msg("Compile failure in shader:\n");

				GLint infoLogLength;
				glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &infoLogLength);
				char* strInfoLog = new char[infoLogLength + 1];
				glGetShaderInfoLog(id_, infoLogLength, NULL, strInfoLog);
				msg += strInfoLog;
				delete[] strInfoLog;

				glDeleteShader(id_);
				id_ = 0;
				throw std::runtime_error(msg);
			}
		}

		~Shader()
		{
			glDeleteShader(id_);
			id_ = 0;
		}

		GLint getID() const
		{
			return id_;
		}
	};
}

ceph::Graphics::Graphics(GLFWwindow* window, ceph::CoordinateSystem system, ceph::CoordinateMapping mapping, const ceph::Vec2<float>& log_size)
{
	// OpenGL settings
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//create the program object
	program_id_ = glCreateProgram();
	if (program_id_ == 0)
		throw std::runtime_error("glCreateProgram failed");

	// load vertex and fragment shaders into opengl
	CreateShaders();
	// create buffer and fill it with the points of the triangle
	CreateQuadVerts();

	// bind the program (the shaders)
	glUseProgram(program_id_);

	setWindow(window);
	setCoordinateSystem(system, mapping, log_size);
}

void ceph::Graphics::setWindow(GLFWwindow* window)
{
	window_ = window;
}

void ceph::Graphics::setCoordinateSystem(ceph::CoordinateSystem system, ceph::CoordinateMapping mapping, const ceph::Vec2<float>& log_size)
{
	coord_sys_mat_ = CreateCoordinateSystemMatrix(system);

	auto wnd_sz = (window_) ? getWindowSize(window_) : ceph::Vec2<float>(0, 0);
	log_sz_ = (log_size.x > 0 && log_size.y > 0) ? log_size : wnd_sz;

	coord_mapping_ = CreateCoordinateMapping(mapping, wnd_sz.x, wnd_sz.y, log_sz_.x, log_sz_.y);

	auto vp = coord_mapping_.viewport;
	if (!vp.isEmpty())
		glViewportIndexedf(0, vp.x, vp.y, vp.wd, vp.hgt);
}

void ceph::Graphics::BeginFrame()
{
	Clear(ceph::ColorRGB(0, 0, 0));
}

void ceph::Graphics::EndFrame()
{
	// swap the display buffers (displays what was just drawn)
	glfwSwapBuffers(window_);
}

void ceph::Graphics::SetCurrentTexture(const std::shared_ptr<ceph::Texture>& tex)
{
	current_texture_ = tex;
	tex->bind();
	GLint uniform_id = glGetUniformLocation(program_id_, "tex");
	glUniform1i(uniform_id, 0);
}

std::shared_ptr<ceph::Texture> ceph::Graphics::GetCurrentTexture() const
{
	return current_texture_;
}

void ceph::Graphics::Blit(const ceph::Rect<float>& dest_rect, const ceph::Rect<int>& src_rect, float alpha)
{
	auto mat = ceph::Mat3x3().scale(dest_rect.wd, dest_rect.hgt).translate(dest_rect.x, dest_rect.y);
	Blit(
		ceph::Mat3x3().translate(dest_rect.x, dest_rect.y).scale(dest_rect.wd, dest_rect.hgt),
		src_rect,
		alpha
	);
}

void ceph::Graphics::Clear(ceph::ColorRGB color)
{
	auto to_float = [](unsigned char c) { return static_cast<float>(c) / 255.0f; };
	glClearColor(to_float(color.r), to_float(color.g), to_float(color.b), 1); 
	glClear(GL_COLOR_BUFFER_BIT);
}

void ceph::Graphics::Blit(const ceph::Mat3x3& matWorld, const ceph::Rect<int>& src_rect, float alpha)
{
	float current_texture_wd = current_texture_->getWidth();
	float current_texture_hgt = current_texture_->getHeight();

	float left = static_cast<float>(src_rect.x) / current_texture_wd;
	float top = static_cast<float>(src_rect.y) / current_texture_hgt;
	float right = static_cast<float>(src_rect.x + src_rect.wd) / current_texture_wd;
	float bottom = static_cast<float>(src_rect.y + src_rect.hgt) / current_texture_hgt;

	float uv[8] = { left,bottom , right,bottom , right,top, left,top };
	glUniform2fv(glGetUniformLocation(program_id_, "uv"), 4, uv);

	ceph::Mat3x3 matWorldView = getViewMatrix() * matWorld;

	auto mat3x3 = matWorldView.get();
	glUniformMatrix3fv(glGetUniformLocation(program_id_, "mat"), 1, GL_FALSE, &(mat3x3[0]));

	glUniform4f(glGetUniformLocation(program_id_, "tintColor"), 1.0, 1.0, 1.0, alpha);

	// draw the VAO
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

ceph::Mat3x3 ceph::Graphics::getViewMatrix() const
{
	return coord_sys_mat_ * coord_mapping_.matrix;
}

void ceph::Graphics::CreateShaders()
{
	shaders_.emplace_back(std::make_unique<ceph::Shader>(vert_shader_code, GL_VERTEX_SHADER));
	shaders_.emplace_back(std::make_unique<ceph::Shader>(frag_shader_code, GL_FRAGMENT_SHADER));

	//attach all the shaders
	for (unsigned i = 0; i < shaders_.size(); ++i)
		glAttachShader(program_id_, shaders_[i]->getID());

	//link the shaders together
	glLinkProgram(program_id_);

	//detach all the shaders
	for (unsigned i = 0; i < shaders_.size(); ++i)
		glDetachShader(program_id_, shaders_[i]->getID());

	//throw exception if linking failed
	GLint status;
	glGetProgramiv(program_id_, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		std::string msg("Program linking failure: ");

		GLint infoLogLength;
		glGetProgramiv(program_id_, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* strInfoLog = new char[infoLogLength + 1];
		glGetProgramInfoLog(program_id_, infoLogLength, NULL, strInfoLog);
		msg += strInfoLog;
		delete[] strInfoLog;

		glDeleteProgram(program_id_);
		program_id_ = 0;
		throw std::runtime_error(msg);
	}
}

void ceph::Graphics::CreateQuadVerts() {
	// make and bind the VAO
	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	// make and bind the VBO
	glGenBuffers(1, &vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);

	// Put the three triangle vertices (XYZ) and texture coordinates (UV) into the VBO
	Vertex verts[] = {
		{{ 0.0f, 0.0f }},
		{{ 1,    0.0f }},
		{{ 1,    1 }},
		{{ 0.0f, 1 }}
	};
	const size_t vert_size = sizeof(verts[0]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	// connect the xyz to the "vert" attribute of the vertex shader
	glEnableVertexAttribArray(VERT_SHADER_XY);
	glVertexAttribPointer(VERT_SHADER_XY, 2, GL_FLOAT, GL_FALSE, vert_size, nullptr);

}

ceph::Graphics::~Graphics()
{
	// unbind the VAO, the program and the texture
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}
