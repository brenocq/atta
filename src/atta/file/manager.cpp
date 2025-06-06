//--------------------------------------------------
// Atta File Module
// manager.cpp
// Date: 2021-09-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/cmakeConfig.h>
#include <atta/component/components/polygonCollider2D.h>
#include <atta/component/components/relationship.h>
#include <atta/component/entity.h>
#include <atta/component/interface.h>
#include <atta/event/events/projectBeforeDeserialize.h>
#include <atta/event/events/projectClose.h>
#include <atta/event/events/projectOpen.h>
#include <atta/event/events/simulationStart.h>
#include <atta/event/events/simulationStop.h>
#include <atta/file/interface.h>
#include <atta/file/manager.h>
#include <atta/file/watchers/linuxFileWatcher.h>
#include <atta/file/watchers/nullFileWatcher.h>
#include <atta/resource/interface.h>

#ifdef ATTA_OS_WEB
// Need to use "linux" build-in commands instead of std::filesystem
#include <dirent.h>
#endif

namespace atta::file {

Manager& Manager::getInstance() {
    static Manager instance;
    return instance;
}

void Manager::startUpImpl() {
    _project = nullptr;
    _projectSerializer = nullptr;
    _simulationRunning = false;

#ifdef ATTA_OS_LINUX
    _fileWatcher = std::static_pointer_cast<FileWatcher>(std::make_shared<LinuxFileWatcher>());
#else
    _fileWatcher = std::static_pointer_cast<FileWatcher>(std::make_shared<NullFileWatcher>());
#endif

    event::subscribe<event::SimulationStart>(BIND_EVENT_FUNC(Manager::onSimulationStateChange));
    event::subscribe<event::SimulationStop>(BIND_EVENT_FUNC(Manager::onSimulationStateChange));
    registerCustomComponentIOs();

    // Default folder to clone published projects
    _defaultProjectFolder = fs::path(ATTA_DIR) / "projects";
}

void Manager::shutDownImpl() {
    _componentSerialize.clear();
    _componentDeserialize.clear();
}

bool Manager::openProjectImpl(fs::path projectFile) {
#ifdef ATTA_STATIC_PROJECT
    if (projectFile != fs::path(ATTA_STATIC_PROJECT_FILE)) {
        LOG_WARN("file::Manager", "It is not possible to open project [w]$0[], atta was built linked statically to [w]$0[]", projectFile,
                 fs::path(ATTA_STATIC_PROJECT_FILE));
        return false;
    }
#endif
    if (projectFile.extension() != ".atta") {
        LOG_WARN("file::Manager", "The project file must have [w].atta[] extension, but it is [w]$0[]", fs::absolute(projectFile));
        return false;
    }

    // Close open project is it exists
    if (_project != nullptr)
        closeProjectImpl();

    // Update project
    _project = std::make_shared<Project>(projectFile);
    _projectSerializer = std::make_shared<ProjectSerializer>(_project);

    // Create .atta file if it does not exists yet
    if (!fs::exists(projectFile))
        _projectSerializer->serialize();

    // Clear components before loading
    component::clear();
    // Clear resources before loading
    resource::destroyResources<resource::Material>();

    // Load project
    event::ProjectBeforeDeserialize ed;
    event::publish(ed);
    bool success = _projectSerializer->deserialize();
    if (!success) {
        LOG_ERROR("file::Manager", "Failed to open project [w]$0[]", fs::absolute(projectFile));
        return false;
    }

    // Watch project directory file changes
    _fileWatcher->addWatch(_project->getDirectory());

    event::ProjectOpen e;
    event::publish(e);

    return true;
}

bool Manager::createProjectImpl(fs::path projectFile) {
#ifndef ATTA_STATIC_PROJECT
    if (projectFile.extension() != ".atta") {
        LOG_WARN("file::Manager", "Project file must have .atta extension [w]$0[]", fs::absolute(projectFile));
        return false;
    }

    // Close if there is an open project
    if (_project != nullptr)
        closeProjectImpl();

    // Update project
    _project = std::make_shared<Project>(projectFile);
    _projectSerializer = std::make_shared<ProjectSerializer>(_project);

    // Write data to the file
    _projectSerializer->serialize();

    // Watch project directory file changes
    _fileWatcher->addWatch(_project->getDirectory());

    event::ProjectOpen e;
    event::publish(e);
#else
    LOG_WARN("file::Manager", "It is not possible to create a project when atta is statically linked to a project");
#endif

    return true;
}

void Manager::saveProjectImpl() {
    if (_simulationRunning) {
        event::SimulationStop e;
        event::publish(e);
    }

    if (_projectSerializer)
        _projectSerializer->serialize();
}

void Manager::closeProjectImpl() {
#ifndef ATTA_STATIC_PROJECT
    if (_project == nullptr)
        return;

    _fileWatcher->removeWatch(_project->getDirectory());

    _project.reset();
    _projectSerializer.reset();

    event::ProjectClose e;
    event::publish(e);
#endif
}

bool Manager::isProjectOpenImpl() const { return _project != nullptr; }

void Manager::registerCustomComponentIOs() {
    file::registerComponentIO<cmp::Relationship>(
        [](Section& section, cmp::Component* c) {
            cmp::Relationship* rel = static_cast<cmp::Relationship*>(c);
            if (rel->getParent() != -1)
                section["relationship.parent"] = rel->getParent().getId();
        },
        [](const Section& section, cmp::Entity entity) {
            if (section.contains("relationship.parent")) {
                cmp::EntityId parent = uint32_t(section["relationship.parent"]);

                // Create parent if it doesn't exist yet
                std::vector<cmp::EntityId> entities = cmp::getEntitiesView();
                bool parentExists = std::find(entities.begin(), entities.end(), parent) != entities.end();
                if (!parentExists)
                    cmp::createEntity(parent);

                // Update relationship component
                entity.add<cmp::Relationship>()->setParent(parent, entity);
            }
        });

    file::registerComponentIO<cmp::PolygonCollider2D>(
        [](Section& section, cmp::Component* c) {
            cmp::PolygonCollider2D* collider = static_cast<cmp::PolygonCollider2D*>(c);
            section["polygonCollider2D.offset"] = collider->offset;
            section["polygonCollider2D.points"] = collider->points;
        },
        [](const Section& section, cmp::Entity entity) {
            if (section.contains("polygonCollider2D.offset")) {
                cmp::PolygonCollider2D* collider = entity.add<cmp::PolygonCollider2D>();
                collider->offset = vec2(section["polygonCollider2D.offset"]);
                collider->points = std::vector<vec2>(section["polygonCollider2D.points"]);
            }
        });
}

void Manager::registerComponentIOImpl(cmp::ComponentId cid, const SerializeFunc& serialize, const DeserializeFunc& deserialize) {
    _componentSerialize[cid] = serialize;
    _componentDeserialize[cid] = deserialize;
}

void Manager::getComponentIOImpl(cmp::ComponentId cid, std::optional<SerializeFunc>& serialize, std::optional<DeserializeFunc>& deserialize) const {
    serialize = std::nullopt;
    deserialize = std::nullopt;
    if (_componentSerialize.find(cid) != _componentSerialize.end())
        serialize = _componentSerialize.at(cid);
    if (_componentDeserialize.find(cid) != _componentDeserialize.end())
        deserialize = _componentDeserialize.at(cid);
}

// TODO remove and use multi threading
void Manager::updateImpl() { _fileWatcher->update(); }

fs::path Manager::solveResourcePathImpl(fs::path relativePath, bool mustExist) {
    if (_project != nullptr)
        return _project->solveResourcePath(relativePath, mustExist);
    else {
        fs::path full = fs::path(ATTA_DIR) / "resources" / relativePath;
        if (!mustExist || fs::exists(full))
            return full;
        else
            return fs::path();
    }
}

std::vector<fs::path> Manager::getResourcePathsImpl() const {
    if (_project != nullptr)
        return _project->getResourceRootPaths();
    else
        return {fs::path(ATTA_DIR) / "resources"};
}

fs::path Manager::getBuildPathImpl() const { return fs::path(ATTA_BUILD_DIR); }

std::vector<fs::path> Manager::getDirectoryFilesRecursiveImpl(const fs::path& directory) {
    std::vector<fs::path> files;
#ifndef ATTA_OS_WEB
    for (const auto& p : fs::recursive_directory_iterator(directory))
        if (!p.is_directory())
            files.push_back(fs::relative(p.path(), directory));
#else
    // If compiling for the web, need to use system calls to get files recursivelly
    DIR* d;
    struct dirent* dir;

    std::stack<fs::path> directories;
    directories.push(directory);

    while (!directories.empty()) {
        fs::path curr = directories.top();
        directories.pop();

        d = opendir(curr.string().c_str());
        // Check if curr is file or directory
        if (d) {
            // Push children directories to stack
            while ((dir = readdir(d)) != NULL)
                if (dir->d_name[0] != '.')
                    directories.push(curr / dir->d_name);
        } else
            // Push file to list
            files.push_back(fs::relative(curr, directory));
        closedir(d);
    }
#endif
    return files;
}

void Manager::onSimulationStateChange(event::Event& event) {
    switch (event.getType()) {
        case event::SimulationStart::type:
            _simulationRunning = true;
            break;
        case event::SimulationStop::type:
            _simulationRunning = false;
            break;
        default:
            LOG_WARN("file::Manager", "Unknown simulation event");
    }
}

} // namespace atta::file
