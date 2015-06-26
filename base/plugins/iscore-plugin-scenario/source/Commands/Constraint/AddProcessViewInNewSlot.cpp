#include "AddProcessViewInNewSlot.hpp"

#include "Document/Constraint/ConstraintModel.hpp"
#include "Document/Constraint/Box/BoxModel.hpp"
#include "Document/Constraint/Box/Slot/SlotModel.hpp"
#include "Document/Constraint/ViewModels/FullView/FullViewConstraintViewModel.hpp"

#include "ProcessInterface/ProcessModel.hpp"
#include "ProcessInterface/ProcessViewModel.hpp"
#include <iscore/tools/SettableIdentifierGeneration.hpp>


using namespace iscore;
using namespace Scenario::Command;

AddProcessViewInNewSlot::AddProcessViewInNewSlot(ObjectPath&& constraintPath,
                                                 id_type<ProcessModel> process) :
    SerializableCommand {"ScenarioControl",
                         commandName(),
                         description()},
    m_path {std::move(constraintPath) },
    m_sharedProcessModelId{process}
{
    auto& constraint = m_path.find<ConstraintModel>();

    if(constraint.boxes().empty())
    {
        m_createdBoxId = getStrongId(constraint.boxes());
        m_existingBox = false;
    }
    else
    {
        m_createdBoxId = (*constraint.boxes().begin())->id();
        m_existingBox = true;
    }

    m_createdSlotId = id_type<SlotModel> (getNextId());
    m_createdProcessViewId = id_type<ProcessViewModel> (getNextId());
    m_processData = constraint.process(m_sharedProcessModelId)->makeViewModelConstructionData();
}

void AddProcessViewInNewSlot::undo()
{
    auto& constraint = m_path.find<ConstraintModel>();
    auto box = constraint.box(m_createdBoxId);

    // Removing the slot is enough
    box->removeSlot(m_createdSlotId);

    // Remove the box
    if(!m_existingBox)
    {
        constraint.removeBox(m_createdBoxId);
    }
}

void AddProcessViewInNewSlot::redo()
{
    auto& constraint = m_path.find<ConstraintModel>();

    // Box
    if(!m_existingBox)
    {
        // TODO refactor with AddBoxToConstraint
        auto box = new BoxModel{m_createdBoxId, &constraint};
        constraint.addBox(box);
        box->metadata.setName(QString{"Box.%1"}.arg(constraint.boxes().size()));

        // If it is the first box created,
        // it is also assigned to all the views of the constraint.
        if(constraint.boxes().size() == 1)
        {
            for(const auto& vm : constraint.viewModels())
            {
                vm->showBox(m_createdBoxId);
            }
        }
    }

    // Slot
    auto box = constraint.box(m_createdBoxId);
    box->addSlot(new SlotModel {m_createdSlotId,
                                box});

    // Process View
    auto slot = box->slot(m_createdSlotId);
    auto proc = constraint.process(m_sharedProcessModelId);

    slot->addProcessViewModel(proc->makeViewModel(m_createdProcessViewId, m_processData, slot));
}

void AddProcessViewInNewSlot::serializeImpl(QDataStream& s) const
{
    s << m_path
      << m_existingBox
      << m_processId
      << m_createdBoxId
      << m_createdSlotId
      << m_createdProcessViewId
      << m_sharedProcessModelId
      << m_processData;
}

void AddProcessViewInNewSlot::deserializeImpl(QDataStream& s)
{
    s >> m_path
      >> m_existingBox
      >> m_processId
      >> m_createdBoxId
      >> m_createdSlotId
      >> m_createdProcessViewId
      >> m_sharedProcessModelId
      >> m_processData;
}
