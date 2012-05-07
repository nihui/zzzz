#ifndef COMPOSERWIDGET_H
#define COMPOSERWIDGET_H

#include "ui_composerwidget.h"

class Account;
class PostWrapper;

class ComposerWidget : public QWidget, public Ui::ComposerWidget
{
    Q_OBJECT
public:
    explicit ComposerWidget(QWidget* parent = 0);
    virtual ~ComposerWidget();
    void setCharLimit(int limit);
    void reset();
    void composeReply(const PostWrapper& post);
Q_SIGNALS:
    void postComposed(const PostWrapper& post);
    void postDiscarded();
protected:
    virtual bool eventFilter(QObject* obj, QEvent* event);
private Q_SLOTS:
    void updateIndicator();
private:
    int m_limit;
    Account* m_replyAccount;
    QString m_replyToStatusId;
};

#endif // COMPOSERWIDGET_H
