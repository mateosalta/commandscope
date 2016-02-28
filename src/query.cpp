#include <query.h>
#include <localization.h>

#include <unity/scopes/Annotation.h>
#include <unity/scopes/CategorisedResult.h>
#include <unity/scopes/CategoryRenderer.h>
#include <unity/scopes/QueryBase.h>
#include <unity/scopes/SearchReply.h>

#include <unity/scopes/FilterBase.h>
#include <unity/scopes/FilterOption.h>
#include <unity/scopes/FilterState.h>
#include <unity/scopes/OptionSelectorFilter.h>

#include <iomanip>
#include <iostream>
#include <sstream>

namespace sc = unity::scopes;

using namespace std;

const static string MAIN_TEMPLATE =
        R"(
{
        "schema-version": 1,
        "template": {
        "category-layout": "vertical-journal",
        "card-layout": "vertical",
        "card-size": "large",
                     "card-background": "color:///#000000",
                    "overlay": true

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
const static string NM_TEMPLATE =
        R"(
{
        "schema-version": 1,
        "template": {
        "category-layout": "vertical-journal",
        "card-layout": "horizontal",
        "card-size": "large",
                     "card-background": "color:///#FFFFFF"

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
const static string OCD_TEMPLATE =
        R"(
{
        "schema-version": 1,
        "template": {
        "category-layout": "grid",
        "card-layout": "horizontal",
        "card-size": "large",
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
const static string PHONE_TEMPLATE =
        R"(
{
        "schema-version": 1,
        "template": {
        "category-layout": "grid",
        "card-layout": "horizontal",
        "card-size": "large",
                     "card-background": "color:///#000000"

        },
        "components": {
        "title": "title",
        "mascot" : {
        "field": "phone"
        },
        "subtitle": "summary"

        }
        }
        )";
const static string BOTH_TEMPLATE =
        R"(
{
        "schema-version": 1,
        "template": {
        "category-layout": "grid",
        "card-layout": "horizontal",
        "card-size": "large",
                     "card-background": "color:///#FFFFFF"

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
        "card-layout": "horizontal-list",
        "card-size": "large",
        "card-background": "color:///#2E0000",
        "overlay": true

        },
        "components": {
        "title": "title",
        "mascot" : {
        "field": "warning"
        },
        "subtitle": "subtitle",
        "summary": "summary"
        }
        }
        )";
const static string WOCD_TEMPLATE =
        R"(
{
        "schema-version": 1,
        "template": {
        "category-layout": "grid",
        "card-layout": "horizontal-list",
        "card-size": "large",
        "card-background": "color:///#2E0000",
        "overlay": true

        },
        "components": {
        "title": "title",
        "mascot" : {
        "field": "warning"
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

    //sc::CannedQuery qwerty(sc::SearchQueryBase::query());


    sc::Department::SPtr all_depts = sc::Department::create("", query(), _("Departments"));

    sc::Department::SPtr ubu_department = sc::Department::create("ubuntu-phone", query(), _("ubuntu-phone"));
    sc::Department::SPtr forthewicked_department = sc::Department::create("forthewicked", query(), _("for-the-wicked"));

    sc::Department::SPtr tags_department = sc::Department::create("tags", query(), _("tags"));

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

        ///FILTER STUFF - Thank you Cinema scope for working filters///////////////////////////////////////////
        sc::Filters filters;
        std::string filterid;
        sc::OptionSelectorFilter::SPtr optionsFilter = sc::OptionSelectorFilter::create("category", _("Filter"));
        optionsFilter->set_display_hints(1);
        optionsFilter->add_option("votes", _("sort-by-votes"));
        optionsFilter->add_option("date", _("sort-by-date"));
        optionsFilter->active_options(query.filter_state());
        filters.push_back(optionsFilter);
        reply->push(filters, query.filter_state());
        if (optionsFilter->has_active_option(query.filter_state())){
           auto o = *(optionsFilter->active_options(query.filter_state()).begin());
           filterid = o->id(); }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////

        string query_string = query.query_string();
        const string department = query.department_id();

        Client::Command commandr;

        //if (settings().at("sort").get_bool() == true) //settings remnant




        ////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////
        /// //////////////////////////////////////
        /// /
if (settings().at("ocd").get_bool() == true && settings().at("nm").get_bool() == false) {


    /////////////////////////


    if (department == "ubuntu-phone") {
    auto new_sys = "tagged/2987/ubuntu-phone";
    if (filterid == "votes") { auto sort_sys = "sort-by-votes";
    if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
    } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
    }} else { auto sort_sys = "";
        if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
        } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }}  auto commandr_cat = reply->register_category("Commands", _("Commands"), "",
                                                                                                                                      sc::CategoryRenderer(PHONE_TEMPLATE));
                                                                                         for (const auto &weather : commandr.commands) {
                                                                                             sc::CategorisedResult res(commandr_cat);
                                                                                             res.set_uri(weather.url);
                                                                                             res["title"] = weather.summary;
                                                                                             res["mascot"] = "http://www.commandlinefu.com/images/favicon.ico";
                                                                                             res["subtitle"] = "⇳" + weather.votes;
                                                                                             res["summary"] = weather.command;
                                                                                             res["term"] = "appid://com.ubuntu.terminal/terminal/current-user-version";
                                                                                             res["clp"] = "scope://com.canonical.scopes.clickstore?q=terminal";
                                                                                             res["art"] = "/usr/share/icons/suru/actions/scalable/info.svg";
                                                                                             res["phone"] = "/usr/share/icons/suru/devices/scalable/phone-smartphone-symbolic.svg";

                                                                                             res["mmm"] = weather.command;
                                                                                             if (!reply->push(res)) {
                                                                                                 return;
                                                                                             }}}

        if (department == "tags") {
        auto new_sys = "matching";
        if (filterid == "votes") { auto sort_sys = "sort-by-votes";
        if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
        } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
        }} else { auto sort_sys = "";
            if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
            } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

        if (department == "awk") {
        auto new_sys = "using/awk";
        if (filterid == "votes") { auto sort_sys = "sort-by-votes";
        if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
        } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
        }} else { auto sort_sys = "";
            if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
            } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

        if (department == "bash") {
        auto new_sys = "tagged/34/bash";
        if (filterid == "votes") { auto sort_sys = "sort-by-votes";
        if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
        } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
        }} else { auto sort_sys = "";
            if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
            } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

        if (department == "curl") {
        auto new_sys = "tagged/235/curl";
        if (filterid == "votes") { auto sort_sys = "sort-by-votes";
        if (query_string.empty()) {
            commandr = client_.command_list("", new_sys, sort_sys);
        } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
        }} else { auto sort_sys = "";
            if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
            } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

        if (department == "find") {
        auto new_sys = "tagged/123/find";
        if (filterid == "votes") { auto sort_sys = "sort-by-votes";
        if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
        } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
        }} else { auto sort_sys = "";
            if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
            } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

        if (department == "grep") {
        auto new_sys = "tagged/163/grep";
        if (filterid == "votes") { auto sort_sys = "sort-by-votes";
        if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
        } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
        }} else { auto sort_sys = "";
            if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
            } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

        if (department == "sed") {
        auto new_sys = "tagged/110/sed";
        if (filterid == "votes") { auto sort_sys = "sort-by-votes";
        if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
        } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
        }} else { auto sort_sys = "";
            if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
            } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

        if (department == "echo") {
        auto new_sys = "using/echo";
        if (filterid == "votes") { auto sort_sys = "sort-by-votes";
        if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
        } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
        }} else { auto sort_sys = "";
            if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
            } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

        if (department == "ls") {
        auto new_sys = "using/ls";
        if (filterid == "votes") { auto sort_sys = "sort-by-votes";
        if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
        } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
        }} else { auto sort_sys = "";
            if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
            } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

        if (department == "cat") {
        auto new_sys = "using/cat";
        if (filterid == "votes") { auto sort_sys = "sort-by-votes";
        if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
        } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
        }} else { auto sort_sys = "";
            if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
            } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

        auto commandr_cat = reply->register_category("Commands", ("Commands"), "",
                                                     sc::CategoryRenderer(OCD_TEMPLATE));
        for (const auto &weather : commandr.commands) {
            sc::CategorisedResult res(commandr_cat);
            res.set_uri(weather.url);
            res["title"] = weather.summary;
            res["mascot"] = "http://www.commandlinefu.com/images/favicon.ico";
            res["subtitle"] = "⇳" + weather.votes;
            res["summary"] = weather.command;
            res["term"] = "appid://com.ubuntu.terminal/terminal/current-user-version";
            res["clp"] = "scope://com.canonical.scopes.clickstore?q=terminal";
            res["phone"] = "/usr/share/icons/suru/devices/scalable/phone-smartphone-symbolic.svg";

            res["mmm"] = weather.command;
            if (!reply->push(res)) {
                return;
            }}


        if (department == "forthewicked") {
        auto new_sys = "forthewicked";
        if (filterid == "votes") { auto sort_sys = "sort-by-votes";
        if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
        } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
        }} else { auto sort_sys = "";
            if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
            } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }}

        auto commandr_cat = reply->register_category("forecast", _("Commands"), "",
                                                     sc::CategoryRenderer(WOCD_TEMPLATE));
        for (const auto &weather : commandr.commands) {
            sc::CategorisedResult res(commandr_cat);
            res.set_uri(weather.url);
            res["title"] = weather.summary;
            res["mascot"] = "http://www.commandlinefu.com/images/favicon.ico";
            res["subtitle"] = "⇳" + weather.votes;
            res["summary"] = weather.command;
            res["term"] = "appid://com.ubuntu.terminal/terminal/current-user-version";
            res["clp"] = "scope://com.canonical.scopes.clickstore?q=terminal";
            res["mmm"] = weather.command;
            res["art"] = "/usr/share/icons/suru/actions/scalable/info.svg";
            res["phone"] = "/usr/share/icons/suru/devices/scalable/phone-smartphone-symbolic.svg";
            res["warning"] = "/usr/share/icons/suru/actions/scalable/security-alert.svg";


            if (!reply->push(res)) {
                return;}} }

        if (department == "") {
        auto new_sys = "matching/";
        if (settings().at("sort").get_bool() == true) { auto sort_sys = "/sort-by-votes";
        if (!query_string.empty()) {commandr = client_.command_list(query_string, new_sys, sort_sys);}
        } else { auto sort_sys = "";
            if (!query_string.empty()) { commandr = client_.command_list(query_string, new_sys, sort_sys); } }

        auto commandr_cat = reply->register_category("matching", (query_string), "",
                                                     sc::CategoryRenderer(OCD_TEMPLATE));
        for (const auto &weather : commandr.commands) {
            sc::CategorisedResult res(commandr_cat);
            res.set_uri(weather.url);
            res["title"] = weather.summary;
            res["mascot"] = "http://www.commandlinefu.com/images/favicon.ico";
            res["subtitle"] = "⇳" + weather.votes;
            res["summary"] = weather.command;
            res["term"] = "appid://com.ubuntu.terminal/terminal/current-user-version";
            res["clp"] = "scope://com.canonical.scopes.clickstore?q=terminal";
            res["mmm"] = weather.command;
            if (!reply->push(res)) {
                return;
            }}}


        if (department == "") {
        auto new_sys = "browse/last-month";
        if (filterid == "votes") { auto sort_sys = "sort-by-votes";
        if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
        } } else { auto sort_sys = "";
            if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
            } }
       if (query_string.empty()){ auto commandr_cat = reply->register_category("last", ("last-month"), "",
                                                     sc::CategoryRenderer(OCD_TEMPLATE));
        for (const auto &weather : commandr.commands) {
            sc::CategorisedResult res(commandr_cat);
            res.set_uri(weather.url);
            res["title"] = weather.summary;
            res["mascot"] = "http://www.commandlinefu.com/images/favicon.ico";
            res["subtitle"] = "⇳" + weather.votes;
            res["summary"] = weather.command;
            res["term"] = "appid://com.ubuntu.terminal/terminal/current-user-version";
            res["clp"] = "scope://com.canonical.scopes.clickstore?q=terminal";
            res["mmm"] = weather.command;
            if (!reply->push(res)) {
                return;
            }}}}

        if (department == "") {
            auto new_sys = "browse";
            auto sort_sys = "sort-by-votes";
            if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
            } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }
       if (query_string.empty()){ auto commandr_cat = reply->register_category("alltime", ("all-time-greats"), "",
                                                     sc::CategoryRenderer(OCD_TEMPLATE));
        for (const auto &weather : commandr.commands) {
            sc::CategorisedResult res(commandr_cat);
            res.set_uri(weather.url);
            res["title"] = weather.summary;
            res["mascot"] = "http://www.commandlinefu.com/images/favicon.ico";
            res["subtitle"] = "⇳" + weather.votes;
            res["summary"] = weather.command;
            res["term"] = "appid://com.ubuntu.terminal/terminal/current-user-version";
            res["clp"] = "scope://com.canonical.scopes.clickstore?q=terminal";
            res["mmm"] = weather.command;
            if (!reply->push(res)) {
                return;
            }}}}


} else if (settings().at("nm").get_bool() == true && settings().at("ocd").get_bool() == false) {


            /////////////////////////



    if (department == "ubuntu-phone") {
    auto new_sys = "tagged/2987/ubuntu-phone";
    if (filterid == "votes") { auto sort_sys = "sort-by-votes";
    if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
    } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
    }} else { auto sort_sys = "";
        if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
        } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }}

    auto commandr_cat = reply->register_category("Commands", _("Commands"), "",
                                                               sc::CategoryRenderer(PHONE_TEMPLATE));
                                                                                             for (const auto &weather : commandr.commands) {
                                                                                                 sc::CategorisedResult res(commandr_cat);
                                                                                                 res.set_uri(weather.url);
                                                                                                 res["title"] = weather.summary;
                                                                                                 res["mascot"] = "http://www.commandlinefu.com/images/favicon.ico";
                                                                                                 res["subtitle"] = "⇳" + weather.votes;
                                                                                                 res["summary"] = weather.command;
                                                                                                 res["term"] = "appid://com.ubuntu.terminal/terminal/current-user-version";
                                                                                                 res["clp"] = "scope://com.canonical.scopes.clickstore?q=terminal";
                                                                                                 res["art"] = "/usr/share/icons/suru/actions/scalable/info.svg";
                                                                                                 res["phone"] = "/usr/share/icons/suru/devices/scalable/phone-smartphone-symbolic.svg";

                                                                                                 res["mmm"] = weather.command;
                                                                                                 if (!reply->push(res)) {
                                                                                                     return;
                                                                                                 }}}


                if (department == "tags") {
                auto new_sys = "matching";
                if (filterid == "votes") { auto sort_sys = "sort-by-votes";
                if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
                }} else { auto sort_sys = "";
                    if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                    } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

                if (department == "awk") {
                auto new_sys = "using/awk";
                if (filterid == "votes") { auto sort_sys = "sort-by-votes";
                if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
                }} else { auto sort_sys = "";
                    if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                    } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

                if (department == "bash") {
                auto new_sys = "tagged/34/bash";
                if (filterid == "votes") { auto sort_sys = "sort-by-votes";
                if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
                }} else { auto sort_sys = "";
                    if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                    } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

                if (department == "curl") {
                auto new_sys = "tagged/235/curl";
                if (filterid == "votes") { auto sort_sys = "sort-by-votes";
                if (query_string.empty()) {
                    commandr = client_.command_list("", new_sys, sort_sys);
                } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
                }} else { auto sort_sys = "";
                    if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                    } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

                if (department == "find") {
                auto new_sys = "tagged/123/find";
                if (filterid == "votes") { auto sort_sys = "sort-by-votes";
                if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
                }} else { auto sort_sys = "";
                    if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                    } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

                if (department == "grep") {
                auto new_sys = "tagged/163/grep";
                if (filterid == "votes") { auto sort_sys = "sort-by-votes";
                if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
                }} else { auto sort_sys = "";
                    if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                    } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

                if (department == "sed") {
                auto new_sys = "tagged/110/sed";
                if (filterid == "votes") { auto sort_sys = "sort-by-votes";
                if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
                }} else { auto sort_sys = "";
                    if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                    } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

                if (department == "echo") {
                auto new_sys = "using/echo";
                if (filterid == "votes") { auto sort_sys = "sort-by-votes";
                if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
                }} else { auto sort_sys = "";
                    if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                    } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

                if (department == "ls") {
                auto new_sys = "using/ls";
                if (filterid == "votes") { auto sort_sys = "sort-by-votes";
                if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
                }} else { auto sort_sys = "";
                    if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                    } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

                if (department == "cat") {
                auto new_sys = "using/cat";
                if (filterid == "votes") { auto sort_sys = "sort-by-votes";
                if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
                }} else { auto sort_sys = "";
                    if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                    } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

                auto commandr_cat = reply->register_category("Commands", _("Commands"), "",
                                                             sc::CategoryRenderer(NM_TEMPLATE));
                for (const auto &weather : commandr.commands) {
                    sc::CategorisedResult res(commandr_cat);
                    res.set_uri(weather.url);
                    res["title"] = weather.summary;
                    res["mascot"] = "http://www.commandlinefu.com/images/favicon.ico";
                    res["subtitle"] = "⇳" + weather.votes;
                    res["summary"] = weather.command;
                    res["term"] = "appid://com.ubuntu.terminal/terminal/current-user-version";
                    res["clp"] = "scope://com.canonical.scopes.clickstore?q=terminal";
                    res["phone"] = "/usr/share/icons/suru/devices/scalable/phone-smartphone-symbolic.svg";

                    res["mmm"] = weather.command;
                    if (!reply->push(res)) {
                        return;
                    }}


                if (department == "forthewicked") {
                auto new_sys = "forthewicked";
                if (filterid == "votes") { auto sort_sys = "sort-by-votes";
                if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
                }} else { auto sort_sys = "";
                    if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                    } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }}

                auto commandr_cat = reply->register_category("forecast", _("Commands"), "",
                                                             sc::CategoryRenderer(WICKED_TEMPLATE));
                for (const auto &weather : commandr.commands) {
                    sc::CategorisedResult res(commandr_cat);
                    res.set_uri(weather.url);
                    res["title"] = weather.summary;
                    res["mascot"] = "http://www.commandlinefu.com/images/favicon.ico";
                    res["subtitle"] = "⇳" + weather.votes;
                    res["summary"] = weather.command;
                    res["term"] = "appid://com.ubuntu.terminal/terminal/current-user-version";
                    res["clp"] = "scope://com.canonical.scopes.clickstore?q=terminal";
                    res["mmm"] = weather.command;
                    res["art"] = "/usr/share/icons/suru/actions/scalable/info.svg";
                    res["warning"] = "/usr/share/icons/suru/actions/scalable/security-alert.svg";

                    if (!reply->push(res)) {
                        return;}} }

                if (department == "") {
                auto new_sys = "matching/";
                if (settings().at("sort").get_bool() == true) { auto sort_sys = "/sort-by-votes";
                if (!query_string.empty()) {commandr = client_.command_list(query_string, new_sys, sort_sys);}
                } else { auto sort_sys = "";
                    if (!query_string.empty()) { commandr = client_.command_list(query_string, new_sys, sort_sys); } }

                auto commandr_cat = reply->register_category("matching", (query_string), "",
                                                             sc::CategoryRenderer(NM_TEMPLATE));
                for (const auto &weather : commandr.commands) {
                    sc::CategorisedResult res(commandr_cat);
                    res.set_uri(weather.url);
                    res["title"] = weather.summary;
                    res["mascot"] = "http://www.commandlinefu.com/images/favicon.ico";
                    res["subtitle"] = "⇳" + weather.votes;
                    res["summary"] = weather.command;
                    res["term"] = "appid://com.ubuntu.terminal/terminal/current-user-version";
                    res["clp"] = "scope://com.canonical.scopes.clickstore?q=terminal";
                    res["mmm"] = weather.command;
                    if (!reply->push(res)) {
                        return;
                    }}}


                if (department == "") {
                auto new_sys = "browse/last-month";
                if (filterid == "votes") { auto sort_sys = "sort-by-votes";
                if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                } } else { auto sort_sys = "";
                    if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                    } }
               if (query_string.empty()){ auto commandr_cat = reply->register_category("last", _("last-month"), "",
                                                             sc::CategoryRenderer(NM_TEMPLATE));
                for (const auto &weather : commandr.commands) {
                    sc::CategorisedResult res(commandr_cat);
                    res.set_uri(weather.url);
                    res["title"] = weather.summary;
                    res["mascot"] = "http://www.commandlinefu.com/images/favicon.ico";
                    res["subtitle"] = "⇳" + weather.votes;
                    res["summary"] = weather.command;
                    res["term"] = "appid://com.ubuntu.terminal/terminal/current-user-version";
                    res["clp"] = "scope://com.canonical.scopes.clickstore?q=terminal";
                    res["mmm"] = weather.command;
                    if (!reply->push(res)) {
                        return;
                    }}}}

                if (department == "") {
                    auto new_sys = "browse";
                    auto sort_sys = "sort-by-votes";
                    if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                    } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }
               if (query_string.empty()){ auto commandr_cat = reply->register_category("alltime", _("all-time-greats"), "",
                                                             sc::CategoryRenderer(NM_TEMPLATE));
                for (const auto &weather : commandr.commands) {
                    sc::CategorisedResult res(commandr_cat);
                    res.set_uri(weather.url);
                    res["title"] = weather.summary;
                    res["mascot"] = "http://www.commandlinefu.com/images/favicon.ico";
                    res["subtitle"] = "⇳" + weather.votes;
                    res["summary"] = weather.command;
                    res["term"] = "appid://com.ubuntu.terminal/terminal/current-user-version";
                    res["clp"] = "scope://com.canonical.scopes.clickstore?q=terminal";
                    res["mmm"] = weather.command;
                    if (!reply->push(res)) {
                        return;
                    }}}}



        } else if (settings().at("ocd").get_bool() == true && settings().at("nm").get_bool() == true) {


                    /////////////////////////

    if (department == "ubuntu-phone") {
    auto new_sys = "tagged/2987/ubuntu-phone";
    if (filterid == "votes") { auto sort_sys = "sort-by-votes";
    if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
    } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
    }} else { auto sort_sys = "";
        if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
        } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }}  auto commandr_cat = reply->register_category("Commands", _("Commands"), "",
                                                                                                                                      sc::CategoryRenderer(PHONE_TEMPLATE));
                                                                                         for (const auto &weather : commandr.commands) {
                                                                                             sc::CategorisedResult res(commandr_cat);
                                                                                             res.set_uri(weather.url);
                                                                                             res["title"] = weather.summary;
                                                                                             res["mascot"] = "http://www.commandlinefu.com/images/favicon.ico";
                                                                                             res["subtitle"] = "⇳" + weather.votes;
                                                                                             res["summary"] = weather.command;
                                                                                             res["term"] = "appid://com.ubuntu.terminal/terminal/current-user-version";
                                                                                             res["clp"] = "scope://com.canonical.scopes.clickstore?q=terminal";
                                                                                             res["art"] = "/usr/share/icons/suru/actions/scalable/info.svg";
                                                                                             res["phone"] = "/usr/share/icons/suru/devices/scalable/phone-smartphone-symbolic.svg";

                                                                                             res["mmm"] = weather.command;
                                                                                             if (!reply->push(res)) {
                                                                                                 return;
                                                                                             }}}


                        if (department == "tags") {
                        auto new_sys = "matching";
                        if (filterid == "votes") { auto sort_sys = "sort-by-votes";
                        if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                        } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
                        }} else { auto sort_sys = "";
                            if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                            } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

                        if (department == "awk") {
                        auto new_sys = "using/awk";
                        if (filterid == "votes") { auto sort_sys = "sort-by-votes";
                        if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                        } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
                        }} else { auto sort_sys = "";
                            if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                            } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

                        if (department == "bash") {
                        auto new_sys = "tagged/34/bash";
                        if (filterid == "votes") { auto sort_sys = "sort-by-votes";
                        if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                        } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
                        }} else { auto sort_sys = "";
                            if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                            } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

                        if (department == "curl") {
                        auto new_sys = "tagged/235/curl";
                        if (filterid == "votes") { auto sort_sys = "sort-by-votes";
                        if (query_string.empty()) {
                            commandr = client_.command_list("", new_sys, sort_sys);
                        } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
                        }} else { auto sort_sys = "";
                            if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                            } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

                        if (department == "find") {
                        auto new_sys = "tagged/123/find";
                        if (filterid == "votes") { auto sort_sys = "sort-by-votes";
                        if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                        } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
                        }} else { auto sort_sys = "";
                            if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                            } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

                        if (department == "grep") {
                        auto new_sys = "tagged/163/grep";
                        if (filterid == "votes") { auto sort_sys = "sort-by-votes";
                        if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                        } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
                        }} else { auto sort_sys = "";
                            if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                            } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

                        if (department == "sed") {
                        auto new_sys = "tagged/110/sed";
                        if (filterid == "votes") { auto sort_sys = "sort-by-votes";
                        if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                        } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
                        }} else { auto sort_sys = "";
                            if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                            } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

                        if (department == "echo") {
                        auto new_sys = "using/echo";
                        if (filterid == "votes") { auto sort_sys = "sort-by-votes";
                        if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                        } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
                        }} else { auto sort_sys = "";
                            if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                            } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

                        if (department == "ls") {
                        auto new_sys = "using/ls";
                        if (filterid == "votes") { auto sort_sys = "sort-by-votes";
                        if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                        } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
                        }} else { auto sort_sys = "";
                            if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                            } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

                        if (department == "cat") {
                        auto new_sys = "using/cat";
                        if (filterid == "votes") { auto sort_sys = "sort-by-votes";
                        if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                        } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
                        }} else { auto sort_sys = "";
                            if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                            } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

                        auto commandr_cat = reply->register_category("Commands", ("Commands"), "",
                                                                     sc::CategoryRenderer(BOTH_TEMPLATE));
                        for (const auto &weather : commandr.commands) {
                            sc::CategorisedResult res(commandr_cat);
                            res.set_uri(weather.url);
                            res["title"] = weather.summary;
                            res["mascot"] = "http://www.commandlinefu.com/images/favicon.ico";
                            res["subtitle"] = "⇳" + weather.votes;
                            res["summary"] = weather.command;
                            res["term"] = "appid://com.ubuntu.terminal/terminal/current-user-version";
                            res["clp"] = "scope://com.canonical.scopes.clickstore?q=terminal";
                            res["phone"] = "/usr/share/icons/suru/devices/scalable/phone-smartphone-symbolic.svg";

                            res["mmm"] = weather.command;
                            if (!reply->push(res)) {
                                return;
                            }}


                        if (department == "forthewicked") {
                        auto new_sys = "forthewicked";
                        if (filterid == "votes") { auto sort_sys = "sort-by-votes";
                        if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                        } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
                        }} else { auto sort_sys = "";
                            if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                            } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }}

                        auto commandr_cat = reply->register_category("forecast", _("Commands"), "",
                                                                     sc::CategoryRenderer(WOCD_TEMPLATE));
                        for (const auto &weather : commandr.commands) {
                            sc::CategorisedResult res(commandr_cat);
                            res.set_uri(weather.url);
                            res["title"] = weather.summary;
                            res["mascot"] = "http://www.commandlinefu.com/images/favicon.ico";
                            res["subtitle"] = "⇳" + weather.votes;
                            res["summary"] = weather.command;
                            res["term"] = "appid://com.ubuntu.terminal/terminal/current-user-version";
                            res["clp"] = "scope://com.canonical.scopes.clickstore?q=terminal";
                            res["mmm"] = weather.command;
                            res["art"] = "/usr/share/icons/suru/actions/scalable/info.svg";
                            res["warning"] = "/usr/share/icons/suru/actions/scalable/security-alert.svg";

                            if (!reply->push(res)) {
                                return;}} }

                        if (department == "") {
                        auto new_sys = "matching/";
                        if (settings().at("sort").get_bool() == true) { auto sort_sys = "/sort-by-votes";
                        if (!query_string.empty()) {commandr = client_.command_list(query_string, new_sys, sort_sys);}
                        } else { auto sort_sys = "";
                            if (!query_string.empty()) { commandr = client_.command_list(query_string, new_sys, sort_sys); } }

                        auto commandr_cat = reply->register_category("matching", (query_string), "",
                                                                     sc::CategoryRenderer(BOTH_TEMPLATE));
                        for (const auto &weather : commandr.commands) {
                            sc::CategorisedResult res(commandr_cat);
                            res.set_uri(weather.url);
                            res["title"] = weather.summary;
                            res["mascot"] = "http://www.commandlinefu.com/images/favicon.ico";
                            res["subtitle"] = "⇳" + weather.votes;
                            res["summary"] = weather.command;
                            res["term"] = "appid://com.ubuntu.terminal/terminal/current-user-version";
                            res["clp"] = "scope://com.canonical.scopes.clickstore?q=terminal";
                            res["mmm"] = weather.command;
                            if (!reply->push(res)) {
                                return;
                            }}}


                        if (department == "") {
                        auto new_sys = "browse/last-month";
                        if (filterid == "votes") { auto sort_sys = "sort-by-votes";
                        if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                        } } else { auto sort_sys = "";
                            if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                            } }
                       if (query_string.empty()){ auto commandr_cat = reply->register_category("last", _("last-month"), "",
                                                                     sc::CategoryRenderer(BOTH_TEMPLATE));
                        for (const auto &weather : commandr.commands) {
                            sc::CategorisedResult res(commandr_cat);
                            res.set_uri(weather.url);
                            res["title"] = weather.summary;
                            res["mascot"] = "http://www.commandlinefu.com/images/favicon.ico";
                            res["subtitle"] = "⇳" + weather.votes;
                            res["summary"] = weather.command;
                            res["term"] = "appid://com.ubuntu.terminal/terminal/current-user-version";
                            res["clp"] = "scope://com.canonical.scopes.clickstore?q=terminal";
                            res["mmm"] = weather.command;
                            if (!reply->push(res)) {
                                return;
                            }}}}

                        if (department == "") {
                            auto new_sys = "browse";
                            auto sort_sys = "sort-by-votes";
                            if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                            } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }
                       if (query_string.empty()){ auto commandr_cat = reply->register_category("alltime", _("all-time-greats"), "",
                                                                     sc::CategoryRenderer(BOTH_TEMPLATE));
                        for (const auto &weather : commandr.commands) {
                            sc::CategorisedResult res(commandr_cat);
                            res.set_uri(weather.url);
                            res["title"] = weather.summary;
                            res["mascot"] = "http://www.commandlinefu.com/images/favicon.ico";
                            res["subtitle"] = "⇳" + weather.votes;
                            res["summary"] = weather.command;
                            res["term"] = "appid://com.ubuntu.terminal/terminal/current-user-version";
                            res["clp"] = "scope://com.canonical.scopes.clickstore?q=terminal";
                            res["mmm"] = weather.command;
                            if (!reply->push(res)) {
                                return;
                            }}} }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////
        /// /////////////////////////////////////////////////////////////
        /// ///////////////////////////////////////////////////////
} else {

    if (department == "ubuntu-phone") {
    auto new_sys = "tagged/2987/ubuntu-phone";
    if (filterid == "votes") { auto sort_sys = "sort-by-votes";
    if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
    } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
    }} else { auto sort_sys = "";
        if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
        } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }}

    auto commandr_cat = reply->register_category("Commands", ("Commands"), "",
                                                 sc::CategoryRenderer(PHONE_TEMPLATE));
    for (const auto &weather : commandr.commands) {
        sc::CategorisedResult res(commandr_cat);
        res.set_uri(weather.url);
        res["title"] = weather.summary;
        res["mascot"] = "http://www.commandlinefu.com/images/favicon.ico";
        res["subtitle"] = "⇳" + weather.votes;
        res["summary"] = weather.command;
        res["term"] = "appid://com.ubuntu.terminal/terminal/current-user-version";
        res["clp"] = "scope://com.canonical.scopes.clickstore?q=terminal";
        res["art"] = "/usr/share/icons/suru/actions/scalable/info.svg";
        res["phone"] = "/usr/share/icons/suru/devices/scalable/phone-smartphone-symbolic.svg";

        res["mmm"] = weather.command;
        if (!reply->push(res)) {
            return;
        }}}


            if (department == "tags") {
            auto new_sys = "matching";
            if (filterid == "votes") { auto sort_sys = "sort-by-votes";
            if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
            } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
            }} else { auto sort_sys = "";
                if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

            if (department == "awk") {
            auto new_sys = "using/awk";
            if (filterid == "votes") { auto sort_sys = "sort-by-votes";
            if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
            } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
            }} else { auto sort_sys = "";
                if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

            if (department == "bash") {
            auto new_sys = "tagged/34/bash";
            if (filterid == "votes") { auto sort_sys = "sort-by-votes";
            if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
            } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
            }} else { auto sort_sys = "";
                if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

            if (department == "curl") {
            auto new_sys = "tagged/235/curl";
            if (filterid == "votes") { auto sort_sys = "sort-by-votes";
            if (query_string.empty()) {
                commandr = client_.command_list("", new_sys, sort_sys);
            } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
            }} else { auto sort_sys = "";
                if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

            if (department == "find") {
            auto new_sys = "tagged/123/find";
            if (filterid == "votes") { auto sort_sys = "sort-by-votes";
            if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
            } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
            }} else { auto sort_sys = "";
                if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

            if (department == "grep") {
            auto new_sys = "tagged/163/grep";
            if (filterid == "votes") { auto sort_sys = "sort-by-votes";
            if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
            } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
            }} else { auto sort_sys = "";
                if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

            if (department == "sed") {
            auto new_sys = "tagged/110/sed";
            if (filterid == "votes") { auto sort_sys = "sort-by-votes";
            if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
            } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
            }} else { auto sort_sys = "";
                if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

            if (department == "echo") {
            auto new_sys = "using/echo";
            if (filterid == "votes") { auto sort_sys = "sort-by-votes";
            if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
            } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
            }} else { auto sort_sys = "";
                if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

            if (department == "ls") {
            auto new_sys = "using/ls";
            if (filterid == "votes") { auto sort_sys = "sort-by-votes";
            if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
            } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
            }} else { auto sort_sys = "";
                if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

            if (department == "cat") {
            auto new_sys = "using/cat";
            if (filterid == "votes") { auto sort_sys = "sort-by-votes";
            if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
            } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
            }} else { auto sort_sys = "";
                if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }} }

            auto commandr_cat = reply->register_category("Commands", _("Commands"), "",
                                                         sc::CategoryRenderer(MAIN_TEMPLATE));
            for (const auto &weather : commandr.commands) {
                sc::CategorisedResult res(commandr_cat);
                res.set_uri(weather.url);
                res["title"] = weather.summary;
                res["mascot"] = "http://www.commandlinefu.com/images/favicon.ico";
                res["subtitle"] = "⇳" + weather.votes;
                res["summary"] = weather.command;
                res["term"] = "appid://com.ubuntu.terminal/terminal/current-user-version";
                res["clp"] = "scope://com.canonical.scopes.clickstore?q=terminal";
                res["art"] = "/usr/share/icons/suru/actions/scalable/info.svg";
                res["mmm"] = weather.command;
                if (!reply->push(res)) {
                    return;
                }}


            if (department == "forthewicked") {
            auto new_sys = "forthewicked";
            if (filterid == "votes") { auto sort_sys = "sort-by-votes";
            if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
            } else { commandr = client_.command_list(query_string, new_sys, sort_sys);
            }} else { auto sort_sys = "";
                if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }}

            auto commandr_cat = reply->register_category("forecast", _("Commands"), "",
                                                         sc::CategoryRenderer(WICKED_TEMPLATE));
            for (const auto &weather : commandr.commands) {
                sc::CategorisedResult res(commandr_cat);
                res.set_uri(weather.url);
                res["title"] = weather.summary;
                res["mascot"] = "http://www.commandlinefu.com/images/favicon.ico";
                res["subtitle"] = "⇳" + weather.votes;
                res["summary"] = weather.command;
                res["term"] = "appid://com.ubuntu.terminal/terminal/current-user-version";
                res["clp"] = "scope://com.canonical.scopes.clickstore?q=terminal";
                res["mmm"] = weather.command;
                res["art"] = "/usr/share/icons/suru/actions/scalable/info.svg";
                res["warning"] = "/usr/share/icons/suru/actions/scalable/security-alert.svg";

                if (!reply->push(res)) {
                    return;}} }

            if (department == "") {
            auto new_sys = "matching/";
            if (settings().at("sort").get_bool() == true) { auto sort_sys = "/sort-by-votes";
            if (!query_string.empty()) {commandr = client_.command_list(query_string, new_sys, sort_sys);}
            } else { auto sort_sys = "";
                if (!query_string.empty()) { commandr = client_.command_list(query_string, new_sys, sort_sys); } }

            auto commandr_cat = reply->register_category("matching", (query_string), "",
                                                         sc::CategoryRenderer(MAIN_TEMPLATE));
            for (const auto &weather : commandr.commands) {
                sc::CategorisedResult res(commandr_cat);
                res.set_uri(weather.url);
                res["title"] = weather.summary;
                res["mascot"] = "http://www.commandlinefu.com/images/favicon.ico";
                res["subtitle"] = "⇳" + weather.votes;
                res["summary"] = weather.command;
                res["term"] = "appid://com.ubuntu.terminal/terminal/current-user-version";
                res["art"] = "/usr/share/icons/suru/actions/scalable/info.svg";
                res["clp"] = "scope://com.canonical.scopes.clickstore?q=terminal";
                res["mmm"] = weather.command;
                if (!reply->push(res)) {
                    return;
                }}}


            if (department == "") {
            auto new_sys = "browse/last-month";
            if (filterid == "votes") { auto sort_sys = "sort-by-votes";
            if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
            } } else { auto sort_sys = "";
                if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                } }
           if (query_string.empty()){ auto commandr_cat = reply->register_category("last", _("last-month"), "",
                                                         sc::CategoryRenderer(MAIN_TEMPLATE));
            for (const auto &weather : commandr.commands) {
                sc::CategorisedResult res(commandr_cat);
                res.set_uri(weather.url);
                res["title"] = weather.summary;
                res["mascot"] = "http://www.commandlinefu.com/images/favicon.ico";
                res["subtitle"] = "⇳" + weather.votes;
                res["summary"] = weather.command;
                res["term"] = "appid://com.ubuntu.terminal/terminal/current-user-version";
                res["clp"] = "scope://com.canonical.scopes.clickstore?q=terminal";
                res["art"] = "/usr/share/icons/suru/actions/scalable/info.svg";
                res["mmm"] = weather.command;
                if (!reply->push(res)) {
                    return;
                }}}}

            if (department == "") {
                auto new_sys = "browse";
                auto sort_sys = "sort-by-votes";
                if (query_string.empty()) { commandr = client_.command_list("", new_sys, sort_sys);
                } else { commandr = client_.command_list(query_string, new_sys, sort_sys); }
           if (query_string.empty()){ auto commandr_cat = reply->register_category("alltime", _("all-time-greats"), "",
                                                         sc::CategoryRenderer(MAIN_TEMPLATE));
            for (const auto &weather : commandr.commands) {
                sc::CategorisedResult res(commandr_cat);
                res.set_uri(weather.url);
                res["title"] = weather.summary;
                res["mascot"] = "http://www.commandlinefu.com/images/favicon.ico";
                res["subtitle"] = "⇳" + weather.votes;
                res["summary"] = weather.command;
                res["term"] = "appid://com.ubuntu.terminal/terminal/current-user-version";
                res["art"] = "/usr/share/icons/suru/actions/scalable/info.svg";
                res["clp"] = "scope://com.canonical.scopes.clickstore?q=terminal";
                res["mmm"] = weather.command;
                if (!reply->push(res)) {
                    return;
                }}}}}

     } catch (domain_error &e) {
    // Handle exceptions being thrown by the client API
    //cerr << e.what() << endl;
    reply->error(current_exception());
    }
}

