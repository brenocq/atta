//--------------------------------------------------
// Atta File Module
// projectSerializer.h
// Date: 2021-09-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_FILE_PROJECT_PROJECT_SERIALIZER_H
#define ATTA_FILE_PROJECT_PROJECT_SERIALIZER_H
#include <atta/file/project/project.h>
#include <atta/file/serializer/serializer.h>

namespace atta::file {

class ProjectSerializer final {
  public:
    ProjectSerializer(std::shared_ptr<Project> project);
    ~ProjectSerializer();

    void serialize();
    void deserialize();

  private:
    Section serializeProject();
    Section serializeConfig();
    Section serializeGraphicsModule();
    Section serializePhysicsModule();
    Section serializeSensorModule();
    std::vector<Section> serializeResources();
    std::vector<Section> serializeNodes();

    void deserializeProject(Section& section);
    void deserializeConfig(Section& section);
    void deserializeGraphicsModule(Section& section);
    void deserializeResourceModule(Section& section);
    void deserializePhysicsModule(Section& section);
    void deserializeSensorModule(Section& section);
    void deserializeNode(Section& section);

    std::shared_ptr<Project> _project;
};

} // namespace atta::file

#endif // ATTA_FILE_PROJECT_PROJECT_SERIALIZER_H
