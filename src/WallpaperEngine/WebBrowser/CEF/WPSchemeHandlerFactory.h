#pragma once

#include "include/cef_scheme.h"
#include <map>
#include <mutex>
#include <string>

namespace WallpaperEngine::Data::Model {
struct Project;
}

namespace WallpaperEngine::WebBrowser::CEF {
using namespace WallpaperEngine::Data::Model;

/**
 * Creates the scheme handlers for the wp:// scheme, which serves the files of web wallpapers.
 *
 * Custom schemes can only be declared to CEF once, before it starts, so there is a single scheme for every
 * wallpaper and the wallpaper is picked by the host part of the url (wp://<host>/<file>). Wallpapers
 * are registered here as they get loaded, which lets a playlist switch to wallpapers that were not known
 * when CEF started.
 */
class WPSchemeHandlerFactory : public CefSchemeHandlerFactory {
public:
    WPSchemeHandlerFactory () = default;

    CefRefPtr<CefResourceHandler> Create (
	CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& scheme_name,
	CefRefPtr<CefRequest> request
    ) override;

    /**
     * Makes the project's files available under its own host
     *
     * @return The host to use in urls for this project
     */
    std::string registerProject (const Project& project);

    /**
     * Stops serving the project's files, only the address of the project is used so it can be gone already
     */
    void unregisterProject (const Project* project);

private:
    std::mutex m_mutex;
    std::map<std::string, const Project*> m_projects = {};
    uint64_t m_nextId = 0;

    IMPLEMENT_REFCOUNTING (WPSchemeHandlerFactory);
    DISALLOW_COPY_AND_ASSIGN (WPSchemeHandlerFactory);
};
} // namespace WallpaperEngine::WebBrowser::CEF
