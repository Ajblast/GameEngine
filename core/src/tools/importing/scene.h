#pragma once

#include "common.h"
#include "types/geometry.h"
#include "types/mats/mat4x4.h"

namespace GRAVEngine
{
	namespace Tools
	{
		namespace Importing
		{
			// A node in an imported hierarchy
			struct GRAVAPI node
			{
				std::string m_Name;
				mat4x4 m_Transformation;
				ref<node> m_Parent;

				uint32 m_ChildCount;
				scope<ref<node>[]> m_Children;
				meshIndex m_MeshCount;
				scope<meshIndex[]> m_Meshes;

				node() : node("") {};
				node(const std::string& name) : m_Name(name), m_Parent(nullptr), m_ChildCount(0), m_Children(nullptr), m_MeshCount(0), m_Meshes(nullptr) {};

				//inline const ref<node>& findNode(const std::string& name) const;

				//void addChild(const ref<node>& child);
				//void addChildren()
			};

			class GRAVAPI scene
			{
			public:
				std::string m_Name;

				ref<node> m_RootNode;

				meshIndex m_MeshCount;
				scope<ref<mesh>[]> m_Meshes;
				//materialIndex m_MaterialCount;
				//scope<scope<material>[]> m_Materials;

			public:
				scene() : m_MeshCount(0), m_Meshes(nullptr), m_RootNode(nullptr)  {}
				~scene() {}

				inline const bool hasMeshes() const { return m_MeshCount > 0 && m_Meshes != nullptr; }
				//inline const bool hasMaterials() const { return m_MaterialCount > 0 && m_Materials != nullptr; }

			};
		}
	}
}