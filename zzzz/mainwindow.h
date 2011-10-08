#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <microblog.h>

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
        void slotConfigure();
        void updateTimelines();
//         void updateHomeTimeline();
//         void updatePublicTimeline();

        void updateTimeline( Zzzz::MicroBlog::Timeline t );
        void updateUserTimeline( const PostWrapper* post );
        void slotUpdateTimeline( KJob* job );
        void createPost( const PostWrapper* post );
        void slotCreatePost( KJob* job );
        void retweetPost( const PostWrapper* post );
    private:
        void setupActions();
        void createTimelineWidget( Zzzz::MicroBlog::Timeline t, const QString& iconName );
    private:
        NavButtonsWidget* m_buttonsWidget;
        QStackedLayout* m_stackedLayout;
        QHash<Zzzz::MicroBlog::Timeline, TimelineWidget*> m_timelineWidget;

        ComposerWidget* m_composerWidget;
        QHash<KJob*, Account*> m_jobAccount;
        QHash<KJob*, Zzzz::MicroBlog::Timeline> m_jobTimeline;
//         QHash<KJob*, PostWrapper*> m_jobPost;
        KSettings::Dialog* m_settingsDialog;
};

#endif // MAINWINDOW_H
