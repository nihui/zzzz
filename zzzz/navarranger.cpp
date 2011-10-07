#include "navarranger.h"

NavArranger::NavArranger( QWidget* parent )
: KDialog(parent)
{
    setButtons( KDialog::Default | KDialog::Ok | KDialog::Apply | KDialog::Cancel );
}

NavArranger::~NavArranger()
{
}
