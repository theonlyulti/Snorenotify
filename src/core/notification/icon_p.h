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

#ifndef ICONDATA_H
#define ICONDATA_H


#include "notification.h"

#include <QImage>
#include <QSharedData>
#include <QBuffer>
#include <QFile>
#include <QDebug>
#include <QDir>

#include <QMutex>

namespace Snore{

class IconData : public QSharedData
{
public:
    IconData(const QString &url);
    IconData(const QImage &img);
    ~IconData();

    const QImage &image();
    QString localUrl();
    void download();


    QImage m_img;
    QString m_url;
    QString m_hash;
    QString m_localUrl;
    bool m_isLocalFile;
    bool m_isResource;
    bool m_isRemoteFile;
    QMutex m_mutex;

private:
    Q_DISABLE_COPY(IconData)

    inline QString createLocalFileName(const QString &hash)
    {
        static QString tmp;
        if(tmp.isNull())
        {
            tmp = QString("%1/libsnore/").arg(QDir::tempPath());
            QDir(tmp).mkpath(".");
        }
        return QString("%1%2.png").arg(tmp, hash);
    }

};

}
#endif // ICONDATA_H
