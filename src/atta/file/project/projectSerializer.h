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

    void serializeComponentModule(Section& section);
    void deserializeComponentModule(Section& section);

    void serializeGraphicsModule(Section& section);
    void deserializeGraphicsModule(Section& section);

    void serializeResourceModule(Section& section);
    void deserializeResourceModule(Section& section);

    void serializePhysicsModule(Section& section);
    void deserializePhysicsModule(Section& section);

    void serializeSensorModule(Section& section);
    void deserializeSensorModule(Section& section);

    void serializeProcessorModule(Section& section);
    void deserializeProcessorModule(Section& section);

    std::shared_ptr<Project> _project;
};

} // namespace atta::file

#endif // ATTA_FILE_PROJECT_PROJECT_SERIALIZER_H
