#ifndef NAVARRANGER_H
#define NAVARRANGER_H

#include <KDialog>

class NavArranger : public KDialog
{
    Q_OBJECT
    public:
        explicit NavArranger( QWidget* parent = 0 );
        virtual ~NavArranger();
};

#endif // NAVARRANGER_H
