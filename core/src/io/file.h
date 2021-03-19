#include <string>
			file();
			file(const std::string& filePath, fileMode fileMode, bool flushAfterWrite);
			virtual ~file();
			inline const std::string& filePath()
			bool tryOpen(const std::string& filePath, fileMode fileMode, bool flushAfterWrite);
			void open(const std::string& filePath, fileMode fileMode, bool flushAfterWrite);
			inline bool isOpen() { return m_FileHandle != nullptr; }
			std::string m_FilePath;	// File path