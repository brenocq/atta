//--------------------------------------------------
// Atta Component System
// relationshipComponent.cpp
// Date: 2021-11-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/componentSystem/components/relationshipComponent.h>
#include <atta/componentSystem/componentManager.h>
#include <atta/componentSystem/componentManager.h>

namespace atta
{
    // Parent operations
    void RelationshipComponent::setParent(EntityId parent, EntityId child)
    {
        // Get relationships
        RelationshipComponent* parentRel = ComponentManager::getEntityComponent<RelationshipComponent>(parent);
        if(!parentRel) parentRel = ComponentManager::addEntityComponent<RelationshipComponent>(parent);
        RelationshipComponent* childRel = ComponentManager::getEntityComponent<RelationshipComponent>(child);
        if(!childRel) childRel = ComponentManager::addEntityComponent<RelationshipComponent>(child);

        // Check if entity can be children (avoid recursive)
        bool canBeParent = true;
        RelationshipComponent* r = parentRel;
        if(r && r->_parent != -1)
        {
            // From the parent, going up in the hierarchy, can't find the child
            if(r->_parent == child) canBeParent = false;
            while(canBeParent && r != nullptr)
            {
                if(r->_parent == child) canBeParent = false;

                if(r->_parent == -1) r = nullptr;
                else r = ComponentManager::getEntityComponent<RelationshipComponent>(r->_parent);
            }
        }

        // Update relationship components
        if(canBeParent)
        {
            // Remove old parent
            if(childRel->_parent != -1)
                removeParent(childRel->_parent, child);

            // Add new parent
            parentRel->_children.push_back(child);
            childRel->_parent = parent;
        }
    }

    void RelationshipComponent::removeParent(EntityId parent, EntityId child)
    {
        // Get relationships
        RelationshipComponent* parentRel = ComponentManager::getEntityComponent<RelationshipComponent>(parent);
        if(!parentRel && parent != -1)
        {
            LOG_WARN("RelationshipComponent", "Trying to remove wrong parent [w]$0[] from child [w]$1[]", parent, child);
            return;
        }

        RelationshipComponent* childRel = ComponentManager::getEntityComponent<RelationshipComponent>(child);
        if(!childRel)
        {
            LOG_WARN("RelationshipComponent", "Trying to remove parent [w]$0[] from child [w]$1[] that does not have a parent", parent, child);
            return;
        }

        childRel->_parent = -1;
        if(parentRel)
        {
            bool found = false;
            for(unsigned i = 0; i < parentRel->_children.size(); i++)
                if(parentRel->_children[i] == child)
                {
                    parentRel->_children.erase(parentRel->_children.begin()+i);
                    found = true;
                    break;
                }
            if(!found)
                LOG_WARN("RelationshipComponent", "Removed wrong parent [w]$0[] from child [w]$1[]", parent, child);
        }
    }

    // Child operations
    void RelationshipComponent::addChild(EntityId parent, EntityId child)
    {
        setParent(parent, child);
    }

    void RelationshipComponent::removeChild(EntityId parent, EntityId child)
    {
        removeParent(parent, child);
    }
}
