#pragma once

#include <vector>
#include <string>

#include "io/textFile.h"
#include "types/vecs/vecs.h"
#include "objModel.h"

namespace GRAVEngine
{
	namespace Tools
	{
		namespace Importing
		{
			class objParser
			{
				typedef std::vector<std::string> tokenArray;

			public:
				objParser(IO::textFile& file, const std::string& modelName, const std::string& folderName) : m_File(file), m_ModelName(modelName), m_FolderName(folderName)
				{
					parseFile();
				}
				objParser(const objParser& other) = delete;
				objParser& operator=(const objParser&) = delete;

				~objParser();

				inline const ref<objModel>& getModel() const { return m_Model; }

			private:
				void parseFile();

				// Get a homogeneous coordinate. Divides the x, y, and z by w
				void getHomogeneousVec3(std::vector<vec3>& pointArray, tokenArray& tokens);
				// Get a vec3 from the tokens
				void getVec3(std::vector<vec3>& pointArray, tokenArray& tokens);
				// Get a vec2 from the tokens
				void getVec2(std::vector<vec2>& pointArray, tokenArray& tokens);

				// Get a face from the tokens
				void getFace(primitiveType type, tokenArray& tokens);
				// Get a texture coordinate from the tokens
				uint8 getTextureCoordinate(std::vector<vec2>& textArray, tokenArray& tokens);


				void getGroupName(tokenArray& tokens);
				void getGroupNumber(tokenArray& tokens);

				void getObjectName(tokenArray& tokens);
				void createObject(const std::string& objectName);

				void createMesh(const std::string& meshName);
			private:
				IO::textFile& m_File;
				const std::string& m_ModelName;
				const std::string& m_FolderName;

				ref<objModel> m_Model;
			};
		}
	}
}