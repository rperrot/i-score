#pragma once
#include <iscore/plugins/documentdelegate/plugin/DocumentDelegatePluginModel.hpp>

#include <iscore/serialization/VisitorCommon.hpp>
#include <DeviceExplorer/Protocol/DeviceList.hpp>
#include <DeviceExplorer/Node/Node.hpp>
namespace iscore
{
    class Document;
}

// The DeviceDocumentPlugin is informed of new addresses by the plugin-provided models of the
// devices, and propagates them.
// Exploration should be a Command, which registers all the new addresses in its constructor
// - via a thread and updateNamespace()-
// and add them correctly in the explorer model.
class DeviceDocumentPlugin : public iscore::DocumentDelegatePluginModel
{
        Q_OBJECT
    public:
        DeviceDocumentPlugin(QObject* parent);
        DeviceDocumentPlugin(const VisitorVariant& loader,
                             QObject* parent);

        void serialize(const VisitorVariant&) const override;

        iscore::Node& rootNode()
        { return m_rootNode; }

        DeviceList& list()
        { return m_list; }

        const DeviceList& list() const
        { return m_list; }

        // TODO make functions that take an address and call list().device(...).TheRelevantMethod

    private:
        iscore::Node m_rootNode{InvisibleRootNodeTag{}};
        DeviceList m_list;
};

