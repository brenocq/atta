//--------------------------------------------------
// Atta File Module
// projectSerializer.h
// Date: 2021-09-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_FILE_PROJECT_PROJECT_SERIALIZER_H
#define ATTA_FILE_PROJECT_PROJECT_SERIALIZER_H
#include <atta/file/project/project.h>
#include <atta/file/serializer/section.h>

namespace atta::file {

class ProjectSerializer final {
  public:
    ProjectSerializer(std::shared_ptr<Project> project);
    ~ProjectSerializer();

    void serialize();
    void deserialize();

  private:
    void serializeHeader(Section& section);
    void deserializeHeader(Section& section);

    void serializeConfig(Section& section);
    void deserializeConfig(Section& section);

    void serializeComponentSystem(Section& section);
    void deserializeComponentSystem(Section& section);

    void serializeGraphicsSystem(Section& section);
    void deserializeGraphicsSystem(Section& section);

    std::shared_ptr<Project> _project;
};

} // namespace atta::file

#endif // ATTA_FILE_PROJECT_PROJECT_SERIALIZER_H
