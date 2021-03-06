#include <Process/LayerModelPanelProxy.hpp>
#include <Scenario/Process/Temporal/TemporalScenarioLayerModel.hpp>
#include "TemporalScenarioPanelProxy.hpp"

class QObject;
namespace Scenario
{
TemporalScenarioPanelProxy::TemporalScenarioPanelProxy(
        const TemporalScenarioLayerModel& lm,
        QObject* parent):
    GraphicsViewLayerModelPanelProxy{lm, parent}
{

}
}
