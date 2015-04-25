#pragma once
#include <iscore/tools/NamedObject.hpp>
#include <iscore/tools/SettableIdentifier.hpp>

class EventModel;
class EventView;
class TemporalScenarioPresenter;
class EventPresenter : public NamedObject
{
        Q_OBJECT

    public:
        EventPresenter(EventModel* model,
                       QGraphicsObject* parentview,
                       QObject* parent);
        virtual ~EventPresenter();

        id_type<EventModel> id() const;

        EventView* view() const;
        EventModel* model() const;

        bool isSelected() const;

    signals:
        void eventHoverEnter();
        void eventHoverLeave();

        void linesExtremityChange(int, double);

    private:
        EventModel* m_model {};
        EventView* m_view {};
};

