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

        void serializeComponentSystem(std::ofstream& os);
        void deserializeComponentSystem(std::ifstream& is);

        // Helpers to write binary data
        template <typename T>
        void write(std::ofstream& os, T x);
        template<typename T, size_t N>
        void write(std::ofstream& os, T(&x)[N]);
        template <typename It>
        void write(std::ofstream& os, It begin, It end);

        // Helpers to read binary data
        template <typename T>
        void read(std::ifstream& is, T& x);
        template<typename T, size_t N>
        void read(std::ifstream& is, T(&x)[N]);

        std::shared_ptr<Project> _project;
    };
}

#include <atta/fileSystem/project/projectSerializer.inl>
#endif// ATTA_FILE_SYSTEM_PROJECT_PROJECT_SERIALIZER_H
