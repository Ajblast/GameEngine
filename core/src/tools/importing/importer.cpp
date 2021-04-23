#include "gravpch.h"
#include "importer.h"

GRAVEngine::Tools::Importing::importer::importer(const importer&& other) noexcept :
	m_ApplicationUnits(other.m_ApplicationUnits), m_ImporterUnits(other.m_ImporterUnits), m_ImporterScale(other.m_ImporterScale), m_FileScale(other.m_FileScale), m_FileExtensions(other.m_FileExtensions)
{
}

GRAVEngine::Tools::Importing::importer& GRAVEngine::Tools::Importing::importer::operator=(const importer&&other) noexcept
{
	if (this != &other)
	{
		// Take the other's information
		m_ApplicationUnits = other.m_ApplicationUnits;
		m_ImporterUnits = other.m_ImporterUnits;
		m_ImporterScale = other.m_ImporterScale;
		m_FileScale = other.m_FileScale;

		m_FileExtensions = other.m_FileExtensions;
	}

	return *this;
}

GRAVEngine::scope<GRAVEngine::Tools::Importing::scene> GRAVEngine::Tools::Importing::importer::readFile(const std::string& path)
{
	// Create a new scene object
	scope<scene> sc = createScope<scene>();

	// Import the file
	try
	{
		// Run the internal read file
		internalReadFile(path, sc);
	}
	catch (const std::exception& e)
	{
		// An error occured
		GRAV_LOG_ERROR(e.what());
		return nullptr;
	}

	return sc;
}
