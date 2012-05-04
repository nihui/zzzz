#ifndef COMPOSERWIDGET_H
#define COMPOSERWIDGET_H

#include <KTextEdit>

class Account;
class PostWrapper;

class ComposerWidget : public KTextEdit
{
    Q_OBJECT
public:
    static ComposerWidget* self();
    virtual ~ComposerWidget();
    void setCharLimit(int limit);
    void reset();
Q_SIGNALS:
    void postComposed(const PostWrapper& post);
public Q_SLOTS:
    void composeReply(const PostWrapper& post);
protected:
    virtual void focusOutEvent(QFocusEvent* event);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void resizeEvent(QResizeEvent* event);
private Q_SLOTS:
    void slotTextChanged();
private:
    explicit ComposerWidget(QWidget* parent = 0);
    static ComposerWidget* m_self;
    int m_limit;
    Account* m_replyAccount;
    QString m_replyToStatusId;
};

#endif // COMPOSERWIDGET_H
