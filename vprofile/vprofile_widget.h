#ifdef V_HAS_QT
#ifndef VPROFILE_WIDGET_H
#define VPROFILE_WIDGET_H

#include <QTableWidget>
#include "vsettings.h"

//=======================================================================================
class VProfile_Widget : public QTableWidget
{
    Q_OBJECT
public:
    explicit VProfile_Widget( QWidget* parent = nullptr );
    ~VProfile_Widget() override;

    void set_settings( vsettings settings );
    void save_settings();

private:
    vsettings _settings;
};
//=======================================================================================


#endif // VPROFILE_WIDGET_H
#endif // V_HAS_QT
