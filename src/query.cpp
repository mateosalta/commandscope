#include <query.h>
#include <localization.h>

#include <unity/scopes/Annotation.h>
#include <unity/scopes/CategorisedResult.h>
#include <unity/scopes/CategoryRenderer.h>
#include <unity/scopes/QueryBase.h>
#include <unity/scopes/SearchReply.h>

#include <iomanip>
#include <iostream>
#include <sstream>

namespace sc = unity::scopes;

using namespace std;

const static string FORECAST_TEMPLATE =
        R"(
{
        "schema-version": 1,
        "template": {
        "category-layout": "vertical-journal",
        "card-layout": "horizontal",
        "card-size": "small",
                     "card-background": "color:///#000000"

        },
        "components": {
        "title": "title",
        "emblem" : {
        "field": "mascot"
        },

        "subtitle": "subtitle",
        "summary": "summary"
        }
        }
        )";

const static string WICKED_TEMPLATE =
        R"(
{
        "schema-version": 1,
        "template": {
        "category-layout": "vertical-journal",
        "card-layout": "horizontal",
        "card-size": "small",
        "card-background": "color:///#ED3146"
        },
        "components": {
        "title": "title",
        "emblem" : {
        "field": "mascot"
        },
        "subtitle": "subtitle",
        "summary": "summary"
        }
        }
        )";

Query::Query(const sc::CannedQuery &query, const sc::SearchMetadata &metadata,
             Client::Config::Ptr config) :
    sc::SearchQueryBase(query, metadata), client_(config) {
}

void Query::cancelled() {
    client_.cancel();
}


