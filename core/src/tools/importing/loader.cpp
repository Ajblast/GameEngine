#include "gravpch.h"
#include "loader.h"

GRAVEngine::Tools::Importing::loader::loader()
{
	// Potentially have a default list of importers and add them here
	// getImporterInstanceList(m_Importers)
	// Potentially have a default list of post processing steps and add them here
	// getPostProcessingStepInstanceList(m_Importers)
}

GRAVEngine::Tools::Importing::loader::~loader()
{
	// Delete all import plugins
	m_Importers.clear();

	// Delete all postprocessing steps
	//m_PostProcessingSteps.clear()
}

void GRAVEngine::Tools::Importing::loader::registerImporter(const ref<importer>& importer)
{
	GRAV_PROFILE_FUNCTION();

	std::string newExtensions;

	std::set<std::string> ext = importer->getExtensionList();
	for (auto it = ext.begin(); it != ext.end(); it++)
	{
		// Check if this importer's extensions are already supported
		// Not an error, but should still warn that it exists
		#ifdef GRAVCORE_DEBUG
		if (isExtentensionSupported(*it))
			GRAV_LOG_LINE_WARN("The file extension %s is already in use", *it->c_str());
		#endif

		// Add the extension to the list of extensions
		newExtensions += *it;
	}

	// Add this to the list of importers
	m_Importers.push_back(importer);
	GRAV_LOG_LINE_INFO("Registered custom importer with the following extensions: %s", newExtensions.c_str());
}

void GRAVEngine::Tools::Importing::loader::unregisterImporter(const ref<importer>& importer)
{
	auto it = std::find(m_Importers.begin(), m_Importers.end(), importer);

	if (it != m_Importers.end())
	{
		m_Importers.erase(it);
		GRAV_LOG_LINE_INFO("Unregistering custom importer: ");
		return;
	}

	GRAV_LOG_LINE_WARN("Unable to find and remove custom importer");
}

GRAVEngine::scope<GRAVEngine::Tools::Importing::scene> GRAVEngine::Tools::Importing::loader::readFile(const std::string& file, uint32 flags)
{
	GRAV_PROFILE_FUNCTION();

	// Check if the file exists
	if (GRAVEngine::IO::exists(file) && GRAVEngine::IO::isFile(file) == false)
	{
		GRAV_LOG_ERROR("Unable to open file %s", file.c_str());
		return nullptr;
	}

	// Try to find a valid importer
	ref<importer> possibleImporter = nullptr;
	for (auto it = m_Importers.begin(); it != m_Importers.end(); it++)
	{
		if ((*it)->canRead(file, false))
		{
			possibleImporter = *it;
			break;
		}
	}

	// Unable to find valid importrer, now try to find an importer while checking the contents of the file
	if (possibleImporter == nullptr)
	{
		// Check each importer but with checking the contents also
		for (auto it = m_Importers.begin(); it != m_Importers.end(); it++)
		{
			if ((*it)->canRead(file, true))
			{
				possibleImporter = *it;
				break;
			}
		}

		// Still not able to find a valid importer
		if (possibleImporter == nullptr)
		{
			GRAV_LOG_LINE_ERROR("Unable to find suitable importer to the following file format: %s", file.c_str());
			return nullptr;
		}
	}

	std::string extension("Unknown");
	importerDescription desc = possibleImporter->getDescription();
	if (desc.m_Name != nullptr)
		extension = desc.m_Name;

	GRAV_LOG_LINE_INFO("Found matching importer for the following file format: %s", extension.c_str());


	// Read the file
	scope<scene> scene = possibleImporter->readFile(file);

	// If successful, apply the active post processing steps
	if (scene)
	{
		//if (!pimpl->mScene->mMetaData || !pimpl->mScene->mMetaData->HasKey(AI_METADATA_SOURCE_FORMAT)) {
		//	if (!pimpl->mScene->mMetaData) {
		//		pimpl->mScene->mMetaData = new aiMetadata;
		//	}
		//	pimpl->mScene->mMetaData->Add(AI_METADATA_SOURCE_FORMAT, aiString(ext));

		// Preprocess the scene for post-processing
		//{
		//	GRAV_PROFILE_SCOPE("Preprocessing");

			//ScenePreprocessor pre(scene);
			//pre.processScene();
		//}

		// Postprocessing
		//applyPostProcessing(scene, flags);
	}

	// Clear any data allocated by post-process steps
	//mPPShared->clean()

	return scene;
}

