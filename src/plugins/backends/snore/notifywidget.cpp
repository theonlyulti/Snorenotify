/*
    SnoreNotify is a Notification Framework based on Qt
    Copyright (C) 2014  Patrick von Reth <vonreth@kde.org>


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

#include "notifywidget.h"
#include "ui_notifywidget.h"
#include "core/log.h"

#include <QDesktopWidget>
#include <QPicture>

using namespace Snore;

NotifyWidget::NotifyWidget(int pos, int corner, QWidget *parent) :
    QWidget(parent, Qt::SplashScreen | Qt::WindowStaysOnTopHint),
    ui(new Ui::NotifyWidget),
    m_desktop(QDesktopWidget().availableGeometry()),
    m_id(pos),
    m_corner(corner)
{
    ui->setupUi(this);

    TomahawkUtils::DpiScaler::setFontSize(this->ui->titel->fontInfo().pointSize());
    m_scaler = new TomahawkUtils::DpiScaler(this);
    ui->closeButton->setMaximumWidth(ui->closeButton->height());

    setFixedSize( m_scaler->scaled(300, 80));

    switch(m_corner)
    {
    case 0:
        m_dest = QPoint(m_desktop.topLeft().x(), m_desktop.topLeft().y() + (m_scaler->scaledY(10) + height()) * m_id);
        m_start = QPoint(m_desktop.topLeft().x() - width(), m_desktop.topLeft().y() + (m_scaler->scaledY(10) + height()) * m_id );
        m_xDir = 1;
        m_yDir = 0;
        break;
    case 1:
        m_dest = QPoint(m_desktop.topRight().x() - width(), m_desktop.topRight().y() + (m_scaler->scaledY(10) + height()) * m_id);
        m_start = QPoint(m_desktop.topRight().x(), m_desktop.topRight().y() + (m_scaler->scaledY(10) + height()) * m_id);
        m_xDir = -1;
        m_yDir = 0;
        break;
    case 2:
        m_dest = QPoint(m_desktop.bottomRight().x() - width(), m_desktop.bottomRight().y() - (m_scaler->scaledY(10) + height()) * m_id);
        m_start = QPoint(m_desktop.bottomRight().x(), m_desktop.bottomRight().y() + (m_scaler->scaledY(10) + height()) * m_id);
        m_xDir = -1;
        m_yDir = 0;
        break;
    case 3:
        m_dest = QPoint(m_desktop.bottomLeft().x(), m_desktop.bottomLeft().y() - (m_scaler->scaledY(10) + height()) * m_id);
        m_start = QPoint(m_desktop.bottomLeft().x() - width(), m_desktop.bottomLeft().y() + (m_scaler->scaledY(10) + height()) * m_id);
        m_xDir = 1;
        m_yDir = 0;
        break;
    }
}

NotifyWidget::~NotifyWidget()
{
    delete m_scaler;
    delete ui;
}

void NotifyWidget::display(const Notification &notification)
{
    update(notification);
    move(m_start);
    show();
    m_moveTimer = new QTimer(this);
    m_moveTimer->setInterval(2);
    connect( m_moveTimer, SIGNAL(timeout()), this, SLOT(slotMove()));
    m_moveTimer->start();

}

void NotifyWidget::update(const Notification &notification)
{
    m_notification = notification;
    ui->titel->setText(notification.title());
    ui->body->setText(notification.text());

    QSize iconSize = m_scaler->scaled(65,65);
    ui->icon->setPixmap(QPixmap::fromImage(notification.icon().image().scaled(iconSize, Qt::KeepAspectRatio, Qt::SmoothTransformation)));

    iconSize = m_scaler->scaled(20,20);
    QImage img = notification.application().icon().image().scaled(iconSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->appIcon->setPixmap(QPixmap::fromImage(img));
    setPalette(img);
}

Notification &NotifyWidget::notification()
{
    return m_notification;
}

int NotifyWidget::id()
{
    return m_id;
}

void NotifyWidget::slotMove()
{
    move(pos().x() + m_xDir, pos().y() + m_yDir);
    if(m_dest == pos())
    {
        m_moveTimer->deleteLater();
    }
}

void NotifyWidget::on_closeButton_clicked()
{
    emit dismissed();
    hide();
}

void NotifyWidget::mousePressEvent(QMouseEvent *e)
{
    emit invoked();
    hide();
    QWidget::mousePressEvent(e);
}

void NotifyWidget::setPalette(const QImage &img)
{
    qulonglong r = 0;
    qulonglong g = 0;
    qulonglong b = 0;
    for(int x=0;x<img.width();++x)
    {
        for(int y=0;y<img.height();++y)
        {
            QRgb c = img.pixel(x,y);
            r += qRed(c);
            g += qGreen(c);
            b += qBlue(c);
        }
    }
    int s = img.width()*img.height();

    QPalette p = palette();
    QColor bg = QColor(r/s, g/s, b/s);
    p.setColor(QPalette::All, QPalette::Window, bg);
    p.setColor(QPalette::All, QPalette::Background, bg);
    p.setColor(QPalette::All, QPalette::Base, bg);
    p.setColor(QPalette::All, QPalette::Text, Qt::white);
    p.setColor(QPalette::All, QPalette::BrightText, Qt::white);
    p.setColor(QPalette::All, QPalette::ButtonText, Qt::white);
    p.setColor(QPalette::All, QPalette::WindowText, Qt::white);
    QWidget::setPalette(p);
    ui->closeButton->setPalette(p);
    ui->body->setPalette(p);
    ui->titel->setPalette(p);
}
