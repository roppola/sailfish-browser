/****************************************************************************
**
** Copyright (C) 2013 Jolla Ltd.
** Contact: Raine Makelainen <raine.makelainen@jollamobile.com>
**
****************************************************************************/

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef DECLARATIVEWEBCONTAINER_H
#define DECLARATIVEWEBCONTAINER_H

#include "settingmanager.h"
#include "tab.h"
#include "webpages.h"

#include <QQuickItem>
#include <QPointer>
#include <QImage>
#include <QFutureWatcher>

class QTimerEvent;
class DeclarativeTabModel;
class DeclarativeWebPage;

class DeclarativeWebContainer : public QQuickItem {
    Q_OBJECT

    Q_PROPERTY(DeclarativeWebPage *contentItem READ webPage NOTIFY contentItemChanged FINAL)
    Q_PROPERTY(DeclarativeTabModel *tabModel READ tabModel WRITE setTabModel NOTIFY tabModelChanged FINAL)
    Q_PROPERTY(bool foreground READ foreground WRITE setForeground NOTIFY foregroundChanged FINAL)
    Q_PROPERTY(int maxLiveTabCount READ maxLiveTabCount WRITE setMaxLiveTabCount NOTIFY maxLiveTabCountChanged FINAL)
    // This property should cover all possible popus
    Q_PROPERTY(bool popupActive MEMBER m_popupActive NOTIFY popupActiveChanged FINAL)
    Q_PROPERTY(bool portrait MEMBER m_portrait NOTIFY portraitChanged FINAL)
    Q_PROPERTY(bool fullscreenMode MEMBER m_fullScreenMode NOTIFY fullscreenModeChanged FINAL)
    Q_PROPERTY(qreal fullscreenHeight MEMBER m_fullScreenHeight NOTIFY fullscreenHeightChanged FINAL)
    Q_PROPERTY(bool inputPanelVisible READ inputPanelVisible NOTIFY inputPanelVisibleChanged FINAL)
    Q_PROPERTY(qreal inputPanelHeight READ inputPanelHeight WRITE setInputPanelHeight NOTIFY inputPanelHeightChanged FINAL)
    Q_PROPERTY(qreal inputPanelOpenHeight MEMBER m_inputPanelOpenHeight NOTIFY inputPanelOpenHeightChanged FINAL)
    Q_PROPERTY(qreal toolbarHeight MEMBER m_toolbarHeight NOTIFY toolbarHeightChanged FINAL)
    Q_PROPERTY(bool background READ background NOTIFY backgroundChanged FINAL)
    Q_PROPERTY(bool allowHiding MEMBER m_allowHiding NOTIFY allowHidingChanged FINAL)

    Q_PROPERTY(QString favicon MEMBER m_favicon NOTIFY faviconChanged)

    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged FINAL)
    Q_PROPERTY(int loadProgress READ loadProgress WRITE setLoadProgress NOTIFY loadProgressChanged FINAL)

    // Navigation related properties
    Q_PROPERTY(bool canGoForward READ canGoForward NOTIFY canGoForwardChanged FINAL)
    Q_PROPERTY(bool canGoBack READ canGoBack NOTIFY canGoBackChanged FINAL)

    Q_PROPERTY(int tabId READ tabId NOTIFY tabIdChanged FINAL)
    Q_PROPERTY(QString title READ title NOTIFY titleChanged FINAL)
    Q_PROPERTY(QString url READ url NOTIFY urlChanged FINAL)

    Q_PROPERTY(QQmlComponent* webPageComponent MEMBER m_webPageComponent NOTIFY webPageComponentChanged FINAL)

    // "private" properties.
    Q_PROPERTY(bool _readyToLoad READ readyToLoad WRITE setReadyToLoad NOTIFY _readyToLoadChanged FINAL)

