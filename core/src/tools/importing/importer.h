#pragma once

#include <string>
#include <set>
#include <map>

#include "common.h"
#include "importerDescription.h"
#include "scene.h"

namespace GRAVEngine
{
	namespace Tools
	{
		namespace Importing
		{
			enum class importerUnits{ M, MM, CM, INCHES, FEET };

			class importer
			{
			public:
				importer() : m_ImporterScale(1), m_FileScale(1) {}
				importer(const importer& other) = delete;		// Copy constructor deleted due to memory allocation
				importer(const importer&& other) noexcept;

				importer& operator= (const importer&) = delete;	// Copy operator deleted due to memory allocation
				importer& operator= (const importer&& other) noexcept;

				virtual ~importer() {}

				// Can the importer read this file type
				// If checkSig is enabled, the function will peek into the file to find magic numbers or known keywords
				virtual bool canRead(const std::string& path, bool checkSig) const = 0;


				// Read a file and return a new scene object
				scope<scene> readFile(const std::string& path);

				// Is an extension supported
				inline const bool isExtentensionSupported(const std::string& extenstion) const { auto ext = getExtensionList(); return ext.find(extenstion) != ext.end(); }
				// Get the list of extensions
				inline std::set<std::string> getExtensionList() const { return m_FileExtensions; }


				// Update the importer's configuration
				virtual void setupProperties(const ref<importer>& importer) = 0;
				// Get an importer's description info
				virtual const importerDescription getDescription() const = 0;


				virtual void setFileScale(double scale) { m_FileScale = scale; }
				virtual double getFileScale() const { return m_FileScale; }
				virtual void setApplicationUints(const importerUnits& unit) { m_ImporterScale = m_ImporterUnits[unit]; m_ApplicationUnits = unit; }
				virtual const importerUnits& getApplicationUnits() { return m_ApplicationUnits; }
			protected:
				// The internal read function that should be overloaded
				virtual void internalReadFile(const std::string& path, scope<scene>& scene) = 0;
			private:
				// Imports the given file into the scene structure
				// At least a root node must be there
				// A vertex index may not occur twice in a single mesh
				// m_Animation::m_Duration may be -1. Determines the length of the animation automatically
				// m_Mesh::m_Bitangents may be nullptr if tangents and normals are given.In this case bitangents are computed as the cross product between normaland tangent
				// There needn't be a material. If none is there, a default material is generated.
				virtual void updateImporterScale(const ref<importer>& importer);

			public:
				// Map used for storing importer units
				std::map<importerUnits, double> m_ImporterUnits = {
					{importerUnits::M,		1.0},
					{importerUnits::MM,		0.01},
					{importerUnits::CM,		0.001},
					{importerUnits::INCHES, 0.0254},
					{importerUnits::FEET,	0.3048}
				};

			protected:
				importerUnits m_ApplicationUnits = importerUnits::M;	// Applicatoin importer units
				double m_ImporterScale;									// This importer's scale
				double m_FileScale;										// The file's scale

				// List of file extensions an importer can handle separated by spaces. Extensions do not have the leading dot.
				std::set<std::string> m_FileExtensions;

			};
		}
	}
}