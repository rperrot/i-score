#pragma once
#include <Process/ProcessFactory.hpp>
#include <QByteArray>
#include <QString>


#include <Process/TimeValue.hpp>

namespace Process {
class LayerModel;
class LayerPresenter;
class LayerView;
class ProcessModel;
}
class QGraphicsItem;
class QObject;
struct VisitorVariant;
#include <iscore/tools/SettableIdentifier.hpp>

namespace Scenario
{
class EditionSettings;

class ScenarioFactory final :
        public Process::ProcessFactory
{
    public:
        ScenarioFactory(Scenario::EditionSettings&);
        const UuidKey<Process::ProcessFactory>& concreteFactoryKey() const override;
        QString prettyName() const override;

        Process::ProcessModel* makeModel(
                const TimeValue& duration,
                const Id<Process::ProcessModel>& id,
                QObject* parent) override;

        Process::ProcessModel* load(
                const VisitorVariant&,
                QObject* parent) override;

        QByteArray makeStaticLayerConstructionData() const override;

        Process::LayerPresenter* makeLayerPresenter(
                const Process::LayerModel&,
                Process::LayerView*,
                const Process::ProcessPresenterContext& context,
                QObject* parent) override;

        Process::LayerView* makeLayerView(
                const Process::LayerModel& viewmodel,
                QGraphicsItem* parent) override;

    private:
        Scenario::EditionSettings& m_editionSettings;

};
}
