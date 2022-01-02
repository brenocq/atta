//--------------------------------------------------
// Atta Component System
// meshComponent.h
// Date: 2021-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENTS_MESH_COMPONENT_H
#define ATTA_COMPONENT_SYSTEM_COMPONENTS_MESH_COMPONENT_H
#include <atta/componentSystem/componentManager.h>

namespace atta
{
	/// %Component to define entity mesh
	/** The entity must also have a TransformComponent to be rendered.
	 */
    struct MeshComponent final : public Component
    {
        MeshComponent();
		/** The StringId string is the mesh path relative to the resource directory. 
		 * The resource directory can be the project resource directory or the atta 
		 * default resource directory.
		 *
		 * For example, if the mesh file is located at "<project-path>/resources/myMeshes/robot.obj", 
		 * the StringId string should be "myMeshes/robot.obj".
		 * */
        StringId sid;///< Mesh relative path
    };
    ATTA_REGISTER_COMPONENT(MeshComponent)
    template<> ComponentDescription& TypedComponentRegistry<MeshComponent>::getDescription();
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_MESH_COMPONENT_H
