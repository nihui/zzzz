#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <KXmlGuiWindow>

class QStackedLayout;
class KJob;
namespace KSettings { class Dialog; }

class Account;
class TimelineWidget;
class ComposerWidget;
class NavButtonsWidget;
class PostWrapper;

class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT
    public:
        explicit MainWindow();
        virtual ~MainWindow();

    private Q_SLOTS:
        void setCurrentTimeline( const QString& timelineName );
        void slotConfigure();
        void updateTimelines();

        void updateTimeline( const QString& timelineName );
        void updateUserTimeline( const PostWrapper* post );
        void slotUpdateTimeline( KJob* job );
        void createPost( const PostWrapper* post );
        void slotCreatePost( KJob* job );
        void retweetPost( const PostWrapper* post );
    private:
        void setupActions();
        void createTimelineWidget( const QString& timelineName, const QString& iconName );
    private:
        NavButtonsWidget* m_buttonsWidget;
        QStackedLayout* m_stackedLayout;

        QHash<QString, TimelineWidget*> m_timelineWidget;

        ComposerWidget* m_composerWidget;
        QHash<KJob*, Account*> m_jobAccount;
        QHash<KJob*, QString> m_jobTimeline;
//         QHash<KJob*, PostWrapper*> m_jobPost;
        KSettings::Dialog* m_settingsDialog;
};

#endif // MAINWINDOW_H
