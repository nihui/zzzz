#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <QListView>
class PostWrapper;

class TimelineWidget : public QListView
{
    Q_OBJECT
public:
    explicit TimelineWidget(QWidget* parent = 0);
    virtual ~TimelineWidget();
    virtual void resizeEvent(QResizeEvent* event);
Q_SIGNALS:
    void userClicked(const PostWrapper& post);
    void replyClicked(const PostWrapper& post);
    void retweetClicked(const PostWrapper& post);
    void usernameClicked(const PostWrapper& post, const QString& username);
    void topicClicked(const PostWrapper& post, const QString& topic);
    void postComposed(const PostWrapper& post);
private Q_SLOTS:
    void slotAnchorClicked(const QString& anchor, const QModelIndex& index);
private:
};

#endif // TIMELINEWIDGET_H
