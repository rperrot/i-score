#include <Scenario/Document/State/StateModel.hpp>
#include <OSSIA/iscore2OSSIA.hpp>

#include <Scenario/Document/State/ItemModel/MessageItemModel.hpp>
#include "StateElement.hpp"

namespace RecreateOnPlay
{
StateElement::StateElement(
        const Scenario::StateModel &element,
        std::shared_ptr<OSSIA::State> root,
        const RecreateOnPlay::Context& ctx,
        QObject *parent):
    QObject{parent},
    m_iscore_state{element},
    m_ossia_state{root},
    m_context{ctx}
{
    iscore::convert::state(
                m_ossia_state,
                m_iscore_state,
                m_context);
}

const Scenario::StateModel& StateElement::iscoreState() const
{
    return m_iscore_state;
}

}
