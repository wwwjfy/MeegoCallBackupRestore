#include <iostream>

#include "util.h"
    
void Util::importCall(QTextStream &in) {
    QString line;
    QList<Event> events;
    int count = 0;
    QMap<QString, int> phoneGroupMap;
    while (!(line = in.readLine()).isNull()) {
        count ++;
        std::cout << "Handling " << count << " messages" << std::endl;
        QStringList stringList = line.split(QChar(','));

        QString localUid = stringList.at(0);
        if (!localUid.startsWith(TELEPATHY_ACCOUNT_PREFIX)) {
            localUid.prepend(TELEPATHY_ACCOUNT_PREFIX);
        }
        QString remoteUid = stringList.at(1);

        QDateTime startTime = QDateTime::fromString(stringList.at(3), "yyyy-MM-dd hh:mm:ss");
        QDateTime endTime = QDateTime::fromString(stringList.at(4), "yyyy-MM-dd hh:mm:ss");

        Event::EventDirection direction = Event::UnknownDirection;
        bool isMissed = false;
        if (stringList.at(2) == QString("1")) {
            direction = Event::Inbound;
        } else if (stringList.at(2) == QString("2")) {
            direction = Event::Inbound;
            isMissed = true;
        } else {
            direction = Event::Outbound;
        }

        Event e;
        e.setDirection(direction);
        e.setIsMissedCall(isMissed);
        e.setType(Event::CallEvent);
        e.setLocalUid(localUid);

        e.setStartTime(startTime);
        e.setEndTime(endTime);
        e.setGroupId(-1);
        e.setRemoteUid(remoteUid);

        events.append(e);

        std::cout << "Handled " << count << " messages" << std::endl;
    }
    if (events.count() > 0) {
        std::cout << "About to commit the changes" << std::endl;
        EventModel model;
        Catcher c(&model);
        if (!model.addEvents(events, false)) {
            qCritical() << "Error adding events";
            return;
        }

        c.waitCommit(events.count());
    }
    
}

void Util::exportCall(QTextStream &out) {
    CallModel model;
    model.enableContactChanges(false);
    model.setQueryMode(EventModel::SyncQuery);
    CallModel::Sorting sorting = CallModel::SortByTime;
    model.setFilter(sorting);
    QString direction;

    int limit = 1000;
    int offset = 0;
    bool keepgoing = true;

    while(keepgoing) {
        int count = 0;
        model.setLimit(limit);
        model.setOffset(offset);
        std::cout << "  getEvents() from " << offset << " to " << offset + limit << std::endl;
        model.getEvents();
        std::cout << "  got " << model.rowCount() << " events" << std::endl;
        count = model.rowCount();
        offset += limit;

        // Stop when no more events found
        keepgoing = (count > 0);

        for (int i = 0; i < model.rowCount(); i++) {
            Event event = model.event(model.index(i, 0));
            if (event.direction() == Event::Inbound) {
                direction = QString("1");
                if (event.isMissedCall()) {
                    direction = QString("2");
                }
            } else {
                direction = QString("0");
            }
            out << QString(event.localUid() % QChar(',') %
                           event.remoteUid() % QChar(',') %
                           direction % QChar(',') %
                           event.startTime().toLocalTime().toString(QString("yyyy-MM-dd hh:mm:ss")) % QChar(',') %
                           event.endTime().toLocalTime().toString(QString("yyyy-MM-dd hh:mm:ss")));
            out << "\n";
        }
    }
}
