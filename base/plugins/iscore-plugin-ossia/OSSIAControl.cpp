#include "OSSIAControl.hpp"
#include "DocumentPlugin/OSSIADocumentPlugin.hpp"
#include "DocumentPlugin/OSSIABaseScenarioElement.hpp"

#include "Control/ScenarioControl.hpp"

#include <API/Headers/Network/Device.h>
#include <API/Headers/Network/Node.h>
#include <API/Headers/Network/Protocol/Local.h>

#include <API/Headers/Editor/TimeEvent.h>
#include <API/Headers/Editor/TimeConstraint.h>
#include <API/Headers/Editor/TimeProcess.h>
#include <API/Headers/Editor/Expression.h>
#include <API/Headers/Editor/ExpressionNot.h>
#include <API/Headers/Editor/ExpressionAtom.h>
#include <API/Headers/Editor/ExpressionComposition.h>
#include <API/Headers/Editor/Value.h>
#if defined(__APPLE__) && defined(ISCORE_DEPLOYMENT_BUILD)
#include <TTFoundationAPI.h>
#include <TTModular.h>
#endif

#include <core/document/DocumentModel.hpp>
#include <iscore/document/DocumentInterface.hpp>
#include <core/document/Document.hpp>

OSSIAControl::OSSIAControl(iscore::Presenter* pres):
    iscore::PluginControlInterface {pres, "OSSIAControl", nullptr}
{
// Here we try to load the extensions first because of buggy behaviour in TTExtensionLoader and API.
#if defined(__APPLE__) && defined(ISCORE_DEPLOYMENT_BUILD)
    auto contents = QFileInfo(qApp->applicationDirPath()).dir().path() + "/Frameworks/jamoma/extensions";
    TTFoundationInit(contents.toUtf8().constData(), true);
    TTModularInit(contents.toUtf8().constData(), true);
#endif
    using namespace OSSIA;
    auto localDevice = OSSIA::Local::create();
    m_localDevice = Device::create(localDevice, "i-score");
    // Two parts :
    // One that maintains the devices for each document
    // (and disconnects / reconnects them when the current document changes)
    // Also during execution, one shouldn't be able to switch document.

    // Another part that, at execution time, creates structures corresponding
    // to the Scenario plug-in with the OSSIA API.

    auto ctrl = ScenarioControl::instance();
    auto acts = ctrl->actions();
    for(const auto& act : acts)
    {
        if(act->objectName() == "Play")
        {
            connect(act, &QAction::toggled,
                    this, [&] (bool b) {
                if(b)
                {
                    if(m_playing)
                        baseConstraint().constraint()->resume();
                    else
                        baseConstraint().play();

                    m_playing = true;
                }
                else
                {
                    baseConstraint().constraint()->pause();
                } });
        }
        if(act->objectName() == "Stop")
        {
            connect(act, &QAction::triggered,
                    this, [&] {
                baseConstraint().stop();
                m_playing = false;
            });
        }
    }
}

OSSIAControl::~OSSIAControl()
{
    // TODO doesn't handle the case where
    // two scenarios are playing in two ducments (we have to
    // stop them both)

    // TODO check the deletion order.
    // Maybe we should have a dependency graph of some kind ??
    if(currentDocument())
        baseConstraint().stop();
}


OSSIAConstraintElement &OSSIAControl::baseConstraint() const
{
    return *currentDocument()->model().pluginModel<OSSIADocumentPlugin>()->baseScenario()->baseConstraint();
}

void OSSIAControl::populateMenus(iscore::MenubarManager* menu)
{
}

iscore::DocumentDelegatePluginModel*OSSIAControl::loadDocumentPlugin(
        const QString& name,
        const VisitorVariant& var,
        iscore::DocumentModel* model)
{
    // We don't have anything to load; it's easier to just recreate.
    return nullptr;
}

void OSSIAControl::on_newDocument(iscore::Document* doc)
{
    doc->model().addPluginModel(new OSSIADocumentPlugin{doc->model(), &doc->model()});
}

void OSSIAControl::on_loadedDocument(iscore::Document *doc)
{
    if(auto plugmodel = doc->model().pluginModel<OSSIADocumentPlugin>())
    {
        plugmodel->reload(doc->model());
    }
    else
    {
        on_newDocument(doc);
    }
}

void OSSIAControl::on_documentChanged()
{
    ISCORE_TODO;
}

