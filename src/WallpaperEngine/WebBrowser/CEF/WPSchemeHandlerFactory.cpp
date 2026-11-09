#include "WPSchemeHandlerFactory.h"
#include "WPSchemeHandler.h"
#include "WallpaperEngine/WebBrowser/WebBrowserContext.h"
#include "include/cef_parser.h"
#include "include/wrapper/cef_helpers.h"

using namespace WallpaperEngine::WebBrowser::CEF;

CefRefPtr<CefResourceHandler> WPSchemeHandlerFactory::Create (
    CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& scheme_name,
    CefRefPtr<CefRequest> request
) {
    CEF_REQUIRE_IO_THREAD ();

    CefURLParts parts;

    if (!CefParseURL (request->GetURL (), parts)) {
	return nullptr;
    }

    const std::string host = CefString (&parts.host);

    const Project* project = nullptr;

    {
	std::lock_guard lock (this->m_mutex);

	if (const auto it = this->m_projects.find (host); it != this->m_projects.end ()) {
	    project = it->second;
	}
    }

    // a wallpaper that is gone (switched away from) can still have requests on the way, they just fail
    if (project == nullptr) {
	return nullptr;
    }

    return new WPSchemeHandler (*project);
}

std::string WPSchemeHandlerFactory::registerProject (const Project& project) {
    std::lock_guard lock (this->m_mutex);

    // the same wallpaper can be loaded more than once (one per screen), each one gets its own host
    const std::string host = "wp" + std::to_string (this->m_nextId++);
    this->m_projects[host] = &project;

    return host;
}

void WPSchemeHandlerFactory::unregisterProject (const Project* project) {
    std::lock_guard lock (this->m_mutex);

    std::erase_if (this->m_projects, [project] (const auto& entry) { return entry.second == project; });
}
