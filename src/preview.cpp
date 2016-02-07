#include <preview.h>

#include <unity/scopes/ColumnLayout.h>
#include <unity/scopes/PreviewWidget.h>
#include <unity/scopes/PreviewReply.h>
#include <unity/scopes/Result.h>
#include <unity/scopes/VariantBuilder.h>

#include <iostream>

namespace sc = unity::scopes;

using namespace std;
///
/// \brief Preview::Preview
/// \param result
/// \param metadata
///
Preview::Preview(const sc::Result &result, const sc::ActionMetadata &metadata) :
    sc::PreviewQueryBase(result, metadata) {
}
///
/// \brief Preview::cancelled
///
void Preview::cancelled() {
}
///
/// \brief Preview::run
/// \param reply
///
void Preview::run(sc::PreviewReplyProxy const& reply) {
    sc::ColumnLayout layout1col(1), layout2col(2), layout3col(3);
    //ONE
    layout1col.add_column( { "header_widget", "summary_widget", "rating", "actions", "comment", "warning", "refrence", "drive", "power"} );
    //TWO
    layout2col.add_column( { "header_widget", "summary_widget", "rating", "actions", "icon_actions" } );
    layout2col.add_column( { "image_widget", "comment", "warining", "refrence", "drive", "power" } );
    //THREE
    layout3col.add_column( { "header_widget", "rating", "actions" });
    layout3col.add_column( { "summary_widget", "actions", "icon_actions" } );
    layout3col.add_column( { "image_widget", "comment", "warining", "refrence", "drive", "power" } );
    ///
    reply->register_layout( { layout1col, layout2col, layout3col } );

    //IMAGE
    sc::PreviewWidget image("image_widget", "image");
    image.add_attribute_mapping("source", "art");

    //HEADER
    sc::PreviewWidget header("header_widget", "header");
    header.add_attribute_mapping("title", "title");
    header.add_attribute_mapping("subtitle", "subtitle");

    //SUMMARY
    sc::PreviewWidget summary("summary_widget", "text");
    summary.add_attribute_mapping("text", "summary");
    ///
    sc::Result result = PreviewQueryBase::result();

    //ICON-ACTIONS
    sc::PreviewWidget actions("actions", "icon-actions");
    sc::VariantBuilder builder;
     builder.add_tuple({
        {"id", sc::Variant("open")},
        {"label", sc::Variant("Open Terminal")},
        {"icon", sc::Variant("http://www.commandlinefu.com/images/favicon.ico")},
        {"temporaryIcon", sc::Variant("/usr/share/icons/suru/actions/scalable/tick.svg")},
        {"uri", result["term"]} });
     builder.add_tuple({
        {"id", sc::Variant("open")},
        {"label", sc::Variant("Get Terminal")},
        {"icon", sc::Variant("//usr/share/icons/suru/apps/sources/ubuntu-store.svg")},
        {"temporaryIcon", sc::Variant("/usr/share/icons/suru/actions/scalable/tick.svg")},
        {"uri", result["clp"]} });
     actions.add_attribute_value("actions", builder.end());
     // Define the actions section
/*     sc::PreviewWidget icon_actions("actions", "actions");
     //sc::VariantBuilder builder;
     builder.add_tuple({
         {"id", sc::Variant("open")},
         {"label", sc::Variant("Open")},
         {"icon", sc::Variant("/usr/share/icons/suru/devices/scalable/phone-smartphone-symbolic.svg")},
         {"uri", result["uri"]}
                                  });
      builder.add_tuple({
         {"id", sc::Variant("open")},
         {"label", sc::Variant("Open Terminal")},
         {"uri", result["term"]}
                                  });
      builder.add_tuple({
         {"id", sc::Variant("open")},
         {"label", sc::Variant("Get Terminal")},
         {"uri", result["clp"]}
                                  });
     icon_actions.add_attribute_value("actions", builder.end()); */
     ///
     /// \brief rating
     ///
     sc::PreviewWidget rating("rating", "rating-edit");
     rating.add_attribute_value("visible", sc::Variant("review"));
     rating.add_attribute_value("required", sc::Variant("review"));
     rating.add_attribute_value("review-label", sc::Variant("Adapt and Copy/Paste Command:"));
     rating.add_attribute_value("submit-label", sc::Variant("Open"));
     rating.add_attribute_value("review", result["mmm"]);
     ///
     /// \brief comment
     ///
     sc::PreviewWidget comment("comment", "comment");
     comment.add_attribute_value("submit-label", sc::Variant("Comment it!"));
     comment.add_attribute_value("author", sc::Variant("WARNING:"));
     comment.add_attribute_value("source", sc::Variant("/usr/share/icons/suru/actions/scalable/security-alert.svg"));
     comment.add_attribute_value("comment", sc::Variant("The Terminal is a magical and powerful thing, on the phone it runs unconfined. Any  move can and will spell certain doom, delete your files, launch missiles, destroy your phone, or worse. YOU HAVE BEEN WARNED!"));
     ///
     /// \brief warning
     ///
     sc::PreviewWidget warning("warning", "comment");
     warning.add_attribute_value("submit-label", sc::Variant("Comment it!"));
     warning.add_attribute_value("author", sc::Variant("WARNING:"));
     warning.add_attribute_value("source", sc::Variant("/usr/share/icons/suru/devices/scalable/phone-smartphone-symbolic.svg"));
     warning.add_attribute_value("comment", sc::Variant("If using this scope on phone/tablet be aware that: <br> - The system image is read only for a reason, enabling read/write will compromise the system in a way that will ultimately brick or cause you to reflash the phone. <br> - If you choose to enable read/write be warned that you will not get system image updates <br> - Do not use apt/apt-get or any such utilites, they will break your phone."));
     ///
     /// \brief refrence
     ///
     sc::PreviewWidget refrence("refrence", "comment");
     refrence.add_attribute_value("submit-label", sc::Variant("Comment it!"));
     refrence.add_attribute_value("author", sc::Variant("WARNING:"));
     refrence.add_attribute_value("source", sc::Variant("/usr/share/icons/suru/actions/scalable/edit-paste.svg"));
     refrence.add_attribute_value("comment", sc::Variant("Use for documentation purposes only, copy/pasting and running commands is a terrible idea and is not supported nor recommended"));
     ///
     /// \brief drive
     ///
     sc::PreviewWidget drive("drive", "comment");
     drive.add_attribute_value("submit-label", sc::Variant("Comment it!"));
     drive.add_attribute_value("author", sc::Variant("WARNING:"));
     drive.add_attribute_value("source", sc::Variant("/usr/share/icons/suru/devices/scalable/audio-carkit-symbolic.svg"));
     drive.add_attribute_value("comment", sc::Variant("Do not use while driving."));
     ///
     /// \brief power
     ///
     sc::PreviewWidget power("power", "comment");
     power.add_attribute_value("submit-label", sc::Variant("Comment it!"));
     power.add_attribute_value("author", sc::Variant("Powered By:"));
     power.add_attribute_value("source", sc::Variant("/usr/share/icons/suru/status/scalable/system-devices-panel-information.svg"));
     power.add_attribute_value("comment", sc::Variant("www.commandlinefu.com"));

        //rating.add_attribute_value("rating-icon-empty", unity::scopes::Variant("file:///tmp/star-empty.svg"));
        //rating.add_attribute_value("rating-icon-full", unity::scopes::Variant("file:///tmp/star-full.svg"));

    // Push each of the sections
    reply->push( { image, header, summary, actions, rating, comment, warning, refrence, drive, power} );
}

