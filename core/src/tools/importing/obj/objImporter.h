#pragma once

#include "common.h"
#include "tools/importing/importer.h"
#include "objModel.h"

namespace GRAVEngine
{
	namespace Tools
	{
		namespace Importing
		{
			class objImporter : public importer
			{
			public:
				objImporter();
				~objImporter() {};

				bool canRead(const std::string& path, bool checkSig) const override;

			private:
				void internalReadFile(const std::string& path, scope<scene>& scene) override;
				void setupProperties(const ref<importer>& importer) override;
				const importerDescription getDescription() const override;

				// Create the scene from an import
				void createDataFromImport(const ref<objModel>& model, scope<scene>& scene);
				// Create a node and its subnodes
				ref<node> createNode(const ref<objModel>& model, const ref<objObject>& object, ref<node>& parent, scope<scene>& scene, std::vector<ref<mesh>>& meshArray);
				// Create the topology for a specific mesh
				ref<mesh> createTopology(const ref<objModel>& model, const ref<objObject>& object, meshIndex meshIndex);

				void createVertexArray(const ref<objModel>& model, const ref<objObject>& object, meshIndex meshIndex, ref<mesh>& newMesh, vertexIndex vertCount);

				////! \brief  Creates vertices from model.
				//void createVertexArray(const ObjFile::Model* pModel, const ObjFile::Object* pCurrentObject,
				//	unsigned int uiMeshIndex, aiMesh* pMesh, unsigned int numIndices);
			};
		}
	}
}