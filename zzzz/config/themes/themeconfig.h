#ifndef THEMECONFIG_H
#define THEMECONFIG_H

#include <KCModule>
#include <QTextEdit>

class QListWidget;
class KAction;

class ZzzzThemeConfig : public KCModule
{
    Q_OBJECT
public:
    explicit ZzzzThemeConfig(QWidget* parent, const QVariantList& args);
    virtual ~ZzzzThemeConfig();
public Q_SLOTS:
    virtual void load();
    virtual void save();
    virtual void defaults();
private:
    QListWidget* m_themeListView;
    QTextEdit* m_preview;
};

#endif // THEMECONFIG_H
