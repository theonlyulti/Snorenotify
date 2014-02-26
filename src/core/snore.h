/*
    SnoreNotify is a Notification Framework based on Qt
    Copyright (C) 2013-2014  Patrick von Reth <vonreth@kde.org>


    SnoreNotify is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SnoreNotify is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with SnoreNotify.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SNORESERVER_H
#define SNORESERVER_H

#include "snore_exports.h"
#include "log.h"
#include "application.h"
#include "notification/notification.h"
#include "plugins/plugins.h"
#include "hint.h"

#include <QStringList>
#include <QTextDocument>
#include <QTextDocumentFragment>

class QSystemTrayIcon;


/**
 * Snore is a platform independent Qt notification framework.
 *
 * Environment variable             | Effect
 * ---------------------------------|-------------------------------
 * LIBSNORE_DEBUG_LVL               |   Value betwene 0 and 3 @see SnoreDebugLevels
 * LIBSNORE_LOG_TO_FILE             |   If 1 write to a logfile in tmp/libsnore/appname-log.txt
 * LIBSNORE_LOGFILE                 |   Use with LIBSNORE_LOG_TO_FILE, sets the file to log to
 * LIBSNORE_FORCE_CHACHE_UPDATE     |   Always update the plugin cache
 *
 *
 * @author Patrick von Reth \<vonreth at kde.org\>
 */

namespace Snore{
class SnoreCorePrivate;

/**
 *  SnoreCore is used to manage and emit Notifications
 *
 * @author Patrick von Reth \<vonreth at kde.org\>
 */

class SNORE_EXPORT SnoreCore : public QObject
{
    Q_DECLARE_PRIVATE(SnoreCore)
    Q_OBJECT
public:

    /**
     * Creates a Notification Manager SnoreCore
     * @param trayIcon a QSystemTrayIcon which can later be used by the fallback notification backend.
     */
    SnoreCore (QSystemTrayIcon *trayIcon = NULL );
    ~SnoreCore();

    /**
     * Load a set of plugins
     *
     * @param types the type of tha plugin
     */
    void loadPlugins ( SnorePlugin::PluginTypes types );


    /**
     * Broadcast a notification.
     * @param notification the Notification
     */
    void broadcastNotification( Notification notification );

    /**
     * Register an application.
     * Each application should only be registered once.
     * An application must be registered before a notification can be broadcasted.
     * @see deregisterApplication
     * @see broadcastNotification
     * @param application the application
     */
    void registerApplication(const Application &application );

    /**
     * Deregisters an application.
     * Should be called if an application is no loger used.
     * Is called automatically if the backend changes.
     * @see registerApplication
     * @see setPrimaryNotificationBackend
     * @param application the application
     */
    void deregisterApplication(const Application &application );

    /**
     *
     * @return a QHash of all registered applications
     */
    const QHash<QString, Application> &aplications() const;

    /**
     *
     * @return a list of all known notification backends
     */
    const QStringList notificationBackends() const;

    /**
     *
     * @return a list of all known notification frontends
     */
    const QStringList notificationFrontends() const;

    /**
     *
     * @return a list of all known notification secondary backends
     */
    const QStringList secondaryNotificationBackends() const;

    /**
     * Sets the primary backend.
     *
     * @param backend the name of the backend
     * @return whether the backend was initialied succesfully.
     */
    bool setPrimaryNotificationBackend( const QString &backend );

    /**
     * Tries to set one of all backends availible on this platform as backend.
     * @see primaryNotificationBackend
     * @return whether a backend was succesfully set
     */
    bool setPrimaryNotificationBackend();
    /**
     *
     * @return the name of the active primary backend
     */
    const QString primaryNotificationBackend() const;

    /**
     *
     * @return a pointer to a QSystemTrayIcon if availible, otherwise NULL
     */
    QSystemTrayIcon *trayIcon();

    /**
     * Tries to get an Notification by id.
     * @param id the id of the Notification
     * @return the Notification or an invalid Notification if the Notification was not found
     * @see Notification::isValid
     */
    Notification getActiveNotificationByID(uint id);

    /**
     * Try to close a Notification if the backend supports the action.
     * @see SnoreBackend::canCloseNotification
     */
    void requestCloseNotification(Notification,Notification::CloseReasons);

    /**
     *
     * @return whether the backend supports rhichtext encoding
     */
    bool primaryBackendSupportsRichtext();


    /**
     *
     * @return a pointer to the private class, for internal use only.
     */
    const SnoreCorePrivate *d();


signals:
    /**
     * This signal is emitted when an action on the Notification was performed.
     * Some notification systems don't support actions but will report one if the notification was clicked,
     * in this case the Action will be invalid.
     * @todo maybe introduce a pecial action state for this case
     * @see Action
     */
    void actionInvoked( Snore::Notification );

    /**
     * This signal is emitted when a Notification is closed.
     * @see Notification::CloseReasons
     */
    void notificationClosed(Snore::Notification );

private:
    SnoreCorePrivate *d_ptr;


};

/**
 *
 * @param string A string to decode of if needed.
 * @return if the string was rhichtext or html encoded a decoded string, else the original string.
 */

static inline QString toPlainText ( const QString &string)
{
    if(Qt::mightBeRichText(string))
    {
        return QTextDocumentFragment::fromHtml(string).toPlainText();
    }
    else
    {
        return string;
    }
}

}

#endif // SNORESERVER_H
