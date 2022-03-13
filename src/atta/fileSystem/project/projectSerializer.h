//--------------------------------------------------
// Atta File System
// projectSerializer.h
// Date: 2021-09-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_FILE_SYSTEM_PROJECT_PROJECT_SERIALIZER_H
#define ATTA_FILE_SYSTEM_PROJECT_PROJECT_SERIALIZER_H
#include <atta/fileSystem/project/project.h>

namespace atta
{
    class ProjectSerializer final
    {
    public:
        ProjectSerializer(std::shared_ptr<Project> project);
        ~ProjectSerializer();

        void serialize();
        void deserialize();

    private:
        struct Header {
            // Atta
            uint16_t version[4];

            // Project
            std::string projectName;	
            uint32_t saveCounter;	
        };

        void serializeHeader(std::ofstream& os);
        Header deserializeHeader(std::ifstream& is);

        void serializeConfig(std::ofstream& os);
        void deserializeConfig(std::ifstream& is);

        void serializeComponentSystem(std::ofstream& os);
        void deserializeComponentSystem(std::ifstream& is);

        void serializeGraphicsSystem(std::ofstream& os);
        void deserializeGraphicsSystem(std::ifstream& is);

        std::shared_ptr<Project> _project;
    };
}

#endif// ATTA_FILE_SYSTEM_PROJECT_PROJECT_SERIALIZER_H
