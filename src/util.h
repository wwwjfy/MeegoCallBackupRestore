#include <CommHistory/CallModel>
#include <CommHistory/Event>
#include <CommHistory/Group>
#include <CommHistory/TrackerIO>
#include <CommHistory/ConversationModel>
#include <QStringBuilder>
#include <QUuid>
#include "catcher.h"

using namespace CommHistory;

#define TELEPATHY_ACCOUNT_PREFIX       QLatin1String("/org/freedesktop/Telepathy/Account/")

class Util {
public:
	static void importCall(QTextStream &in);
	static void exportCall(QTextStream &out);
};