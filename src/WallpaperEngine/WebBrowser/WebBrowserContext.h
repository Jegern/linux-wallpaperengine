#pragma once

#include "WallpaperEngine/Application/ApplicationContext.h"
#include "WallpaperEngine/Application/WallpaperApplication.h"
#include "WallpaperEngine/WebBrowser/CEF/WPSchemeHandlerFactory.h"
#include "include/cef_app.h"
#include "include/cef_browser_process_handler.h"
#include "include/wrapper/cef_helpers.h"

#define WPENGINE_SCHEME "wp"

namespace WallpaperEngine::Data::Model {
struct Project;
}

namespace WallpaperEngine::WebBrowser::CEF {
class BrowserApp;
}

namespace WallpaperEngine::WebBrowser {
class WebBrowserContext {
public:
    explicit WebBrowserContext (WallpaperEngine::Application::WallpaperApplication& wallpaperApplication);
    ~WebBrowserContext ();

    /**
     * @return The factory that serves the files of wallpapers through the wp scheme. It is reference counted so
     * users can keep it past the lifetime of this context (wallpapers can outlive it while shutting down)
     */
    [[nodiscard]] CefRefPtr<CEF::WPSchemeHandlerFactory> getSchemeFactory () const;

private:
    CefRefPtr<CefApp> m_browserApplication = nullptr;
    /** The same application as above when this is the main process (subprocesses do not serve files) */
    CEF::BrowserApp* m_mainApplication = nullptr;
    CefRefPtr<CefCommandLine> m_commandLine = nullptr;
    WallpaperEngine::Application::WallpaperApplication& m_wallpaperApplication;
};
} // namespace WallpaperEngine::WebBrowser
