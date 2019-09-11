/****************************************************************************************
**
**  VLIBS codebase, NIIAS
**
**  Authors:
**  Alexandre Gromtsev aka elapidae     elapidae@yandex.ru
**  Nadezhda Churikova aka claorisel    claorisel@gmail.com
**  Ekaterina Boltenkova aka kataretta  kitkat52@yandex.ru
**  Ivan Deylid aka sid1057             ivanov.dale@gmail.com>
**
**  GNU Lesser General Public License Usage
**  This file may be used under the terms of the GNU Lesser General Public License
**  version 3 as published by the Free Software Foundation and appearing in the file
**  LICENSE.LGPL3 included in the packaging of this file. Please review the following
**  information to ensure the GNU Lesser General Public License version 3 requirements
**  will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
****************************************************************************************/

#ifdef V_HAS_QT

#include "vcat_qt_deploy.h"

#include <QString>
#include <QByteArray>
#include <QPoint>

std::ostream &operator <<( std::ostream &os, const QString &qstr )
{
    os << qstr.toStdString();
    return os;
}

std::ostream &operator <<( std::ostream &os, const QByteArray &qba )
{
    os << qba.toStdString();
    return os;
}

std::ostream &operator <<( std::ostream &os, const QPoint &p )
{
    os << "QPoint(" << p.x() << ',' << p.y() << ')';
    return os;
}

std::ostream &operator <<( std::ostream &os, const QPointF &p )
{
    os << "QPointF(" << p.x() << ',' << p.y() << ')';
    return os;
}

#endif // V_HAS_QT
