#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <KXmlGuiWindow>

class QStackedLayout;
class KJob;
namespace KSettings
{
class Dialog;
}

class Account;
class TimelineWidget;
class NavButtonsWidget;
class PostWrapper;
class MultiPartDevice;
class TimelineModel;

class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT
public:
    explicit MainWindow();
    virtual ~MainWindow();

private Q_SLOTS:
    void setCurrentTimeline(const QString& timelineName);
    void closeTimeline(const QString& timelineName);
    void slotConfigure();
    void updateTimelines();

    void updateTimeline(const QString& timelineName);
    void updateUserTimeline(const PostWrapper& post);
    void updateUserTimeline(const PostWrapper& post, const QString& username);
    void slotUpdateTimeline(KJob* job);
    void createPost(const PostWrapper& post);
    void slotCreatePost(KJob* job);
    void retweetPost(const PostWrapper& post);
private:
    void setupActions();
    void createTimelineWidget(const QString& timelineName, const QString& iconName, bool checked);
private:
    NavButtonsWidget* m_buttonsWidget;
    QStackedLayout* m_stackedLayout;

    QHash<QString, TimelineModel*> m_timelineModel;
    QHash<QString, TimelineWidget*> m_timelineWidget;

    QHash<KJob*, Account*> m_jobAccount;
    QHash<KJob*, QString> m_jobTimeline;
    QHash<KJob*, PostWrapper> m_jobPost;
    QHash<KJob*, MultiPartDevice*> m_jobMultiPart;
    KSettings::Dialog* m_settingsDialog;
};

#endif // MAINWINDOW_H
