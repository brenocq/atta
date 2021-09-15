//--------------------------------------------------
// Atta File System
// projectSerializer.cpp
// Date: 2021-09-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/fileSystem/project/projectSerializer.h>
#include <atta/cmakeConfig.h>

#define WRITE_BIN(s,x)
#define WRITE_VEC_BIN(s,vec,size) s.write(reinterpret_cast<const char*>(vec), size);

namespace atta
{
	ProjectSerializer::ProjectSerializer(std::shared_ptr<Project> project):
		_project(project)
	{
		serialize();
	}

	void ProjectSerializer::serialize()
	{
		fs::path attaTemp = _project->getFile();
		attaTemp.replace_extension(".atta.temp");

		std::ofstream os(attaTemp, std::ofstream::trunc | std::ofstream::binary);

		serializeHeader(os);

		os.close();

		// Override atta file with temp file
		fs::rename(attaTemp, _project->getFile());

		LOG_WARN("ProjectSerializer", "New file:$0 -> ext:$1", fs::absolute(attaTemp), attaTemp.extension());
	}

	void ProjectSerializer::deserialize()
	{
		fs::path attaFile = _project->getFile();

		std::ifstream is(attaFile, std::ifstream::in | std::ifstream::binary);

		Header header = deserializeHeader(is);
			LOG_WARN("ProjectSerializer", "[*y]Header:[]\n\tversion:$0.$1.$2.$3\n\tproj:$4\n\tcounter:$5", 
					header.version[0], header.version[1], header.version[2], header.version[3], 
					header.projectName, header.saveCounter);

		is.close();
	}

	void ProjectSerializer::serializeHeader(std::ofstream& os)
	{
		// Atta info
		write(os, "atta");
		uint16_t version[] = { ATTA_VERSION_MAJOR, ATTA_VERSION_MINOR, ATTA_VERSION_PATCH, ATTA_VERSION_TWEAK };
		write(os, version);// Version

		// Project info
		write(os, "proj");
		write(os, _project->getName());// Project name
		uint32_t saveCounter = 1;
		write(os, saveCounter);// Save counter (number of times that was saved)
		write(os, "hend");
	}

	ProjectSerializer::Header ProjectSerializer::deserializeHeader(std::ifstream& is)
	{
		std::string marker;
		Header header;

		while(true)
		{
			read(is, marker);
			if(marker == "hend")
			{
				return header;
			}
			else if(marker == "atta")
			{
				read(is, header.version);
			}
			else if(marker == "proj")
			{
				read(is, header.projectName);
				read(is, header.saveCounter);
			}
			else
			{
				LOG_WARN("ProjectSerializer", "Unknown marker found at the header [w]$0[]", marker);
				return header;
			}
		}
		return header;
	}
}

