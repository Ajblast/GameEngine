#pragma once

#include "common.h"
#include "rendering/textures/texture2D.h"

#include <glad/glad.h>

namespace GRAVEngine
{
	namespace Rendering
	{
		class openglTexture2D : public texture2D
		{
		public:
			openglTexture2D(uint32 width, uint32 height);
			openglTexture2D(const std::string& path);
			virtual ~openglTexture2D();

			virtual void setData(void* data, uint32 size) override;

			virtual void bind(uint32 slot = 0) const override;

			virtual bool operator==(const texture& other) const override
			{
				return m_RendererID == ((openglTexture2D&)other).m_RendererID;
			}

		private:
			GLenum m_InternalFormat, m_DataFormat;
		};
	}
}