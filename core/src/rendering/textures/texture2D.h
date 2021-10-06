#pragma once

#include "common.h"
#include "texture.h"

namespace GRAVEngine
{
	namespace Rendering
	{
		class GRAVAPI texture2D : public texture
		{
		public:
			texture2D(uint32 width, uint32 height) : m_Width(width), m_Height(height), m_RendererID(0) {}
			texture2D(const std::string& path) : m_Width(0), m_Height(0), m_Path(path), m_RendererID(0) {}
			virtual ~texture2D() = default;

			virtual uint32_t getWidth() const override { return m_Width; }
			virtual uint32_t getHeight() const override { return m_Height; }
			virtual uint32_t getRendererID() const override { return m_RendererID; }
		protected:
			std::string m_Path;
			uint32 m_Width, m_Height;
			uint32 m_RendererID;
			//public:
				//static texture2D* create(uint32 width, uint32 height);
				//static texture2D* create(const std::string& path);
		};
	}
}