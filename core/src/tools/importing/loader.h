#pragma once

#include "common.h"
#include "importer.h"
#include "io/files/io.h"
#include <set>
#include <algorithm>
#include <locale>

namespace GRAVEngine
{
	namespace Tools
	{
		namespace Importing
		{
			class GRAVAPI loader
			{
			public:
				loader();
				loader(const loader& other) = delete;
				loader& operator= (const loader& other) = delete;

				~loader();

				// Register an importer
				void registerImporter(const ref<importer>& importer);
				// Unregister an importer
				void unregisterImporter(const ref<importer>& importer);

				//void registerPostProcessingStep(const ref<process>& postProcessStep);
				//void unregisterPostProcessingStep(const ref<process>& postProcessStep);
				//// Check wheter a set of post processing flags is supported
				//bool validatePostProcessingFlags(uint32 flags) const;
				//

				// Read a file
				scope<scene> readFile(const std::string& file, uint32 flags);


				// Is an extension supported
				inline bool isExtentensionSupported(const std::string& extenstion) const { return getImporter(extenstion) != nullptr; }
				// Get the list of extensions
				std::set<std::string> getExtensionList() const;

				inline size_t importerCount() const { return m_Importers.size(); }

				// Get an importer from index
				inline ref<importer> getImporter(size_t index) const {
					if (index >= m_Importers.size())
						return nullptr;
					return m_Importers[index];
				}
				// Get the first importer based on an extension
				inline ref<importer> getImporter(const std::string& extension) const { return getImporter(getImporterIndex(extension)); }

				// Get an importer's index from its extension
				size_t getImporterIndex(const std::string& extension) const;

			private:
				std::vector<ref<importer>> m_Importers;				// Importers associated with this loader
				//std::vector<ref<process>> m_PostProcessingSteps;	// Post processing steps associated with this loader
				// Potential sharedPostProcessInfomation object

				// Maybe don't store scenes in the loader. It will just load the scene and return it to the callng function
				//// Apply post-processing to an already imported scene
				//void applyPostProcessing(const ref<scene>& scene, uint32 flags);


				// A unique scope scene
				//scope<scene> m_Scene;
			};


		}
	}
}