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

    void serialize();
    bool deserialize();

  private:
    Section serializeProject();
    Section serializeConfig();
    Section serializeGraphicsModule();
    Section serializePhysicsModule();
    Section serializeSensorModule();
    std::vector<Section> serializeResources();
    std::vector<Section> serializeWindows();
    std::vector<Section> serializeViewports();
    std::vector<Section> serializeNodes();

    bool deserializeProject(const Section& section);
    void deserializeConfig(const Section& section);
    void deserializeGraphicsModule(const Section& section);
    void deserializePhysicsModule(const Section& section);
    void deserializeSensorModule(const Section& section);
    void deserializeMaterial(const Section& section);
    void deserializeViewport(const Section& section);
    void deserializeNode(const Section& section);

    std::shared_ptr<Project> _project;
    bool _firstViewport = true;
};

} // namespace atta::file

#endif // ATTA_FILE_PROJECT_PROJECT_SERIALIZER_H
