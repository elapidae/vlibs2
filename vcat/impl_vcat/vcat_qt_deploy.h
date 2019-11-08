/****************************************************************************************
**
**  VLIBS codebase, NIIAS
**
**  GNU Lesser General Public License Usage
**  This file may be used under the terms of the GNU Lesser General Public License
**  version 3 as published by the Free Software Foundation and appearing in the file
**  LICENSE.LGPL3 included in the packaging of this file. Please review the following
**  information to ensure the GNU Lesser General Public License version 3 requirements
**  will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
****************************************************************************************/

#ifndef IMPL_VCAT_QT_DEPLOY_H
#define IMPL_VCAT_QT_DEPLOY_H

#ifdef V_HAS_QT
    #include <ostream>

    class QString;
    std::ostream& operator <<( std::ostream& os, const QString& );

    class QByteArray;
    std::ostream& operator <<( std::ostream& os, const QByteArray& );

    class QPoint;
    std::ostream& operator <<( std::ostream& os, const QPoint& );

    class QPointF;
    std::ostream& operator <<( std::ostream& os, const QPointF& );
#endif // V_HAS_QT

#endif // IMPL_VCAT_QT_DEPLOY_H
