//--------------------------------------------------
// Atta Component Module
// mesh.h
// Date: 2021-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_COMPONENTS_MESH_H
#define ATTA_COMPONENT_COMPONENTS_MESH_H

#include <atta/component/interface.h>

namespace atta::component {

/// %Component to define entity mesh
/** The entity must also have a Transform to be rendered.
 */
struct Mesh final : public Component {
    Mesh();
    /** The StringId string is the mesh path relative to the resource directory.
     * The resource directory can be the project resource directory or the atta
     * default resource directory.
     *
     * For example, if the mesh file is located at "<project-path>/resources/myMeshes/robot.obj",
     * the StringId string should be "myMeshes/robot.obj".
     * */
    StringId sid; ///< Mesh relative path
    void set(std::string mesh);
};
ATTA_REGISTER_COMPONENT(Mesh)
template <>
ComponentDescription& TypedComponentRegistry<Mesh>::getDescription();

} // namespace atta::component

#endif // ATTA_COMPONENT_COMPONENTS_MESH_H