std::set<std::string> GRAVEngine::Tools::Importing::loader::getExtensionList() const
{
	std::set<std::string> retVal;
	for (auto it = m_Importers.begin(); it != m_Importers.end(); it++)
	{
		std::set<std::string> importerExtensions = (*it)->getExtensionList();
		retVal.insert(importerExtensions.begin(), importerExtensions.end());
	}

	return retVal;
}

size_t GRAVEngine::Tools::Importing::loader::getImporterIndex(const std::string& extension) const
{
	// Return -1 if there is no extension to find
	if (extension == "")
		return -1;

	// Transform the extension to lowercase
	std::locale loc;
	std::string ext = extension;
	for (auto it = ext.begin(); it != ext.end(); it++)
		ext.replace(it, it, 1, std::tolower(*it, loc));

	// Check each importer
	for (auto importer = m_Importers.begin(); importer != m_Importers.end(); importer++)
	{
		// Get this importer's extensions
		auto extensions = (*importer)->getExtensionList();
		// Check each possible extension
		for (auto it = extensions.begin(); it != extensions.end(); it++)
		{
			// If the extensions match, return the distance between the beginning and the found importer
			if (extension == *it)
				return std::distance(m_Importers.begin(), importer);
		}
	}

	// Return -1 if no importer is found
	return -1;
}

//bool GRAVEngine::Tools::Importing::loader::validatePostProcessingFlags(uint32 flags) const
//{
//	ASSIMP_BEGIN_EXCEPTION_REGION();
//	// run basic checks for mutually exclusive flags
//	if (!_ValidateFlags(pFlags)) {
//		return false;
//	}
//
//	// ValidateDS does not anymore occur in the pp list, it plays an awesome extra role ...
//#ifdef ASSIMP_BUILD_NO_VALIDATEDS_PROCESS
//	if (pFlags & aiProcess_ValidateDataStructure) {
//		return false;
//	}
//#endif
//	pFlags &= ~aiProcess_ValidateDataStructure;
//
//	// Now iterate through all bits which are set in the flags and check whether we find at least
//	// one pp plugin which handles it.
//	for (unsigned int mask = 1; mask < (1u << (sizeof(unsigned int) * 8 - 1)); mask <<= 1) {
//
//		if (pFlags & mask) {
//
//			bool have = false;
//			for (unsigned int a = 0; a < pimpl->mPostProcessingSteps.size(); a++) {
//				if (pimpl->mPostProcessingSteps[a]->IsActive(mask)) {
//
//					have = true;
//					break;
//				}
//			}
//			if (!have) {
//				return false;
//			}
//		}
//	}
//	ASSIMP_END_EXCEPTION_REGION(bool);
//	return true;
//}

//void GRAVEngine::Tools::Importing::loader::applyPostProcessing(const ref<scene>& scene, uint32 flags)
//{
//	// Do nothing if there is not a scene
//	if (scene == nullptr)
//		return;
//	// Do nothing if there are no flags
//	if (flags == 0)
//		return;
//
//	for (auto it = m_PostProcessingSteps.begin(); it != m_PostProcessingSteps.end(); it++)
//	{
//		if ((*it)->isActive(flags))
//		{
//			GRAV_PROFILE_SCOPE("Post Process");
//
//			(*it)->execute(scene);
//		}
//
//		// The post processing step invalidated the scene
//		if (scene == nullptr)
//			break;
//	}
//
//	// Add the list of preprocessing flags
//	if (scene)
//		scene->mPPStepsApplied |= flags;
//
//	// Clear any data allocated by post-process steps
//	mPPShared->clean();
//}
