#ifdef V_HAS_QT

#include "vprofile_widget.h"

#include <QHeaderView>
#include <QTimer>

#include "vprofile.h"
#include "vcat.h"

using namespace std::chrono;

//=======================================================================================
VProfile_Widget::VProfile_Widget(QWidget *parent)
    : QTableWidget( parent )
{
    V_PROFILE;

    setColumnCount(4);
    horizontalHeader()->setVisible(true);
    setHorizontalHeaderItem(0, new QTableWidgetItem("file") );
    setHorizontalHeaderItem(1, new QTableWidgetItem("line") );
    setHorizontalHeaderItem(2, new QTableWidgetItem("function") );
    setHorizontalHeaderItem(3, new QTableWidgetItem("time, sec") );

    auto timer = new QTimer(this);
    timer->start( 250 );
    connect( timer, &QTimer::timeout, [this]
    {
        V_PROFILE;
        setRowCount(0);
        auto list = vprofile::current();

        setRowCount( list.size() );
        int row = 0;

        for ( auto& rec: list )
        {
            auto line = QString::number( rec.line );
            auto sec = duration_cast<nanoseconds>(rec.duration).count() / 1e9;
            auto time = QString::number( sec );

            auto twitem = [](QString text){ return new QTableWidgetItem(text); };

            setItem( row, 0, twitem(rec.file)   );
            setItem( row, 1, twitem(line)       );
            setItem( row, 2, twitem(rec.func)   );
            setItem( row, 3, twitem(time)       );
            ++row;
        }
    });
    horizontalHeader()->setStretchLastSection(true);
}
//=======================================================================================
VProfile_Widget::~VProfile_Widget()
{
    _settings.set( "geometry", saveGeometry().toStdString() );
    _settings.set( "headers", horizontalHeader()->saveState().toStdString() );
}
//=======================================================================================
void VProfile_Widget::set_settings( vsettings settings )
{
    _settings = settings;

    auto head = _settings.safe_get( "headers" );
    auto geom = _settings.safe_get( "geometry" );
    restoreGeometry( QByteArray(geom.c_str(),geom.size()) );
    horizontalHeader()->restoreState( QByteArray(head.c_str(),head.size()) );
}
//=======================================================================================
#endif // V_HAS_QT