void Query::run(sc::SearchReplyProxy const& reply) {

    sc::Department::SPtr all_depts = sc::Department::create("", query(), ("Deaprtments"));

    sc::Department::SPtr ubu_department = sc::Department::create("ubuntu-phone", query(), ("ubuntu-phone"));
    sc::Department::SPtr new_department = sc::Department::create("browse/last-month", query(), ("new-last-month"));
    sc::Department::SPtr top_department = sc::Department::create("browse/sort-by-votes", query(), ("all-time-greats"));
    sc::Department::SPtr forthewicked_department = sc::Department::create("forthewicked", query(), ("for-the-wicked"));

    sc::Department::SPtr tags_department = sc::Department::create("tags", query(), ("tags"));

    sc::Department::SPtr awk_department = sc::Department::create("awk", query(), ("awk"));
    sc::Department::SPtr bash_department = sc::Department::create("bash", query(), ("bash"));
    sc::Department::SPtr curl_department = sc::Department::create("curl", query(), ("curl"));
    sc::Department::SPtr find_department = sc::Department::create("find", query(), ("find"));
    sc::Department::SPtr grep_department = sc::Department::create("grep", query(), ("grep"));
    sc::Department::SPtr sed_department = sc::Department::create("sed", query(), ("sed"));
    sc::Department::SPtr echo_department = sc::Department::create("echo", query(), ("echo"));
    sc::Department::SPtr ls_department = sc::Department::create("ls", query(), ("ls"));
    sc::Department::SPtr cat_department = sc::Department::create("cat", query(), ("cat"));

    all_depts->set_subdepartments({ubu_department,
                                   new_department,
                                   top_department,
                                   forthewicked_department,
                                   tags_department
                                   });
    tags_department->set_subdepartments({awk_department,
                                        bash_department,
                                        curl_department,
                                        find_department,
                                        grep_department,
                                        sed_department,
                                        echo_department,
                                        ls_department,
                                        cat_department,
                                        });
    reply->register_departments(all_depts);

    try {
        const sc::CannedQuery &query(sc::SearchQueryBase::query());

        string query_string = query.query_string();
        const string department = query.department_id();

        Client::Forecast forecast;

        if (department == "") {
        auto new_sys = "matching/";
        if (settings().at("sort").get_bool() == true) { auto sort_sys = "sort-by-votes";
        if (query_string.empty()) { forecast = client_.forecast_daily("ubuntu", new_sys, sort_sys);
        } else { forecast = client_.forecast_daily(query_string, new_sys, sort_sys);
        }} else { auto sort_sys = "";
            if (query_string.empty()) { forecast = client_.forecast_daily("ubuntu", new_sys, sort_sys);
            } else { forecast = client_.forecast_daily(query_string, new_sys, sort_sys); }}
        }

        if (department == "browse/last-month") {
        auto new_sys = "browse/last-month";
        if (settings().at("sort").get_bool() == true) { auto sort_sys = "sort-by-votes";
        if (query_string.empty()) { forecast = client_.forecast_daily("", new_sys, sort_sys);
        } else { forecast = client_.forecast_daily(query_string, new_sys, sort_sys);
        }} else { auto sort_sys = "";
            if (query_string.empty()) { forecast = client_.forecast_daily("", new_sys, sort_sys);
            } else { forecast = client_.forecast_daily(query_string, new_sys, sort_sys); }}
        }

        if (department == "ubuntu-phone") {
        auto new_sys = "tagged/2987/ubuntu-phone";
        if (settings().at("sort").get_bool() == true) { auto sort_sys = "sort-by-votes";
        if (query_string.empty()) { forecast = client_.forecast_daily("", new_sys, sort_sys);
        } else { forecast = client_.forecast_daily(query_string, new_sys, sort_sys);
        }} else { auto sort_sys = "";
            if (query_string.empty()) { forecast = client_.forecast_daily("", new_sys, sort_sys);
            } else { forecast = client_.forecast_daily(query_string, new_sys, sort_sys); }}
        }

        if (department == "forthewicked") {
        auto new_sys = "forthewicked";
        if (settings().at("sort").get_bool() == true) { auto sort_sys = "sort-by-votes";
        if (query_string.empty()) { forecast = client_.forecast_daily("", new_sys, sort_sys);
        } else { forecast = client_.forecast_daily(query_string, new_sys, sort_sys);
        }} else { auto sort_sys = "";
            if (query_string.empty()) { forecast = client_.forecast_daily("", new_sys, sort_sys);
            } else { forecast = client_.forecast_daily(query_string, new_sys, sort_sys); }}

        auto forecast_cat = reply->register_category("forecast", _("Commands"), "",
                                                     sc::CategoryRenderer(WICKED_TEMPLATE));
        for (const auto &weather : forecast.weather) {
            sc::CategorisedResult res(forecast_cat);
            res.set_uri(weather.url);
            res["title"] = weather.summary;
            res["mascot"] = "http://www.commandlinefu.com/images/favicon.ico";
            res["subtitle"] = "⇳" + weather.votes;
            res["summary"] = weather.command;
            res["term"] = "application:///com.ubuntu.terminal_terminal_0.7.140.desktop";
            res["clp"] = "scope://com.canonical.scopes.clickstore?q=terminal";
            res["mmm"] = weather.command;
            res["art"] = "/usr/share/icons/suru/actions/scalable/info.svg";
            if (!reply->push(res)) {
                return;
            }
        }
        }

        if (department == "tags") {
        auto new_sys = "matching";
        if (settings().at("sort").get_bool() == true) { auto sort_sys = "sort-by-votes";
        if (query_string.empty()) { forecast = client_.forecast_daily("", new_sys, sort_sys);
        } else { forecast = client_.forecast_daily(query_string, new_sys, sort_sys);
        }} else { auto sort_sys = "";
            if (query_string.empty()) { forecast = client_.forecast_daily("", new_sys, sort_sys);
            } else { forecast = client_.forecast_daily(query_string, new_sys, sort_sys); }}
        }

        if (department == "awk") {
        auto new_sys = "using/awk";
        if (settings().at("sort").get_bool() == true) { auto sort_sys = "sort-by-votes";
        if (query_string.empty()) { forecast = client_.forecast_daily("", new_sys, sort_sys);
        } else { forecast = client_.forecast_daily(query_string, new_sys, sort_sys);
        }} else { auto sort_sys = "";
            if (query_string.empty()) { forecast = client_.forecast_daily("", new_sys, sort_sys);
            } else { forecast = client_.forecast_daily(query_string, new_sys, sort_sys); }}
        }

        if (department == "bash") {
        auto new_sys = "tagged/34/bash";
        if (settings().at("sort").get_bool() == true) { auto sort_sys = "sort-by-votes";
        if (query_string.empty()) { forecast = client_.forecast_daily("", new_sys, sort_sys);
        } else { forecast = client_.forecast_daily(query_string, new_sys, sort_sys);
        }} else { auto sort_sys = "";
            if (query_string.empty()) { forecast = client_.forecast_daily("", new_sys, sort_sys);
            } else { forecast = client_.forecast_daily(query_string, new_sys, sort_sys); }}
        }

        if (department == "curl") {
        auto new_sys = "tagged/235/curl";
        if (settings().at("sort").get_bool() == true) { auto sort_sys = "sort-by-votes";
        if (query_string.empty()) {
            forecast = client_.forecast_daily("", new_sys, sort_sys);
        } else { forecast = client_.forecast_daily(query_string, new_sys, sort_sys);
        }} else { auto sort_sys = "";
            if (query_string.empty()) { forecast = client_.forecast_daily("", new_sys, sort_sys);
            } else { forecast = client_.forecast_daily(query_string, new_sys, sort_sys); }}
        }

        if (department == "find") {
        auto new_sys = "tagged/123/find";
        if (settings().at("sort").get_bool() == true) { auto sort_sys = "sort-by-votes";
        if (query_string.empty()) { forecast = client_.forecast_daily("", new_sys, sort_sys);
        } else { forecast = client_.forecast_daily(query_string, new_sys, sort_sys);
        }} else { auto sort_sys = "";
            if (query_string.empty()) { forecast = client_.forecast_daily("", new_sys, sort_sys);
            } else { forecast = client_.forecast_daily(query_string, new_sys, sort_sys); }}
        }

        if (department == "grep") {
        auto new_sys = "tagged/163/grep";
        if (settings().at("sort").get_bool() == true) { auto sort_sys = "sort-by-votes";
        if (query_string.empty()) { forecast = client_.forecast_daily("", new_sys, sort_sys);
        } else { forecast = client_.forecast_daily(query_string, new_sys, sort_sys);
        }} else { auto sort_sys = "";
            if (query_string.empty()) { forecast = client_.forecast_daily("", new_sys, sort_sys);
            } else { forecast = client_.forecast_daily(query_string, new_sys, sort_sys); }}
        }

        if (department == "sed") {
        auto new_sys = "tagged/110/sed";
        if (settings().at("sort").get_bool() == true) { auto sort_sys = "sort-by-votes";
        if (query_string.empty()) { forecast = client_.forecast_daily("", new_sys, sort_sys);
        } else { forecast = client_.forecast_daily(query_string, new_sys, sort_sys);
        }} else { auto sort_sys = "";
            if (query_string.empty()) { forecast = client_.forecast_daily("", new_sys, sort_sys);
            } else { forecast = client_.forecast_daily(query_string, new_sys, sort_sys); }}
        }

        if (department == "echo") {
        auto new_sys = "using/echo";
        if (settings().at("sort").get_bool() == true) { auto sort_sys = "sort-by-votes";
        if (query_string.empty()) { forecast = client_.forecast_daily("", new_sys, sort_sys);
        } else { forecast = client_.forecast_daily(query_string, new_sys, sort_sys);
        }} else { auto sort_sys = "";
            if (query_string.empty()) { forecast = client_.forecast_daily("", new_sys, sort_sys);
            } else { forecast = client_.forecast_daily(query_string, new_sys, sort_sys); }}
        }

        if (department == "ls") {
        auto new_sys = "using/ls";
        if (settings().at("sort").get_bool() == true) { auto sort_sys = "sort-by-votes";
        if (query_string.empty()) { forecast = client_.forecast_daily("", new_sys, sort_sys);
        } else { forecast = client_.forecast_daily(query_string, new_sys, sort_sys);
        }} else { auto sort_sys = "";
            if (query_string.empty()) { forecast = client_.forecast_daily("", new_sys, sort_sys);
            } else { forecast = client_.forecast_daily(query_string, new_sys, sort_sys); }}
        }

        if (department == "cat") {
        auto new_sys = "using/cat";
        if (settings().at("sort").get_bool() == true) { auto sort_sys = "sort-by-votes";
        if (query_string.empty()) { forecast = client_.forecast_daily("", new_sys, sort_sys);
        } else { forecast = client_.forecast_daily(query_string, new_sys, sort_sys);
        }} else { auto sort_sys = "";
            if (query_string.empty()) { forecast = client_.forecast_daily("", new_sys, sort_sys);
            } else { forecast = client_.forecast_daily(query_string, new_sys, sort_sys); }}
        }

        if (department == "browse/sort-by-votes") {
        auto new_sys = "browse";
        auto sort_sys = "sort-by-votes";
        if (query_string.empty()) { forecast = client_.forecast_daily("", new_sys, sort_sys);
        } else { forecast = client_.forecast_daily(query_string, new_sys, sort_sys); }}
        auto forecast_cat = reply->register_category("forecast", _("Commands"), "",
                                                     sc::CategoryRenderer(FORECAST_TEMPLATE));
        for (const auto &weather : forecast.weather) {
            sc::CategorisedResult res(forecast_cat);
            res.set_uri(weather.url);
            res["title"] = weather.summary;
            res["mascot"] = "http://www.commandlinefu.com/images/favicon.ico";
            res["subtitle"] = "⇳" + weather.votes;
            res["summary"] = weather.command;
            res["term"] = "application:///com.ubuntu.terminal_terminal_0.7.140.desktop";
            res["clp"] = "scope://com.canonical.scopes.clickstore?q=terminal";
            res["mmm"] = weather.command;
            if (!reply->push(res)) {
                return;
            }
        }


    } catch (domain_error &e) {
        // Handle exceptions being thrown by the client API
        //cerr << e.what() << endl;
        reply->error(current_exception());
    }
}

