/*
 * Qt-ponies - ponies on the desktop
 * Copyright (C) 2013 funeralismatic, XRevan86
 * Copyright (C) 2012-2013 mysha
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtWidgets/QApplication>
#include <QtCore/QLocale>
#include <QTranslator>
#include <QDebug>
#include <QFile>
#include <QSettings>

#include "csv_parser.h"
#include "behavior.h"
#include "effect.h"
#include "speak.h"

#include "singleapplication.h"
#include "configwindow.h"
#include "pony.h"

#define APPID "qt-ponies"

int main(int argc, char *argv[])
{
    CSVParser::AddParseTypes("Behavior", Behavior::OptionTypes);
    CSVParser::AddParseTypes("Effect", Effect::OptionTypes);
    CSVParser::AddParseTypes("Speak", Speak::OptionTypes);

    DSingleApplication app(APPID, argc, argv);
    QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath());
    QCoreApplication::setOrganizationName("qt-ponies");
    QCoreApplication::setApplicationName("qt-ponies");

    QString locale = QLocale::system().name().left(2).toLatin1();
    QString trans_dir = QCoreApplication::applicationDirPath() + "/translations";

    QTranslator translator;
    translator.load(QString("qt-ponies_%1").arg(locale), trans_dir);
    app.installTranslator(&translator);

    app.setQuitOnLastWindowClosed(false);
    QSettings::setDefaultFormat(QSettings::IniFormat);

    if ( app.isRunning() ) {
        app.sendMessage("showConfigWindow");
        return 0;
    }

    QFile qss(":/styles/res/style.qss");
    qss.open(QFile::ReadOnly);
    app.setStyleSheet( QString::fromUtf8(qss.readAll()) );
    qss.close();

    ConfigWindow config;
    QObject::connect(qApp, SIGNAL(messageReceived(QString)), &config, SLOT(receiveFromInstance(QString)));

    qDebug() << "Locale:" << locale;

    if(config.ponies.size() == 0) {
        config.show();
    }

    return app.exec();
}