public:
    DeclarativeWebContainer(QQuickItem *parent = 0);
    ~DeclarativeWebContainer();

    DeclarativeWebPage *webPage() const;

    DeclarativeTabModel *tabModel() const;
    void setTabModel(DeclarativeTabModel *model);

    bool foreground() const;
    void setForeground(bool active);

    int maxLiveTabCount() const;
    void setMaxLiveTabCount(int count);

    bool background() const;

    bool loading() const;

    int loadProgress() const;
    void setLoadProgress(int loadProgress);

    bool inputPanelVisible() const;

    qreal inputPanelHeight() const;
    void setInputPanelHeight(qreal height);

    bool canGoForward() const;
    void setCanGoForward(bool canGoForward);

    bool canGoBack() const;
    void setCanGoBack(bool canGoBack);

    int tabId() const;
    QString title() const;
    QString url() const;
    QString thumbnailPath() const;

    bool readyToLoad() const;
    void setReadyToLoad(bool readyToLoad);

    bool isActiveTab(int tabId);

    Q_INVOKABLE void load(QString url = "", QString title = "", bool force = false);
    Q_INVOKABLE void goForward();
    Q_INVOKABLE void goBack();
    Q_INVOKABLE bool activatePage(int tabId, bool force = false);
    Q_INVOKABLE void loadNewTab(QString url, QString title, int parentId);
    Q_INVOKABLE bool alive(int tabId);

    Q_INVOKABLE void dumpPages() const;

signals:
    void contentItemChanged();
    void tabModelChanged();
    void pageStackChanged();
    void foregroundChanged();
    void backgroundChanged();
    void allowHidingChanged();
    void maxLiveTabCountChanged();
    void popupActiveChanged();
    void portraitChanged();
    void fullscreenModeChanged();
    void fullscreenHeightChanged();
    void inputPanelVisibleChanged();
    void inputPanelHeightChanged();
    void inputPanelOpenHeightChanged();
    void toolbarHeightChanged();

    void faviconChanged();
    void loadingChanged();
    void loadProgressChanged();

    void canGoForwardChanged();
    void canGoBackChanged();

    void tabIdChanged();
    void titleChanged();
    void urlChanged();
    void thumbnailPathChanged();

    void _readyToLoadChanged();

    void webPageComponentChanged();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

public slots:
    void resetHeight(bool respectContentHeight = true);

private slots:
    void imeNotificationChanged(int state, bool open, int cause, int focusChange, const QString& type);
    void handleEnabledChanged();
    void onActiveTabChanged(int oldTabId, int activeTabId, bool loadActiveTab);
    void onModelLoaded();
    void onDownloadStarted();
    void onNewTabRequested(QString url, QString title);
    void onReadyToLoad();
    void onTabsCleared();
    void releasePage(int tabId, bool virtualize = false);
    void closeWindow();
    void onPageUrlChanged();
    void onPageTitleChanged();
    void setActiveTabData();

    // These are here to inform embedlite-components that keyboard is open or close
    // matching composition metrics.
    void sendVkbOpenCompositionMetrics();

private:
    void setWebPage(DeclarativeWebPage *webPage);
    qreal contentHeight() const;
    int parentTabId(int tabId) const;
    void updateNavigationStatus(const Tab &tab);
    void updateVkbHeight();
    void updateUrl(const QString &newUrl);
    void updateTitle(const QString &newTitle);

    QPointer<DeclarativeWebPage> m_webPage;
    QPointer<DeclarativeTabModel> m_model;
    QPointer<QQmlComponent> m_webPageComponent;
    QPointer<SettingManager> m_settingManager;
    QScopedPointer<WebPages> m_webPages;
    bool m_foreground;
    bool m_allowHiding;
    bool m_popupActive;
    bool m_portrait;
    bool m_fullScreenMode;
    qreal m_fullScreenHeight;
    bool m_inputPanelVisible;
    qreal m_inputPanelHeight;
    qreal m_inputPanelOpenHeight;
    qreal m_toolbarHeight;

    QString m_favicon;
    QString m_initialUrl;

    QString m_url;
    QString m_title;
    int m_tabId;

    bool m_loading;
    int m_loadProgress;
    bool m_canGoForward;
    bool m_canGoBack;
    bool m_realNavigation;
    bool m_readyToLoad;

    friend class tst_webview;
};

QML_DECLARE_TYPE(DeclarativeWebContainer)

#endif // DECLARATIVEWEBCONTAINER_H
