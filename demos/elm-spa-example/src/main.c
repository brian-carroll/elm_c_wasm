#include <stdio.h>
#include "../../../src/kernel/kernel.h"
enum {
  CTOR_AddRight,
  CTOR_Append,
  CTOR_Apr,
  CTOR_Article,
  CTOR_Aug,
  CTOR_Avatar,
  CTOR_Bad,
  CTOR_Black,
  CTOR_Body,
  CTOR_ChangedUrl,
  CTOR_ClickedDeleteArticle,
  CTOR_ClickedDeleteComment,
  CTOR_ClickedDismissErrors,
  CTOR_ClickedFavorite,
  CTOR_ClickedFeedPage,
  CTOR_ClickedFollow,
  CTOR_ClickedLink,
  CTOR_ClickedPostComment,
  CTOR_ClickedSave,
  CTOR_ClickedTab,
  CTOR_ClickedTag,
  CTOR_ClickedUnfavorite,
  CTOR_ClickedUnfollow,
  CTOR_Comment,
  CTOR_CommentId,
  CTOR_CompletedArticleLoad,
  CTOR_CompletedAuthorLoad,
  CTOR_CompletedCreate,
  CTOR_CompletedDeleteArticle,
  CTOR_CompletedDeleteComment,
  CTOR_CompletedEdit,
  CTOR_CompletedFavorite,
  CTOR_CompletedFavoriteChange,
  CTOR_CompletedFeedLoad,
  CTOR_CompletedFollowChange,
  CTOR_CompletedFormLoad,
  CTOR_CompletedLoadArticle,
  CTOR_CompletedLoadComments,
  CTOR_CompletedLogin,
  CTOR_CompletedPostComment,
  CTOR_CompletedRegister,
  CTOR_CompletedSave,
  CTOR_CompletedTagsLoad,
  CTOR_Creating,
  CTOR_Cred,
  CTOR_Dec,
  CTOR_EditArticle,
  CTOR_Editing,
  CTOR_EditingNew,
  CTOR_Editor,
  CTOR_Email,
  CTOR_Empty,
  CTOR_EmptyBody,
  CTOR_Endpoint,
  CTOR_EnteredAvatar,
  CTOR_EnteredBio,
  CTOR_EnteredBody,
  CTOR_EnteredCommentText,
  CTOR_EnteredDescription,
  CTOR_EnteredEmail,
  CTOR_EnteredPassword,
  CTOR_EnteredTags,
  CTOR_EnteredTitle,
  CTOR_EnteredUsername,
  CTOR_Err,
  CTOR_ExpectingEnd,
  CTOR_ExpectingSymbol,
  CTOR_Failed,
  CTOR_False,
  CTOR_FavoritedArticles,
  CTOR_Feb,
  CTOR_FollowedAuthor,
  CTOR_Full,
  CTOR_GlobalFeed,
  CTOR_Good,
  CTOR_GotArticleMsg,
  CTOR_GotEditorMsg,
  CTOR_GotFeedMsg,
  CTOR_GotHomeMsg,
  CTOR_GotLoginMsg,
  CTOR_GotProfileMsg,
  CTOR_GotRegisterMsg,
  CTOR_GotSession,
  CTOR_GotSettingsMsg,
  CTOR_GotTimeZone,
  CTOR_Guest,
  CTOR_Header,
  CTOR_Home,
  CTOR_Image,
  CTOR_InvalidEntry,
  CTOR_IsFollowing,
  CTOR_IsNotFollowing,
  CTOR_IsViewer,
  CTOR_Jan,
  CTOR_Jul,
  CTOR_Jun,
  CTOR_Just,
  CTOR_Loaded,
  CTOR_Loading,
  CTOR_LoadingFailed,
  CTOR_LoadingSlowly,
  CTOR_LoggedIn,
  CTOR_Login,
  CTOR_Logout,
  CTOR_Mar,
  CTOR_May,
  CTOR_MayPreventDefault,
  CTOR_MayStopPropagation,
  CTOR_Model,
  CTOR_MyArticles,
  CTOR_NewArticle,
  CTOR_Normal,
  CTOR_NotFound,
  CTOR_Nothing,
  CTOR_Nov,
  CTOR_Oct,
  CTOR_Ok,
  CTOR_Other,
  CTOR_PaginatedList,
  CTOR_Parser,
  CTOR_PassedSlowLoadThreshold,
  CTOR_Password,
  CTOR_Perform,
  CTOR_Posix,
  CTOR_Preview,
  CTOR_Problem,
  CTOR_Profile,
  CTOR_QueryParameter,
  CTOR_RBEmpty_elm_builtin,
  CTOR_RBNode_elm_builtin,
  CTOR_Red,
  CTOR_Redirect,
  CTOR_Register,
  CTOR_Request,
  CTOR_Saving,
  CTOR_Sending,
  CTOR_Sep,
  CTOR_ServerError,
  CTOR_Settings,
  CTOR_Slug,
  CTOR_StringBody,
  CTOR_SubmittedForm,
  CTOR_Tag,
  CTOR_TagFeed,
  CTOR_Title,
  CTOR_Token,
  CTOR_Trimmed,
  CTOR_True,
  CTOR_UnfollowedAuthor,
  CTOR_Username,
  CTOR_Viewer,
  CTOR_YourFeed,
  CTOR_Zone,
};
enum {
  JS_Browser_application,
  JS_Browser_load,
  JS_Browser_pushUrl,
  JS_Browser_replaceUrl,
  JS_Browser_setViewport,
  JS_Http_expectStringResponse,
  JS_Http_toTask,
  JS_Json_addEntry,
  JS_Json_addField,
  JS_Json_andThen,
  JS_Json_decodeBool,
  JS_Json_decodeField,
  JS_Json_decodeInt,
  JS_Json_decodeKeyValuePairs,
  JS_Json_decodeList,
  JS_Json_decodeNull,
  JS_Json_decodeString,
  JS_Json_decodeValue,
  JS_Json_emptyArray,
  JS_Json_emptyObject,
  JS_Json_encode,
  JS_Json_encodeNull,
  JS_Json_fail,
  JS_Json_map1,
  JS_Json_map2,
  JS_Json_oneOf,
  JS_Json_run,
  JS_Json_runOnString,
  JS_Json_succeed,
  JS_Json_wrap,
  JS_List_fromArray,
  JS_List_toArray,
  JS_Markdown_toHtml,
  JS_Parser_isSubChar,
  JS_Parser_isSubString,
  JS_Platform_batch,
  JS_Platform_incomingPort,
  JS_Platform_leaf,
  JS_Platform_map,
  JS_Platform_outgoingPort,
  JS_Process_sleep,
  JS_Scheduler_andThen,
  JS_Scheduler_fail,
  JS_Scheduler_onError,
  JS_Scheduler_succeed,
  JS_String_join,
  JS_String_split,
  JS_Time_here,
  JS_Url_percentDecode,
  JS_Url_percentEncode,
  JS_VirtualDom_attribute,
  JS_VirtualDom_map,
  JS_VirtualDom_noJavaScriptOrHtmlUri,
  JS_VirtualDom_noJavaScriptUri,
  JS_VirtualDom_noOnOrFormAction,
  JS_VirtualDom_node,
  JS_VirtualDom_on,
  JS_VirtualDom_property,
  JS_VirtualDom_style,
  JS_VirtualDom_text,
};
enum {
  FIELD_article,
  FIELD_articles,
  FIELD_author,
  FIELD_avatar,
  FIELD_bio,
  FIELD_body,
  FIELD_breaks,
  FIELD_col,
  FIELD_comments,
  FIELD_content,
  FIELD_context,
  FIELD_contextStack,
  FIELD_createdAt,
  FIELD_day,
  FIELD_defaultHighlighting,
  FIELD_description,
  FIELD_email,
  FIELD_errors,
  FIELD_expect,
  FIELD_favorited,
  FIELD_favoritesCount,
  FIELD_feed,
  FIELD_feedPage,
  FIELD_feedTab,
  FIELD_form,
  FIELD_frag,
  FIELD_githubFlavored,
  FIELD_headers,
  FIELD_id,
  FIELD_indent,
  FIELD_init,
  FIELD_isLoading,
  FIELD_metadata,
  FIELD_method,
  FIELD_month,
  FIELD_offset,
  FIELD_onUrlChange,
  FIELD_onUrlRequest,
  FIELD_page,
  FIELD_params,
  FIELD_password,
  FIELD_problem,
  FIELD_problems,
  FIELD_resultsPerPage,
  FIELD_row,
  FIELD_sanitize,
  FIELD_session,
  FIELD_slug,
  FIELD_smartypants,
  FIELD_src,
  FIELD_status,
  FIELD_subscriptions,
  FIELD_tables,
  FIELD_tags,
  FIELD_timeZone,
  FIELD_timeout,
  FIELD_title,
  FIELD_total,
  FIELD_unvisited,
  FIELD_update,
  FIELD_url,
  FIELD_username,
  FIELD_value,
  FIELD_values,
  FIELD_view,
  FIELD_visited,
  FIELD_withCredentials,
  FIELD_year,
};
Closure VirtualDom_text = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_VirtualDom_text,
};
Closure VirtualDom_style = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_VirtualDom_style,
};
Closure VirtualDom_property = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_VirtualDom_property,
};
Closure VirtualDom_on = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_VirtualDom_on,
};
Closure VirtualDom_node = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_VirtualDom_node,
};
Closure VirtualDom_noOnOrFormAction = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_VirtualDom_noOnOrFormAction,
};
Closure VirtualDom_noJavaScriptUri = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_VirtualDom_noJavaScriptUri,
};
Closure VirtualDom_noJavaScriptOrHtmlUri = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_VirtualDom_noJavaScriptOrHtmlUri,
};
Closure VirtualDom_map = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_VirtualDom_map,
};
Closure VirtualDom_attribute = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_VirtualDom_attribute,
};
Closure Url_percentEncode = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Url_percentEncode,
};
Closure Url_percentDecode = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Url_percentDecode,
};
Closure Time_here = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Time_here,
};
Closure String_split = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_String_split,
};
Closure String_join = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_String_join,
};
Closure Scheduler_succeed = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Scheduler_succeed,
};
Closure Scheduler_onError = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Scheduler_onError,
};
Closure Scheduler_fail = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Scheduler_fail,
};
Closure Scheduler_andThen = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Scheduler_andThen,
};
Closure Process_sleep = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Process_sleep,
};
Closure Platform_outgoingPort = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Platform_outgoingPort,
};
Closure Platform_map = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Platform_map,
};
Closure Platform_leaf = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Platform_leaf,
};
Closure Platform_incomingPort = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Platform_incomingPort,
};
Closure Platform_batch = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Platform_batch,
};
Closure Parser_isSubString = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Parser_isSubString,
};
Closure Parser_isSubChar = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Parser_isSubChar,
};
Closure Markdown_toHtml = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Markdown_toHtml,
};
Closure List_toArray = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_List_toArray,
};
Closure List_fromArray = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_List_fromArray,
};
Closure Json_wrap = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Json_wrap,
};
Closure Json_succeed = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Json_succeed,
};
Closure Json_runOnString = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Json_runOnString,
};
Closure Json_run = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Json_run,
};
Closure Json_oneOf = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Json_oneOf,
};
Closure Json_map2 = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Json_map2,
};
Closure Json_map1 = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Json_map1,
};
Closure Json_fail = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Json_fail,
};
Closure Json_encodeNull = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Json_encodeNull,
};
Closure Json_encode = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Json_encode,
};
Closure Json_emptyObject = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Json_emptyObject,
};
Closure Json_emptyArray = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Json_emptyArray,
};
Closure Json_decodeValue = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Json_decodeValue,
};
Closure Json_decodeString = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Json_decodeString,
};
Closure Json_decodeNull = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Json_decodeNull,
};
Closure Json_decodeList = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Json_decodeList,
};
Closure Json_decodeKeyValuePairs = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Json_decodeKeyValuePairs,
};
Closure Json_decodeInt = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Json_decodeInt,
};
Closure Json_decodeField = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Json_decodeField,
};
Closure Json_decodeBool = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Json_decodeBool,
};
Closure Json_andThen = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Json_andThen,
};
Closure Json_addField = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Json_addField,
};
Closure Json_addEntry = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Json_addEntry,
};
Closure Http_toTask = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Http_toTask,
};
Closure Http_expectStringResponse = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Http_expectStringResponse,
};
Closure Browser_setViewport = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Browser_setViewport,
};
Closure Browser_replaceUrl = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Browser_replaceUrl,
};
Closure Browser_pushUrl = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Browser_pushUrl,
};
Closure Browser_load = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Browser_load,
};
Closure Browser_application = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Browser_application,
};
FieldGroup fg_total_values = {
    .size = 2,
    .fields =
        {
            FIELD_total,
            FIELD_values,
        },
};
FieldGroup fg_session_status = {
    .size = 2,
    .fields =
        {
            FIELD_session,
            FIELD_status,
        },
};
FieldGroup fg_problems_session_status = {
    .size = 3,
    .fields =
        {
            FIELD_problems,
            FIELD_session,
            FIELD_status,
        },
};
FieldGroup fg_page_resultsPerPage = {
    .size = 2,
    .fields =
        {
            FIELD_page,
            FIELD_resultsPerPage,
        },
};
FieldGroup fg_init_onUrlChange_onUrlRequest_subscriptions_update_view = {
    .size = 6,
    .fields =
        {
            FIELD_init,
            FIELD_onUrlChange,
            FIELD_onUrlRequest,
            FIELD_subscriptions,
            FIELD_update,
            FIELD_view,
        },
};
FieldGroup fg_frag_params_unvisited_value_visited = {
    .size = 5,
    .fields =
        {
            FIELD_frag,
            FIELD_params,
            FIELD_unvisited,
            FIELD_value,
            FIELD_visited,
        },
};
FieldGroup fg_form_problems_session = {
    .size = 3,
    .fields =
        {
            FIELD_form,
            FIELD_problems,
            FIELD_session,
        },
};
FieldGroup fg_feed_feedPage_feedTab_session_tags_timeZone = {
    .size = 6,
    .fields =
        {
            FIELD_feed,
            FIELD_feedPage,
            FIELD_feedTab,
            FIELD_session,
            FIELD_tags,
            FIELD_timeZone,
        },
};
FieldGroup fg_email_password_username = {
    .size = 3,
    .fields =
        {
            FIELD_email,
            FIELD_password,
            FIELD_username,
        },
};
FieldGroup fg_email_password = {
    .size = 2,
    .fields =
        {
            FIELD_email,
            FIELD_password,
        },
};
FieldGroup fg_defaultHighlighting_githubFlavored_sanitize_smartypants = {
    .size = 4,
    .fields =
        {
            FIELD_defaultHighlighting,
            FIELD_githubFlavored,
            FIELD_sanitize,
            FIELD_smartypants,
        },
};
FieldGroup fg_day_month_year = {
    .size = 3,
    .fields =
        {
            FIELD_day,
            FIELD_month,
            FIELD_year,
        },
};
FieldGroup fg_createdAt_description_favorited_favoritesCount_tags_title = {
    .size = 6,
    .fields =
        {
            FIELD_createdAt,
            FIELD_description,
            FIELD_favorited,
            FIELD_favoritesCount,
            FIELD_tags,
            FIELD_title,
        },
};
FieldGroup fg_content_title = {
    .size = 2,
    .fields =
        {
            FIELD_content,
            FIELD_title,
        },
};
FieldGroup fg_col_problem_row = {
    .size = 3,
    .fields =
        {
            FIELD_col,
            FIELD_problem,
            FIELD_row,
        },
};
FieldGroup fg_col_contextStack_problem_row = {
    .size = 4,
    .fields =
        {
            FIELD_col,
            FIELD_contextStack,
            FIELD_problem,
            FIELD_row,
        },
};
FieldGroup fg_col_context_indent_offset_row_src = {
    .size = 6,
    .fields =
        {
            FIELD_col,
            FIELD_context,
            FIELD_indent,
            FIELD_offset,
            FIELD_row,
            FIELD_src,
        },
};
FieldGroup fg_breaks_tables = {
    .size = 2,
    .fields =
        {
            FIELD_breaks,
            FIELD_tables,
        },
};
FieldGroup fg_body_title = {
    .size = 2,
    .fields =
        {
            FIELD_body,
            FIELD_title,
        },
};
FieldGroup fg_body_expect_headers_method_timeout_url_withCredentials = {
    .size = 7,
    .fields =
        {
            FIELD_body,
            FIELD_expect,
            FIELD_headers,
            FIELD_method,
            FIELD_timeout,
            FIELD_url,
            FIELD_withCredentials,
        },
};
FieldGroup fg_body_description_tags_title = {
    .size = 4,
    .fields =
        {
            FIELD_body,
            FIELD_description,
            FIELD_tags,
            FIELD_title,
        },
};
FieldGroup fg_avatar_bio_email_password_username = {
    .size = 5,
    .fields =
        {
            FIELD_avatar,
            FIELD_bio,
            FIELD_email,
            FIELD_password,
            FIELD_username,
        },
};
FieldGroup fg_avatar_bio = {
    .size = 2,
    .fields =
        {
            FIELD_avatar,
            FIELD_bio,
        },
};
FieldGroup fg_author_metadata_slug = {
    .size = 3,
    .fields =
        {
            FIELD_author,
            FIELD_metadata,
            FIELD_slug,
        },
};
FieldGroup fg_author_errors_feed_feedPage_feedTab_session_timeZone = {
    .size = 7,
    .fields =
        {
            FIELD_author,
            FIELD_errors,
            FIELD_feed,
            FIELD_feedPage,
            FIELD_feedTab,
            FIELD_session,
            FIELD_timeZone,
        },
};
FieldGroup fg_author_body_createdAt_id = {
    .size = 4,
    .fields =
        {
            FIELD_author,
            FIELD_body,
            FIELD_createdAt,
            FIELD_id,
        },
};
FieldGroup fg_articles_errors_isLoading_session = {
    .size = 4,
    .fields =
        {
            FIELD_articles,
            FIELD_errors,
            FIELD_isLoading,
            FIELD_session,
        },
};
FieldGroup fg_article_comments_errors_session_timeZone = {
    .size = 5,
    .fields =
        {
            FIELD_article,
            FIELD_comments,
            FIELD_errors,
            FIELD_session,
            FIELD_timeZone,
        },
};
ElmString16 literal_string_width = {
    .header = HEADER_STRING(5),
    .words16 =
        {
            0x77,
            0x69,
            0x64,
            0x74,
            0x68,
        },
};
ElmString16 literal_string_value = {
    .header = HEADER_STRING(5),
    .words16 =
        {
            0x76,
            0x61,
            0x6c,
            0x75,
            0x65,
        },
};
ElmString16 literal_string_users = {
    .header = HEADER_STRING(5),
    .words16 =
        {
            0x75,
            0x73,
            0x65,
            0x72,
            0x73,
        },
};
ElmString16 literal_string_username_20can_5c_27t_20be_20blank_2e = {
    .header = HEADER_STRING(25),
    .words16 =
        {
            0x75,
            0x73,
            0x65,
            0x72,
            0x6e,
            0x61,
            0x6d,
            0x65,
            0x20,
            0x63,
            0x61,
            0x6e,
            0x5c,
            0x27,
            0x74,
            0x20,
            0x62,
            0x65,
            0x20,
            0x62,
            0x6c,
            0x61,
            0x6e,
            0x6b,
            0x2e,
        },
};
ElmString16 literal_string_username = {
    .header = HEADER_STRING(8),
    .words16 =
        {
            0x75,
            0x73,
            0x65,
            0x72,
            0x6e,
            0x61,
            0x6d,
            0x65,
        },
};
ElmString16 literal_string_user_2dpic = {
    .header = HEADER_STRING(8),
    .words16 =
        {
            0x75,
            0x73,
            0x65,
            0x72,
            0x2d,
            0x70,
            0x69,
            0x63,
        },
};
ElmString16 literal_string_user_2dinfo = {
    .header = HEADER_STRING(9),
    .words16 =
        {
            0x75,
            0x73,
            0x65,
            0x72,
            0x2d,
            0x69,
            0x6e,
            0x66,
            0x6f,
        },
};
ElmString16 literal_string_user_2dimg = {
    .header = HEADER_STRING(8),
    .words16 =
        {
            0x75,
            0x73,
            0x65,
            0x72,
            0x2d,
            0x69,
            0x6d,
            0x67,
        },
};
ElmString16 literal_string_user = {
    .header = HEADER_STRING(4),
    .words16 =
        {
            0x75,
            0x73,
            0x65,
            0x72,
        },
};
ElmString16 literal_string_ul = {
    .header = HEADER_STRING(2),
    .words16 =
        {
            0x75,
            0x6c,
        },
};
ElmString16 literal_string_type = {
    .header = HEADER_STRING(4),
    .words16 =
        {
            0x74,
            0x79,
            0x70,
            0x65,
        },
};
ElmString16 literal_string_top = {
    .header = HEADER_STRING(3),
    .words16 =
        {
            0x74,
            0x6f,
            0x70,
        },
};
ElmString16 literal_string_token = {
    .header = HEADER_STRING(5),
    .words16 =
        {
            0x74,
            0x6f,
            0x6b,
            0x65,
            0x6e,
        },
};
ElmString16 literal_string_title_20can_5c_27t_20be_20blank_2e = {
    .header = HEADER_STRING(22),
    .words16 =
        {
            0x74,
            0x69,
            0x74,
            0x6c,
            0x65,
            0x20,
            0x63,
            0x61,
            0x6e,
            0x5c,
            0x27,
            0x74,
            0x20,
            0x62,
            0x65,
            0x20,
            0x62,
            0x6c,
            0x61,
            0x6e,
            0x6b,
            0x2e,
        },
};
ElmString16 literal_string_title = {
    .header = HEADER_STRING(5),
    .words16 =
        {
            0x74,
            0x69,
            0x74,
            0x6c,
            0x65,
        },
};
ElmString16 literal_string_textarea = {
    .header = HEADER_STRING(8),
    .words16 =
        {
            0x74,
            0x65,
            0x78,
            0x74,
            0x61,
            0x72,
            0x65,
            0x61,
        },
};
ElmString16 literal_string_text_2dxs_2dcenter = {
    .header = HEADER_STRING(14),
    .words16 =
        {
            0x74,
            0x65,
            0x78,
            0x74,
            0x2d,
            0x78,
            0x73,
            0x2d,
            0x63,
            0x65,
            0x6e,
            0x74,
            0x65,
            0x72,
        },
};
ElmString16 literal_string_target = {
    .header = HEADER_STRING(6),
    .words16 =
        {
            0x74,
            0x61,
            0x72,
            0x67,
            0x65,
            0x74,
        },
};
ElmString16 literal_string_tags = {
    .header = HEADER_STRING(4),
    .words16 =
        {
            0x74,
            0x61,
            0x67,
            0x73,
        },
};
ElmString16 literal_string_tagList = {
    .header = HEADER_STRING(7),
    .words16 =
        {
            0x74,
            0x61,
            0x67,
            0x4c,
            0x69,
            0x73,
            0x74,
        },
};
ElmString16 literal_string_tag_2dpill_20tag_2ddefault = {
    .header = HEADER_STRING(20),
    .words16 =
        {
            0x74,
            0x61,
            0x67,
            0x2d,
            0x70,
            0x69,
            0x6c,
            0x6c,
            0x20,
            0x74,
            0x61,
            0x67,
            0x2d,
            0x64,
            0x65,
            0x66,
            0x61,
            0x75,
            0x6c,
            0x74,
        },
};
ElmString16 literal_string_tag_2dlist = {
    .header = HEADER_STRING(8),
    .words16 =
        {
            0x74,
            0x61,
            0x67,
            0x2d,
            0x6c,
            0x69,
            0x73,
            0x74,
        },
};
ElmString16 literal_string_tag_2ddefault_20tag_2dpill_20tag_2doutline = {
    .header = HEADER_STRING(32),
    .words16 =
        {
            0x74,
            0x61,
            0x67,
            0x2d,
            0x64,
            0x65,
            0x66,
            0x61,
            0x75,
            0x6c,
            0x74,
            0x20,
            0x74,
            0x61,
            0x67,
            0x2d,
            0x70,
            0x69,
            0x6c,
            0x6c,
            0x20,
            0x74,
            0x61,
            0x67,
            0x2d,
            0x6f,
            0x75,
            0x74,
            0x6c,
            0x69,
            0x6e,
            0x65,
        },
};
ElmString16 literal_string_tag = {
    .header = HEADER_STRING(3),
    .words16 =
        {
            0x74,
            0x61,
            0x67,
        },
};
ElmString16 literal_string_tabIndex = {
    .header = HEADER_STRING(8),
    .words16 =
        {
            0x74,
            0x61,
            0x62,
            0x49,
            0x6e,
            0x64,
            0x65,
            0x78,
        },
};
ElmString16 literal_string_submit = {
    .header = HEADER_STRING(6),
    .words16 =
        {
            0x73,
            0x75,
            0x62,
            0x6d,
            0x69,
            0x74,
        },
};
ElmString16 literal_string_storeCache = {
    .header = HEADER_STRING(10),
    .words16 =
        {
            0x73,
            0x74,
            0x6f,
            0x72,
            0x65,
            0x43,
            0x61,
            0x63,
            0x68,
            0x65,
        },
};
ElmString16 literal_string_src = {
    .header = HEADER_STRING(3),
    .words16 =
        {
            0x73,
            0x72,
            0x63,
        },
};
ElmString16 literal_string_span = {
    .header = HEADER_STRING(4),
    .words16 =
        {
            0x73,
            0x70,
            0x61,
            0x6e,
        },
};
ElmString16 literal_string_smiley_2dcyrus_2ejpg = {
    .header = HEADER_STRING(16),
    .words16 =
        {
            0x73,
            0x6d,
            0x69,
            0x6c,
            0x65,
            0x79,
            0x2d,
            0x63,
            0x79,
            0x72,
            0x75,
            0x73,
            0x2e,
            0x6a,
            0x70,
            0x67,
        },
};
ElmString16 literal_string_slug = {
    .header = HEADER_STRING(4),
    .words16 =
        {
            0x73,
            0x6c,
            0x75,
            0x67,
        },
};
ElmString16 literal_string_sign_20up = {
    .header = HEADER_STRING(7),
    .words16 =
        {
            0x73,
            0x69,
            0x67,
            0x6e,
            0x20,
            0x75,
            0x70,
        },
};
ElmString16 literal_string_sidebar = {
    .header = HEADER_STRING(7),
    .words16 =
        {
            0x73,
            0x69,
            0x64,
            0x65,
            0x62,
            0x61,
            0x72,
        },
};
ElmString16 literal_string_settings_2dpage = {
    .header = HEADER_STRING(13),
    .words16 =
        {
            0x73,
            0x65,
            0x74,
            0x74,
            0x69,
            0x6e,
            0x67,
            0x73,
            0x2d,
            0x70,
            0x61,
            0x67,
            0x65,
        },
};
ElmString16 literal_string_settings = {
    .header = HEADER_STRING(8),
    .words16 =
        {
            0x73,
            0x65,
            0x74,
            0x74,
            0x69,
            0x6e,
            0x67,
            0x73,
        },
};
ElmString16 literal_string_rows = {
    .header = HEADER_STRING(4),
    .words16 =
        {
            0x72,
            0x6f,
            0x77,
            0x73,
        },
};
ElmString16 literal_string_row_20article_2dcontent = {
    .header = HEADER_STRING(19),
    .words16 =
        {
            0x72,
            0x6f,
            0x77,
            0x20,
            0x61,
            0x72,
            0x74,
            0x69,
            0x63,
            0x6c,
            0x65,
            0x2d,
            0x63,
            0x6f,
            0x6e,
            0x74,
            0x65,
            0x6e,
            0x74,
        },
};
ElmString16 literal_string_row = {
    .header = HEADER_STRING(3),
    .words16 =
        {
            0x72,
            0x6f,
            0x77,
        },
};
ElmString16 literal_string_rgb_28250_2c_20250_2c_20250_29 = {
    .header = HEADER_STRING(18),
    .words16 =
        {
            0x72,
            0x67,
            0x62,
            0x28,
            0x32,
            0x35,
            0x30,
            0x2c,
            0x20,
            0x32,
            0x35,
            0x30,
            0x2c,
            0x20,
            0x32,
            0x35,
            0x30,
            0x29,
        },
};
ElmString16 literal_string_register = {
    .header = HEADER_STRING(8),
    .words16 =
        {
            0x72,
            0x65,
            0x67,
            0x69,
            0x73,
            0x74,
            0x65,
            0x72,
        },
};
ElmString16 literal_string_pull_2dxs_2dright = {
    .header = HEADER_STRING(13),
    .words16 =
        {
            0x70,
            0x75,
            0x6c,
            0x6c,
            0x2d,
            0x78,
            0x73,
            0x2d,
            0x72,
            0x69,
            0x67,
            0x68,
            0x74,
        },
};
ElmString16 literal_string_profiles = {
    .header = HEADER_STRING(8),
    .words16 =
        {
            0x70,
            0x72,
            0x6f,
            0x66,
            0x69,
            0x6c,
            0x65,
            0x73,
        },
};
ElmString16 literal_string_profile_2dpage = {
    .header = HEADER_STRING(12),
    .words16 =
        {
            0x70,
            0x72,
            0x6f,
            0x66,
            0x69,
            0x6c,
            0x65,
            0x2d,
            0x70,
            0x61,
            0x67,
            0x65,
        },
};
ElmString16 literal_string_profile = {
    .header = HEADER_STRING(7),
    .words16 =
        {
            0x70,
            0x72,
            0x6f,
            0x66,
            0x69,
            0x6c,
            0x65,
        },
};
ElmString16 literal_string_preview_2dlink = {
    .header = HEADER_STRING(12),
    .words16 =
        {
            0x70,
            0x72,
            0x65,
            0x76,
            0x69,
            0x65,
            0x77,
            0x2d,
            0x6c,
            0x69,
            0x6e,
            0x6b,
        },
};
ElmString16 literal_string_position = {
    .header = HEADER_STRING(8),
    .words16 =
        {
            0x70,
            0x6f,
            0x73,
            0x69,
            0x74,
            0x69,
            0x6f,
            0x6e,
        },
};
ElmString16 literal_string_placeholder = {
    .header = HEADER_STRING(11),
    .words16 =
        {
            0x70,
            0x6c,
            0x61,
            0x63,
            0x65,
            0x68,
            0x6f,
            0x6c,
            0x64,
            0x65,
            0x72,
        },
};
ElmString16 literal_string_password_20must_20be_20at_20least_20 = {
    .header = HEADER_STRING(26),
    .words16 =
        {
            0x70,
            0x61,
            0x73,
            0x73,
            0x77,
            0x6f,
            0x72,
            0x64,
            0x20,
            0x6d,
            0x75,
            0x73,
            0x74,
            0x20,
            0x62,
            0x65,
            0x20,
            0x61,
            0x74,
            0x20,
            0x6c,
            0x65,
            0x61,
            0x73,
            0x74,
            0x20,
        },
};
ElmString16 literal_string_password_20can_5c_27t_20be_20blank_2e = {
    .header = HEADER_STRING(25),
    .words16 =
        {
            0x70,
            0x61,
            0x73,
            0x73,
            0x77,
            0x6f,
            0x72,
            0x64,
            0x20,
            0x63,
            0x61,
            0x6e,
            0x5c,
            0x27,
            0x74,
            0x20,
            0x62,
            0x65,
            0x20,
            0x62,
            0x6c,
            0x61,
            0x6e,
            0x6b,
            0x2e,
        },
};
ElmString16 literal_string_password = {
    .header = HEADER_STRING(8),
    .words16 =
        {
            0x70,
            0x61,
            0x73,
            0x73,
            0x77,
            0x6f,
            0x72,
            0x64,
        },
};
ElmString16 literal_string_pagination = {
    .header = HEADER_STRING(10),
    .words16 =
        {
            0x70,
            0x61,
            0x67,
            0x69,
            0x6e,
            0x61,
            0x74,
            0x69,
            0x6f,
            0x6e,
        },
};
ElmString16 literal_string_page_2dlink = {
    .header = HEADER_STRING(9),
    .words16 =
        {
            0x70,
            0x61,
            0x67,
            0x65,
            0x2d,
            0x6c,
            0x69,
            0x6e,
            0x6b,
        },
};
ElmString16 literal_string_page_2ditem = {
    .header = HEADER_STRING(9),
    .words16 =
        {
            0x70,
            0x61,
            0x67,
            0x65,
            0x2d,
            0x69,
            0x74,
            0x65,
            0x6d,
        },
};
ElmString16 literal_string_padding = {
    .header = HEADER_STRING(7),
    .words16 =
        {
            0x70,
            0x61,
            0x64,
            0x64,
            0x69,
            0x6e,
            0x67,
        },
};
ElmString16 literal_string_p = {
    .header = HEADER_STRING(1),
    .words16 =
        {
            0x70,
        },
};
ElmString16 literal_string_onStoreChange = {
    .header = HEADER_STRING(13),
    .words16 =
        {
            0x6f,
            0x6e,
            0x53,
            0x74,
            0x6f,
            0x72,
            0x65,
            0x43,
            0x68,
            0x61,
            0x6e,
            0x67,
            0x65,
        },
};
ElmString16 literal_string_offset = {
    .header = HEADER_STRING(6),
    .words16 =
        {
            0x6f,
            0x66,
            0x66,
            0x73,
            0x65,
            0x74,
        },
};
ElmString16 literal_string_navbar_2dbrand = {
    .header = HEADER_STRING(12),
    .words16 =
        {
            0x6e,
            0x61,
            0x76,
            0x62,
            0x61,
            0x72,
            0x2d,
            0x62,
            0x72,
            0x61,
            0x6e,
            0x64,
        },
};
ElmString16 literal_string_navbar_20navbar_2dlight = {
    .header = HEADER_STRING(19),
    .words16 =
        {
            0x6e,
            0x61,
            0x76,
            0x62,
            0x61,
            0x72,
            0x20,
            0x6e,
            0x61,
            0x76,
            0x62,
            0x61,
            0x72,
            0x2d,
            0x6c,
            0x69,
            0x67,
            0x68,
            0x74,
        },
};
ElmString16 literal_string_nav_2dlink = {
    .header = HEADER_STRING(8),
    .words16 =
        {
            0x6e,
            0x61,
            0x76,
            0x2d,
            0x6c,
            0x69,
            0x6e,
            0x6b,
        },
};
ElmString16 literal_string_nav_2ditem = {
    .header = HEADER_STRING(8),
    .words16 =
        {
            0x6e,
            0x61,
            0x76,
            0x2d,
            0x69,
            0x74,
            0x65,
            0x6d,
        },
};
ElmString16 literal_string_nav_20navbar_2dnav_20pull_2dxs_2dright = {
    .header = HEADER_STRING(28),
    .words16 =
        {
            0x6e,
            0x61,
            0x76,
            0x20,
            0x6e,
            0x61,
            0x76,
            0x62,
            0x61,
            0x72,
            0x2d,
            0x6e,
            0x61,
            0x76,
            0x20,
            0x70,
            0x75,
            0x6c,
            0x6c,
            0x2d,
            0x78,
            0x73,
            0x2d,
            0x72,
            0x69,
            0x67,
            0x68,
            0x74,
        },
};
ElmString16 literal_string_nav_20nav_2dpills_20outline_2dactive = {
    .header = HEADER_STRING(28),
    .words16 =
        {
            0x6e,
            0x61,
            0x76,
            0x20,
            0x6e,
            0x61,
            0x76,
            0x2d,
            0x70,
            0x69,
            0x6c,
            0x6c,
            0x73,
            0x20,
            0x6f,
            0x75,
            0x74,
            0x6c,
            0x69,
            0x6e,
            0x65,
            0x2d,
            0x61,
            0x63,
            0x74,
            0x69,
            0x76,
            0x65,
        },
};
ElmString16 literal_string_nav = {
    .header = HEADER_STRING(3),
    .words16 =
        {
            0x6e,
            0x61,
            0x76,
        },
};
ElmString16 literal_string_mod_2doptions = {
    .header = HEADER_STRING(11),
    .words16 =
        {
            0x6d,
            0x6f,
            0x64,
            0x2d,
            0x6f,
            0x70,
            0x74,
            0x69,
            0x6f,
            0x6e,
            0x73,
        },
};
ElmString16 literal_string_main = {
    .header = HEADER_STRING(4),
    .words16 =
        {
            0x6d,
            0x61,
            0x69,
            0x6e,
        },
};
ElmString16 literal_string_logout = {
    .header = HEADER_STRING(6),
    .words16 =
        {
            0x6c,
            0x6f,
            0x67,
            0x6f,
            0x75,
            0x74,
        },
};
ElmString16 literal_string_logo_2dfont = {
    .header = HEADER_STRING(9),
    .words16 =
        {
            0x6c,
            0x6f,
            0x67,
            0x6f,
            0x2d,
            0x66,
            0x6f,
            0x6e,
            0x74,
        },
};
ElmString16 literal_string_login = {
    .header = HEADER_STRING(5),
    .words16 =
        {
            0x6c,
            0x6f,
            0x67,
            0x69,
            0x6e,
        },
};
ElmString16 literal_string_loading_2esvg = {
    .header = HEADER_STRING(11),
    .words16 =
        {
            0x6c,
            0x6f,
            0x61,
            0x64,
            0x69,
            0x6e,
            0x67,
            0x2e,
            0x73,
            0x76,
            0x67,
        },
};
ElmString16 literal_string_limit = {
    .header = HEADER_STRING(5),
    .words16 =
        {
            0x6c,
            0x69,
            0x6d,
            0x69,
            0x74,
        },
};
ElmString16 literal_string_li = {
    .header = HEADER_STRING(2),
    .words16 =
        {
            0x6c,
            0x69,
        },
};
ElmString16 literal_string_ion_2dtrash_2da = {
    .header = HEADER_STRING(11),
    .words16 =
        {
            0x69,
            0x6f,
            0x6e,
            0x2d,
            0x74,
            0x72,
            0x61,
            0x73,
            0x68,
            0x2d,
            0x61,
        },
};
ElmString16 literal_string_ion_2dplus_2dround = {
    .header = HEADER_STRING(14),
    .words16 =
        {
            0x69,
            0x6f,
            0x6e,
            0x2d,
            0x70,
            0x6c,
            0x75,
            0x73,
            0x2d,
            0x72,
            0x6f,
            0x75,
            0x6e,
            0x64,
        },
};
ElmString16 literal_string_ion_2dheart = {
    .header = HEADER_STRING(9),
    .words16 =
        {
            0x69,
            0x6f,
            0x6e,
            0x2d,
            0x68,
            0x65,
            0x61,
            0x72,
            0x74,
        },
};
ElmString16 literal_string_ion_2dgear_2da = {
    .header = HEADER_STRING(10),
    .words16 =
        {
            0x69,
            0x6f,
            0x6e,
            0x2d,
            0x67,
            0x65,
            0x61,
            0x72,
            0x2d,
            0x61,
        },
};
ElmString16 literal_string_ion_2dedit = {
    .header = HEADER_STRING(8),
    .words16 =
        {
            0x69,
            0x6f,
            0x6e,
            0x2d,
            0x65,
            0x64,
            0x69,
            0x74,
        },
};
ElmString16 literal_string_ion_2dcompose = {
    .header = HEADER_STRING(11),
    .words16 =
        {
            0x69,
            0x6f,
            0x6e,
            0x2d,
            0x63,
            0x6f,
            0x6d,
            0x70,
            0x6f,
            0x73,
            0x65,
        },
};
ElmString16 literal_string_input = {
    .header = HEADER_STRING(5),
    .words16 =
        {
            0x69,
            0x6e,
            0x70,
            0x75,
            0x74,
        },
};
ElmString16 literal_string_info = {
    .header = HEADER_STRING(4),
    .words16 =
        {
            0x69,
            0x6e,
            0x66,
            0x6f,
        },
};
ElmString16 literal_string_img = {
    .header = HEADER_STRING(3),
    .words16 =
        {
            0x69,
            0x6d,
            0x67,
        },
};
ElmString16 literal_string_image = {
    .header = HEADER_STRING(5),
    .words16 =
        {
            0x69,
            0x6d,
            0x61,
            0x67,
            0x65,
        },
};
ElmString16 literal_string_id = {
    .header = HEADER_STRING(2),
    .words16 =
        {
            0x69,
            0x64,
        },
};
ElmString16 literal_string_i = {
    .header = HEADER_STRING(1),
    .words16 =
        {
            0x69,
        },
};
ElmString16 literal_string_https_3a_2f_2fthinkster_2eio = {
    .header = HEADER_STRING(20),
    .words16 =
        {
            0x68,
            0x74,
            0x74,
            0x70,
            0x73,
            0x3a,
            0x2f,
            0x2f,
            0x74,
            0x68,
            0x69,
            0x6e,
            0x6b,
            0x73,
            0x74,
            0x65,
            0x72,
            0x2e,
            0x69,
            0x6f,
        },
};
ElmString16 literal_string_https_3a_2f_2fconduit_2eproductionready_2eio = {
    .header = HEADER_STRING(34),
    .words16 =
        {
            0x68,
            0x74,
            0x74,
            0x70,
            0x73,
            0x3a,
            0x2f,
            0x2f,
            0x63,
            0x6f,
            0x6e,
            0x64,
            0x75,
            0x69,
            0x74,
            0x2e,
            0x70,
            0x72,
            0x6f,
            0x64,
            0x75,
            0x63,
            0x74,
            0x69,
            0x6f,
            0x6e,
            0x72,
            0x65,
            0x61,
            0x64,
            0x79,
            0x2e,
            0x69,
            0x6f,
        },
};
ElmString16 literal_string_https_3a_2f_2f = {
    .header = HEADER_STRING(8),
    .words16 =
        {
            0x68,
            0x74,
            0x74,
            0x70,
            0x73,
            0x3a,
            0x2f,
            0x2f,
        },
};
ElmString16 literal_string_http_3a_2f_2f = {
    .header = HEADER_STRING(7),
    .words16 =
        {
            0x68,
            0x74,
            0x74,
            0x70,
            0x3a,
            0x2f,
            0x2f,
        },
};
ElmString16 literal_string_href = {
    .header = HEADER_STRING(4),
    .words16 =
        {
            0x68,
            0x72,
            0x65,
            0x66,
        },
};
ElmString16 literal_string_hr = {
    .header = HEADER_STRING(2),
    .words16 =
        {
            0x68,
            0x72,
        },
};
ElmString16 literal_string_home_2dpage = {
    .header = HEADER_STRING(9),
    .words16 =
        {
            0x68,
            0x6f,
            0x6d,
            0x65,
            0x2d,
            0x70,
            0x61,
            0x67,
            0x65,
        },
};
ElmString16 literal_string_height = {
    .header = HEADER_STRING(6),
    .words16 =
        {
            0x68,
            0x65,
            0x69,
            0x67,
            0x68,
            0x74,
        },
};
ElmString16 literal_string_h4 = {
    .header = HEADER_STRING(2),
    .words16 =
        {
            0x68,
            0x34,
        },
};
ElmString16 literal_string_h1 = {
    .header = HEADER_STRING(2),
    .words16 =
        {
            0x68,
            0x31,
        },
};
ElmString16 literal_string_form_2dgroup = {
    .header = HEADER_STRING(10),
    .words16 =
        {
            0x66,
            0x6f,
            0x72,
            0x6d,
            0x2d,
            0x67,
            0x72,
            0x6f,
            0x75,
            0x70,
        },
};
ElmString16 literal_string_form_2dcontrol_20form_2dcontrol_2dlg = {
    .header = HEADER_STRING(28),
    .words16 =
        {
            0x66,
            0x6f,
            0x72,
            0x6d,
            0x2d,
            0x63,
            0x6f,
            0x6e,
            0x74,
            0x72,
            0x6f,
            0x6c,
            0x20,
            0x66,
            0x6f,
            0x72,
            0x6d,
            0x2d,
            0x63,
            0x6f,
            0x6e,
            0x74,
            0x72,
            0x6f,
            0x6c,
            0x2d,
            0x6c,
            0x67,
        },
};
ElmString16 literal_string_form_2dcontrol = {
    .header = HEADER_STRING(12),
    .words16 =
        {
            0x66,
            0x6f,
            0x72,
            0x6d,
            0x2d,
            0x63,
            0x6f,
            0x6e,
            0x74,
            0x72,
            0x6f,
            0x6c,
        },
};
ElmString16 literal_string_form = {
    .header = HEADER_STRING(4),
    .words16 =
        {
            0x66,
            0x6f,
            0x72,
            0x6d,
        },
};
ElmString16 literal_string_footer = {
    .header = HEADER_STRING(6),
    .words16 =
        {
            0x66,
            0x6f,
            0x6f,
            0x74,
            0x65,
            0x72,
        },
};
ElmString16 literal_string_following = {
    .header = HEADER_STRING(9),
    .words16 =
        {
            0x66,
            0x6f,
            0x6c,
            0x6c,
            0x6f,
            0x77,
            0x69,
            0x6e,
            0x67,
        },
};
ElmString16 literal_string_follow = {
    .header = HEADER_STRING(6),
    .words16 =
        {
            0x66,
            0x6f,
            0x6c,
            0x6c,
            0x6f,
            0x77,
        },
};
ElmString16 literal_string_fixed = {
    .header = HEADER_STRING(5),
    .words16 =
        {
            0x66,
            0x69,
            0x78,
            0x65,
            0x64,
        },
};
ElmString16 literal_string_fieldset = {
    .header = HEADER_STRING(8),
    .words16 =
        {
            0x66,
            0x69,
            0x65,
            0x6c,
            0x64,
            0x73,
            0x65,
            0x74,
        },
};
ElmString16 literal_string_feed_2dtoggle = {
    .header = HEADER_STRING(11),
    .words16 =
        {
            0x66,
            0x65,
            0x65,
            0x64,
            0x2d,
            0x74,
            0x6f,
            0x67,
            0x67,
            0x6c,
            0x65,
        },
};
ElmString16 literal_string_feed = {
    .header = HEADER_STRING(4),
    .words16 =
        {
            0x66,
            0x65,
            0x65,
            0x64,
        },
};
ElmString16 literal_string_favoritesCount = {
    .header = HEADER_STRING(14),
    .words16 =
        {
            0x66,
            0x61,
            0x76,
            0x6f,
            0x72,
            0x69,
            0x74,
            0x65,
            0x73,
            0x43,
            0x6f,
            0x75,
            0x6e,
            0x74,
        },
};
ElmString16 literal_string_favorited = {
    .header = HEADER_STRING(9),
    .words16 =
        {
            0x66,
            0x61,
            0x76,
            0x6f,
            0x72,
            0x69,
            0x74,
            0x65,
            0x64,
        },
};
ElmString16 literal_string_favorite = {
    .header = HEADER_STRING(8),
    .words16 =
        {
            0x66,
            0x61,
            0x76,
            0x6f,
            0x72,
            0x69,
            0x74,
            0x65,
        },
};
ElmString16 literal_string_errors = {
    .header = HEADER_STRING(6),
    .words16 =
        {
            0x65,
            0x72,
            0x72,
            0x6f,
            0x72,
            0x73,
        },
};
ElmString16 literal_string_error_2ejpg = {
    .header = HEADER_STRING(9),
    .words16 =
        {
            0x65,
            0x72,
            0x72,
            0x6f,
            0x72,
            0x2e,
            0x6a,
            0x70,
            0x67,
        },
};
ElmString16 literal_string_error_2dmessages = {
    .header = HEADER_STRING(14),
    .words16 =
        {
            0x65,
            0x72,
            0x72,
            0x6f,
            0x72,
            0x2d,
            0x6d,
            0x65,
            0x73,
            0x73,
            0x61,
            0x67,
            0x65,
            0x73,
        },
};
ElmString16 literal_string_email_20can_5c_27t_20be_20blank_2e = {
    .header = HEADER_STRING(22),
    .words16 =
        {
            0x65,
            0x6d,
            0x61,
            0x69,
            0x6c,
            0x20,
            0x63,
            0x61,
            0x6e,
            0x5c,
            0x27,
            0x74,
            0x20,
            0x62,
            0x65,
            0x20,
            0x62,
            0x6c,
            0x61,
            0x6e,
            0x6b,
            0x2e,
        },
};
ElmString16 literal_string_email = {
    .header = HEADER_STRING(5),
    .words16 =
        {
            0x65,
            0x6d,
            0x61,
            0x69,
            0x6c,
        },
};
ElmString16 literal_string_editor_2dpage = {
    .header = HEADER_STRING(11),
    .words16 =
        {
            0x65,
            0x64,
            0x69,
            0x74,
            0x6f,
            0x72,
            0x2d,
            0x70,
            0x61,
            0x67,
            0x65,
        },
};
ElmString16 literal_string_editor = {
    .header = HEADER_STRING(6),
    .words16 =
        {
            0x65,
            0x64,
            0x69,
            0x74,
            0x6f,
            0x72,
        },
};
ElmString16 literal_string_div = {
    .header = HEADER_STRING(3),
    .words16 =
        {
            0x64,
            0x69,
            0x76,
        },
};
ElmString16 literal_string_disabled = {
    .header = HEADER_STRING(8),
    .words16 =
        {
            0x64,
            0x69,
            0x73,
            0x61,
            0x62,
            0x6c,
            0x65,
            0x64,
        },
};
ElmString16 literal_string_description = {
    .header = HEADER_STRING(11),
    .words16 =
        {
            0x64,
            0x65,
            0x73,
            0x63,
            0x72,
            0x69,
            0x70,
            0x74,
            0x69,
            0x6f,
            0x6e,
        },
};
ElmString16 literal_string_date_2dposted = {
    .header = HEADER_STRING(11),
    .words16 =
        {
            0x64,
            0x61,
            0x74,
            0x65,
            0x2d,
            0x70,
            0x6f,
            0x73,
            0x74,
            0x65,
            0x64,
        },
};
ElmString16 literal_string_date = {
    .header = HEADER_STRING(4),
    .words16 =
        {
            0x64,
            0x61,
            0x74,
            0x65,
        },
};
ElmString16 literal_string_cred_2dpage = {
    .header = HEADER_STRING(9),
    .words16 =
        {
            0x63,
            0x72,
            0x65,
            0x64,
            0x2d,
            0x70,
            0x61,
            0x67,
            0x65,
        },
};
ElmString16 literal_string_createdAt = {
    .header = HEADER_STRING(9),
    .words16 =
        {
            0x63,
            0x72,
            0x65,
            0x61,
            0x74,
            0x65,
            0x64,
            0x41,
            0x74,
        },
};
ElmString16 literal_string_content = {
    .header = HEADER_STRING(7),
    .words16 =
        {
            0x63,
            0x6f,
            0x6e,
            0x74,
            0x65,
            0x6e,
            0x74,
        },
};
ElmString16 literal_string_container_20page = {
    .header = HEADER_STRING(14),
    .words16 =
        {
            0x63,
            0x6f,
            0x6e,
            0x74,
            0x61,
            0x69,
            0x6e,
            0x65,
            0x72,
            0x20,
            0x70,
            0x61,
            0x67,
            0x65,
        },
};
ElmString16 literal_string_container = {
    .header = HEADER_STRING(9),
    .words16 =
        {
            0x63,
            0x6f,
            0x6e,
            0x74,
            0x61,
            0x69,
            0x6e,
            0x65,
            0x72,
        },
};
ElmString16 literal_string_conduit = {
    .header = HEADER_STRING(7),
    .words16 =
        {
            0x63,
            0x6f,
            0x6e,
            0x64,
            0x75,
            0x69,
            0x74,
        },
};
ElmString16 literal_string_comments = {
    .header = HEADER_STRING(8),
    .words16 =
        {
            0x63,
            0x6f,
            0x6d,
            0x6d,
            0x65,
            0x6e,
            0x74,
            0x73,
        },
};
ElmString16 literal_string_comment_2dauthor_2dimg = {
    .header = HEADER_STRING(18),
    .words16 =
        {
            0x63,
            0x6f,
            0x6d,
            0x6d,
            0x65,
            0x6e,
            0x74,
            0x2d,
            0x61,
            0x75,
            0x74,
            0x68,
            0x6f,
            0x72,
            0x2d,
            0x69,
            0x6d,
            0x67,
        },
};
ElmString16 literal_string_comment_2dauthor = {
    .header = HEADER_STRING(14),
    .words16 =
        {
            0x63,
            0x6f,
            0x6d,
            0x6d,
            0x65,
            0x6e,
            0x74,
            0x2d,
            0x61,
            0x75,
            0x74,
            0x68,
            0x6f,
            0x72,
        },
};
ElmString16 literal_string_comment = {
    .header = HEADER_STRING(7),
    .words16 =
        {
            0x63,
            0x6f,
            0x6d,
            0x6d,
            0x65,
            0x6e,
            0x74,
        },
};
ElmString16 literal_string_col_2dxs_2d12_20col_2dmd_2d8_20offset_2dmd_2d2 = {
    .header = HEADER_STRING(30),
    .words16 =
        {
            0x63,
            0x6f,
            0x6c,
            0x2d,
            0x78,
            0x73,
            0x2d,
            0x31,
            0x32,
            0x20,
            0x63,
            0x6f,
            0x6c,
            0x2d,
            0x6d,
            0x64,
            0x2d,
            0x38,
            0x20,
            0x6f,
            0x66,
            0x66,
            0x73,
            0x65,
            0x74,
            0x2d,
            0x6d,
            0x64,
            0x2d,
            0x32,
        },
};
ElmString16 literal_string_col_2dxs_2d12_20col_2dmd_2d10_20offset_2dmd_2d1 = {
    .header = HEADER_STRING(31),
    .words16 =
        {
            0x63,
            0x6f,
            0x6c,
            0x2d,
            0x78,
            0x73,
            0x2d,
            0x31,
            0x32,
            0x20,
            0x63,
            0x6f,
            0x6c,
            0x2d,
            0x6d,
            0x64,
            0x2d,
            0x31,
            0x30,
            0x20,
            0x6f,
            0x66,
            0x66,
            0x73,
            0x65,
            0x74,
            0x2d,
            0x6d,
            0x64,
            0x2d,
            0x31,
        },
};
ElmString16 literal_string_col_2dmd_2d9 = {
    .header = HEADER_STRING(8),
    .words16 =
        {
            0x63,
            0x6f,
            0x6c,
            0x2d,
            0x6d,
            0x64,
            0x2d,
            0x39,
        },
};
ElmString16 literal_string_col_2dmd_2d6_20offset_2dmd_2d3_20col_2dxs_2d12 = {
    .header = HEADER_STRING(30),
    .words16 =
        {
            0x63,
            0x6f,
            0x6c,
            0x2d,
            0x6d,
            0x64,
            0x2d,
            0x36,
            0x20,
            0x6f,
            0x66,
            0x66,
            0x73,
            0x65,
            0x74,
            0x2d,
            0x6d,
            0x64,
            0x2d,
            0x33,
            0x20,
            0x63,
            0x6f,
            0x6c,
            0x2d,
            0x78,
            0x73,
            0x2d,
            0x31,
            0x32,
        },
};
ElmString16 literal_string_col_2dmd_2d3 = {
    .header = HEADER_STRING(8),
    .words16 =
        {
            0x63,
            0x6f,
            0x6c,
            0x2d,
            0x6d,
            0x64,
            0x2d,
            0x33,
        },
};
ElmString16 literal_string_col_2dmd_2d12 = {
    .header = HEADER_STRING(9),
    .words16 =
        {
            0x63,
            0x6f,
            0x6c,
            0x2d,
            0x6d,
            0x64,
            0x2d,
            0x31,
            0x32,
        },
};
ElmString16 literal_string_col_2dmd_2d10_20offset_2dmd_2d1_20col_2dxs_2d12 = {
    .header = HEADER_STRING(31),
    .words16 =
        {
            0x63,
            0x6f,
            0x6c,
            0x2d,
            0x6d,
            0x64,
            0x2d,
            0x31,
            0x30,
            0x20,
            0x6f,
            0x66,
            0x66,
            0x73,
            0x65,
            0x74,
            0x2d,
            0x6d,
            0x64,
            0x2d,
            0x31,
            0x20,
            0x63,
            0x6f,
            0x6c,
            0x2d,
            0x78,
            0x73,
            0x2d,
            0x31,
            0x32,
        },
};
ElmString16 literal_string_click = {
    .header = HEADER_STRING(5),
    .words16 =
        {
            0x63,
            0x6c,
            0x69,
            0x63,
            0x6b,
        },
};
ElmString16 literal_string_className = {
    .header = HEADER_STRING(9),
    .words16 =
        {
            0x63,
            0x6c,
            0x61,
            0x73,
            0x73,
            0x4e,
            0x61,
            0x6d,
            0x65,
        },
};
ElmString16 literal_string_card_2dtext = {
    .header = HEADER_STRING(9),
    .words16 =
        {
            0x63,
            0x61,
            0x72,
            0x64,
            0x2d,
            0x74,
            0x65,
            0x78,
            0x74,
        },
};
ElmString16 literal_string_card_2dfooter = {
    .header = HEADER_STRING(11),
    .words16 =
        {
            0x63,
            0x61,
            0x72,
            0x64,
            0x2d,
            0x66,
            0x6f,
            0x6f,
            0x74,
            0x65,
            0x72,
        },
};
ElmString16 literal_string_card_2dblock = {
    .header = HEADER_STRING(10),
    .words16 =
        {
            0x63,
            0x61,
            0x72,
            0x64,
            0x2d,
            0x62,
            0x6c,
            0x6f,
            0x63,
            0x6b,
        },
};
ElmString16 literal_string_card_20comment_2dform = {
    .header = HEADER_STRING(17),
    .words16 =
        {
            0x63,
            0x61,
            0x72,
            0x64,
            0x20,
            0x63,
            0x6f,
            0x6d,
            0x6d,
            0x65,
            0x6e,
            0x74,
            0x2d,
            0x66,
            0x6f,
            0x72,
            0x6d,
        },
};
ElmString16 literal_string_card = {
    .header = HEADER_STRING(4),
    .words16 =
        {
            0x63,
            0x61,
            0x72,
            0x64,
        },
};
ElmString16 literal_string_button = {
    .header = HEADER_STRING(6),
    .words16 =
        {
            0x62,
            0x75,
            0x74,
            0x74,
            0x6f,
            0x6e,
        },
};
ElmString16 literal_string_btn_2dsecondary = {
    .header = HEADER_STRING(13),
    .words16 =
        {
            0x62,
            0x74,
            0x6e,
            0x2d,
            0x73,
            0x65,
            0x63,
            0x6f,
            0x6e,
            0x64,
            0x61,
            0x72,
            0x79,
        },
};
ElmString16 literal_string_btn_2doutline_2dsecondary = {
    .header = HEADER_STRING(21),
    .words16 =
        {
            0x62,
            0x74,
            0x6e,
            0x2d,
            0x6f,
            0x75,
            0x74,
            0x6c,
            0x69,
            0x6e,
            0x65,
            0x2d,
            0x73,
            0x65,
            0x63,
            0x6f,
            0x6e,
            0x64,
            0x61,
            0x72,
            0x79,
        },
};
ElmString16 literal_string_btn_20btn_2dsm_20btn_2dprimary = {
    .header = HEADER_STRING(22),
    .words16 =
        {
            0x62,
            0x74,
            0x6e,
            0x20,
            0x62,
            0x74,
            0x6e,
            0x2d,
            0x73,
            0x6d,
            0x20,
            0x62,
            0x74,
            0x6e,
            0x2d,
            0x70,
            0x72,
            0x69,
            0x6d,
            0x61,
            0x72,
            0x79,
        },
};
ElmString16 literal_string_btn_20btn_2dsm_20btn_2doutline_2dprimary = {
    .header = HEADER_STRING(30),
    .words16 =
        {
            0x62,
            0x74,
            0x6e,
            0x20,
            0x62,
            0x74,
            0x6e,
            0x2d,
            0x73,
            0x6d,
            0x20,
            0x62,
            0x74,
            0x6e,
            0x2d,
            0x6f,
            0x75,
            0x74,
            0x6c,
            0x69,
            0x6e,
            0x65,
            0x2d,
            0x70,
            0x72,
            0x69,
            0x6d,
            0x61,
            0x72,
            0x79,
        },
};
ElmString16 literal_string_btn_20btn_2dsm_20 = {
    .header = HEADER_STRING(11),
    .words16 =
        {
            0x62,
            0x74,
            0x6e,
            0x20,
            0x62,
            0x74,
            0x6e,
            0x2d,
            0x73,
            0x6d,
            0x20,
        },
};
ElmString16 literal_string_btn_20btn_2doutline_2dsecondary_20btn_2dsm = {
    .header = HEADER_STRING(32),
    .words16 =
        {
            0x62,
            0x74,
            0x6e,
            0x20,
            0x62,
            0x74,
            0x6e,
            0x2d,
            0x6f,
            0x75,
            0x74,
            0x6c,
            0x69,
            0x6e,
            0x65,
            0x2d,
            0x73,
            0x65,
            0x63,
            0x6f,
            0x6e,
            0x64,
            0x61,
            0x72,
            0x79,
            0x20,
            0x62,
            0x74,
            0x6e,
            0x2d,
            0x73,
            0x6d,
        },
};
ElmString16 literal_string_btn_20btn_2doutline_2ddanger_20btn_2dsm = {
    .header = HEADER_STRING(29),
    .words16 =
        {
            0x62,
            0x74,
            0x6e,
            0x20,
            0x62,
            0x74,
            0x6e,
            0x2d,
            0x6f,
            0x75,
            0x74,
            0x6c,
            0x69,
            0x6e,
            0x65,
            0x2d,
            0x64,
            0x61,
            0x6e,
            0x67,
            0x65,
            0x72,
            0x20,
            0x62,
            0x74,
            0x6e,
            0x2d,
            0x73,
            0x6d,
        },
};
ElmString16 literal_string_btn_20btn_2dlg_20pull_2dxs_2dright_20btn_2dprimary = {
    .header = HEADER_STRING(36),
    .words16 =
        {
            0x62,
            0x74,
            0x6e,
            0x20,
            0x62,
            0x74,
            0x6e,
            0x2d,
            0x6c,
            0x67,
            0x20,
            0x70,
            0x75,
            0x6c,
            0x6c,
            0x2d,
            0x78,
            0x73,
            0x2d,
            0x72,
            0x69,
            0x67,
            0x68,
            0x74,
            0x20,
            0x62,
            0x74,
            0x6e,
            0x2d,
            0x70,
            0x72,
            0x69,
            0x6d,
            0x61,
            0x72,
            0x79,
        },
};
ElmString16 literal_string_btn_20btn_2dlg_20btn_2dprimary_20pull_2dxs_2dright = {
    .header = HEADER_STRING(36),
    .words16 =
        {
            0x62,
            0x74,
            0x6e,
            0x20,
            0x62,
            0x74,
            0x6e,
            0x2d,
            0x6c,
            0x67,
            0x20,
            0x62,
            0x74,
            0x6e,
            0x2d,
            0x70,
            0x72,
            0x69,
            0x6d,
            0x61,
            0x72,
            0x79,
            0x20,
            0x70,
            0x75,
            0x6c,
            0x6c,
            0x2d,
            0x78,
            0x73,
            0x2d,
            0x72,
            0x69,
            0x67,
            0x68,
            0x74,
        },
};
ElmString16 literal_string_border = {
    .header = HEADER_STRING(6),
    .words16 =
        {
            0x62,
            0x6f,
            0x72,
            0x64,
            0x65,
            0x72,
        },
};
ElmString16 literal_string_body_20can_5c_27t_20be_20blank_2e = {
    .header = HEADER_STRING(21),
    .words16 =
        {
            0x62,
            0x6f,
            0x64,
            0x79,
            0x20,
            0x63,
            0x61,
            0x6e,
            0x5c,
            0x27,
            0x74,
            0x20,
            0x62,
            0x65,
            0x20,
            0x62,
            0x6c,
            0x61,
            0x6e,
            0x6b,
            0x2e,
        },
};
ElmString16 literal_string_body = {
    .header = HEADER_STRING(4),
    .words16 =
        {
            0x62,
            0x6f,
            0x64,
            0x79,
        },
};
ElmString16 literal_string_bio = {
    .header = HEADER_STRING(3),
    .words16 =
        {
            0x62,
            0x69,
            0x6f,
        },
};
ElmString16 literal_string_banner = {
    .header = HEADER_STRING(6),
    .words16 =
        {
            0x62,
            0x61,
            0x6e,
            0x6e,
            0x65,
            0x72,
        },
};
ElmString16 literal_string_background = {
    .header = HEADER_STRING(10),
    .words16 =
        {
            0x62,
            0x61,
            0x63,
            0x6b,
            0x67,
            0x72,
            0x6f,
            0x75,
            0x6e,
            0x64,
        },
};
ElmString16 literal_string_authorization = {
    .header = HEADER_STRING(13),
    .words16 =
        {
            0x61,
            0x75,
            0x74,
            0x68,
            0x6f,
            0x72,
            0x69,
            0x7a,
            0x61,
            0x74,
            0x69,
            0x6f,
            0x6e,
        },
};
ElmString16 literal_string_author = {
    .header = HEADER_STRING(6),
    .words16 =
        {
            0x61,
            0x75,
            0x74,
            0x68,
            0x6f,
            0x72,
        },
};
ElmString16 literal_string_attribution = {
    .header = HEADER_STRING(11),
    .words16 =
        {
            0x61,
            0x74,
            0x74,
            0x72,
            0x69,
            0x62,
            0x75,
            0x74,
            0x69,
            0x6f,
            0x6e,
        },
};
ElmString16 literal_string_articlesCount = {
    .header = HEADER_STRING(13),
    .words16 =
        {
            0x61,
            0x72,
            0x74,
            0x69,
            0x63,
            0x6c,
            0x65,
            0x73,
            0x43,
            0x6f,
            0x75,
            0x6e,
            0x74,
        },
};
ElmString16 literal_string_articles_2dtoggle = {
    .header = HEADER_STRING(15),
    .words16 =
        {
            0x61,
            0x72,
            0x74,
            0x69,
            0x63,
            0x6c,
            0x65,
            0x73,
            0x2d,
            0x74,
            0x6f,
            0x67,
            0x67,
            0x6c,
            0x65,
        },
};
ElmString16 literal_string_articles = {
    .header = HEADER_STRING(8),
    .words16 =
        {
            0x61,
            0x72,
            0x74,
            0x69,
            0x63,
            0x6c,
            0x65,
            0x73,
        },
};
ElmString16 literal_string_article_2dpreview = {
    .header = HEADER_STRING(15),
    .words16 =
        {
            0x61,
            0x72,
            0x74,
            0x69,
            0x63,
            0x6c,
            0x65,
            0x2d,
            0x70,
            0x72,
            0x65,
            0x76,
            0x69,
            0x65,
            0x77,
        },
};
ElmString16 literal_string_article_2dpage = {
    .header = HEADER_STRING(12),
    .words16 =
        {
            0x61,
            0x72,
            0x74,
            0x69,
            0x63,
            0x6c,
            0x65,
            0x2d,
            0x70,
            0x61,
            0x67,
            0x65,
        },
};
ElmString16 literal_string_article_2dmeta = {
    .header = HEADER_STRING(12),
    .words16 =
        {
            0x61,
            0x72,
            0x74,
            0x69,
            0x63,
            0x6c,
            0x65,
            0x2d,
            0x6d,
            0x65,
            0x74,
            0x61,
        },
};
ElmString16 literal_string_article_2dactions = {
    .header = HEADER_STRING(15),
    .words16 =
        {
            0x61,
            0x72,
            0x74,
            0x69,
            0x63,
            0x6c,
            0x65,
            0x2d,
            0x61,
            0x63,
            0x74,
            0x69,
            0x6f,
            0x6e,
            0x73,
        },
};
ElmString16 literal_string_article = {
    .header = HEADER_STRING(7),
    .words16 =
        {
            0x61,
            0x72,
            0x74,
            0x69,
            0x63,
            0x6c,
            0x65,
        },
};
ElmString16 literal_string_application_2fjson = {
    .header = HEADER_STRING(16),
    .words16 =
        {
            0x61,
            0x70,
            0x70,
            0x6c,
            0x69,
            0x63,
            0x61,
            0x74,
            0x69,
            0x6f,
            0x6e,
            0x2f,
            0x6a,
            0x73,
            0x6f,
            0x6e,
        },
};
ElmString16 literal_string_api = {
    .header = HEADER_STRING(3),
    .words16 =
        {
            0x61,
            0x70,
            0x69,
        },
};
ElmString16 literal_string_alt = {
    .header = HEADER_STRING(3),
    .words16 =
        {
            0x61,
            0x6c,
            0x74,
        },
};
ElmString16 literal_string_active = {
    .header = HEADER_STRING(6),
    .words16 =
        {
            0x61,
            0x63,
            0x74,
            0x69,
            0x76,
            0x65,
        },
};
ElmString16 literal_string_a = {
    .header = HEADER_STRING(1),
    .words16 =
        {
            0x61,
        },
};
ElmString16 literal_string__5d = {
    .header = HEADER_STRING(1),
    .words16 =
        {
            0x5d,
        },
};
ElmString16 literal_string__5cu00A0Settings = {
    .header = HEADER_STRING(14),
    .words16 =
        {
            0x5c,
            0x75,
            0x30,
            0x30,
            0x41,
            0x30,
            0x53,
            0x65,
            0x74,
            0x74,
            0x69,
            0x6e,
            0x67,
            0x73,
        },
};
ElmString16 literal_string__5cu00A0New_20Post = {
    .header = HEADER_STRING(14),
    .words16 =
        {
            0x5c,
            0x75,
            0x30,
            0x30,
            0x41,
            0x30,
            0x4e,
            0x65,
            0x77,
            0x20,
            0x50,
            0x6f,
            0x73,
            0x74,
        },
};
ElmString16 literal_string__5cu00A0 = {
    .header = HEADER_STRING(6),
    .words16 =
        {
            0x5c,
            0x75,
            0x30,
            0x30,
            0x41,
            0x30,
        },
};
ElmString16 literal_string__5cn_5cn_28 = {
    .header = HEADER_STRING(5),
    .words16 =
        {
            0x5c,
            0x6e,
            0x5c,
            0x6e,
            0x28,
        },
};
ElmString16 literal_string__5cn_5cn = {
    .header = HEADER_STRING(4),
    .words16 =
        {
            0x5c,
            0x6e,
            0x5c,
            0x6e,
        },
};
ElmString16 literal_string__5cn_20_20_20_20 = {
    .header = HEADER_STRING(6),
    .words16 =
        {
            0x5c,
            0x6e,
            0x20,
            0x20,
            0x20,
            0x20,
        },
};
ElmString16 literal_string__5cn = {
    .header = HEADER_STRING(2),
    .words16 =
        {
            0x5c,
            0x6e,
        },
};
ElmString16 literal_string__5c_27_5d = {
    .header = HEADER_STRING(3),
    .words16 =
        {
            0x5c,
            0x27,
            0x5d,
        },
};
ElmString16 literal_string__5c_22 = {
    .header = HEADER_STRING(2),
    .words16 =
        {
            0x5c,
            0x22,
        },
};
ElmString16 literal_string__5b_5c_27 = {
    .header = HEADER_STRING(3),
    .words16 =
        {
            0x5b,
            0x5c,
            0x27,
        },
};
ElmString16 literal_string__5b = {
    .header = HEADER_STRING(1),
    .words16 =
        {
            0x5b,
        },
};
ElmString16 literal_string_Z = {
    .header = HEADER_STRING(1),
    .words16 =
        {
            0x5a,
        },
};
ElmString16 literal_string_Your_20Settings = {
    .header = HEADER_STRING(13),
    .words16 =
        {
            0x59,
            0x6f,
            0x75,
            0x72,
            0x20,
            0x53,
            0x65,
            0x74,
            0x74,
            0x69,
            0x6e,
            0x67,
            0x73,
        },
};
ElmString16 literal_string_Your_20Feed = {
    .header = HEADER_STRING(9),
    .words16 =
        {
            0x59,
            0x6f,
            0x75,
            0x72,
            0x20,
            0x46,
            0x65,
            0x65,
            0x64,
        },
};
ElmString16 literal_string_Write_20your_20article_20_28in_20markdown_29 = {
    .header = HEADER_STRING(32),
    .words16 =
        {
            0x57,
            0x72,
            0x69,
            0x74,
            0x65,
            0x20,
            0x79,
            0x6f,
            0x75,
            0x72,
            0x20,
            0x61,
            0x72,
            0x74,
            0x69,
            0x63,
            0x6c,
            0x65,
            0x20,
            0x28,
            0x69,
            0x6e,
            0x20,
            0x6d,
            0x61,
            0x72,
            0x6b,
            0x64,
            0x6f,
            0x77,
            0x6e,
            0x29,
        },
};
ElmString16 literal_string_Write_20a_20comment_2e_2e_2e = {
    .header = HEADER_STRING(18),
    .words16 =
        {
            0x57,
            0x72,
            0x69,
            0x74,
            0x65,
            0x20,
            0x61,
            0x20,
            0x63,
            0x6f,
            0x6d,
            0x6d,
            0x65,
            0x6e,
            0x74,
            0x2e,
            0x2e,
            0x2e,
        },
};
ElmString16 literal_string_What_5c_27s_20this_20article_20about_3f = {
    .header = HEADER_STRING(27),
    .words16 =
        {
            0x57,
            0x68,
            0x61,
            0x74,
            0x5c,
            0x27,
            0x73,
            0x20,
            0x74,
            0x68,
            0x69,
            0x73,
            0x20,
            0x61,
            0x72,
            0x74,
            0x69,
            0x63,
            0x6c,
            0x65,
            0x20,
            0x61,
            0x62,
            0x6f,
            0x75,
            0x74,
            0x3f,
        },
};
ElmString16 literal_string_Username = {
    .header = HEADER_STRING(8),
    .words16 =
        {
            0x55,
            0x73,
            0x65,
            0x72,
            0x6e,
            0x61,
            0x6d,
            0x65,
        },
};
ElmString16 literal_string_Update_20Settings = {
    .header = HEADER_STRING(15),
    .words16 =
        {
            0x55,
            0x70,
            0x64,
            0x61,
            0x74,
            0x65,
            0x20,
            0x53,
            0x65,
            0x74,
            0x74,
            0x69,
            0x6e,
            0x67,
            0x73,
        },
};
ElmString16 literal_string_Update_20Article = {
    .header = HEADER_STRING(14),
    .words16 =
        {
            0x55,
            0x70,
            0x64,
            0x61,
            0x74,
            0x65,
            0x20,
            0x41,
            0x72,
            0x74,
            0x69,
            0x63,
            0x6c,
            0x65,
        },
};
ElmString16 literal_string_Unfollow = {
    .header = HEADER_STRING(8),
    .words16 =
        {
            0x55,
            0x6e,
            0x66,
            0x6f,
            0x6c,
            0x6c,
            0x6f,
            0x77,
        },
};
ElmString16 literal_string_USERNAME = {
    .header = HEADER_STRING(8),
    .words16 =
        {
            0x55,
            0x53,
            0x45,
            0x52,
            0x4e,
            0x41,
            0x4d,
            0x45,
        },
};
ElmString16 literal_string_URL_20of_20profile_20picture = {
    .header = HEADER_STRING(22),
    .words16 =
        {
            0x55,
            0x52,
            0x4c,
            0x20,
            0x6f,
            0x66,
            0x20,
            0x70,
            0x72,
            0x6f,
            0x66,
            0x69,
            0x6c,
            0x65,
            0x20,
            0x70,
            0x69,
            0x63,
            0x74,
            0x75,
            0x72,
            0x65,
        },
};
ElmString16 literal_string_Token_20 = {
    .header = HEADER_STRING(6),
    .words16 =
        {
            0x54,
            0x6f,
            0x6b,
            0x65,
            0x6e,
            0x20,
        },
};
ElmString16 literal_string_Thinkster = {
    .header = HEADER_STRING(9),
    .words16 =
        {
            0x54,
            0x68,
            0x69,
            0x6e,
            0x6b,
            0x73,
            0x74,
            0x65,
            0x72,
        },
};
ElmString16 literal_string_The_20Json_2eDecode_2eoneOf_20at_20json = {
    .header = HEADER_STRING(29),
    .words16 =
        {
            0x54,
            0x68,
            0x65,
            0x20,
            0x4a,
            0x73,
            0x6f,
            0x6e,
            0x2e,
            0x44,
            0x65,
            0x63,
            0x6f,
            0x64,
            0x65,
            0x2e,
            0x6f,
            0x6e,
            0x65,
            0x4f,
            0x66,
            0x20,
            0x61,
            0x74,
            0x20,
            0x6a,
            0x73,
            0x6f,
            0x6e,
        },
};
ElmString16 literal_string_Task = {
    .header = HEADER_STRING(4),
    .words16 =
        {
            0x54,
            0x61,
            0x73,
            0x6b,
        },
};
ElmString16 literal_string_TODO_20deadEndsToString = {
    .header = HEADER_STRING(21),
    .words16 =
        {
            0x54,
            0x4f,
            0x44,
            0x4f,
            0x20,
            0x64,
            0x65,
            0x61,
            0x64,
            0x45,
            0x6e,
            0x64,
            0x73,
            0x54,
            0x6f,
            0x53,
            0x74,
            0x72,
            0x69,
            0x6e,
            0x67,
        },
};
ElmString16 literal_string_T = {
    .header = HEADER_STRING(1),
    .words16 =
        {
            0x54,
        },
};
ElmString16 literal_string_Sign_20up = {
    .header = HEADER_STRING(7),
    .words16 =
        {
            0x53,
            0x69,
            0x67,
            0x6e,
            0x20,
            0x75,
            0x70,
        },
};
ElmString16 literal_string_Sign_20out = {
    .header = HEADER_STRING(8),
    .words16 =
        {
            0x53,
            0x69,
            0x67,
            0x6e,
            0x20,
            0x6f,
            0x75,
            0x74,
        },
};
ElmString16 literal_string_Sign_20in_20to_20view_20your_20settings_2e = {
    .header = HEADER_STRING(30),
    .words16 =
        {
            0x53,
            0x69,
            0x67,
            0x6e,
            0x20,
            0x69,
            0x6e,
            0x20,
            0x74,
            0x6f,
            0x20,
            0x76,
            0x69,
            0x65,
            0x77,
            0x20,
            0x79,
            0x6f,
            0x75,
            0x72,
            0x20,
            0x73,
            0x65,
            0x74,
            0x74,
            0x69,
            0x6e,
            0x67,
            0x73,
            0x2e,
        },
};
ElmString16 literal_string_Sign_20in_20to_20edit_20this_20article_2e = {
    .header = HEADER_STRING(29),
    .words16 =
        {
            0x53,
            0x69,
            0x67,
            0x6e,
            0x20,
            0x69,
            0x6e,
            0x20,
            0x74,
            0x6f,
            0x20,
            0x65,
            0x64,
            0x69,
            0x74,
            0x20,
            0x74,
            0x68,
            0x69,
            0x73,
            0x20,
            0x61,
            0x72,
            0x74,
            0x69,
            0x63,
            0x6c,
            0x65,
            0x2e,
        },
};
ElmString16 literal_string_Sign_20in = {
    .header = HEADER_STRING(7),
    .words16 =
        {
            0x53,
            0x69,
            0x67,
            0x6e,
            0x20,
            0x69,
            0x6e,
        },
};
ElmString16 literal_string_Short_20bio_20about_20you = {
    .header = HEADER_STRING(19),
    .words16 =
        {
            0x53,
            0x68,
            0x6f,
            0x72,
            0x74,
            0x20,
            0x62,
            0x69,
            0x6f,
            0x20,
            0x61,
            0x62,
            0x6f,
            0x75,
            0x74,
            0x20,
            0x79,
            0x6f,
            0x75,
        },
};
ElmString16 literal_string_Settings = {
    .header = HEADER_STRING(8),
    .words16 =
        {
            0x53,
            0x65,
            0x74,
            0x74,
            0x69,
            0x6e,
            0x67,
            0x73,
        },
};
ElmString16 literal_string_Server_20error = {
    .header = HEADER_STRING(12),
    .words16 =
        {
            0x53,
            0x65,
            0x72,
            0x76,
            0x65,
            0x72,
            0x20,
            0x65,
            0x72,
            0x72,
            0x6f,
            0x72,
        },
};
ElmString16 literal_string_September = {
    .header = HEADER_STRING(9),
    .words16 =
        {
            0x53,
            0x65,
            0x70,
            0x74,
            0x65,
            0x6d,
            0x62,
            0x65,
            0x72,
        },
};
ElmString16 literal_string_SLUG = {
    .header = HEADER_STRING(4),
    .words16 =
        {
            0x53,
            0x4c,
            0x55,
            0x47,
        },
};
ElmString16 literal_string_Register = {
    .header = HEADER_STRING(8),
    .words16 =
        {
            0x52,
            0x65,
            0x67,
            0x69,
            0x73,
            0x74,
            0x65,
            0x72,
        },
};
ElmString16 literal_string_Read_20more_2e_2e_2e = {
    .header = HEADER_STRING(12),
    .words16 =
        {
            0x52,
            0x65,
            0x61,
            0x64,
            0x20,
            0x6d,
            0x6f,
            0x72,
            0x65,
            0x2e,
            0x2e,
            0x2e,
        },
};
ElmString16
    literal_string_Ran_20into_20a_20Json_2eDecode_2eoneOf_20with_20no_20possibilities = {
        .header = HEADER_STRING(50),
        .words16 =
            {
                0x52,
                0x61,
                0x6e,
                0x20,
                0x69,
                0x6e,
                0x74,
                0x6f,
                0x20,
                0x61,
                0x20,
                0x4a,
                0x73,
                0x6f,
                0x6e,
                0x2e,
                0x44,
                0x65,
                0x63,
                0x6f,
                0x64,
                0x65,
                0x2e,
                0x6f,
                0x6e,
                0x65,
                0x4f,
                0x66,
                0x20,
                0x77,
                0x69,
                0x74,
                0x68,
                0x20,
                0x6e,
                0x6f,
                0x20,
                0x70,
                0x6f,
                0x73,
                0x73,
                0x69,
                0x62,
                0x69,
                0x6c,
                0x69,
                0x74,
                0x69,
                0x65,
                0x73,
            },
};
ElmString16 literal_string_Publish_20Article = {
    .header = HEADER_STRING(15),
    .words16 =
        {
            0x50,
            0x75,
            0x62,
            0x6c,
            0x69,
            0x73,
            0x68,
            0x20,
            0x41,
            0x72,
            0x74,
            0x69,
            0x63,
            0x6c,
            0x65,
        },
};
ElmString16 literal_string_Profile_20_2014_20 = {
    .header = HEADER_STRING(10),
    .words16 =
        {
            0x50,
            0x72,
            0x6f,
            0x66,
            0x69,
            0x6c,
            0x65,
            0x20,
            0x2014,
            0x20,
        },
};
ElmString16 literal_string_Profile = {
    .header = HEADER_STRING(7),
    .words16 =
        {
            0x50,
            0x72,
            0x6f,
            0x66,
            0x69,
            0x6c,
            0x65,
        },
};
ElmString16 literal_string_Problem_20with_20the_20value_20at_20json = {
    .header = HEADER_STRING(30),
    .words16 =
        {
            0x50,
            0x72,
            0x6f,
            0x62,
            0x6c,
            0x65,
            0x6d,
            0x20,
            0x77,
            0x69,
            0x74,
            0x68,
            0x20,
            0x74,
            0x68,
            0x65,
            0x20,
            0x76,
            0x61,
            0x6c,
            0x75,
            0x65,
            0x20,
            0x61,
            0x74,
            0x20,
            0x6a,
            0x73,
            0x6f,
            0x6e,
        },
};
ElmString16 literal_string_Problem_20with_20the_20given_20value_3a_5cn_5cn = {
    .header = HEADER_STRING(33),
    .words16 =
        {
            0x50,
            0x72,
            0x6f,
            0x62,
            0x6c,
            0x65,
            0x6d,
            0x20,
            0x77,
            0x69,
            0x74,
            0x68,
            0x20,
            0x74,
            0x68,
            0x65,
            0x20,
            0x67,
            0x69,
            0x76,
            0x65,
            0x6e,
            0x20,
            0x76,
            0x61,
            0x6c,
            0x75,
            0x65,
            0x3a,
            0x5c,
            0x6e,
            0x5c,
            0x6e,
        },
};
ElmString16 literal_string_Post_20Comment = {
    .header = HEADER_STRING(12),
    .words16 =
        {
            0x50,
            0x6f,
            0x73,
            0x74,
            0x20,
            0x43,
            0x6f,
            0x6d,
            0x6d,
            0x65,
            0x6e,
            0x74,
        },
};
ElmString16 literal_string_Popular_20Tags = {
    .header = HEADER_STRING(12),
    .words16 =
        {
            0x50,
            0x6f,
            0x70,
            0x75,
            0x6c,
            0x61,
            0x72,
            0x20,
            0x54,
            0x61,
            0x67,
            0x73,
        },
};
ElmString16 literal_string_Password = {
    .header = HEADER_STRING(8),
    .words16 =
        {
            0x50,
            0x61,
            0x73,
            0x73,
            0x77,
            0x6f,
            0x72,
            0x64,
        },
};
ElmString16 literal_string_Page_20Not_20Found = {
    .header = HEADER_STRING(14),
    .words16 =
        {
            0x50,
            0x61,
            0x67,
            0x65,
            0x20,
            0x4e,
            0x6f,
            0x74,
            0x20,
            0x46,
            0x6f,
            0x75,
            0x6e,
            0x64,
        },
};
ElmString16 literal_string_PUT = {
    .header = HEADER_STRING(3),
    .words16 =
        {
            0x50,
            0x55,
            0x54,
        },
};
ElmString16 literal_string_POST = {
    .header = HEADER_STRING(4),
    .words16 =
        {
            0x50,
            0x4f,
            0x53,
            0x54,
        },
};
ElmString16 literal_string_Ok = {
    .header = HEADER_STRING(2),
    .words16 =
        {
            0x4f,
            0x6b,
        },
};
ElmString16 literal_string_October = {
    .header = HEADER_STRING(7),
    .words16 =
        {
            0x4f,
            0x63,
            0x74,
            0x6f,
            0x62,
            0x65,
            0x72,
        },
};
ElmString16 literal_string_November = {
    .header = HEADER_STRING(8),
    .words16 =
        {
            0x4e,
            0x6f,
            0x76,
            0x65,
            0x6d,
            0x62,
            0x65,
            0x72,
        },
};
ElmString16 literal_string_Not_20Found = {
    .header = HEADER_STRING(9),
    .words16 =
        {
            0x4e,
            0x6f,
            0x74,
            0x20,
            0x46,
            0x6f,
            0x75,
            0x6e,
            0x64,
        },
};
ElmString16 literal_string_New_20Article = {
    .header = HEADER_STRING(11),
    .words16 =
        {
            0x4e,
            0x65,
            0x77,
            0x20,
            0x41,
            0x72,
            0x74,
            0x69,
            0x63,
            0x6c,
            0x65,
        },
};
ElmString16 literal_string_Need_20an_20account_3f = {
    .header = HEADER_STRING(16),
    .words16 =
        {
            0x4e,
            0x65,
            0x65,
            0x64,
            0x20,
            0x61,
            0x6e,
            0x20,
            0x61,
            0x63,
            0x63,
            0x6f,
            0x75,
            0x6e,
            0x74,
            0x3f,
        },
};
ElmString16 literal_string_My_20Profile = {
    .header = HEADER_STRING(10),
    .words16 =
        {
            0x4d,
            0x79,
            0x20,
            0x50,
            0x72,
            0x6f,
            0x66,
            0x69,
            0x6c,
            0x65,
        },
};
ElmString16 literal_string_My_20Articles = {
    .header = HEADER_STRING(11),
    .words16 =
        {
            0x4d,
            0x79,
            0x20,
            0x41,
            0x72,
            0x74,
            0x69,
            0x63,
            0x6c,
            0x65,
            0x73,
        },
};
ElmString16 literal_string_May = {
    .header = HEADER_STRING(3),
    .words16 =
        {
            0x4d,
            0x61,
            0x79,
        },
};
ElmString16 literal_string_March = {
    .header = HEADER_STRING(5),
    .words16 =
        {
            0x4d,
            0x61,
            0x72,
            0x63,
            0x68,
        },
};
ElmString16 literal_string_Login = {
    .header = HEADER_STRING(5),
    .words16 =
        {
            0x4c,
            0x6f,
            0x67,
            0x69,
            0x6e,
        },
};
ElmString16 literal_string_Loading_2e_2e_2e = {
    .header = HEADER_STRING(10),
    .words16 =
        {
            0x4c,
            0x6f,
            0x61,
            0x64,
            0x69,
            0x6e,
            0x67,
            0x2e,
            0x2e,
            0x2e,
        },
};
ElmString16 literal_string_June = {
    .header = HEADER_STRING(4),
    .words16 =
        {
            0x4a,
            0x75,
            0x6e,
            0x65,
        },
};
ElmString16 literal_string_July = {
    .header = HEADER_STRING(4),
    .words16 =
        {
            0x4a,
            0x75,
            0x6c,
            0x79,
        },
};
ElmString16 literal_string_Json_2eDecode_2eoneOf = {
    .header = HEADER_STRING(17),
    .words16 =
        {
            0x4a,
            0x73,
            0x6f,
            0x6e,
            0x2e,
            0x44,
            0x65,
            0x63,
            0x6f,
            0x64,
            0x65,
            0x2e,
            0x6f,
            0x6e,
            0x65,
            0x4f,
            0x66,
        },
};
ElmString16 literal_string_January = {
    .header = HEADER_STRING(7),
    .words16 =
        {
            0x4a,
            0x61,
            0x6e,
            0x75,
            0x61,
            0x72,
            0x79,
        },
};
ElmString16 literal_string_Invalid_20month_3a_20_5c_22 = {
    .header = HEADER_STRING(17),
    .words16 =
        {
            0x49,
            0x6e,
            0x76,
            0x61,
            0x6c,
            0x69,
            0x64,
            0x20,
            0x6d,
            0x6f,
            0x6e,
            0x74,
            0x68,
            0x3a,
            0x20,
            0x5c,
            0x22,
        },
};
ElmString16 literal_string_Invalid_20integer_3a_20_5c_22 = {
    .header = HEADER_STRING(19),
    .words16 =
        {
            0x49,
            0x6e,
            0x76,
            0x61,
            0x6c,
            0x69,
            0x64,
            0x20,
            0x69,
            0x6e,
            0x74,
            0x65,
            0x67,
            0x65,
            0x72,
            0x3a,
            0x20,
            0x5c,
            0x22,
        },
};
ElmString16 literal_string_Invalid_20day_3a_20 = {
    .header = HEADER_STRING(13),
    .words16 =
        {
            0x49,
            0x6e,
            0x76,
            0x61,
            0x6c,
            0x69,
            0x64,
            0x20,
            0x64,
            0x61,
            0x79,
            0x3a,
            0x20,
        },
};
ElmString16 literal_string_Home = {
    .header = HEADER_STRING(4),
    .words16 =
        {
            0x48,
            0x6f,
            0x6d,
            0x65,
        },
};
ElmString16 literal_string_Have_20an_20account_3f = {
    .header = HEADER_STRING(16),
    .words16 =
        {
            0x48,
            0x61,
            0x76,
            0x65,
            0x20,
            0x61,
            0x6e,
            0x20,
            0x61,
            0x63,
            0x63,
            0x6f,
            0x75,
            0x6e,
            0x74,
            0x3f,
        },
};
ElmString16 literal_string_Global_20Feed = {
    .header = HEADER_STRING(11),
    .words16 =
        {
            0x47,
            0x6c,
            0x6f,
            0x62,
            0x61,
            0x6c,
            0x20,
            0x46,
            0x65,
            0x65,
            0x64,
        },
};
ElmString16 literal_string_GET = {
    .header = HEADER_STRING(3),
    .words16 =
        {
            0x47,
            0x45,
            0x54,
        },
};
ElmString16 literal_string_Follow = {
    .header = HEADER_STRING(6),
    .words16 =
        {
            0x46,
            0x6f,
            0x6c,
            0x6c,
            0x6f,
            0x77,
        },
};
ElmString16 literal_string_February = {
    .header = HEADER_STRING(8),
    .words16 =
        {
            0x46,
            0x65,
            0x62,
            0x72,
            0x75,
            0x61,
            0x72,
            0x79,
        },
};
ElmString16 literal_string_Favorited_20Articles = {
    .header = HEADER_STRING(18),
    .words16 =
        {
            0x46,
            0x61,
            0x76,
            0x6f,
            0x72,
            0x69,
            0x74,
            0x65,
            0x64,
            0x20,
            0x41,
            0x72,
            0x74,
            0x69,
            0x63,
            0x6c,
            0x65,
            0x73,
        },
};
ElmString16 literal_string_Expected_20 = {
    .header = HEADER_STRING(9),
    .words16 =
        {
            0x45,
            0x78,
            0x70,
            0x65,
            0x63,
            0x74,
            0x65,
            0x64,
            0x20,
        },
};
ElmString16 literal_string_Error_20saving_20article = {
    .header = HEADER_STRING(20),
    .words16 =
        {
            0x45,
            0x72,
            0x72,
            0x6f,
            0x72,
            0x20,
            0x73,
            0x61,
            0x76,
            0x69,
            0x6e,
            0x67,
            0x20,
            0x61,
            0x72,
            0x74,
            0x69,
            0x63,
            0x6c,
            0x65,
        },
};
ElmString16 literal_string_Error_20loading_20page_2e = {
    .header = HEADER_STRING(19),
    .words16 =
        {
            0x45,
            0x72,
            0x72,
            0x6f,
            0x72,
            0x20,
            0x6c,
            0x6f,
            0x61,
            0x64,
            0x69,
            0x6e,
            0x67,
            0x20,
            0x70,
            0x61,
            0x67,
            0x65,
            0x2e,
        },
};
ElmString16 literal_string_Error_20loading_20 = {
    .header = HEADER_STRING(14),
    .words16 =
        {
            0x45,
            0x72,
            0x72,
            0x6f,
            0x72,
            0x20,
            0x6c,
            0x6f,
            0x61,
            0x64,
            0x69,
            0x6e,
            0x67,
            0x20,
        },
};
ElmString16 literal_string_Enter_20tags = {
    .header = HEADER_STRING(10),
    .words16 =
        {
            0x45,
            0x6e,
            0x74,
            0x65,
            0x72,
            0x20,
            0x74,
            0x61,
            0x67,
            0x73,
        },
};
ElmString16 literal_string_Email = {
    .header = HEADER_STRING(5),
    .words16 =
        {
            0x45,
            0x6d,
            0x61,
            0x69,
            0x6c,
        },
};
ElmString16 literal_string_Edit_20Article_20_2d_20 = {
    .header = HEADER_STRING(15),
    .words16 =
        {
            0x45,
            0x64,
            0x69,
            0x74,
            0x20,
            0x41,
            0x72,
            0x74,
            0x69,
            0x63,
            0x6c,
            0x65,
            0x20,
            0x2d,
            0x20,
        },
};
ElmString16 literal_string_December = {
    .header = HEADER_STRING(8),
    .words16 =
        {
            0x44,
            0x65,
            0x63,
            0x65,
            0x6d,
            0x62,
            0x65,
            0x72,
        },
};
ElmString16 literal_string_DELETE = {
    .header = HEADER_STRING(6),
    .words16 =
        {
            0x44,
            0x45,
            0x4c,
            0x45,
            0x54,
            0x45,
        },
};
ElmString16 literal_string_Conduit = {
    .header = HEADER_STRING(7),
    .words16 =
        {
            0x43,
            0x6f,
            0x6e,
            0x64,
            0x75,
            0x69,
            0x74,
        },
};
ElmString16 literal_string_August = {
    .header = HEADER_STRING(6),
    .words16 =
        {
            0x41,
            0x75,
            0x67,
            0x75,
            0x73,
            0x74,
        },
};
ElmString16 literal_string_Article_20failed_20to_20load_2e = {
    .header = HEADER_STRING(23),
    .words16 =
        {
            0x41,
            0x72,
            0x74,
            0x69,
            0x63,
            0x6c,
            0x65,
            0x20,
            0x66,
            0x61,
            0x69,
            0x6c,
            0x65,
            0x64,
            0x20,
            0x74,
            0x6f,
            0x20,
            0x6c,
            0x6f,
            0x61,
            0x64,
            0x2e,
        },
};
ElmString16 literal_string_Article_20Title = {
    .header = HEADER_STRING(13),
    .words16 =
        {
            0x41,
            0x72,
            0x74,
            0x69,
            0x63,
            0x6c,
            0x65,
            0x20,
            0x54,
            0x69,
            0x74,
            0x6c,
            0x65,
        },
};
ElmString16 literal_string_Article = {
    .header = HEADER_STRING(7),
    .words16 =
        {
            0x41,
            0x72,
            0x74,
            0x69,
            0x63,
            0x6c,
            0x65,
        },
};
ElmString16 literal_string_April = {
    .header = HEADER_STRING(5),
    .words16 =
        {
            0x41,
            0x70,
            0x72,
            0x69,
            0x6c,
        },
};
ElmString16 literal_string_An_20interactive_20learning_20project_20from_20 = {
    .header = HEADER_STRING(37),
    .words16 =
        {
            0x41,
            0x6e,
            0x20,
            0x69,
            0x6e,
            0x74,
            0x65,
            0x72,
            0x61,
            0x63,
            0x74,
            0x69,
            0x76,
            0x65,
            0x20,
            0x6c,
            0x65,
            0x61,
            0x72,
            0x6e,
            0x69,
            0x6e,
            0x67,
            0x20,
            0x70,
            0x72,
            0x6f,
            0x6a,
            0x65,
            0x63,
            0x74,
            0x20,
            0x66,
            0x72,
            0x6f,
            0x6d,
            0x20,
        },
};
ElmString16 literal_string_A_20place_20to_20share_20your_20knowledge_2e = {
    .header = HEADER_STRING(32),
    .words16 =
        {
            0x41,
            0x20,
            0x70,
            0x6c,
            0x61,
            0x63,
            0x65,
            0x20,
            0x74,
            0x6f,
            0x20,
            0x73,
            0x68,
            0x61,
            0x72,
            0x65,
            0x20,
            0x79,
            0x6f,
            0x75,
            0x72,
            0x20,
            0x6b,
            0x6e,
            0x6f,
            0x77,
            0x6c,
            0x65,
            0x64,
            0x67,
            0x65,
            0x2e,
        },
};
ElmString16 literal_string__3f = {
    .header = HEADER_STRING(1),
    .words16 =
        {
            0x3f,
        },
};
ElmString16 literal_string__3d = {
    .header = HEADER_STRING(1),
    .words16 =
        {
            0x3d,
        },
};
ElmString16 literal_string__3a_5cn_5cn_20_20_20_20 = {
    .header = HEADER_STRING(9),
    .words16 =
        {
            0x3a,
            0x5c,
            0x6e,
            0x5c,
            0x6e,
            0x20,
            0x20,
            0x20,
            0x20,
        },
};
ElmString16 literal_string__3a = {
    .header = HEADER_STRING(1),
    .words16 =
        {
            0x3a,
        },
};
ElmString16 literal_string_8 = {
    .header = HEADER_STRING(1),
    .words16 =
        {
            0x38,
        },
};
ElmString16 literal_string_3 = {
    .header = HEADER_STRING(1),
    .words16 =
        {
            0x33,
        },
};
ElmString16 literal_string_20px = {
    .header = HEADER_STRING(4),
    .words16 =
        {
            0x32,
            0x30,
            0x70,
            0x78,
        },
};
ElmString16 literal_string_1px_20solid = {
    .header = HEADER_STRING(9),
    .words16 =
        {
            0x31,
            0x70,
            0x78,
            0x20,
            0x73,
            0x6f,
            0x6c,
            0x69,
            0x64,
        },
};
ElmString16 literal_string_0 = {
    .header = HEADER_STRING(1),
    .words16 =
        {
            0x30,
        },
};
ElmString16 literal_string__2fassets_2fimages_2f = {
    .header = HEADER_STRING(15),
    .words16 =
        {
            0x2f,
            0x61,
            0x73,
            0x73,
            0x65,
            0x74,
            0x73,
            0x2f,
            0x69,
            0x6d,
            0x61,
            0x67,
            0x65,
            0x73,
            0x2f,
        },
};
ElmString16 literal_string__2f = {
    .header = HEADER_STRING(1),
    .words16 =
        {
            0x2f,
        },
};
ElmString16 literal_string__2e_20Code_20_26_20design_20licensed_20under_20MIT_2e = {
    .header = HEADER_STRING(35),
    .words16 =
        {
            0x2e,
            0x20,
            0x43,
            0x6f,
            0x64,
            0x65,
            0x20,
            0x26,
            0x20,
            0x64,
            0x65,
            0x73,
            0x69,
            0x67,
            0x6e,
            0x20,
            0x6c,
            0x69,
            0x63,
            0x65,
            0x6e,
            0x73,
            0x65,
            0x64,
            0x20,
            0x75,
            0x6e,
            0x64,
            0x65,
            0x72,
            0x20,
            0x4d,
            0x49,
            0x54,
            0x2e,
        },
};
ElmString16 literal_string__2e = {
    .header = HEADER_STRING(1),
    .words16 =
        {
            0x2e,
        },
};
ElmString16 literal_string__2d = {
    .header = HEADER_STRING(1),
    .words16 =
        {
            0x2d,
        },
};
ElmString16 literal_string__2c_20 = {
    .header = HEADER_STRING(2),
    .words16 =
        {
            0x2c,
            0x20,
        },
};
ElmString16 literal_string__2b = {
    .header = HEADER_STRING(1),
    .words16 =
        {
            0x2b,
        },
};
ElmString16 literal_string__29_20 = {
    .header = HEADER_STRING(2),
    .words16 =
        {
            0x29,
            0x20,
        },
};
ElmString16 literal_string__29 = {
    .header = HEADER_STRING(1),
    .words16 =
        {
            0x29,
        },
};
ElmString16 literal_string__26 = {
    .header = HEADER_STRING(1),
    .words16 =
        {
            0x26,
        },
};
ElmString16 literal_string__23_2f = {
    .header = HEADER_STRING(2),
    .words16 =
        {
            0x23,
            0x2f,
        },
};
ElmString16 literal_string__23 = {
    .header = HEADER_STRING(1),
    .words16 =
        {
            0x23,
        },
};
ElmString16 literal_string__21 = {
    .header = HEADER_STRING(1),
    .words16 =
        {
            0x21,
        },
};
ElmString16 literal_string__20ways_3a = {
    .header = HEADER_STRING(6),
    .words16 =
        {
            0x20,
            0x77,
            0x61,
            0x79,
            0x73,
            0x3a,
        },
};
ElmString16 literal_string__20to_20comment_2e = {
    .header = HEADER_STRING(12),
    .words16 =
        {
            0x20,
            0x74,
            0x6f,
            0x20,
            0x63,
            0x6f,
            0x6d,
            0x6d,
            0x65,
            0x6e,
            0x74,
            0x2e,
        },
};
ElmString16 literal_string__20or_20 = {
    .header = HEADER_STRING(4),
    .words16 =
        {
            0x20,
            0x6f,
            0x72,
            0x20,
        },
};
ElmString16 literal_string__20failed_20in_20the_20following_20 = {
    .header = HEADER_STRING(25),
    .words16 =
        {
            0x20,
            0x66,
            0x61,
            0x69,
            0x6c,
            0x65,
            0x64,
            0x20,
            0x69,
            0x6e,
            0x20,
            0x74,
            0x68,
            0x65,
            0x20,
            0x66,
            0x6f,
            0x6c,
            0x6c,
            0x6f,
            0x77,
            0x69,
            0x6e,
            0x67,
            0x20,
        },
};
ElmString16 literal_string__20digits_2c_20but_20got_20 = {
    .header = HEADER_STRING(17),
    .words16 =
        {
            0x20,
            0x64,
            0x69,
            0x67,
            0x69,
            0x74,
            0x73,
            0x2c,
            0x20,
            0x62,
            0x75,
            0x74,
            0x20,
            0x67,
            0x6f,
            0x74,
            0x20,
        },
};
ElmString16 literal_string__20characters_20long_2e = {
    .header = HEADER_STRING(17),
    .words16 =
        {
            0x20,
            0x63,
            0x68,
            0x61,
            0x72,
            0x61,
            0x63,
            0x74,
            0x65,
            0x72,
            0x73,
            0x20,
            0x6c,
            0x6f,
            0x6e,
            0x67,
            0x2e,
        },
};
ElmString16 literal_string__20at_20json = {
    .header = HEADER_STRING(8),
    .words16 =
        {
            0x20,
            0x61,
            0x74,
            0x20,
            0x6a,
            0x73,
            0x6f,
            0x6e,
        },
};
ElmString16 literal_string__20action_2dbtn = {
    .header = HEADER_STRING(11),
    .words16 =
        {
            0x20,
            0x61,
            0x63,
            0x74,
            0x69,
            0x6f,
            0x6e,
            0x2d,
            0x62,
            0x74,
            0x6e,
        },
};
ElmString16 literal_string__20Favorite_20Article_20_28 = {
    .header = HEADER_STRING(19),
    .words16 =
        {
            0x20,
            0x46,
            0x61,
            0x76,
            0x6f,
            0x72,
            0x69,
            0x74,
            0x65,
            0x20,
            0x41,
            0x72,
            0x74,
            0x69,
            0x63,
            0x6c,
            0x65,
            0x20,
            0x28,
        },
};
ElmString16 literal_string__20Edit_20Article = {
    .header = HEADER_STRING(13),
    .words16 =
        {
            0x20,
            0x45,
            0x64,
            0x69,
            0x74,
            0x20,
            0x41,
            0x72,
            0x74,
            0x69,
            0x63,
            0x6c,
            0x65,
        },
};
ElmString16 literal_string__20Delete_20Article = {
    .header = HEADER_STRING(15),
    .words16 =
        {
            0x20,
            0x44,
            0x65,
            0x6c,
            0x65,
            0x74,
            0x65,
            0x20,
            0x41,
            0x72,
            0x74,
            0x69,
            0x63,
            0x6c,
            0x65,
        },
};
ElmString16 literal_string__20_2d_20Conduit = {
    .header = HEADER_STRING(10),
    .words16 =
        {
            0x20,
            0x2d,
            0x20,
            0x43,
            0x6f,
            0x6e,
            0x64,
            0x75,
            0x69,
            0x74,
        },
};
ElmString16 literal_string__20 = {
    .header = HEADER_STRING(1),
    .words16 =
        {
            0x20,
        },
};
ElmString16 literal_string_ = {
    .header = HEADER_STRING(0),
    .words16 = {},
};
ElmInt literal_int_31536000000 = {
    .header = HEADER_INT,
    .value = 31536000000,
};
ElmInt literal_int_28857600000 = {
    .header = HEADER_INT,
    .value = 28857600000,
};
ElmInt literal_int_26265600000 = {
    .header = HEADER_INT,
    .value = 26265600000,
};
ElmInt literal_int_23587200000 = {
    .header = HEADER_INT,
    .value = 23587200000,
};
ElmInt literal_int_20995200000 = {
    .header = HEADER_INT,
    .value = 20995200000,
};
ElmInt literal_int_18316800000 = {
    .header = HEADER_INT,
    .value = 18316800000,
};
ElmInt literal_int_15638400000 = {
    .header = HEADER_INT,
    .value = 15638400000,
};
ElmInt literal_int_13046400000 = {
    .header = HEADER_INT,
    .value = 13046400000,
};
ElmInt literal_int_10368000000 = {
    .header = HEADER_INT,
    .value = 10368000000,
};
ElmInt literal_int_7776000000 = {
    .header = HEADER_INT,
    .value = 7776000000,
};
ElmInt literal_int_5097600000 = {
    .header = HEADER_INT,
    .value = 5097600000,
};
ElmInt literal_int_2678400000 = {
    .header = HEADER_INT,
    .value = 2678400000,
};
ElmInt literal_int_86400000 = {
    .header = HEADER_INT,
    .value = 86400000,
};
ElmInt literal_int_719468 = {
    .header = HEADER_INT,
    .value = 719468,
};
ElmInt literal_int_146097 = {
    .header = HEADER_INT,
    .value = 146097,
};
ElmInt literal_int_146096 = {
    .header = HEADER_INT,
    .value = 146096,
};
ElmInt literal_int_60000 = {
    .header = HEADER_INT,
    .value = 60000,
};
ElmInt literal_int_36524 = {
    .header = HEADER_INT,
    .value = 36524,
};
ElmInt literal_int_1970 = {
    .header = HEADER_INT,
    .value = 1970,
};
ElmInt literal_int_1460 = {
    .header = HEADER_INT,
    .value = 1460,
};
ElmInt literal_int_1000 = {
    .header = HEADER_INT,
    .value = 1000,
};
ElmInt literal_int_500 = {
    .header = HEADER_INT,
    .value = 500,
};
ElmInt literal_int_400 = {
    .header = HEADER_INT,
    .value = 400,
};
ElmInt literal_int_365 = {
    .header = HEADER_INT,
    .value = 365,
};
ElmInt literal_int_153 = {
    .header = HEADER_INT,
    .value = 153,
};
ElmInt literal_int_122 = {
    .header = HEADER_INT,
    .value = 122,
};
ElmInt literal_int_100 = {
    .header = HEADER_INT,
    .value = 100,
};
ElmInt literal_int_97 = {
    .header = HEADER_INT,
    .value = 97,
};
ElmInt literal_int_90 = {
    .header = HEADER_INT,
    .value = 90,
};
ElmInt literal_int_65 = {
    .header = HEADER_INT,
    .value = 65,
};
ElmInt literal_int_64 = {
    .header = HEADER_INT,
    .value = 64,
};
ElmInt literal_int_60 = {
    .header = HEADER_INT,
    .value = 60,
};
ElmInt literal_int_57 = {
    .header = HEADER_INT,
    .value = 57,
};
ElmInt literal_int_48 = {
    .header = HEADER_INT,
    .value = 48,
};
ElmInt literal_int_31 = {
    .header = HEADER_INT,
    .value = 31,
};
ElmInt literal_int_30 = {
    .header = HEADER_INT,
    .value = 30,
};
ElmInt literal_int_29 = {
    .header = HEADER_INT,
    .value = 29,
};
ElmInt literal_int_24 = {
    .header = HEADER_INT,
    .value = 24,
};
ElmInt literal_int_10 = {
    .header = HEADER_INT,
    .value = 10,
};
ElmInt literal_int_9 = {
    .header = HEADER_INT,
    .value = 9,
};
ElmInt literal_int_6 = {
    .header = HEADER_INT,
    .value = 6,
};
ElmInt literal_int_5 = {
    .header = HEADER_INT,
    .value = 5,
};
ElmInt literal_int_4 = {
    .header = HEADER_INT,
    .value = 4,
};
ElmInt literal_int_3 = {
    .header = HEADER_INT,
    .value = 3,
};
ElmInt literal_int_2 = {
    .header = HEADER_INT,
    .value = 2,
};
ElmInt literal_int_1 = {
    .header = HEADER_INT,
    .value = 1,
};
ElmInt literal_int_0 = {
    .header = HEADER_INT,
    .value = 0,
};
FieldGroup* app_field_groups[] = {
    &fg_article_comments_errors_session_timeZone,
    &fg_articles_errors_isLoading_session,
    &fg_author_body_createdAt_id,
    &fg_author_errors_feed_feedPage_feedTab_session_timeZone,
    &fg_author_metadata_slug,
    &fg_avatar_bio,
    &fg_avatar_bio_email_password_username,
    &fg_body_description_tags_title,
    &fg_body_expect_headers_method_timeout_url_withCredentials,
    &fg_body_title,
    &fg_breaks_tables,
    &fg_col_context_indent_offset_row_src,
    &fg_col_contextStack_problem_row,
    &fg_col_problem_row,
    &fg_content_title,
    &fg_createdAt_description_favorited_favoritesCount_tags_title,
    &fg_day_month_year,
    &fg_defaultHighlighting_githubFlavored_sanitize_smartypants,
    &fg_email_password,
    &fg_email_password_username,
    &fg_feed_feedPage_feedTab_session_tags_timeZone,
    &fg_form_problems_session,
    &fg_frag_params_unvisited_value_visited,
    &fg_init_onUrlChange_onUrlRequest_subscriptions_update_view,
    &fg_page_resultsPerPage,
    &fg_problems_session_status,
    &fg_session_status,
    &fg_total_values,
    NULL,
};

void* eval_author_project_Main_ChangedUrl(void* args[]) {
  return ctorCustom(CTOR_ChangedUrl, 1, args);
}
Closure author_project_Main_ChangedUrl = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Main_ChangedUrl,
};

void* eval_author_project_Main_ClickedLink(void* args[]) {
  return ctorCustom(CTOR_ClickedLink, 1, args);
}
Closure author_project_Main_ClickedLink = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Main_ClickedLink,
};

void* eval_elm_core_Result_Err(void* args[]) {
  return ctorCustom(CTOR_Err, 1, args);
}
Closure elm_core_Result_Err = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_core_Result_Err,
};
void* eval_elm_core_Result_andThen(void* args[]) {
  void* x_callback = args[0];
  void* x_result = args[1];
  void* tmp0;
  void* tmp1 = ((Custom*)x_result)->ctor;
  if (tmp1 == CTOR_Ok) {
    void* x_value = Utils_destruct_index(x_result, 0);
    tmp0 = A1(x_callback, x_value);
  } else {
    void* x_msg = Utils_destruct_index(x_result, 0);
    tmp0 = A1(&elm_core_Result_Err, x_msg);
  };
  return tmp0;
}
Closure elm_core_Result_andThen = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_core_Result_andThen,
};

void* eval_elm_core_Basics_apR(void* args[]) {
  void* x_x = args[0];
  void* x_f = args[1];
  return A1(x_f, x_x);
}
Closure elm_core_Basics_apR = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_core_Basics_apR,
};

#define elm_browser_Browser_application Browser_application

#define elm_json_Json_Decode_decodeString Json_runOnString

#define elm_json_Json_Decode_decodeValue Json_run

void* eval_author_project_Api_Cred(void* args[]) {
  return ctorCustom(CTOR_Cred, 2, args);
}
Closure author_project_Api_Cred = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Api_Cred,
};

void* eval_author_project_Username_Username(void* args[]) {
  return ctorCustom(CTOR_Username, 1, args);
}
Closure author_project_Username_Username = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Username_Username,
};

void* eval_elm_core_Basics_identity(void* args[]) {
  void* x_x = args[0];
  return x_x;
}
Closure elm_core_Basics_identity = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_core_Basics_identity,
};

#define elm_json_Json_Decode_map Json_map1

#define elm_json_Json_Decode_string Json_decodeString
#define author_project_Username_decoder (*ptr_author_project_Username_decoder)
ElmValue* ptr_author_project_Username_decoder;
void* init_author_project_Username_decoder() {
  return A2(&elm_json_Json_Decode_map,
      &author_project_Username_Username,
      &elm_json_Json_Decode_string);
}

#define elm_json_Json_Decode_map2 Json_map2
#define NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_custom \
  (*ptr_NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_custom)
ElmValue* ptr_NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_custom;
void* init_NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_custom() {
  return A1(&elm_json_Json_Decode_map2, &elm_core_Basics_apR);
}

#define elm_json_Json_Decode_field Json_decodeField
void* eval_NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_required(void* args[]) {
  void* x_key = args[0];
  void* x_valDecoder = args[1];
  void* x_decoder = args[2];
  return A2(&NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_custom,
      A2(&elm_json_Json_Decode_field, x_key, x_valDecoder),
      x_decoder);
}
Closure NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_required = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_required,
};

#define elm_json_Json_Decode_succeed Json_succeed
#define author_project_Api_credDecoder (*ptr_author_project_Api_credDecoder)
ElmValue* ptr_author_project_Api_credDecoder;
void* init_author_project_Api_credDecoder() {
  return A2(&elm_core_Basics_apR,
      A2(&elm_core_Basics_apR,
          A1(&elm_json_Json_Decode_succeed, &author_project_Api_Cred),
          A2(&NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_required,
              &literal_string_username,
              &author_project_Username_decoder)),
      A2(&NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_required,
          &literal_string_token,
          &elm_json_Json_Decode_string));
}
void* eval_author_project_Api_decoderFromCred_lambda0(void* args[]) {
  void* x_fromCred = args[0];
  void* x_cred = args[1];
  return A1(x_fromCred, x_cred);
}
void* eval_author_project_Api_decoderFromCred(void* args[]) {
  void* x_decoder = args[0];
  return A3(&elm_json_Json_Decode_map2,
      NEW_CLOSURE(0, 2, &eval_author_project_Api_decoderFromCred_lambda0, ((void* []){})),
      x_decoder,
      &author_project_Api_credDecoder);
}
Closure author_project_Api_decoderFromCred = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Api_decoderFromCred,
};
void* eval_author_project_Api_storageDecoder(void* args[]) {
  void* x_viewerDecoder = args[0];
  return A2(&elm_json_Json_Decode_field,
      &literal_string_user,
      A1(&author_project_Api_decoderFromCred, x_viewerDecoder));
}
Closure author_project_Api_storageDecoder = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Api_storageDecoder,
};

void* eval_elm_core_Maybe_Just(void* args[]) {
  return ctorCustom(CTOR_Just, 1, args);
}
Closure elm_core_Maybe_Just = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_core_Maybe_Just,
};

Custom elm_core_Maybe_Nothing = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Nothing,
};
void* eval_elm_core_Result_toMaybe(void* args[]) {
  void* x_result = args[0];
  void* tmp0;
  void* tmp1 = ((Custom*)x_result)->ctor;
  if (tmp1 == CTOR_Ok) {
    void* x_v = Utils_destruct_index(x_result, 0);
    tmp0 = A1(&elm_core_Maybe_Just, x_v);
  } else {
    tmp0 = &elm_core_Maybe_Nothing;
  };
  return tmp0;
}
Closure elm_core_Result_toMaybe = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_core_Result_toMaybe,
};
void* eval_author_project_Api_application_lambda0(void* args[]) {
  void* x_config = args[0];
  void* x_viewerDecoder = args[1];
  void* x_flags = args[2];
  void* x_url = args[3];
  void* x_navKey = args[4];
  void* x_maybeViewer = A2(&elm_core_Basics_apR,
      A2(&elm_core_Basics_apR,
          A2(&elm_json_Json_Decode_decodeValue, &elm_json_Json_Decode_string, x_flags),
          A1(&elm_core_Result_andThen,
              A1(&elm_json_Json_Decode_decodeString,
                  A1(&author_project_Api_storageDecoder, x_viewerDecoder)))),
      &elm_core_Result_toMaybe);
  return A3(Utils_access_eval(((void* []){
                (void*)FIELD_init,
                x_config,
            })),
      x_maybeViewer,
      x_url,
      x_navKey);
}
void* eval_author_project_Api_application(void* args[]) {
  void* x_viewerDecoder = args[0];
  void* x_config = args[1];
  void* x_init = NEW_CLOSURE(2,
      5,
      &eval_author_project_Api_application_lambda0,
      ((void* []){
          x_config,
          x_viewerDecoder,
      }));
  return A1(&elm_browser_Browser_application,
      NEW_RECORD(&fg_init_onUrlChange_onUrlRequest_subscriptions_update_view,
          6,
          ((void* []){
              x_init,
              Utils_access_eval(((void* []){
                  (void*)FIELD_onUrlChange,
                  x_config,
              })),
              Utils_access_eval(((void* []){
                  (void*)FIELD_onUrlRequest,
                  x_config,
              })),
              Utils_access_eval(((void* []){
                  (void*)FIELD_subscriptions,
                  x_config,
              })),
              Utils_access_eval(((void* []){
                  (void*)FIELD_update,
                  x_config,
              })),
              Utils_access_eval(((void* []){
                  (void*)FIELD_view,
                  x_config,
              })),
          })));
}
Closure author_project_Api_application = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Api_application,
};

void* eval_author_project_Viewer_Viewer(void* args[]) {
  return ctorCustom(CTOR_Viewer, 2, args);
}
Closure author_project_Viewer_Viewer = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Viewer_Viewer,
};

void* eval_author_project_Avatar_Avatar(void* args[]) {
  return ctorCustom(CTOR_Avatar, 1, args);
}
Closure author_project_Avatar_Avatar = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Avatar_Avatar,
};

#define elm_json_Json_Decode_null Json_decodeNull

#define elm_json_Json_Decode_oneOf Json_oneOf
void* eval_elm_json_Json_Decode_nullable(void* args[]) {
  void* x_decoder = args[0];
  return A1(&elm_json_Json_Decode_oneOf,
      List_fromArray(2,
          ((void* []){
              A1(&elm_json_Json_Decode_null, &elm_core_Maybe_Nothing),
              A2(&elm_json_Json_Decode_map, &elm_core_Maybe_Just, x_decoder),
          })));
}
Closure elm_json_Json_Decode_nullable = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_json_Json_Decode_nullable,
};
#define author_project_Avatar_decoder (*ptr_author_project_Avatar_decoder)
ElmValue* ptr_author_project_Avatar_decoder;
void* init_author_project_Avatar_decoder() {
  return A2(&elm_json_Json_Decode_map,
      &author_project_Avatar_Avatar,
      A1(&elm_json_Json_Decode_nullable, &elm_json_Json_Decode_string));
}
#define author_project_Viewer_decoder (*ptr_author_project_Viewer_decoder)
ElmValue* ptr_author_project_Viewer_decoder;
void* init_author_project_Viewer_decoder() {
  return A2(&elm_core_Basics_apR,
      A1(&elm_json_Json_Decode_succeed, &author_project_Viewer_Viewer),
      A1(&NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_custom,
          A2(&elm_json_Json_Decode_field,
              &literal_string_image,
              &author_project_Avatar_decoder)));
}

void* eval_author_project_Main_Redirect(void* args[]) {
  return ctorCustom(CTOR_Redirect, 1, args);
}
Closure author_project_Main_Redirect = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Main_Redirect,
};

void* eval_author_project_Main_Article(void* args[]) {
  return ctorCustom(CTOR_Article, 1, args);
}
Closure author_project_Main_Article = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Main_Article,
};

void* eval_author_project_Main_Editor(void* args[]) {
  return ctorCustom(CTOR_Editor, 2, args);
}
Closure author_project_Main_Editor = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Main_Editor,
};

void* eval_author_project_Main_GotArticleMsg(void* args[]) {
  return ctorCustom(CTOR_GotArticleMsg, 1, args);
}
Closure author_project_Main_GotArticleMsg = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Main_GotArticleMsg,
};

void* eval_author_project_Main_GotEditorMsg(void* args[]) {
  return ctorCustom(CTOR_GotEditorMsg, 1, args);
}
Closure author_project_Main_GotEditorMsg = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Main_GotEditorMsg,
};

void* eval_author_project_Main_GotHomeMsg(void* args[]) {
  return ctorCustom(CTOR_GotHomeMsg, 1, args);
}
Closure author_project_Main_GotHomeMsg = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Main_GotHomeMsg,
};

void* eval_author_project_Main_GotLoginMsg(void* args[]) {
  return ctorCustom(CTOR_GotLoginMsg, 1, args);
}
Closure author_project_Main_GotLoginMsg = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Main_GotLoginMsg,
};

void* eval_author_project_Main_GotProfileMsg(void* args[]) {
  return ctorCustom(CTOR_GotProfileMsg, 1, args);
}
Closure author_project_Main_GotProfileMsg = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Main_GotProfileMsg,
};

void* eval_author_project_Main_GotRegisterMsg(void* args[]) {
  return ctorCustom(CTOR_GotRegisterMsg, 1, args);
}
Closure author_project_Main_GotRegisterMsg = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Main_GotRegisterMsg,
};

void* eval_author_project_Main_GotSettingsMsg(void* args[]) {
  return ctorCustom(CTOR_GotSettingsMsg, 1, args);
}
Closure author_project_Main_GotSettingsMsg = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Main_GotSettingsMsg,
};

void* eval_author_project_Main_Home(void* args[]) {
  return ctorCustom(CTOR_Home, 1, args);
}
Closure author_project_Main_Home = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Main_Home,
};

Custom author_project_Route_Home = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Home,
};

void* eval_author_project_Main_Login(void* args[]) {
  return ctorCustom(CTOR_Login, 1, args);
}
Closure author_project_Main_Login = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Main_Login,
};

void* eval_author_project_Main_NotFound(void* args[]) {
  return ctorCustom(CTOR_NotFound, 1, args);
}
Closure author_project_Main_NotFound = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Main_NotFound,
};

void* eval_author_project_Main_Profile(void* args[]) {
  return ctorCustom(CTOR_Profile, 2, args);
}
Closure author_project_Main_Profile = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Main_Profile,
};

void* eval_author_project_Main_Register(void* args[]) {
  return ctorCustom(CTOR_Register, 1, args);
}
Closure author_project_Main_Register = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Main_Register,
};

void* eval_author_project_Main_Settings(void* args[]) {
  return ctorCustom(CTOR_Settings, 1, args);
}
Closure author_project_Main_Settings = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Main_Settings,
};

void* eval_author_project_Page_Article_CompletedLoadArticle(void* args[]) {
  return ctorCustom(CTOR_CompletedLoadArticle, 1, args);
}
Closure author_project_Page_Article_CompletedLoadArticle = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_CompletedLoadArticle,
};

void* eval_author_project_Page_Article_CompletedLoadComments(void* args[]) {
  return ctorCustom(CTOR_CompletedLoadComments, 1, args);
}
Closure author_project_Page_Article_CompletedLoadComments = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_CompletedLoadComments,
};

void* eval_author_project_Page_Article_GotTimeZone(void* args[]) {
  return ctorCustom(CTOR_GotTimeZone, 1, args);
}
Closure author_project_Page_Article_GotTimeZone = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_GotTimeZone,
};

Custom author_project_Page_Article_Loading = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Loading,
};

Custom author_project_Page_Article_PassedSlowLoadThreshold = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_PassedSlowLoadThreshold,
};

#define elm_core_Platform_Cmd_batch Platform_batch

void* eval_author_project_Viewer_cred(void* args[]) {
  void* x__v0 = args[0];
  void* x_val = Utils_destruct_index(x__v0, 1);
  return x_val;
}
Closure author_project_Viewer_cred = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Viewer_cred,
};
void* eval_author_project_Session_cred(void* args[]) {
  void* x_session = args[0];
  void* tmp0;
  void* tmp1 = ((Custom*)x_session)->ctor;
  if (tmp1 == CTOR_LoggedIn) {
    void* x_val = Utils_destruct_index(x_session, 1);
    tmp0 = A1(&elm_core_Maybe_Just, A1(&author_project_Viewer_cred, x_val));
  } else {
    tmp0 = &elm_core_Maybe_Nothing;
  };
  return tmp0;
}
Closure author_project_Session_cred = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Session_cred,
};

void* eval_author_project_Article_Slug_toString(void* args[]) {
  void* x__v0 = args[0];
  void* x_str = ((Custom*)x__v0)->values[0];
  return x_str;
}
Closure author_project_Article_Slug_toString = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Article_Slug_toString,
};

void* eval_author_project_Api_Endpoint_Endpoint(void* args[]) {
  return ctorCustom(CTOR_Endpoint, 1, args);
}
Closure author_project_Api_Endpoint_Endpoint = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Api_Endpoint_Endpoint,
};

#define elm_core_List_cons List_cons

#define elm_core_Basics_append Utils_append

void* eval_elm_core_String_join(void* args[]) {
  void* x_sep = args[0];
  void* x_chunks = args[1];
  return A2(&String_join, x_sep, A1(&List_toArray, x_chunks));
}
Closure elm_core_String_join = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_core_String_join,
};

#define elm_core_Basics_add Basics_add

void* tce_elm_core_List_foldl(void* args[], void** gc_tce_data) {
tce_loop:;
  void* x_func = args[0];
  void* x_acc = args[1];
  void* x_list = args[2];
  void* tmp0;
  if (x_list == &Nil) {
    tmp0 = x_acc;
  } else {
    void* x_x = Utils_destruct_index(x_list, 0);
    void* x_xs = Utils_destruct_index(x_list, 1);
    void* tmp1 = x_func;
    void* tmp2 = A2(x_func, x_x, x_acc);
    void* tmp3 = x_xs;
    *gc_tce_data = CAN_THROW(GC_tce_iteration(3));
    args[2] = tmp3;
    args[1] = tmp2;
    args[0] = tmp1;
    goto tce_loop;
    tmp0 = NULL;
  };
  return tmp0;
}
void* eval_elm_core_List_foldl() {
  return GC_tce_eval(&tce_elm_core_List_foldl, &eval_elm_core_List_foldl, 3, args);
}
Closure elm_core_List_foldl = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_elm_core_List_foldl,
};

#define elm_core_Basics_gt Utils_gt

void* eval_elm_core_List_reverse(void* args[]) {
  void* x_list = args[0];
  return A3(&elm_core_List_foldl, &elm_core_List_cons, &Nil, x_list);
}
Closure elm_core_List_reverse = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_core_List_reverse,
};
void* eval_elm_core_List_foldrHelper(void* args[]) {
  void* x_fn = args[0];
  void* x_acc = args[1];
  void* x_ctr = args[2];
  void* x_ls = args[3];
  void* tmp0;
  if (x_ls == &Nil) {
    tmp0 = x_acc;
  } else {
    void* x_a = Utils_destruct_index(x_ls, 0);
    void* x_r1 = Utils_destruct_index(x_ls, 1);
    void* tmp1;
    if (x_r1 == &Nil) {
      tmp1 = A2(x_fn, x_a, x_acc);
    } else {
      void* x_b = Utils_destruct_index(x_r1, 0);
      void* x_r2 = Utils_destruct_index(x_r1, 1);
      void* tmp2;
      if (x_r2 == &Nil) {
        tmp2 = A2(x_fn, x_a, A2(x_fn, x_b, x_acc));
      } else {
        void* x_c = Utils_destruct_index(x_r2, 0);
        void* x_r3 = Utils_destruct_index(x_r2, 1);
        void* tmp3;
        if (x_r3 == &Nil) {
          tmp3 = A2(x_fn, x_a, A2(x_fn, x_b, A2(x_fn, x_c, x_acc)));
        } else {
          void* x_d = Utils_destruct_index(x_r3, 0);
          void* x_r4 = Utils_destruct_index(x_r3, 1);
          void* tmp4;
          if (A2(&elm_core_Basics_gt, x_ctr, &literal_int_500) == &True) {
            tmp4 =
                A3(&elm_core_List_foldl, x_fn, x_acc, A1(&elm_core_List_reverse, x_r4));
          } else {
            tmp4 = A4(&elm_core_List_foldrHelper,
                x_fn,
                x_acc,
                A2(&elm_core_Basics_add, x_ctr, &literal_int_1),
                x_r4);
          };
          void* x_res = tmp4;
          tmp3 = A2(x_fn, x_a, A2(x_fn, x_b, A2(x_fn, x_c, A2(x_fn, x_d, x_res))));
        };
        tmp2 = tmp3;
      };
      tmp1 = tmp2;
    };
    tmp0 = tmp1;
  };
  return tmp0;
}
Closure elm_core_List_foldrHelper = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x4,
    .evaluator = &eval_elm_core_List_foldrHelper,
};
void* eval_elm_core_List_foldr(void* args[]) {
  void* x_fn = args[0];
  void* x_acc = args[1];
  void* x_ls = args[2];
  return A4(&elm_core_List_foldrHelper, x_fn, x_acc, &literal_int_0, x_ls);
}
Closure elm_core_List_foldr = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_elm_core_List_foldr,
};
void* eval_elm_core_List_map_lambda0(void* args[]) {
  void* x_f = args[0];
  void* x_x = args[1];
  void* x_acc = args[2];
  return A2(&elm_core_List_cons, A1(x_f, x_x), x_acc);
}
void* eval_elm_core_List_map(void* args[]) {
  void* x_f = args[0];
  void* x_xs = args[1];
  return A3(&elm_core_List_foldr,
      NEW_CLOSURE(1,
          3,
          &eval_elm_core_List_map_lambda0,
          ((void* []){
              x_f,
          })),
      &Nil,
      x_xs);
}
Closure elm_core_List_map = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_core_List_map,
};

void* eval_elm_url_Url_Builder_toQueryPair(void* args[]) {
  void* x__v0 = args[0];
  void* x_key = Utils_destruct_index(x__v0, 0);
  void* x_value = Utils_destruct_index(x__v0, 1);
  return A2(&elm_core_Basics_append,
      x_key,
      A2(&elm_core_Basics_append, &literal_string__3d, x_value));
}
Closure elm_url_Url_Builder_toQueryPair = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_url_Url_Builder_toQueryPair,
};
void* eval_elm_url_Url_Builder_toQuery(void* args[]) {
  void* x_parameters = args[0];
  void* tmp0;
  if (x_parameters == &Nil) {
    tmp0 = &literal_string_;
  } else {
    tmp0 = A2(&elm_core_Basics_append,
        &literal_string__3f,
        A2(&elm_core_String_join,
            &literal_string__26,
            A2(&elm_core_List_map, &elm_url_Url_Builder_toQueryPair, x_parameters)));
  };
  return tmp0;
}
Closure elm_url_Url_Builder_toQuery = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_url_Url_Builder_toQuery,
};
void* eval_elm_url_Url_Builder_crossOrigin(void* args[]) {
  void* x_prePath = args[0];
  void* x_pathSegments = args[1];
  void* x_parameters = args[2];
  return A2(&elm_core_Basics_append,
      x_prePath,
      A2(&elm_core_Basics_append,
          &literal_string__2f,
          A2(&elm_core_Basics_append,
              A2(&elm_core_String_join, &literal_string__2f, x_pathSegments),
              A1(&elm_url_Url_Builder_toQuery, x_parameters))));
}
Closure elm_url_Url_Builder_crossOrigin = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_elm_url_Url_Builder_crossOrigin,
};
void* eval_author_project_Api_Endpoint_url(void* args[]) {
  void* x_paths = args[0];
  void* x_queryParams = args[1];
  return A2(&elm_core_Basics_apR,
      A3(&elm_url_Url_Builder_crossOrigin,
          &literal_string_https_3a_2f_2fconduit_2eproductionready_2eio,
          A2(&elm_core_List_cons, &literal_string_api, x_paths),
          x_queryParams),
      &author_project_Api_Endpoint_Endpoint);
}
Closure author_project_Api_Endpoint_url = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Api_Endpoint_url,
};
void* eval_author_project_Api_Endpoint_article(void* args[]) {
  void* x_slug = args[0];
  return A2(&author_project_Api_Endpoint_url,
      List_fromArray(2,
          ((void* []){
              &literal_string_articles,
              A1(&author_project_Article_Slug_toString, x_slug),
          })),
      &Nil);
}
Closure author_project_Api_Endpoint_article = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Api_Endpoint_article,
};

void* eval_author_project_Article_Article(void* args[]) {
  return ctorCustom(CTOR_Article, 2, args);
}
Closure author_project_Article_Article = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Article_Article,
};

void* eval_author_project_Article_Full(void* args[]) {
  return ctorCustom(CTOR_Full, 1, args);
}
Closure author_project_Article_Full = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Article_Full,
};

void* eval_author_project_Article_Body_Body(void* args[]) {
  return ctorCustom(CTOR_Body, 1, args);
}
Closure author_project_Article_Body_Body = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Article_Body_Body,
};
#define author_project_Article_Body_decoder (*ptr_author_project_Article_Body_decoder)
ElmValue* ptr_author_project_Article_Body_decoder;
void* init_author_project_Article_Body_decoder() {
  return A2(&elm_json_Json_Decode_map,
      &author_project_Article_Body_Body,
      &elm_json_Json_Decode_string);
}

void* eval_author_project_Article_Internals(void* args[]) {
  void* x_slug = args[0];
  void* x_author = args[1];
  void* x_metadata = args[2];
  return NEW_RECORD(&fg_author_metadata_slug,
      3,
      ((void* []){
          x_author,
          x_metadata,
          x_slug,
      }));
}
Closure author_project_Article_Internals = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_author_project_Article_Internals,
};

void* eval_author_project_Article_Slug_Slug(void* args[]) {
  return ctorCustom(CTOR_Slug, 1, args);
}
Closure author_project_Article_Slug_Slug = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Article_Slug_Slug,
};
#define author_project_Article_Slug_decoder (*ptr_author_project_Article_Slug_decoder)
ElmValue* ptr_author_project_Article_Slug_decoder;
void* init_author_project_Article_Slug_decoder() {
  return A2(&elm_json_Json_Decode_map,
      &author_project_Article_Slug_Slug,
      &elm_json_Json_Decode_string);
}

#define elm_json_Json_Decode_andThen Json_andThen

void* eval_author_project_Author_IsNotFollowing(void* args[]) {
  return ctorCustom(CTOR_IsNotFollowing, 1, args);
}
Closure author_project_Author_IsNotFollowing = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Author_IsNotFollowing,
};

void* eval_author_project_Author_IsViewer(void* args[]) {
  return ctorCustom(CTOR_IsViewer, 2, args);
}
Closure author_project_Author_IsViewer = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Author_IsViewer,
};

void* eval_author_project_Author_UnfollowedAuthor(void* args[]) {
  return ctorCustom(CTOR_UnfollowedAuthor, 2, args);
}
Closure author_project_Author_UnfollowedAuthor = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Author_UnfollowedAuthor,
};

#define elm_core_Basics_eq Utils_equal

Custom elm_core_Basics_False = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_False,
};

void* eval_author_project_Author_FollowedAuthor(void* args[]) {
  return ctorCustom(CTOR_FollowedAuthor, 2, args);
}
Closure author_project_Author_FollowedAuthor = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Author_FollowedAuthor,
};

void* eval_author_project_Author_IsFollowing(void* args[]) {
  return ctorCustom(CTOR_IsFollowing, 1, args);
}
Closure author_project_Author_IsFollowing = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Author_IsFollowing,
};
void* eval_author_project_Author_authorFromFollowing(void* args[]) {
  void* x_prof = args[0];
  void* x_uname = args[1];
  void* x_isFollowing = args[2];
  void* tmp0;
  if (x_isFollowing == &True) {
    tmp0 = A1(&author_project_Author_IsFollowing,
        A2(&author_project_Author_FollowedAuthor, x_uname, x_prof));
  } else {
    tmp0 = A1(&author_project_Author_IsNotFollowing,
        A2(&author_project_Author_UnfollowedAuthor, x_uname, x_prof));
  };
  return tmp0;
}
Closure author_project_Author_authorFromFollowing = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_author_project_Author_authorFromFollowing,
};

#define elm_json_Json_Decode_bool Json_decodeBool

#define elm_core_String_all String_all

#define elm_core_Basics_and Basics_and

#define elm_json_Json_Encode_encode Json_encode

#define elm_core_String_fromInt String_fromNumber

void* eval_elm_core_String_split(void* args[]) {
  void* x_sep = args[0];
  void* x_string = args[1];
  return A1(&List_fromArray, A2(&String_split, x_sep, x_string));
}
Closure elm_core_String_split = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_core_String_split,
};
void* eval_elm_json_Json_Decode_indent(void* args[]) {
  void* x_str = args[0];
  return A2(&elm_core_String_join,
      &literal_string__5cn_20_20_20_20,
      A2(&elm_core_String_split, &literal_string__5cn, x_str));
}
Closure elm_json_Json_Decode_indent = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_json_Json_Decode_indent,
};

void* eval_elm_core_List_length_lambda0(void* args[]) {
  void* x__v0 = args[0];
  void* x_i = args[1];
  return A2(&elm_core_Basics_add, x_i, &literal_int_1);
}
void* eval_elm_core_List_length(void* args[]) {
  void* x_xs = args[0];
  return A3(&elm_core_List_foldl,
      NEW_CLOSURE(0, 2, &eval_elm_core_List_length_lambda0, ((void* []){})),
      &literal_int_0,
      x_xs);
}
Closure elm_core_List_length = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_core_List_length,
};

#define elm_core_List_map2 List_map2

#define elm_core_Basics_le Utils_le

#define elm_core_Basics_sub Basics_sub
void* tce_elm_core_List_rangeHelp(void* args[], void** gc_tce_data) {
tce_loop:;
  void* x_lo = args[0];
  void* x_hi = args[1];
  void* x_list = args[2];
  void* tmp0;
  if (A2(&elm_core_Basics_le, x_lo, x_hi) == &True) {
    void* tmp1 = x_lo;
    void* tmp2 = A2(&elm_core_Basics_sub, x_hi, &literal_int_1);
    void* tmp3 = A2(&elm_core_List_cons, x_hi, x_list);
    *gc_tce_data = CAN_THROW(GC_tce_iteration(3));
    args[2] = tmp3;
    args[1] = tmp2;
    args[0] = tmp1;
    goto tce_loop;
    tmp0 = NULL;
  } else {
    tmp0 = x_list;
  };
  return tmp0;
}
void* eval_elm_core_List_rangeHelp() {
  return GC_tce_eval(
      &tce_elm_core_List_rangeHelp, &eval_elm_core_List_rangeHelp, 3, args);
}
Closure elm_core_List_rangeHelp = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_elm_core_List_rangeHelp,
};
void* eval_elm_core_List_range(void* args[]) {
  void* x_lo = args[0];
  void* x_hi = args[1];
  return A3(&elm_core_List_rangeHelp, x_lo, x_hi, &Nil);
}
Closure elm_core_List_range = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_core_List_range,
};
void* eval_elm_core_List_indexedMap(void* args[]) {
  void* x_f = args[0];
  void* x_xs = args[1];
  return A3(&elm_core_List_map2,
      x_f,
      A2(&elm_core_List_range,
          &literal_int_0,
          A2(&elm_core_Basics_sub, A1(&elm_core_List_length, x_xs), &literal_int_1)),
      x_xs);
}
Closure elm_core_List_indexedMap = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_core_List_indexedMap,
};

#define elm_core_Char_toCode Char_toCode
void* eval_elm_core_Char_isLower(void* args[]) {
  void* x_char = args[0];
  void* x_code = A1(&elm_core_Char_toCode, x_char);
  return A2(&elm_core_Basics_and,
      A2(&elm_core_Basics_le, &literal_int_97, x_code),
      A2(&elm_core_Basics_le, x_code, &literal_int_122));
}
Closure elm_core_Char_isLower = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_core_Char_isLower,
};

void* eval_elm_core_Char_isUpper(void* args[]) {
  void* x_char = args[0];
  void* x_code = A1(&elm_core_Char_toCode, x_char);
  return A2(&elm_core_Basics_and,
      A2(&elm_core_Basics_le, x_code, &literal_int_90),
      A2(&elm_core_Basics_le, &literal_int_65, x_code));
}
Closure elm_core_Char_isUpper = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_core_Char_isUpper,
};

#define elm_core_Basics_or Basics_or
void* eval_elm_core_Char_isAlpha(void* args[]) {
  void* x_char = args[0];
  return A2(&elm_core_Basics_or,
      A1(&elm_core_Char_isLower, x_char),
      A1(&elm_core_Char_isUpper, x_char));
}
Closure elm_core_Char_isAlpha = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_core_Char_isAlpha,
};

void* eval_elm_core_Char_isDigit(void* args[]) {
  void* x_char = args[0];
  void* x_code = A1(&elm_core_Char_toCode, x_char);
  return A2(&elm_core_Basics_and,
      A2(&elm_core_Basics_le, x_code, &literal_int_57),
      A2(&elm_core_Basics_le, &literal_int_48, x_code));
}
Closure elm_core_Char_isDigit = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_core_Char_isDigit,
};
void* eval_elm_core_Char_isAlphaNum(void* args[]) {
  void* x_char = args[0];
  return A2(&elm_core_Basics_or,
      A1(&elm_core_Char_isLower, x_char),
      A2(&elm_core_Basics_or,
          A1(&elm_core_Char_isUpper, x_char),
          A1(&elm_core_Char_isDigit, x_char)));
}
Closure elm_core_Char_isAlphaNum = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_core_Char_isAlphaNum,
};

#define elm_core_String_uncons String_uncons
void* eval_elm_json_Json_Decode_errorOneOf(void* args[]) {
  void* x_i = args[0];
  void* x_error = args[1];
  return A2(&elm_core_Basics_append,
      &literal_string__5cn_5cn_28,
      A2(&elm_core_Basics_append,
          A1(&elm_core_String_fromInt, A2(&elm_core_Basics_add, x_i, &literal_int_1)),
          A2(&elm_core_Basics_append,
              &literal_string__29_20,
              A1(&elm_json_Json_Decode_indent,
                  A1(&elm_json_Json_Decode_errorToString, x_error)))));
}
Closure elm_json_Json_Decode_errorOneOf = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_json_Json_Decode_errorOneOf,
};
void* eval_elm_json_Json_Decode_errorToString(void* args[]) {
  void* x_error = args[0];
  return A2(&elm_json_Json_Decode_errorToStringHelp, x_error, &Nil);
}
Closure elm_json_Json_Decode_errorToString = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_json_Json_Decode_errorToString,
};
void* tce_elm_json_Json_Decode_errorToStringHelp(void* args[], void** gc_tce_data) {
tce_loop:;
  void* x_error = args[0];
  void* x_context = args[1];
  void* tmp0;
  void* tmp1 = ((Custom*)x_error)->ctor;
  if (tmp1 == CTOR_Field) {
    {
      void* x_f = Utils_destruct_index(x_error, 0);
      void* x_err = Utils_destruct_index(x_error, 1);
      void* x__v1 = A1(&elm_core_String_uncons, x_f);
      void* tmp10;
      void* tmp11 = ((Custom*)x__v1)->ctor;
      if (tmp11 == CTOR_Nothing) {
        tmp10 = &False;
      } else {
        void* x__v2 = Utils_destruct_index(x__v1, 0);
        void* x_char = Utils_destruct_index(x__v2, 0);
        void* x_rest = Utils_destruct_index(x__v2, 1);
        tmp10 = A2(&elm_core_Basics_and,
            A1(&elm_core_Char_isAlpha, x_char),
            A2(&elm_core_String_all, &elm_core_Char_isAlphaNum, x_rest));
      };
      void* x_isSimple = tmp10;
      void* tmp12;
      if (x_isSimple == &True) {
        tmp12 = A2(&elm_core_Basics_append, &literal_string__2e, x_f);
      } else {
        tmp12 = A2(&elm_core_Basics_append,
            &literal_string__5b_5c_27,
            A2(&elm_core_Basics_append, x_f, &literal_string__5c_27_5d));
      };
      void* x_fieldName = tmp12;
      void* tmp13 = x_err;
      void* tmp14 = A2(&elm_core_List_cons, x_fieldName, x_context);
      *gc_tce_data = CAN_THROW(GC_tce_iteration(2));
      args[1] = tmp14;
      args[0] = tmp13;
      goto tce_loop;
      tmp0 = NULL;
    };
  } else if (tmp1 == CTOR_Index) {
    {
      void* x_i = Utils_destruct_index(x_error, 0);
      void* x_err = Utils_destruct_index(x_error, 1);
      void* x_indexName = A2(&elm_core_Basics_append,
          &literal_string__5b,
          A2(&elm_core_Basics_append,
              A1(&elm_core_String_fromInt, x_i),
              &literal_string__5d));
      void* tmp8 = x_err;
      void* tmp9 = A2(&elm_core_List_cons, x_indexName, x_context);
      *gc_tce_data = CAN_THROW(GC_tce_iteration(2));
      args[1] = tmp9;
      args[0] = tmp8;
      goto tce_loop;
      tmp0 = NULL;
    };
  } else if (tmp1 == CTOR_OneOf) {
    {
      void* x_errors = Utils_destruct_index(x_error, 0);
      void* tmp3;
      if (x_errors == &Nil) {
        void* tmp4;
        if (x_context == &Nil) {
          tmp4 = &literal_string__21;
        } else {
          tmp4 = A2(&elm_core_Basics_append,
              &literal_string__20at_20json,
              A2(&elm_core_String_join,
                  &literal_string_,
                  A1(&elm_core_List_reverse, x_context)));
        };
        tmp3 = A2(&elm_core_Basics_append,
            &literal_string_Ran_20into_20a_20Json_2eDecode_2eoneOf_20with_20no_20possibilities,
            tmp4);
      } else if (Utils_destruct_index(x_errors, 1) == &Nil) {
        void* x_err = Utils_destruct_index(x_errors, 0);
        void* tmp5 = x_err;
        void* tmp6 = x_context;
        *gc_tce_data = CAN_THROW(GC_tce_iteration(2));
        args[1] = tmp6;
        args[0] = tmp5;
        goto tce_loop;
        tmp3 = NULL;
      } else {
        void* tmp7;
        if (x_context == &Nil) {
          tmp7 = &literal_string_Json_2eDecode_2eoneOf;
        } else {
          tmp7 = A2(&elm_core_Basics_append,
              &literal_string_The_20Json_2eDecode_2eoneOf_20at_20json,
              A2(&elm_core_String_join,
                  &literal_string_,
                  A1(&elm_core_List_reverse, x_context)));
        };
        void* x_starter = tmp7;
        void* x_introduction = A2(&elm_core_Basics_append,
            x_starter,
            A2(&elm_core_Basics_append,
                &literal_string__20failed_20in_20the_20following_20,
                A2(&elm_core_Basics_append,
                    A1(&elm_core_String_fromInt, A1(&elm_core_List_length, x_errors)),
                    &literal_string__20ways_3a)));
        tmp3 = A2(&elm_core_String_join,
            &literal_string__5cn_5cn,
            A2(&elm_core_List_cons,
                x_introduction,
                A2(&elm_core_List_indexedMap,
                    &elm_json_Json_Decode_errorOneOf,
                    x_errors)));
      };
      tmp0 = tmp3;
    };
  } else {
    void* x_msg = Utils_destruct_index(x_error, 0);
    void* x_json = Utils_destruct_index(x_error, 1);
    void* tmp2;
    if (x_context == &Nil) {
      tmp2 = &literal_string_Problem_20with_20the_20given_20value_3a_5cn_5cn;
    } else {
      tmp2 = A2(&elm_core_Basics_append,
          &literal_string_Problem_20with_20the_20value_20at_20json,
          A2(&elm_core_Basics_append,
              A2(&elm_core_String_join,
                  &literal_string_,
                  A1(&elm_core_List_reverse, x_context)),
              &literal_string__3a_5cn_5cn_20_20_20_20));
    };
    void* x_introduction = tmp2;
    tmp0 = A2(&elm_core_Basics_append,
        x_introduction,
        A2(&elm_core_Basics_append,
            A1(&elm_json_Json_Decode_indent,
                A2(&elm_json_Json_Encode_encode, &literal_int_4, x_json)),
            A2(&elm_core_Basics_append, &literal_string__5cn_5cn, x_msg)));
  };
  return tmp0;
}
void* eval_elm_json_Json_Decode_errorToStringHelp() {
  return GC_tce_eval(&tce_elm_json_Json_Decode_errorToStringHelp,
      &eval_elm_json_Json_Decode_errorToStringHelp,
      2,
      args);
}
Closure elm_json_Json_Decode_errorToStringHelp = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_json_Json_Decode_errorToStringHelp,
};

#define elm_json_Json_Decode_fail Json_fail

#define elm_json_Json_Decode_value Json_decodeValue
void* eval_NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_optionalDecoder_lambda0(
    void* args[]) {
  void* x_fallback = args[0];
  void* x_decoder = args[1];
  return A1(&elm_json_Json_Decode_oneOf,
      List_fromArray(2,
          ((void* []){
              x_decoder,
              A1(&elm_json_Json_Decode_null, x_fallback),
          })));
}
void* eval_NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_optionalDecoder_lambda1(
    void* args[]) {
  void* x_fallback = args[0];
  void* x_nullOr = args[1];
  void* x_pathDecoder = args[2];
  void* x_valDecoder = args[3];
  void* x_input = args[4];
  void* x__v0 = A2(&elm_json_Json_Decode_decodeValue, x_pathDecoder, x_input);
  void* tmp2;
  void* tmp3 = ((Custom*)x__v0)->ctor;
  if (tmp3 == CTOR_Ok) {
    void* x_rawValue = Utils_destruct_index(x__v0, 0);
    void* x__v1 =
        A2(&elm_json_Json_Decode_decodeValue, A1(x_nullOr, x_valDecoder), x_rawValue);
    void* tmp4;
    void* tmp5 = ((Custom*)x__v1)->ctor;
    if (tmp5 == CTOR_Ok) {
      void* x_finalResult = Utils_destruct_index(x__v1, 0);
      tmp4 = A1(&elm_json_Json_Decode_succeed, x_finalResult);
    } else {
      void* x_finalErr = Utils_destruct_index(x__v1, 0);
      tmp4 = A1(&elm_json_Json_Decode_fail,
          A1(&elm_json_Json_Decode_errorToString, x_finalErr));
    };
    tmp2 = tmp4;
  } else {
    tmp2 = A1(&elm_json_Json_Decode_succeed, x_fallback);
  };
  return tmp2;
}
void* eval_NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_optionalDecoder(
    void* args[]) {
  void* x_pathDecoder = args[0];
  void* x_valDecoder = args[1];
  void* x_fallback = args[2];
  void* x_nullOr = NEW_CLOSURE(1,
      2,
      &eval_NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_optionalDecoder_lambda0,
      ((void* []){
          x_fallback,
      }));
  void* x_handleResult = NEW_CLOSURE(4,
      5,
      &eval_NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_optionalDecoder_lambda1,
      ((void* []){
          x_fallback,
          x_nullOr,
          x_pathDecoder,
          x_valDecoder,
      }));
  return A2(&elm_core_Basics_apR,
      &elm_json_Json_Decode_value,
      A1(&elm_json_Json_Decode_andThen, x_handleResult));
}
Closure NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_optionalDecoder = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator =
        &eval_NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_optionalDecoder,
};
void* eval_NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_optional(void* args[]) {
  void* x_key = args[0];
  void* x_valDecoder = args[1];
  void* x_fallback = args[2];
  void* x_decoder = args[3];
  return A2(&NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_custom,
      A3(&NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_optionalDecoder,
          A2(&elm_json_Json_Decode_field, x_key, &elm_json_Json_Decode_value),
          x_valDecoder,
          x_fallback),
      x_decoder);
}
Closure NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_optional = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x4,
    .evaluator = &eval_NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_optional,
};
void* eval_author_project_Author_nonViewerDecoder(void* args[]) {
  void* x_prof = args[0];
  void* x_uname = args[1];
  return A2(&elm_core_Basics_apR,
      A1(&elm_json_Json_Decode_succeed,
          A2(&author_project_Author_authorFromFollowing, x_prof, x_uname)),
      A3(&NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_optional,
          &literal_string_following,
          &elm_json_Json_Decode_bool,
          &False));
}
Closure author_project_Author_nonViewerDecoder = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Author_nonViewerDecoder,
};

void* eval_author_project_Api_username(void* args[]) {
  void* x__v0 = args[0];
  void* x_val = Utils_destruct_index(x__v0, 0);
  return x_val;
}
Closure author_project_Api_username = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Api_username,
};
void* eval_author_project_Author_decodeFromPair(void* args[]) {
  void* x_maybeCred = args[0];
  void* x__v0 = args[1];
  void* x_prof = Utils_destruct_index(x__v0, 0);
  void* x_uname = Utils_destruct_index(x__v0, 1);
  void* tmp0;
  void* tmp1 = ((Custom*)x_maybeCred)->ctor;
  if (tmp1 == CTOR_Nothing) {
    tmp0 = A1(&elm_json_Json_Decode_succeed,
        A1(&author_project_Author_IsNotFollowing,
            A2(&author_project_Author_UnfollowedAuthor, x_uname, x_prof)));
  } else {
    void* x_cred = Utils_destruct_index(x_maybeCred, 0);
    void* tmp2;
    if (A2(&elm_core_Basics_eq, x_uname, A1(&author_project_Api_username, x_cred)) ==
        &True) {
      tmp2 = A1(&elm_json_Json_Decode_succeed,
          A2(&author_project_Author_IsViewer, x_cred, x_prof));
    } else {
      tmp2 = A2(&author_project_Author_nonViewerDecoder, x_prof, x_uname);
    };
    tmp0 = tmp2;
  };
  return tmp0;
}
Closure author_project_Author_decodeFromPair = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Author_decodeFromPair,
};

void* eval_author_project_Profile_Internals(void* args[]) {
  void* x_bio = args[0];
  void* x_avatar = args[1];
  return NEW_RECORD(&fg_avatar_bio,
      2,
      ((void* []){
          x_avatar,
          x_bio,
      }));
}
Closure author_project_Profile_Internals = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Profile_Internals,
};

void* eval_author_project_Profile_Profile(void* args[]) {
  return ctorCustom(CTOR_Profile, 1, args);
}
Closure author_project_Profile_Profile = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Profile_Profile,
};
#define author_project_Profile_decoder (*ptr_author_project_Profile_decoder)
ElmValue* ptr_author_project_Profile_decoder;
void* init_author_project_Profile_decoder() {
  return A2(&elm_core_Basics_apR,
      A2(&elm_core_Basics_apR,
          A2(&elm_core_Basics_apR,
              A1(&elm_json_Json_Decode_succeed, &author_project_Profile_Internals),
              A2(&NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_required,
                  &literal_string_bio,
                  A1(&elm_json_Json_Decode_nullable, &elm_json_Json_Decode_string))),
          A2(&NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_required,
              &literal_string_image,
              &author_project_Avatar_decoder)),
      A1(&elm_json_Json_Decode_map, &author_project_Profile_Profile));
}

void* eval_elm_core_Tuple_pair(void* args[]) {
  void* x_a = args[0];
  void* x_b = args[1];
  return NEW_TUPLE2(x_a, x_b);
}
Closure elm_core_Tuple_pair = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_core_Tuple_pair,
};
void* eval_author_project_Author_decoder(void* args[]) {
  void* x_maybeCred = args[0];
  return A2(&elm_core_Basics_apR,
      A2(&elm_core_Basics_apR,
          A2(&elm_core_Basics_apR,
              A1(&elm_json_Json_Decode_succeed, &elm_core_Tuple_pair),
              A1(&NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_custom,
                  &author_project_Profile_decoder)),
          A2(&NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_required,
              &literal_string_username,
              &author_project_Username_decoder)),
      A1(&elm_json_Json_Decode_andThen,
          A1(&author_project_Author_decodeFromPair, x_maybeCred)));
}
Closure author_project_Author_decoder = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Author_decoder,
};

void* eval_author_project_Article_Metadata(void* args[]) {
  void* x_description = args[0];
  void* x_title = args[1];
  void* x_tags = args[2];
  void* x_createdAt = args[3];
  void* x_favorited = args[4];
  void* x_favoritesCount = args[5];
  return NEW_RECORD(&fg_createdAt_description_favorited_favoritesCount_tags_title,
      6,
      ((void* []){
          x_createdAt,
          x_description,
          x_favorited,
          x_favoritesCount,
          x_tags,
          x_title,
      }));
}
Closure author_project_Article_Metadata = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x6,
    .evaluator = &eval_author_project_Article_Metadata,
};

void* eval_elm_core_Basics_apL(void* args[]) {
  void* x_f = args[0];
  void* x_x = args[1];
  return A1(x_f, x_x);
}
Closure elm_core_Basics_apL = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_core_Basics_apL,
};

void* eval_elm_parser_Parser_deadEndsToString(void* args[]) {
  void* x_deadEnds = args[0];
  return &literal_string_TODO_20deadEndsToString;
}
Closure elm_parser_Parser_deadEndsToString = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_parser_Parser_deadEndsToString,
};

void* eval_elm_parser_Parser_Advanced_Bad(void* args[]) {
  return ctorCustom(CTOR_Bad, 2, args);
}
Closure elm_parser_Parser_Advanced_Bad = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_parser_Parser_Advanced_Bad,
};

void* eval_elm_parser_Parser_Advanced_Good(void* args[]) {
  return ctorCustom(CTOR_Good, 3, args);
}
Closure elm_parser_Parser_Advanced_Good = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_elm_parser_Parser_Advanced_Good,
};

void* eval_elm_parser_Parser_Advanced_Parser(void* args[]) {
  return ctorCustom(CTOR_Parser, 1, args);
}
Closure elm_parser_Parser_Advanced_Parser = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_parser_Parser_Advanced_Parser,
};
void* eval_elm_parser_Parser_Advanced_andThen_lambda0(void* args[]) {
  void* x_callback = args[0];
  void* x_parseA = args[1];
  void* x_s0 = args[2];
  void* x__v1 = A1(x_parseA, x_s0);
  void* tmp1;
  void* tmp2 = ((Custom*)x__v1)->ctor;
  if (tmp2 == CTOR_Bad) {
    void* x_p = Utils_destruct_index(x__v1, 0);
    void* x_x = Utils_destruct_index(x__v1, 1);
    tmp1 = A2(&elm_parser_Parser_Advanced_Bad, x_p, x_x);
  } else {
    void* x_p1 = Utils_destruct_index(x__v1, 0);
    void* x_a = Utils_destruct_index(x__v1, 1);
    void* x_s1 = Utils_destruct_index(x__v1, 2);
    void* x__v2 = A1(x_callback, x_a);
    void* x_parseB = ((Custom*)x__v2)->values[0];
    void* x__v3 = A1(x_parseB, x_s1);
    void* tmp3;
    void* tmp4 = ((Custom*)x__v3)->ctor;
    if (tmp4 == CTOR_Bad) {
      void* x_p2 = Utils_destruct_index(x__v3, 0);
      void* x_x = Utils_destruct_index(x__v3, 1);
      tmp3 =
          A2(&elm_parser_Parser_Advanced_Bad, A2(&elm_core_Basics_or, x_p1, x_p2), x_x);
    } else {
      void* x_p2 = Utils_destruct_index(x__v3, 0);
      void* x_b = Utils_destruct_index(x__v3, 1);
      void* x_s2 = Utils_destruct_index(x__v3, 2);
      tmp3 = A3(&elm_parser_Parser_Advanced_Good,
          A2(&elm_core_Basics_or, x_p1, x_p2),
          x_b,
          x_s2);
    };
    tmp1 = tmp3;
  };
  return tmp1;
}
void* eval_elm_parser_Parser_Advanced_andThen(void* args[]) {
  void* x_callback = args[0];
  void* x__v0 = args[1];
  void* x_parseA = ((Custom*)x__v0)->values[0];
  return A2(&elm_core_Basics_apL,
      &elm_parser_Parser_Advanced_Parser,
      NEW_CLOSURE(2,
          3,
          &eval_elm_parser_Parser_Advanced_andThen_lambda0,
          ((void* []){
              x_callback,
              x_parseA,
          })));
}
Closure elm_parser_Parser_Advanced_andThen = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_parser_Parser_Advanced_andThen,
};
#define elm_parser_Parser_andThen elm_parser_Parser_Advanced_andThen

Custom elm_parser_Parser_ExpectingEnd = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_ExpectingEnd,
};

void* eval_elm_parser_Parser_Advanced_AddRight(void* args[]) {
  return ctorCustom(CTOR_AddRight, 2, args);
}
Closure elm_parser_Parser_Advanced_AddRight = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_parser_Parser_Advanced_AddRight,
};

Custom elm_parser_Parser_Advanced_Empty = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Empty,
};

void* eval_elm_parser_Parser_Advanced_Problem(void* args[]) {
  void* x_row = args[0];
  void* x_col = args[1];
  void* x_problem = args[2];
  void* x_contextStack = args[3];
  return NEW_RECORD(&fg_col_contextStack_problem_row,
      4,
      ((void* []){
          x_col,
          x_contextStack,
          x_problem,
          x_row,
      }));
}
Closure elm_parser_Parser_Advanced_Problem = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x4,
    .evaluator = &eval_elm_parser_Parser_Advanced_Problem,
};
void* eval_elm_parser_Parser_Advanced_fromState(void* args[]) {
  void* x_s = args[0];
  void* x_x = args[1];
  return A2(&elm_parser_Parser_Advanced_AddRight,
      &elm_parser_Parser_Advanced_Empty,
      A4(&elm_parser_Parser_Advanced_Problem,
          Utils_access_eval(((void* []){
              (void*)FIELD_row,
              x_s,
          })),
          Utils_access_eval(((void* []){
              (void*)FIELD_col,
              x_s,
          })),
          x_x,
          Utils_access_eval(((void* []){
              (void*)FIELD_context,
              x_s,
          }))));
}
Closure elm_parser_Parser_Advanced_fromState = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_parser_Parser_Advanced_fromState,
};

#define elm_core_String_length String_length
void* eval_elm_parser_Parser_Advanced_end_lambda0(void* args[]) {
  void* x_x = args[0];
  void* x_s = args[1];
  void* tmp1;
  if (A2(&elm_core_Basics_eq,
          A1(&elm_core_String_length,
              Utils_access_eval(((void* []){
                  (void*)FIELD_src,
                  x_s,
              }))),
          Utils_access_eval(((void* []){
              (void*)FIELD_offset,
              x_s,
          }))) == &True) {
    tmp1 = A3(&elm_parser_Parser_Advanced_Good, &False, &Unit, x_s);
  } else {
    tmp1 = A2(&elm_parser_Parser_Advanced_Bad,
        &False,
        A2(&elm_parser_Parser_Advanced_fromState, x_s, x_x));
  };
  return tmp1;
}
void* eval_elm_parser_Parser_Advanced_end(void* args[]) {
  void* x_x = args[0];
  return A2(&elm_core_Basics_apL,
      &elm_parser_Parser_Advanced_Parser,
      NEW_CLOSURE(1,
          2,
          &eval_elm_parser_Parser_Advanced_end_lambda0,
          ((void* []){
              x_x,
          })));
}
Closure elm_parser_Parser_Advanced_end = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_parser_Parser_Advanced_end,
};
#define elm_parser_Parser_end (*ptr_elm_parser_Parser_end)
ElmValue* ptr_elm_parser_Parser_end;
void* init_elm_parser_Parser_end() {
  return A1(&elm_parser_Parser_Advanced_end, &elm_parser_Parser_ExpectingEnd);
}

void* eval_elm_time_Time_Posix(void* args[]) {
  return ctorCustom(CTOR_Posix, 1, args);
}
Closure elm_time_Time_Posix = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_time_Time_Posix,
};
#define elm_time_Time_millisToPosix elm_time_Time_Posix

#define elm_core_Basics_mul Basics_mul
void* eval_rtfeldman_elm_iso8601_date_strings_Iso8601_fromParts(void* args[]) {
  void* x_monthYearDayMs = args[0];
  void* x_hour = args[1];
  void* x_minute = args[2];
  void* x_second = args[3];
  void* x_ms = args[4];
  void* x_utcOffsetMinutes = args[5];
  return A1(&elm_time_Time_millisToPosix,
      A2(&elm_core_Basics_add,
          A2(&elm_core_Basics_add,
              A2(&elm_core_Basics_add,
                  A2(&elm_core_Basics_add,
                      x_monthYearDayMs,
                      A2(&elm_core_Basics_mul,
                          A2(&elm_core_Basics_mul,
                              A2(&elm_core_Basics_mul, x_hour, &literal_int_60),
                              &literal_int_60),
                          &literal_int_1000)),
                  A2(&elm_core_Basics_mul,
                      A2(&elm_core_Basics_mul,
                          A2(&elm_core_Basics_sub, x_minute, x_utcOffsetMinutes),
                          &literal_int_60),
                      &literal_int_1000)),
              A2(&elm_core_Basics_mul, x_second, &literal_int_1000)),
          x_ms));
}
Closure rtfeldman_elm_iso8601_date_strings_Iso8601_fromParts = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x6,
    .evaluator = &eval_rtfeldman_elm_iso8601_date_strings_Iso8601_fromParts,
};

void* eval_elm_core_Basics_always(void* args[]) {
  void* x_a = args[0];
  void* x__v0 = args[1];
  return x_a;
}
Closure elm_core_Basics_always = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_core_Basics_always,
};

void* eval_elm_parser_Parser_Advanced_map2_lambda0(void* args[]) {
  void* x_func = args[0];
  void* x_parseA = args[1];
  void* x_parseB = args[2];
  void* x_s0 = args[3];
  void* x__v2 = A1(x_parseA, x_s0);
  void* tmp1;
  void* tmp2 = ((Custom*)x__v2)->ctor;
  if (tmp2 == CTOR_Bad) {
    void* x_p = Utils_destruct_index(x__v2, 0);
    void* x_x = Utils_destruct_index(x__v2, 1);
    tmp1 = A2(&elm_parser_Parser_Advanced_Bad, x_p, x_x);
  } else {
    void* x_p1 = Utils_destruct_index(x__v2, 0);
    void* x_a = Utils_destruct_index(x__v2, 1);
    void* x_s1 = Utils_destruct_index(x__v2, 2);
    void* x__v3 = A1(x_parseB, x_s1);
    void* tmp3;
    void* tmp4 = ((Custom*)x__v3)->ctor;
    if (tmp4 == CTOR_Bad) {
      void* x_p2 = Utils_destruct_index(x__v3, 0);
      void* x_x = Utils_destruct_index(x__v3, 1);
      tmp3 =
          A2(&elm_parser_Parser_Advanced_Bad, A2(&elm_core_Basics_or, x_p1, x_p2), x_x);
    } else {
      void* x_p2 = Utils_destruct_index(x__v3, 0);
      void* x_b = Utils_destruct_index(x__v3, 1);
      void* x_s2 = Utils_destruct_index(x__v3, 2);
      tmp3 = A3(&elm_parser_Parser_Advanced_Good,
          A2(&elm_core_Basics_or, x_p1, x_p2),
          A2(x_func, x_a, x_b),
          x_s2);
    };
    tmp1 = tmp3;
  };
  return tmp1;
}
void* eval_elm_parser_Parser_Advanced_map2(void* args[]) {
  void* x_func = args[0];
  void* x__v0 = args[1];
  void* x__v1 = args[2];
  void* x_parseA = ((Custom*)x__v0)->values[0];
  void* x_parseB = ((Custom*)x__v1)->values[0];
  return A2(&elm_core_Basics_apL,
      &elm_parser_Parser_Advanced_Parser,
      NEW_CLOSURE(3,
          4,
          &eval_elm_parser_Parser_Advanced_map2_lambda0,
          ((void* []){
              x_func,
              x_parseA,
              x_parseB,
          })));
}
Closure elm_parser_Parser_Advanced_map2 = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_elm_parser_Parser_Advanced_map2,
};
void* eval_elm_parser_Parser_Advanced_ignorer(void* args[]) {
  void* x_keepParser = args[0];
  void* x_ignoreParser = args[1];
  return A3(&elm_parser_Parser_Advanced_map2,
      &elm_core_Basics_always,
      x_keepParser,
      x_ignoreParser);
}
Closure elm_parser_Parser_Advanced_ignorer = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_parser_Parser_Advanced_ignorer,
};
#define elm_parser_Parser_ignorer elm_parser_Parser_Advanced_ignorer

void* eval_elm_parser_Parser_Advanced_keeper(void* args[]) {
  void* x_parseFunc = args[0];
  void* x_parseArg = args[1];
  return A3(
      &elm_parser_Parser_Advanced_map2, &elm_core_Basics_apL, x_parseFunc, x_parseArg);
}
Closure elm_parser_Parser_Advanced_keeper = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_parser_Parser_Advanced_keeper,
};
#define elm_parser_Parser_keeper elm_parser_Parser_Advanced_keeper

void* eval_elm_parser_Parser_Advanced_map_lambda0(void* args[]) {
  void* x_func = args[0];
  void* x_parse = args[1];
  void* x_s0 = args[2];
  void* x__v1 = A1(x_parse, x_s0);
  void* tmp1;
  void* tmp2 = ((Custom*)x__v1)->ctor;
  if (tmp2 == CTOR_Good) {
    void* x_p = Utils_destruct_index(x__v1, 0);
    void* x_a = Utils_destruct_index(x__v1, 1);
    void* x_s1 = Utils_destruct_index(x__v1, 2);
    tmp1 = A3(&elm_parser_Parser_Advanced_Good, x_p, A1(x_func, x_a), x_s1);
  } else {
    void* x_p = Utils_destruct_index(x__v1, 0);
    void* x_x = Utils_destruct_index(x__v1, 1);
    tmp1 = A2(&elm_parser_Parser_Advanced_Bad, x_p, x_x);
  };
  return tmp1;
}
void* eval_elm_parser_Parser_Advanced_map(void* args[]) {
  void* x_func = args[0];
  void* x__v0 = args[1];
  void* x_parse = ((Custom*)x__v0)->values[0];
  return A2(&elm_core_Basics_apL,
      &elm_parser_Parser_Advanced_Parser,
      NEW_CLOSURE(2,
          3,
          &eval_elm_parser_Parser_Advanced_map_lambda0,
          ((void* []){
              x_func,
              x_parse,
          })));
}
Closure elm_parser_Parser_Advanced_map = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_parser_Parser_Advanced_map,
};
#define elm_parser_Parser_map elm_parser_Parser_Advanced_map

#define elm_parser_Parser_Advanced_isSubChar Parser_isSubChar

#define elm_core_Basics_lt Utils_lt

void* eval_elm_core_Basics_negate(void* args[]) {
  void* x_n = args[0];
  return A1(&elm_core_Basics_negate, x_n);
}
Closure elm_core_Basics_negate = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_core_Basics_negate,
};
void* tce_elm_parser_Parser_Advanced_chompWhileHelp(void* args[], void** gc_tce_data) {
tce_loop:;
  void* x_isGood = args[0];
  void* x_offset = args[1];
  void* x_row = args[2];
  void* x_col = args[3];
  void* x_s0 = args[4];
  void* x_newOffset = A3(&elm_parser_Parser_Advanced_isSubChar,
      x_isGood,
      x_offset,
      Utils_access_eval(((void* []){
          (void*)FIELD_src,
          x_s0,
      })));
  void* tmp0;
  if (A2(&elm_core_Basics_eq, x_newOffset, A1(&elm_core_Basics_negate, &literal_int_1)) ==
      &True) {
    tmp0 = A3(&elm_parser_Parser_Advanced_Good,
        A2(&elm_core_Basics_lt,
            Utils_access_eval(((void* []){
                (void*)FIELD_offset,
                x_s0,
            })),
            x_offset),
        &Unit,
        NEW_RECORD(&fg_col_context_indent_offset_row_src,
            6,
            ((void* []){
                x_col,
                Utils_access_eval(((void* []){
                    (void*)FIELD_context,
                    x_s0,
                })),
                Utils_access_eval(((void* []){
                    (void*)FIELD_indent,
                    x_s0,
                })),
                x_offset,
                x_row,
                Utils_access_eval(((void* []){
                    (void*)FIELD_src,
                    x_s0,
                })),
            })));
  } else if (A2(&elm_core_Basics_eq,
                 x_newOffset,
                 A1(&elm_core_Basics_negate, &literal_int_2)) == &True) {
    void* tmp6 = x_isGood;
    void* tmp7 = A2(&elm_core_Basics_add, x_offset, &literal_int_1);
    void* tmp8 = A2(&elm_core_Basics_add, x_row, &literal_int_1);
    void* tmp9 = &literal_int_1;
    void* tmp10 = x_s0;
    *gc_tce_data = CAN_THROW(GC_tce_iteration(5));
    args[4] = tmp10;
    args[3] = tmp9;
    args[2] = tmp8;
    args[1] = tmp7;
    args[0] = tmp6;
    goto tce_loop;
    tmp0 = NULL;
  } else {
    void* tmp1 = x_isGood;
    void* tmp2 = x_newOffset;
    void* tmp3 = x_row;
    void* tmp4 = A2(&elm_core_Basics_add, x_col, &literal_int_1);
    void* tmp5 = x_s0;
    *gc_tce_data = CAN_THROW(GC_tce_iteration(5));
    args[4] = tmp5;
    args[3] = tmp4;
    args[2] = tmp3;
    args[1] = tmp2;
    args[0] = tmp1;
    goto tce_loop;
    tmp0 = NULL;
  };
  return tmp0;
}
void* eval_elm_parser_Parser_Advanced_chompWhileHelp() {
  return GC_tce_eval(&tce_elm_parser_Parser_Advanced_chompWhileHelp,
      &eval_elm_parser_Parser_Advanced_chompWhileHelp,
      5,
      args);
}
Closure elm_parser_Parser_Advanced_chompWhileHelp = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x5,
    .evaluator = &eval_elm_parser_Parser_Advanced_chompWhileHelp,
};
void* eval_elm_parser_Parser_Advanced_chompWhile_lambda0(void* args[]) {
  void* x_isGood = args[0];
  void* x_s = args[1];
  return A5(&elm_parser_Parser_Advanced_chompWhileHelp,
      x_isGood,
      Utils_access_eval(((void* []){
          (void*)FIELD_offset,
          x_s,
      })),
      Utils_access_eval(((void* []){
          (void*)FIELD_row,
          x_s,
      })),
      Utils_access_eval(((void* []){
          (void*)FIELD_col,
          x_s,
      })),
      x_s);
}
void* eval_elm_parser_Parser_Advanced_chompWhile(void* args[]) {
  void* x_isGood = args[0];
  return A2(&elm_core_Basics_apL,
      &elm_parser_Parser_Advanced_Parser,
      NEW_CLOSURE(1,
          2,
          &eval_elm_parser_Parser_Advanced_chompWhile_lambda0,
          ((void* []){
              x_isGood,
          })));
}
Closure elm_parser_Parser_Advanced_chompWhile = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_parser_Parser_Advanced_chompWhile,
};
#define elm_parser_Parser_chompWhile elm_parser_Parser_Advanced_chompWhile

#define elm_core_String_slice String_slice
void* eval_elm_parser_Parser_Advanced_mapChompedString_lambda0(void* args[]) {
  void* x_func = args[0];
  void* x_parse = args[1];
  void* x_s0 = args[2];
  void* x__v1 = A1(x_parse, x_s0);
  void* tmp1;
  void* tmp2 = ((Custom*)x__v1)->ctor;
  if (tmp2 == CTOR_Bad) {
    void* x_p = Utils_destruct_index(x__v1, 0);
    void* x_x = Utils_destruct_index(x__v1, 1);
    tmp1 = A2(&elm_parser_Parser_Advanced_Bad, x_p, x_x);
  } else {
    void* x_p = Utils_destruct_index(x__v1, 0);
    void* x_a = Utils_destruct_index(x__v1, 1);
    void* x_s1 = Utils_destruct_index(x__v1, 2);
    tmp1 = A3(&elm_parser_Parser_Advanced_Good,
        x_p,
        A2(x_func,
            A3(&elm_core_String_slice,
                Utils_access_eval(((void* []){
                    (void*)FIELD_offset,
                    x_s0,
                })),
                Utils_access_eval(((void* []){
                    (void*)FIELD_offset,
                    x_s1,
                })),
                Utils_access_eval(((void* []){
                    (void*)FIELD_src,
                    x_s0,
                }))),
            x_a),
        x_s1);
  };
  return tmp1;
}
void* eval_elm_parser_Parser_Advanced_mapChompedString(void* args[]) {
  void* x_func = args[0];
  void* x__v0 = args[1];
  void* x_parse = ((Custom*)x__v0)->values[0];
  return A2(&elm_core_Basics_apL,
      &elm_parser_Parser_Advanced_Parser,
      NEW_CLOSURE(2,
          3,
          &eval_elm_parser_Parser_Advanced_mapChompedString_lambda0,
          ((void* []){
              x_func,
              x_parse,
          })));
}
Closure elm_parser_Parser_Advanced_mapChompedString = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_parser_Parser_Advanced_mapChompedString,
};
void* eval_elm_parser_Parser_Advanced_getChompedString(void* args[]) {
  void* x_parser = args[0];
  return A2(
      &elm_parser_Parser_Advanced_mapChompedString, &elm_core_Basics_always, x_parser);
}
Closure elm_parser_Parser_Advanced_getChompedString = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_parser_Parser_Advanced_getChompedString,
};
#define elm_parser_Parser_getChompedString elm_parser_Parser_Advanced_getChompedString

void* eval_elm_parser_Parser_Problem(void* args[]) {
  return ctorCustom(CTOR_Problem, 1, args);
}
Closure elm_parser_Parser_Problem = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_parser_Parser_Problem,
};

void* eval_elm_parser_Parser_Advanced_problem_lambda0(void* args[]) {
  void* x_x = args[0];
  void* x_s = args[1];
  return A2(&elm_parser_Parser_Advanced_Bad,
      &False,
      A2(&elm_parser_Parser_Advanced_fromState, x_s, x_x));
}
void* eval_elm_parser_Parser_Advanced_problem(void* args[]) {
  void* x_x = args[0];
  return A2(&elm_core_Basics_apL,
      &elm_parser_Parser_Advanced_Parser,
      NEW_CLOSURE(1,
          2,
          &eval_elm_parser_Parser_Advanced_problem_lambda0,
          ((void* []){
              x_x,
          })));
}
Closure elm_parser_Parser_Advanced_problem = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_parser_Parser_Advanced_problem,
};
void* eval_elm_parser_Parser_problem(void* args[]) {
  void* x_msg = args[0];
  return A1(&elm_parser_Parser_Advanced_problem, A1(&elm_parser_Parser_Problem, x_msg));
}
Closure elm_parser_Parser_problem = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_parser_Parser_problem,
};

void* eval_elm_parser_Parser_Advanced_succeed_lambda0(void* args[]) {
  void* x_a = args[0];
  void* x_s = args[1];
  return A3(&elm_parser_Parser_Advanced_Good, &False, x_a, x_s);
}
void* eval_elm_parser_Parser_Advanced_succeed(void* args[]) {
  void* x_a = args[0];
  return A2(&elm_core_Basics_apL,
      &elm_parser_Parser_Advanced_Parser,
      NEW_CLOSURE(1,
          2,
          &eval_elm_parser_Parser_Advanced_succeed_lambda0,
          ((void* []){
              x_a,
          })));
}
Closure elm_parser_Parser_Advanced_succeed = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_parser_Parser_Advanced_succeed,
};
#define elm_parser_Parser_succeed elm_parser_Parser_Advanced_succeed

#define elm_core_String_toInt String_toInt
void* eval_rtfeldman_elm_iso8601_date_strings_Iso8601_paddedInt_lambda0(void* args[]) {
  void* x_quantity = args[0];
  void* x_str = args[1];
  void* tmp1;
  if (A2(&elm_core_Basics_eq, A1(&elm_core_String_length, x_str), x_quantity) == &True) {
    void* x__v0 = A1(&elm_core_String_toInt, x_str);
    void* tmp2;
    void* tmp3 = ((Custom*)x__v0)->ctor;
    if (tmp3 == CTOR_Just) {
      void* x_intVal = Utils_destruct_index(x__v0, 0);
      tmp2 = A1(&elm_parser_Parser_succeed, x_intVal);
    } else {
      tmp2 = A1(&elm_parser_Parser_problem,
          A2(&elm_core_Basics_append,
              &literal_string_Invalid_20integer_3a_20_5c_22,
              A2(&elm_core_Basics_append, x_str, &literal_string__5c_22)));
    };
    tmp1 = tmp2;
  } else {
    tmp1 = A1(&elm_parser_Parser_problem,
        A2(&elm_core_Basics_append,
            &literal_string_Expected_20,
            A2(&elm_core_Basics_append,
                A1(&elm_core_String_fromInt, x_quantity),
                A2(&elm_core_Basics_append,
                    &literal_string__20digits_2c_20but_20got_20,
                    A1(&elm_core_String_fromInt, A1(&elm_core_String_length, x_str))))));
  };
  return tmp1;
}
void* eval_rtfeldman_elm_iso8601_date_strings_Iso8601_paddedInt(void* args[]) {
  void* x_quantity = args[0];
  return A2(&elm_core_Basics_apR,
      A2(&elm_core_Basics_apR,
          A1(&elm_parser_Parser_chompWhile, &elm_core_Char_isDigit),
          &elm_parser_Parser_getChompedString),
      A1(&elm_parser_Parser_andThen,
          NEW_CLOSURE(1,
              2,
              &eval_rtfeldman_elm_iso8601_date_strings_Iso8601_paddedInt_lambda0,
              ((void* []){
                  x_quantity,
              }))));
}
Closure rtfeldman_elm_iso8601_date_strings_Iso8601_paddedInt = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_rtfeldman_elm_iso8601_date_strings_Iso8601_paddedInt,
};

void* eval_elm_parser_Parser_ExpectingSymbol(void* args[]) {
  return ctorCustom(CTOR_ExpectingSymbol, 1, args);
}
Closure elm_parser_Parser_ExpectingSymbol = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_parser_Parser_ExpectingSymbol,
};

void* eval_elm_parser_Parser_Advanced_Token(void* args[]) {
  return ctorCustom(CTOR_Token, 2, args);
}
Closure elm_parser_Parser_Advanced_Token = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_parser_Parser_Advanced_Token,
};

void* eval_elm_core_String_isEmpty(void* args[]) {
  void* x_string = args[0];
  return A2(&elm_core_Basics_eq, x_string, &literal_string_);
}
Closure elm_core_String_isEmpty = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_core_String_isEmpty,
};

#define elm_parser_Parser_Advanced_isSubString Parser_isSubString

#define elm_core_Basics_not Basics_not
void* eval_elm_parser_Parser_Advanced_token_lambda0(void* args[]) {
  void* x_expecting = args[0];
  void* x_progress = args[1];
  void* x_str = args[2];
  void* x_s = args[3];
  void* x__v1 = A5(&elm_parser_Parser_Advanced_isSubString,
      x_str,
      Utils_access_eval(((void* []){
          (void*)FIELD_offset,
          x_s,
      })),
      Utils_access_eval(((void* []){
          (void*)FIELD_row,
          x_s,
      })),
      Utils_access_eval(((void* []){
          (void*)FIELD_col,
          x_s,
      })),
      Utils_access_eval(((void* []){
          (void*)FIELD_src,
          x_s,
      })));
  void* x_newOffset = Utils_destruct_index(x__v1, 0);
  void* x_newRow = Utils_destruct_index(x__v1, 1);
  void* x_newCol = Utils_destruct_index(x__v1, 2);
  void* tmp1;
  if (A2(&elm_core_Basics_eq, x_newOffset, A1(&elm_core_Basics_negate, &literal_int_1)) ==
      &True) {
    tmp1 = A2(&elm_parser_Parser_Advanced_Bad,
        &False,
        A2(&elm_parser_Parser_Advanced_fromState, x_s, x_expecting));
  } else {
    tmp1 = A3(&elm_parser_Parser_Advanced_Good,
        x_progress,
        &Unit,
        NEW_RECORD(&fg_col_context_indent_offset_row_src,
            6,
            ((void* []){
                x_newCol,
                Utils_access_eval(((void* []){
                    (void*)FIELD_context,
                    x_s,
                })),
                Utils_access_eval(((void* []){
                    (void*)FIELD_indent,
                    x_s,
                })),
                x_newOffset,
                x_newRow,
                Utils_access_eval(((void* []){
                    (void*)FIELD_src,
                    x_s,
                })),
            })));
  };
  return tmp1;
}
void* eval_elm_parser_Parser_Advanced_token(void* args[]) {
  void* x__v0 = args[0];
  void* x_str = Utils_destruct_index(x__v0, 0);
  void* x_expecting = Utils_destruct_index(x__v0, 1);
  void* x_progress = A1(&elm_core_Basics_not, A1(&elm_core_String_isEmpty, x_str));
  return A2(&elm_core_Basics_apL,
      &elm_parser_Parser_Advanced_Parser,
      NEW_CLOSURE(3,
          4,
          &eval_elm_parser_Parser_Advanced_token_lambda0,
          ((void* []){
              x_expecting,
              x_progress,
              x_str,
          })));
}
Closure elm_parser_Parser_Advanced_token = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_parser_Parser_Advanced_token,
};
#define elm_parser_Parser_Advanced_symbol elm_parser_Parser_Advanced_token
void* eval_elm_parser_Parser_symbol(void* args[]) {
  void* x_str = args[0];
  return A1(&elm_parser_Parser_Advanced_symbol,
      A2(&elm_parser_Parser_Advanced_Token,
          x_str,
          A1(&elm_parser_Parser_ExpectingSymbol, x_str)));
}
Closure elm_parser_Parser_symbol = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_parser_Parser_symbol,
};

#define rtfeldman_elm_iso8601_date_strings_Iso8601_epochYear literal_int_1970

void* eval_rtfeldman_elm_iso8601_date_strings_Iso8601_invalidDay(void* args[]) {
  void* x_day = args[0];
  return A1(&elm_parser_Parser_problem,
      A2(&elm_core_Basics_append,
          &literal_string_Invalid_20day_3a_20,
          A1(&elm_core_String_fromInt, x_day)));
}
Closure rtfeldman_elm_iso8601_date_strings_Iso8601_invalidDay = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_rtfeldman_elm_iso8601_date_strings_Iso8601_invalidDay,
};

#define elm_core_Basics_modBy Basics_modBy

#define elm_core_Basics_neq Utils_notEqual
void* eval_rtfeldman_elm_iso8601_date_strings_Iso8601_isLeapYear(void* args[]) {
  void* x_year = args[0];
  return A2(&elm_core_Basics_and,
      A2(&elm_core_Basics_eq,
          A2(&elm_core_Basics_modBy, &literal_int_4, x_year),
          &literal_int_0),
      A2(&elm_core_Basics_or,
          A2(&elm_core_Basics_neq,
              A2(&elm_core_Basics_modBy, &literal_int_100, x_year),
              &literal_int_0),
          A2(&elm_core_Basics_eq,
              A2(&elm_core_Basics_modBy, &literal_int_400, x_year),
              &literal_int_0)));
}
Closure rtfeldman_elm_iso8601_date_strings_Iso8601_isLeapYear = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_rtfeldman_elm_iso8601_date_strings_Iso8601_isLeapYear,
};

#define elm_core_Basics_idiv Basics_idiv
void* eval_rtfeldman_elm_iso8601_date_strings_Iso8601_leapYearsBefore(void* args[]) {
  void* x_y1 = args[0];
  void* x_y = A2(&elm_core_Basics_sub, x_y1, &literal_int_1);
  return A2(&elm_core_Basics_add,
      A2(&elm_core_Basics_sub,
          A2(&elm_core_Basics_idiv, x_y, &literal_int_4),
          A2(&elm_core_Basics_idiv, x_y, &literal_int_100)),
      A2(&elm_core_Basics_idiv, x_y, &literal_int_400));
}
Closure rtfeldman_elm_iso8601_date_strings_Iso8601_leapYearsBefore = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_rtfeldman_elm_iso8601_date_strings_Iso8601_leapYearsBefore,
};

#define rtfeldman_elm_iso8601_date_strings_Iso8601_msPerDay literal_int_86400000

#define rtfeldman_elm_iso8601_date_strings_Iso8601_msPerYear literal_int_31536000000
void* eval_rtfeldman_elm_iso8601_date_strings_Iso8601_yearMonthDay_lambda1(void* args[]) {
  void* x_dayInMonth = args[0];
  void* x_month = args[1];
  void* x_year = args[2];
  void* x_extraMs = args[3];
  void* x_yearMs = A2(&elm_core_Basics_mul,
      &rtfeldman_elm_iso8601_date_strings_Iso8601_msPerYear,
      A2(&elm_core_Basics_sub,
          x_year,
          &rtfeldman_elm_iso8601_date_strings_Iso8601_epochYear));
  void* tmp2;
  if (A2(&elm_core_Basics_or,
          A2(&elm_core_Basics_lt, x_month, &literal_int_3),
          A1(&elm_core_Basics_not,
              A1(&rtfeldman_elm_iso8601_date_strings_Iso8601_isLeapYear, x_year))) ==
      &True) {
    tmp2 = A2(&elm_core_Basics_sub, x_dayInMonth, &literal_int_1);
  } else {
    tmp2 = x_dayInMonth;
  };
  void* x_days = tmp2;
  void* x_dayMs = A2(&elm_core_Basics_mul,
      &rtfeldman_elm_iso8601_date_strings_Iso8601_msPerDay,
      A2(&elm_core_Basics_add,
          x_days,
          A2(&elm_core_Basics_sub,
              A1(&rtfeldman_elm_iso8601_date_strings_Iso8601_leapYearsBefore, x_year),
              A1(&rtfeldman_elm_iso8601_date_strings_Iso8601_leapYearsBefore,
                  &rtfeldman_elm_iso8601_date_strings_Iso8601_epochYear))));
  return A1(&elm_parser_Parser_succeed,
      A2(&elm_core_Basics_add, A2(&elm_core_Basics_add, x_extraMs, x_yearMs), x_dayMs));
}
void* eval_rtfeldman_elm_iso8601_date_strings_Iso8601_yearMonthDay(void* args[]) {
  void* x__v0 = args[0];
  void* x_year = Utils_destruct_index(x__v0, 0);
  void* x_month = Utils_destruct_index(x__v0, 1);
  void* x_dayInMonth = Utils_destruct_index(x__v0, 2);
  void* tmp0;
  if (A2(&elm_core_Basics_lt, x_dayInMonth, &literal_int_0) == &True) {
    tmp0 = A1(&rtfeldman_elm_iso8601_date_strings_Iso8601_invalidDay, x_dayInMonth);
  } else {
    void* x_succeedWith = NEW_CLOSURE(3,
        4,
        &eval_rtfeldman_elm_iso8601_date_strings_Iso8601_yearMonthDay_lambda1,
        ((void* []){
            x_dayInMonth,
            x_month,
            x_year,
        }));
    void* tmp3;
    void* tmp4 = ((ElmInt*)x_month)->value;
    if (tmp4 == 1) {
      {
        void* tmp16;
        if (A2(&elm_core_Basics_gt, x_dayInMonth, &literal_int_31) == &True) {
          tmp16 =
              A1(&rtfeldman_elm_iso8601_date_strings_Iso8601_invalidDay, x_dayInMonth);
        } else {
          tmp16 = A1(x_succeedWith, &literal_int_0);
        };
        tmp3 = tmp16;
      };
    } else if (tmp4 == 2) {
      {
        void* tmp15;
        if (A2(&elm_core_Basics_or,
                A2(&elm_core_Basics_gt, x_dayInMonth, &literal_int_29),
                A2(&elm_core_Basics_and,
                    A2(&elm_core_Basics_eq, x_dayInMonth, &literal_int_29),
                    A1(&elm_core_Basics_not,
                        A1(&rtfeldman_elm_iso8601_date_strings_Iso8601_isLeapYear,
                            x_year)))) == &True) {
          tmp15 =
              A1(&rtfeldman_elm_iso8601_date_strings_Iso8601_invalidDay, x_dayInMonth);
        } else {
          tmp15 = A1(x_succeedWith, &literal_int_2678400000);
        };
        tmp3 = tmp15;
      };
    } else if (tmp4 == 3) {
      {
        void* tmp14;
        if (A2(&elm_core_Basics_gt, x_dayInMonth, &literal_int_31) == &True) {
          tmp14 =
              A1(&rtfeldman_elm_iso8601_date_strings_Iso8601_invalidDay, x_dayInMonth);
        } else {
          tmp14 = A1(x_succeedWith, &literal_int_5097600000);
        };
        tmp3 = tmp14;
      };
    } else if (tmp4 == 4) {
      {
        void* tmp13;
        if (A2(&elm_core_Basics_gt, x_dayInMonth, &literal_int_30) == &True) {
          tmp13 =
              A1(&rtfeldman_elm_iso8601_date_strings_Iso8601_invalidDay, x_dayInMonth);
        } else {
          tmp13 = A1(x_succeedWith, &literal_int_7776000000);
        };
        tmp3 = tmp13;
      };
    } else if (tmp4 == 5) {
      {
        void* tmp12;
        if (A2(&elm_core_Basics_gt, x_dayInMonth, &literal_int_31) == &True) {
          tmp12 =
              A1(&rtfeldman_elm_iso8601_date_strings_Iso8601_invalidDay, x_dayInMonth);
        } else {
          tmp12 = A1(x_succeedWith, &literal_int_10368000000);
        };
        tmp3 = tmp12;
      };
    } else if (tmp4 == 6) {
      {
        void* tmp11;
        if (A2(&elm_core_Basics_gt, x_dayInMonth, &literal_int_30) == &True) {
          tmp11 =
              A1(&rtfeldman_elm_iso8601_date_strings_Iso8601_invalidDay, x_dayInMonth);
        } else {
          tmp11 = A1(x_succeedWith, &literal_int_13046400000);
        };
        tmp3 = tmp11;
      };
    } else if (tmp4 == 7) {
      {
        void* tmp10;
        if (A2(&elm_core_Basics_gt, x_dayInMonth, &literal_int_31) == &True) {
          tmp10 =
              A1(&rtfeldman_elm_iso8601_date_strings_Iso8601_invalidDay, x_dayInMonth);
        } else {
          tmp10 = A1(x_succeedWith, &literal_int_15638400000);
        };
        tmp3 = tmp10;
      };
    } else if (tmp4 == 8) {
      {
        void* tmp9;
        if (A2(&elm_core_Basics_gt, x_dayInMonth, &literal_int_31) == &True) {
          tmp9 = A1(&rtfeldman_elm_iso8601_date_strings_Iso8601_invalidDay, x_dayInMonth);
        } else {
          tmp9 = A1(x_succeedWith, &literal_int_18316800000);
        };
        tmp3 = tmp9;
      };
    } else if (tmp4 == 9) {
      {
        void* tmp8;
        if (A2(&elm_core_Basics_gt, x_dayInMonth, &literal_int_30) == &True) {
          tmp8 = A1(&rtfeldman_elm_iso8601_date_strings_Iso8601_invalidDay, x_dayInMonth);
        } else {
          tmp8 = A1(x_succeedWith, &literal_int_20995200000);
        };
        tmp3 = tmp8;
      };
    } else if (tmp4 == 10) {
      {
        void* tmp7;
        if (A2(&elm_core_Basics_gt, x_dayInMonth, &literal_int_31) == &True) {
          tmp7 = A1(&rtfeldman_elm_iso8601_date_strings_Iso8601_invalidDay, x_dayInMonth);
        } else {
          tmp7 = A1(x_succeedWith, &literal_int_23587200000);
        };
        tmp3 = tmp7;
      };
    } else if (tmp4 == 11) {
      {
        void* tmp6;
        if (A2(&elm_core_Basics_gt, x_dayInMonth, &literal_int_30) == &True) {
          tmp6 = A1(&rtfeldman_elm_iso8601_date_strings_Iso8601_invalidDay, x_dayInMonth);
        } else {
          tmp6 = A1(x_succeedWith, &literal_int_26265600000);
        };
        tmp3 = tmp6;
      };
    } else if (tmp4 == 12) {
      {
        void* tmp5;
        if (A2(&elm_core_Basics_gt, x_dayInMonth, &literal_int_31) == &True) {
          tmp5 = A1(&rtfeldman_elm_iso8601_date_strings_Iso8601_invalidDay, x_dayInMonth);
        } else {
          tmp5 = A1(x_succeedWith, &literal_int_28857600000);
        };
        tmp3 = tmp5;
      };
    } else {
      tmp3 = A1(&elm_parser_Parser_problem,
          A2(&elm_core_Basics_append,
              &literal_string_Invalid_20month_3a_20_5c_22,
              A2(&elm_core_Basics_append,
                  A1(&elm_core_String_fromInt, x_month),
                  &literal_string__5c_22)));
    };
    tmp0 = tmp3;
  };
  return tmp0;
}
Closure rtfeldman_elm_iso8601_date_strings_Iso8601_yearMonthDay = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_rtfeldman_elm_iso8601_date_strings_Iso8601_yearMonthDay,
};
#define rtfeldman_elm_iso8601_date_strings_Iso8601_monthYearDayInMs \
  (*ptr_rtfeldman_elm_iso8601_date_strings_Iso8601_monthYearDayInMs)
ElmValue* ptr_rtfeldman_elm_iso8601_date_strings_Iso8601_monthYearDayInMs;
void* eval_rtfeldman_elm_iso8601_date_strings_Iso8601_monthYearDayInMs_lambda0(
    void* args[]) {
  void* x_year = args[0];
  void* x_month = args[1];
  void* x_day = args[2];
  return NEW_TUPLE3(x_year, x_month, x_day);
}
void* init_rtfeldman_elm_iso8601_date_strings_Iso8601_monthYearDayInMs() {
  return A2(&elm_core_Basics_apR,
      A2(&elm_parser_Parser_keeper,
          A2(&elm_parser_Parser_keeper,
              A2(&elm_parser_Parser_keeper,
                  A1(&elm_parser_Parser_succeed,
                      NEW_CLOSURE(0,
                          3,
                          &eval_rtfeldman_elm_iso8601_date_strings_Iso8601_monthYearDayInMs_lambda0,
                          ((void* []){}))),
                  A2(&elm_parser_Parser_ignorer,
                      A1(&rtfeldman_elm_iso8601_date_strings_Iso8601_paddedInt,
                          &literal_int_4),
                      A1(&elm_parser_Parser_symbol, &literal_string__2d))),
              A2(&elm_parser_Parser_ignorer,
                  A1(&rtfeldman_elm_iso8601_date_strings_Iso8601_paddedInt,
                      &literal_int_2),
                  A1(&elm_parser_Parser_symbol, &literal_string__2d))),
          A1(&rtfeldman_elm_iso8601_date_strings_Iso8601_paddedInt, &literal_int_2)),
      A1(&elm_parser_Parser_andThen,
          &rtfeldman_elm_iso8601_date_strings_Iso8601_yearMonthDay));
}

void* eval_elm_parser_Parser_Advanced_Append(void* args[]) {
  return ctorCustom(CTOR_Append, 2, args);
}
Closure elm_parser_Parser_Advanced_Append = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_parser_Parser_Advanced_Append,
};
void* tce_elm_parser_Parser_Advanced_oneOfHelp(void* args[], void** gc_tce_data) {
tce_loop:;
  void* x_s0 = args[0];
  void* x_bag = args[1];
  void* x_parsers = args[2];
  void* tmp0;
  if (x_parsers == &Nil) {
    tmp0 = A2(&elm_parser_Parser_Advanced_Bad, &False, x_bag);
  } else {
    void* x_parse = ((Custom*)Utils_destruct_index(x_parsers, 0))->values[0];
    void* x_remainingParsers = Utils_destruct_index(x_parsers, 1);
    void* x__v1 = A1(x_parse, x_s0);
    void* tmp1;
    void* tmp2 = ((Custom*)x__v1)->ctor;
    if (tmp2 == CTOR_Good) {
      void* x_step = x__v1;
      tmp1 = x_step;
    } else {
      void* x_step = x__v1;
      void* x_p = Utils_destruct_index(x_step, 0);
      void* x_x = Utils_destruct_index(x_step, 1);
      void* tmp3;
      if (x_p == &True) {
        tmp3 = x_step;
      } else {
        void* tmp4 = x_s0;
        void* tmp5 = A2(&elm_parser_Parser_Advanced_Append, x_bag, x_x);
        void* tmp6 = x_remainingParsers;
        *gc_tce_data = CAN_THROW(GC_tce_iteration(3));
        args[2] = tmp6;
        args[1] = tmp5;
        args[0] = tmp4;
        goto tce_loop;
        tmp3 = NULL;
      };
      tmp1 = tmp3;
    };
    tmp0 = tmp1;
  };
  return tmp0;
}
void* eval_elm_parser_Parser_Advanced_oneOfHelp() {
  return GC_tce_eval(&tce_elm_parser_Parser_Advanced_oneOfHelp,
      &eval_elm_parser_Parser_Advanced_oneOfHelp,
      3,
      args);
}
Closure elm_parser_Parser_Advanced_oneOfHelp = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_elm_parser_Parser_Advanced_oneOfHelp,
};
void* eval_elm_parser_Parser_Advanced_oneOf_lambda0(void* args[]) {
  void* x_parsers = args[0];
  void* x_s = args[1];
  return A3(&elm_parser_Parser_Advanced_oneOfHelp,
      x_s,
      &elm_parser_Parser_Advanced_Empty,
      x_parsers);
}
void* eval_elm_parser_Parser_Advanced_oneOf(void* args[]) {
  void* x_parsers = args[0];
  return A2(&elm_core_Basics_apL,
      &elm_parser_Parser_Advanced_Parser,
      NEW_CLOSURE(1,
          2,
          &eval_elm_parser_Parser_Advanced_oneOf_lambda0,
          ((void* []){
              x_parsers,
          })));
}
Closure elm_parser_Parser_Advanced_oneOf = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_parser_Parser_Advanced_oneOf,
};
#define elm_parser_Parser_oneOf elm_parser_Parser_Advanced_oneOf

void* eval_rtfeldman_elm_iso8601_date_strings_Iso8601_utcOffsetMinutesFromParts(
    void* args[]) {
  void* x_multiplier = args[0];
  void* x_hours = args[1];
  void* x_minutes = args[2];
  return A2(&elm_core_Basics_mul,
      x_multiplier,
      A2(&elm_core_Basics_add,
          A2(&elm_core_Basics_mul, x_hours, &literal_int_60),
          x_minutes));
}
Closure rtfeldman_elm_iso8601_date_strings_Iso8601_utcOffsetMinutesFromParts = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator =
        &eval_rtfeldman_elm_iso8601_date_strings_Iso8601_utcOffsetMinutesFromParts,
};
#define rtfeldman_elm_iso8601_date_strings_Iso8601_iso8601 \
  (*ptr_rtfeldman_elm_iso8601_date_strings_Iso8601_iso8601)
ElmValue* ptr_rtfeldman_elm_iso8601_date_strings_Iso8601_iso8601;
void* eval_rtfeldman_elm_iso8601_date_strings_Iso8601_iso8601_lambda1(void* args[]) {
  void* x__v2 = args[0];
  return A1(&elm_core_Basics_negate, &literal_int_1);
}
void* eval_rtfeldman_elm_iso8601_date_strings_Iso8601_iso8601_lambda2(void* args[]) {
  void* x__v1 = args[0];
  return &literal_int_1;
}
void* eval_rtfeldman_elm_iso8601_date_strings_Iso8601_iso8601_lambda3(void* args[]) {
  void* x__v0 = args[0];
  return &literal_int_0;
}
void* eval_rtfeldman_elm_iso8601_date_strings_Iso8601_iso8601_lambda0(void* args[]) {
  void* x_datePart = args[0];
  return A1(&elm_parser_Parser_oneOf,
      List_fromArray(2,
          ((void* []){
              A2(&elm_parser_Parser_keeper,
                  A2(&elm_parser_Parser_keeper,
                      A2(&elm_parser_Parser_keeper,
                          A2(&elm_parser_Parser_keeper,
                              A2(&elm_parser_Parser_keeper,
                                  A2(&elm_parser_Parser_ignorer,
                                      A1(&elm_parser_Parser_succeed,
                                          A1(&rtfeldman_elm_iso8601_date_strings_Iso8601_fromParts,
                                              x_datePart)),
                                      A1(&elm_parser_Parser_symbol, &literal_string_T)),
                                  A2(&elm_parser_Parser_ignorer,
                                      A1(&rtfeldman_elm_iso8601_date_strings_Iso8601_paddedInt,
                                          &literal_int_2),
                                      A1(&elm_parser_Parser_symbol,
                                          &literal_string__3a))),
                              A2(&elm_parser_Parser_ignorer,
                                  A1(&rtfeldman_elm_iso8601_date_strings_Iso8601_paddedInt,
                                      &literal_int_2),
                                  A1(&elm_parser_Parser_symbol, &literal_string__3a))),
                          A1(&rtfeldman_elm_iso8601_date_strings_Iso8601_paddedInt,
                              &literal_int_2)),
                      A1(&elm_parser_Parser_oneOf,
                          List_fromArray(2,
                              ((void* []){
                                  A2(&elm_parser_Parser_keeper,
                                      A2(&elm_parser_Parser_ignorer,
                                          A1(&elm_parser_Parser_succeed,
                                              &elm_core_Basics_identity),
                                          A1(&elm_parser_Parser_symbol,
                                              &literal_string__2e)),
                                      A1(&rtfeldman_elm_iso8601_date_strings_Iso8601_paddedInt,
                                          &literal_int_3)),
                                  A1(&elm_parser_Parser_succeed, &literal_int_0),
                              })))),
                  A1(&elm_parser_Parser_oneOf,
                      List_fromArray(2,
                          ((void* []){
                              A2(&elm_parser_Parser_map,
                                  NEW_CLOSURE(0,
                                      1,
                                      &eval_rtfeldman_elm_iso8601_date_strings_Iso8601_iso8601_lambda3,
                                      ((void* []){})),
                                  A1(&elm_parser_Parser_symbol, &literal_string_Z)),
                              A2(&elm_parser_Parser_keeper,
                                  A2(&elm_parser_Parser_keeper,
                                      A2(&elm_parser_Parser_keeper,
                                          A1(&elm_parser_Parser_succeed,
                                              &rtfeldman_elm_iso8601_date_strings_Iso8601_utcOffsetMinutesFromParts),
                                          A1(&elm_parser_Parser_oneOf,
                                              List_fromArray(2,
                                                  ((void* []){
                                                      A2(&elm_parser_Parser_map,
                                                          NEW_CLOSURE(0,
                                                              1,
                                                              &eval_rtfeldman_elm_iso8601_date_strings_Iso8601_iso8601_lambda2,
                                                              ((void* []){})),
                                                          A1(&elm_parser_Parser_symbol,
                                                              &literal_string__2b)),
                                                      A2(&elm_parser_Parser_map,
                                                          NEW_CLOSURE(0,
                                                              1,
                                                              &eval_rtfeldman_elm_iso8601_date_strings_Iso8601_iso8601_lambda1,
                                                              ((void* []){})),
                                                          A1(&elm_parser_Parser_symbol,
                                                              &literal_string__2d)),
                                                  })))),
                                      A2(&elm_parser_Parser_ignorer,
                                          A1(&rtfeldman_elm_iso8601_date_strings_Iso8601_paddedInt,
                                              &literal_int_2),
                                          A1(&elm_parser_Parser_symbol,
                                              &literal_string__3a))),
                                  A1(&rtfeldman_elm_iso8601_date_strings_Iso8601_paddedInt,
                                      &literal_int_2)),
                          })))),
              A2(&elm_parser_Parser_ignorer,
                  A1(&elm_parser_Parser_succeed,
                      A6(&rtfeldman_elm_iso8601_date_strings_Iso8601_fromParts,
                          x_datePart,
                          &literal_int_0,
                          &literal_int_0,
                          &literal_int_0,
                          &literal_int_0,
                          &literal_int_0)),
                  &elm_parser_Parser_end),
          })));
}
void* init_rtfeldman_elm_iso8601_date_strings_Iso8601_iso8601() {
  return A2(&elm_core_Basics_apR,
      &rtfeldman_elm_iso8601_date_strings_Iso8601_monthYearDayInMs,
      A1(&elm_parser_Parser_andThen,
          NEW_CLOSURE(0,
              1,
              &eval_rtfeldman_elm_iso8601_date_strings_Iso8601_iso8601_lambda0,
              ((void* []){}))));
}

void* eval_elm_core_Result_Ok(void* args[]) {
  return ctorCustom(CTOR_Ok, 1, args);
}
Closure elm_core_Result_Ok = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_core_Result_Ok,
};

void* eval_elm_parser_Parser_DeadEnd(void* args[]) {
  void* x_row = args[0];
  void* x_col = args[1];
  void* x_problem = args[2];
  return NEW_RECORD(&fg_col_problem_row,
      3,
      ((void* []){
          x_col,
          x_problem,
          x_row,
      }));
}
Closure elm_parser_Parser_DeadEnd = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_elm_parser_Parser_DeadEnd,
};
void* eval_elm_parser_Parser_problemToDeadEnd(void* args[]) {
  void* x_p = args[0];
  return A3(&elm_parser_Parser_DeadEnd,
      Utils_access_eval(((void* []){
          (void*)FIELD_row,
          x_p,
      })),
      Utils_access_eval(((void* []){
          (void*)FIELD_col,
          x_p,
      })),
      Utils_access_eval(((void* []){
          (void*)FIELD_problem,
          x_p,
      })));
}
Closure elm_parser_Parser_problemToDeadEnd = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_parser_Parser_problemToDeadEnd,
};

void* tce_elm_parser_Parser_Advanced_bagToList(void* args[], void** gc_tce_data) {
tce_loop:;
  void* x_bag = args[0];
  void* x_list = args[1];
  void* tmp0;
  void* tmp1 = ((Custom*)x_bag)->ctor;
  if (tmp1 == CTOR_Empty) {
    {
      tmp0 = x_list;
    };
  } else if (tmp1 == CTOR_AddRight) {
    {
      void* x_bag1 = Utils_destruct_index(x_bag, 0);
      void* x_x = Utils_destruct_index(x_bag, 1);
      void* tmp4 = x_bag1;
      void* tmp5 = A2(&elm_core_List_cons, x_x, x_list);
      *gc_tce_data = CAN_THROW(GC_tce_iteration(2));
      args[1] = tmp5;
      args[0] = tmp4;
      goto tce_loop;
      tmp0 = NULL;
    };
  } else {
    void* x_bag1 = Utils_destruct_index(x_bag, 0);
    void* x_bag2 = Utils_destruct_index(x_bag, 1);
    void* tmp2 = x_bag1;
    void* tmp3 = A2(&elm_parser_Parser_Advanced_bagToList, x_bag2, x_list);
    *gc_tce_data = CAN_THROW(GC_tce_iteration(2));
    args[1] = tmp3;
    args[0] = tmp2;
    goto tce_loop;
    tmp0 = NULL;
  };
  return tmp0;
}
void* eval_elm_parser_Parser_Advanced_bagToList() {
  return GC_tce_eval(&tce_elm_parser_Parser_Advanced_bagToList,
      &eval_elm_parser_Parser_Advanced_bagToList,
      2,
      args);
}
Closure elm_parser_Parser_Advanced_bagToList = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_parser_Parser_Advanced_bagToList,
};
void* eval_elm_parser_Parser_Advanced_run(void* args[]) {
  void* x__v0 = args[0];
  void* x_src = args[1];
  void* x_parse = ((Custom*)x__v0)->values[0];
  void* x__v1 = A1(x_parse,
      NEW_RECORD(&fg_col_context_indent_offset_row_src,
          6,
          ((void* []){
              &literal_int_1,
              &Nil,
              &literal_int_1,
              &literal_int_0,
              &literal_int_1,
              x_src,
          })));
  void* tmp0;
  void* tmp1 = ((Custom*)x__v1)->ctor;
  if (tmp1 == CTOR_Good) {
    void* x_value = Utils_destruct_index(x__v1, 1);
    tmp0 = A1(&elm_core_Result_Ok, x_value);
  } else {
    void* x_bag = Utils_destruct_index(x__v1, 1);
    tmp0 =
        A1(&elm_core_Result_Err, A2(&elm_parser_Parser_Advanced_bagToList, x_bag, &Nil));
  };
  return tmp0;
}
Closure elm_parser_Parser_Advanced_run = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_parser_Parser_Advanced_run,
};
void* eval_elm_parser_Parser_run(void* args[]) {
  void* x_parser = args[0];
  void* x_source = args[1];
  void* x__v0 = A2(&elm_parser_Parser_Advanced_run, x_parser, x_source);
  void* tmp0;
  void* tmp1 = ((Custom*)x__v0)->ctor;
  if (tmp1 == CTOR_Ok) {
    void* x_a = Utils_destruct_index(x__v0, 0);
    tmp0 = A1(&elm_core_Result_Ok, x_a);
  } else {
    void* x_problems = Utils_destruct_index(x__v0, 0);
    tmp0 = A1(&elm_core_Result_Err,
        A2(&elm_core_List_map, &elm_parser_Parser_problemToDeadEnd, x_problems));
  };
  return tmp0;
}
Closure elm_parser_Parser_run = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_parser_Parser_run,
};
void* eval_rtfeldman_elm_iso8601_date_strings_Iso8601_toTime(void* args[]) {
  void* x_str = args[0];
  return A2(
      &elm_parser_Parser_run, &rtfeldman_elm_iso8601_date_strings_Iso8601_iso8601, x_str);
}
Closure rtfeldman_elm_iso8601_date_strings_Iso8601_toTime = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_rtfeldman_elm_iso8601_date_strings_Iso8601_toTime,
};
#define rtfeldman_elm_iso8601_date_strings_Iso8601_decoder \
  (*ptr_rtfeldman_elm_iso8601_date_strings_Iso8601_decoder)
ElmValue* ptr_rtfeldman_elm_iso8601_date_strings_Iso8601_decoder;
void* eval_rtfeldman_elm_iso8601_date_strings_Iso8601_decoder_lambda0(void* args[]) {
  void* x_str = args[0];
  void* x__v0 = A1(&rtfeldman_elm_iso8601_date_strings_Iso8601_toTime, x_str);
  void* tmp1;
  void* tmp2 = ((Custom*)x__v0)->ctor;
  if (tmp2 == CTOR_Err) {
    void* x_deadEnds = Utils_destruct_index(x__v0, 0);
    tmp1 = A2(&elm_core_Basics_apL,
        &elm_json_Json_Decode_fail,
        A1(&elm_parser_Parser_deadEndsToString, x_deadEnds));
  } else {
    void* x_time = Utils_destruct_index(x__v0, 0);
    tmp1 = A1(&elm_json_Json_Decode_succeed, x_time);
  };
  return tmp1;
}
void* init_rtfeldman_elm_iso8601_date_strings_Iso8601_decoder() {
  return A2(&elm_core_Basics_apR,
      &elm_json_Json_Decode_string,
      A1(&elm_json_Json_Decode_andThen,
          NEW_CLOSURE(0,
              1,
              &eval_rtfeldman_elm_iso8601_date_strings_Iso8601_decoder_lambda0,
              ((void* []){}))));
}

#define elm_json_Json_Decode_int Json_decodeInt

#define elm_json_Json_Decode_list Json_decodeList

void* eval_elm_core_Maybe_withDefault(void* args[]) {
  void* x_default = args[0];
  void* x_maybe = args[1];
  void* tmp0;
  void* tmp1 = ((Custom*)x_maybe)->ctor;
  if (tmp1 == CTOR_Just) {
    void* x_value = Utils_destruct_index(x_maybe, 0);
    tmp0 = x_value;
  } else {
    tmp0 = x_default;
  };
  return tmp0;
}
Closure elm_core_Maybe_withDefault = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_core_Maybe_withDefault,
};
#define author_project_Article_metadataDecoder \
  (*ptr_author_project_Article_metadataDecoder)
ElmValue* ptr_author_project_Article_metadataDecoder;
void* init_author_project_Article_metadataDecoder() {
  return A2(&elm_core_Basics_apR,
      A2(&elm_core_Basics_apR,
          A2(&elm_core_Basics_apR,
              A2(&elm_core_Basics_apR,
                  A2(&elm_core_Basics_apR,
                      A2(&elm_core_Basics_apR,
                          A1(&elm_json_Json_Decode_succeed,
                              &author_project_Article_Metadata),
                          A2(&NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_required,
                              &literal_string_description,
                              A2(&elm_json_Json_Decode_map,
                                  A1(&elm_core_Maybe_withDefault, &literal_string_),
                                  A1(&elm_json_Json_Decode_nullable,
                                      &elm_json_Json_Decode_string)))),
                      A2(&NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_required,
                          &literal_string_title,
                          &elm_json_Json_Decode_string)),
                  A2(&NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_required,
                      &literal_string_tagList,
                      A1(&elm_json_Json_Decode_list, &elm_json_Json_Decode_string))),
              A2(&NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_required,
                  &literal_string_createdAt,
                  &rtfeldman_elm_iso8601_date_strings_Iso8601_decoder)),
          A2(&NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_required,
              &literal_string_favorited,
              &elm_json_Json_Decode_bool)),
      A2(&NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_required,
          &literal_string_favoritesCount,
          &elm_json_Json_Decode_int));
}
void* eval_author_project_Article_internalsDecoder(void* args[]) {
  void* x_maybeCred = args[0];
  return A2(&elm_core_Basics_apR,
      A2(&elm_core_Basics_apR,
          A2(&elm_core_Basics_apR,
              A1(&elm_json_Json_Decode_succeed, &author_project_Article_Internals),
              A2(&NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_required,
                  &literal_string_slug,
                  &author_project_Article_Slug_decoder)),
          A2(&NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_required,
              &literal_string_author,
              A1(&author_project_Author_decoder, x_maybeCred))),
      A1(&NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_custom,
          &author_project_Article_metadataDecoder));
}
Closure author_project_Article_internalsDecoder = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Article_internalsDecoder,
};
void* eval_author_project_Article_fullDecoder(void* args[]) {
  void* x_maybeCred = args[0];
  return A2(&elm_core_Basics_apR,
      A2(&elm_core_Basics_apR,
          A1(&elm_json_Json_Decode_succeed, &author_project_Article_Article),
          A1(&NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_custom,
              A1(&author_project_Article_internalsDecoder, x_maybeCred))),
      A2(&NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_required,
          &literal_string_body,
          A2(&elm_json_Json_Decode_map,
              &author_project_Article_Full,
              &author_project_Article_Body_decoder)));
}
Closure author_project_Article_fullDecoder = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Article_fullDecoder,
};

void* eval_elm_http_Http_Internal_Header(void* args[]) {
  return ctorCustom(CTOR_Header, 2, args);
}
Closure elm_http_Http_Internal_Header = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_http_Http_Internal_Header,
};
#define elm_http_Http_header elm_http_Http_Internal_Header
void* eval_author_project_Api_credHeader(void* args[]) {
  void* x__v0 = args[0];
  void* x_str = Utils_destruct_index(x__v0, 1);
  return A2(&elm_http_Http_header,
      &literal_string_authorization,
      A2(&elm_core_Basics_append, &literal_string_Token_20, x_str));
}
Closure author_project_Api_credHeader = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Api_credHeader,
};

Custom elm_http_Http_Internal_EmptyBody = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_EmptyBody,
};
#define elm_http_Http_emptyBody elm_http_Http_Internal_EmptyBody

#define elm_http_Http_expectStringResponse Http_expectStringResponse
void* eval_elm_http_Http_expectJson_lambda0(void* args[]) {
  void* x_decoder = args[0];
  void* x_response = args[1];
  void* x__v0 = A2(&elm_json_Json_Decode_decodeString,
      x_decoder,
      Utils_access_eval(((void* []){
          (void*)FIELD_body,
          x_response,
      })));
  void* tmp1;
  void* tmp2 = ((Custom*)x__v0)->ctor;
  if (tmp2 == CTOR_Err) {
    void* x_decodeError = Utils_destruct_index(x__v0, 0);
    tmp1 =
        A1(&elm_core_Result_Err, A1(&elm_json_Json_Decode_errorToString, x_decodeError));
  } else {
    void* x_value = Utils_destruct_index(x__v0, 0);
    tmp1 = A1(&elm_core_Result_Ok, x_value);
  };
  return tmp1;
}
void* eval_elm_http_Http_expectJson(void* args[]) {
  void* x_decoder = args[0];
  return A2(&elm_core_Basics_apL,
      &elm_http_Http_expectStringResponse,
      NEW_CLOSURE(1,
          2,
          &eval_elm_http_Http_expectJson_lambda0,
          ((void* []){
              x_decoder,
          })));
}
Closure elm_http_Http_expectJson = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_http_Http_expectJson,
};

void* eval_elm_http_Http_Internal_Request(void* args[]) {
  return ctorCustom(CTOR_Request, 1, args);
}
Closure elm_http_Http_Internal_Request = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_http_Http_Internal_Request,
};
#define elm_http_Http_request elm_http_Http_Internal_Request

void* eval_author_project_Api_Endpoint_unwrap(void* args[]) {
  void* x__v0 = args[0];
  void* x_str = ((Custom*)x__v0)->values[0];
  return x_str;
}
Closure author_project_Api_Endpoint_unwrap = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Api_Endpoint_unwrap,
};
void* eval_author_project_Api_Endpoint_request(void* args[]) {
  void* x_config = args[0];
  return A1(&elm_http_Http_request,
      NEW_RECORD(&fg_body_expect_headers_method_timeout_url_withCredentials,
          7,
          ((void* []){
              Utils_access_eval(((void* []){
                  (void*)FIELD_body,
                  x_config,
              })),
              Utils_access_eval(((void* []){
                  (void*)FIELD_expect,
                  x_config,
              })),
              Utils_access_eval(((void* []){
                  (void*)FIELD_headers,
                  x_config,
              })),
              Utils_access_eval(((void* []){
                  (void*)FIELD_method,
                  x_config,
              })),
              Utils_access_eval(((void* []){
                  (void*)FIELD_timeout,
                  x_config,
              })),
              A1(&author_project_Api_Endpoint_unwrap,
                  Utils_access_eval(((void* []){
                      (void*)FIELD_url,
                      x_config,
                  }))),
              Utils_access_eval(((void* []){
                  (void*)FIELD_withCredentials,
                  x_config,
              })),
          })));
}
Closure author_project_Api_Endpoint_request = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Api_Endpoint_request,
};
void* eval_author_project_Api_get(void* args[]) {
  void* x_url = args[0];
  void* x_maybeCred = args[1];
  void* x_decoder = args[2];
  void* tmp0;
  void* tmp1 = ((Custom*)x_maybeCred)->ctor;
  if (tmp1 == CTOR_Just) {
    void* x_cred = Utils_destruct_index(x_maybeCred, 0);
    tmp0 = List_fromArray(1,
        ((void* []){
            A1(&author_project_Api_credHeader, x_cred),
        }));
  } else {
    tmp0 = &Nil;
  };
  return A1(&author_project_Api_Endpoint_request,
      NEW_RECORD(&fg_body_expect_headers_method_timeout_url_withCredentials,
          7,
          ((void* []){
              &elm_http_Http_emptyBody,
              A1(&elm_http_Http_expectJson, x_decoder),
              tmp0,
              &literal_string_GET,
              &elm_core_Maybe_Nothing,
              x_url,
              &False,
          })));
}
Closure author_project_Api_get = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_author_project_Api_get,
};
void* eval_author_project_Article_fetch(void* args[]) {
  void* x_maybeCred = args[0];
  void* x_articleSlug = args[1];
  return A2(&elm_core_Basics_apR,
      A2(&elm_json_Json_Decode_field,
          &literal_string_article,
          A1(&author_project_Article_fullDecoder, x_maybeCred)),
      A2(&author_project_Api_get,
          A1(&author_project_Api_Endpoint_article, x_articleSlug),
          x_maybeCred));
}
Closure author_project_Article_fetch = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Article_fetch,
};

#define elm_time_Time_here (*ptr_elm_time_Time_here)
ElmValue* ptr_elm_time_Time_here;
void* init_elm_time_Time_here() {
  return A1(&Time_here, &Unit);
}

void* eval_author_project_Api_Endpoint_comments(void* args[]) {
  void* x_slug = args[0];
  return A2(&author_project_Api_Endpoint_url,
      List_fromArray(3,
          ((void* []){
              &literal_string_articles,
              A1(&author_project_Article_Slug_toString, x_slug),
              &literal_string_comments,
          })),
      &Nil);
}
Closure author_project_Api_Endpoint_comments = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Api_Endpoint_comments,
};

void* eval_author_project_Article_Comment_Comment(void* args[]) {
  return ctorCustom(CTOR_Comment, 1, args);
}
Closure author_project_Article_Comment_Comment = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Article_Comment_Comment,
};

void* eval_author_project_Article_Comment_Internals(void* args[]) {
  void* x_id = args[0];
  void* x_body = args[1];
  void* x_createdAt = args[2];
  void* x_author = args[3];
  return NEW_RECORD(&fg_author_body_createdAt_id,
      4,
      ((void* []){
          x_author,
          x_body,
          x_createdAt,
          x_id,
      }));
}
Closure author_project_Article_Comment_Internals = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x4,
    .evaluator = &eval_author_project_Article_Comment_Internals,
};

void* eval_author_project_CommentId_CommentId(void* args[]) {
  return ctorCustom(CTOR_CommentId, 1, args);
}
Closure author_project_CommentId_CommentId = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_CommentId_CommentId,
};
#define author_project_CommentId_decoder (*ptr_author_project_CommentId_decoder)
ElmValue* ptr_author_project_CommentId_decoder;
void* init_author_project_CommentId_decoder() {
  return A2(&elm_json_Json_Decode_map,
      &author_project_CommentId_CommentId,
      &elm_json_Json_Decode_int);
}
void* eval_author_project_Article_Comment_decoder(void* args[]) {
  void* x_maybeCred = args[0];
  return A2(&elm_core_Basics_apR,
      A2(&elm_core_Basics_apR,
          A2(&elm_core_Basics_apR,
              A2(&elm_core_Basics_apR,
                  A2(&elm_core_Basics_apR,
                      A1(&elm_json_Json_Decode_succeed,
                          &author_project_Article_Comment_Internals),
                      A2(&NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_required,
                          &literal_string_id,
                          &author_project_CommentId_decoder)),
                  A2(&NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_required,
                      &literal_string_body,
                      &elm_json_Json_Decode_string)),
              A2(&NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_required,
                  &literal_string_createdAt,
                  &rtfeldman_elm_iso8601_date_strings_Iso8601_decoder)),
          A2(&NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_required,
              &literal_string_author,
              A1(&author_project_Author_decoder, x_maybeCred))),
      A1(&elm_json_Json_Decode_map, &author_project_Article_Comment_Comment));
}
Closure author_project_Article_Comment_decoder = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Article_Comment_decoder,
};
void* eval_author_project_Article_Comment_list(void* args[]) {
  void* x_maybeCred = args[0];
  void* x_articleSlug = args[1];
  return A2(&elm_core_Basics_apR,
      A2(&elm_json_Json_Decode_field,
          &literal_string_comments,
          A1(&elm_json_Json_Decode_list,
              A1(&author_project_Article_Comment_decoder, x_maybeCred))),
      A2(&author_project_Api_get,
          A1(&author_project_Api_Endpoint_comments, x_articleSlug),
          x_maybeCred));
}
Closure author_project_Article_Comment_list = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Article_Comment_list,
};

void* eval_elm_core_Task_Perform(void* args[]) {
  return ctorCustom(CTOR_Perform, 1, args);
}
Closure elm_core_Task_Perform = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_core_Task_Perform,
};

Closure elm_core_Task_command = {
    .header = HEADER_CLOSURE(1),
    .n_values = 0x1,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Platform_leaf,
    .values =
        {
            &literal_string_Task,
        },
};

#define elm_core_Task_andThen Scheduler_andThen

#define elm_core_Task_succeed Scheduler_succeed
void* eval_elm_core_Task_map_lambda0(void* args[]) {
  void* x_func = args[0];
  void* x_a = args[1];
  return A1(&elm_core_Task_succeed, A1(x_func, x_a));
}
void* eval_elm_core_Task_map(void* args[]) {
  void* x_func = args[0];
  void* x_taskA = args[1];
  return A2(&elm_core_Basics_apR,
      x_taskA,
      A1(&elm_core_Task_andThen,
          NEW_CLOSURE(1,
              2,
              &eval_elm_core_Task_map_lambda0,
              ((void* []){
                  x_func,
              }))));
}
Closure elm_core_Task_map = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_core_Task_map,
};
void* eval_elm_core_Task_perform(void* args[]) {
  void* x_toMessage = args[0];
  void* x_task = args[1];
  return A1(&elm_core_Task_command,
      A1(&elm_core_Task_Perform, A2(&elm_core_Task_map, x_toMessage, x_task)));
}
Closure elm_core_Task_perform = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_core_Task_perform,
};

void* eval_elm_core_Basics_composeL(void* args[]) {
  void* x_g = args[0];
  void* x_f = args[1];
  void* x_x = args[2];
  return A1(x_g, A1(x_f, x_x));
}
Closure elm_core_Basics_composeL = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_elm_core_Basics_composeL,
};

#define elm_core_Task_onError Scheduler_onError
void* eval_elm_core_Task_attempt(void* args[]) {
  void* x_resultToMessage = args[0];
  void* x_task = args[1];
  return A1(&elm_core_Task_command,
      A1(&elm_core_Task_Perform,
          A2(&elm_core_Basics_apR,
              A2(&elm_core_Basics_apR,
                  x_task,
                  A1(&elm_core_Task_andThen,
                      A2(&elm_core_Basics_composeL,
                          A2(&elm_core_Basics_composeL,
                              &elm_core_Task_succeed,
                              x_resultToMessage),
                          &elm_core_Result_Ok))),
              A1(&elm_core_Task_onError,
                  A2(&elm_core_Basics_composeL,
                      A2(&elm_core_Basics_composeL,
                          &elm_core_Task_succeed,
                          x_resultToMessage),
                      &elm_core_Result_Err)))));
}
Closure elm_core_Task_attempt = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_core_Task_attempt,
};

void* eval_elm_http_Http_toTask(void* args[]) {
  void* x__v0 = args[0];
  void* x_request_ = ((Custom*)x__v0)->values[0];
  return A2(&Http_toTask, x_request_, &elm_core_Maybe_Nothing);
}
Closure elm_http_Http_toTask = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_http_Http_toTask,
};
void* eval_elm_http_Http_send(void* args[]) {
  void* x_resultToMessage = args[0];
  void* x_request_ = args[1];
  return A2(
      &elm_core_Task_attempt, x_resultToMessage, A1(&elm_http_Http_toTask, x_request_));
}
Closure elm_http_Http_send = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_http_Http_send,
};

#define elm_core_Process_sleep Process_sleep
#define author_project_Loading_slowThreshold (*ptr_author_project_Loading_slowThreshold)
ElmValue* ptr_author_project_Loading_slowThreshold;
void* init_author_project_Loading_slowThreshold() {
  return A1(&elm_core_Process_sleep, &literal_int_500);
}

void* eval_elm_time_Time_Zone(void* args[]) {
  return ctorCustom(CTOR_Zone, 2, args);
}
Closure elm_time_Time_Zone = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_time_Time_Zone,
};
#define elm_time_Time_utc (*ptr_elm_time_Time_utc)
ElmValue* ptr_elm_time_Time_utc;
void* init_elm_time_Time_utc() {
  return A2(&elm_time_Time_Zone, &literal_int_0, &Nil);
}
void* eval_author_project_Page_Article_init_lambda0(void* args[]) {
  void* x__v0 = args[0];
  return &author_project_Page_Article_PassedSlowLoadThreshold;
}
void* eval_author_project_Page_Article_init(void* args[]) {
  void* x_session = args[0];
  void* x_slug = args[1];
  void* x_maybeCred = A1(&author_project_Session_cred, x_session);
  return NEW_TUPLE2(NEW_RECORD(&fg_article_comments_errors_session_timeZone,
                        5,
                        ((void* []){
                            &author_project_Page_Article_Loading,
                            &author_project_Page_Article_Loading,
                            &Nil,
                            x_session,
                            &elm_time_Time_utc,
                        })),
      A1(&elm_core_Platform_Cmd_batch,
          List_fromArray(4,
              ((void* []){
                  A2(&elm_core_Basics_apR,
                      A2(&author_project_Article_fetch, x_maybeCred, x_slug),
                      A1(&elm_http_Http_send,
                          &author_project_Page_Article_CompletedLoadArticle)),
                  A2(&elm_core_Basics_apR,
                      A2(&author_project_Article_Comment_list, x_maybeCred, x_slug),
                      A1(&elm_http_Http_send,
                          &author_project_Page_Article_CompletedLoadComments)),
                  A2(&elm_core_Task_perform,
                      &author_project_Page_Article_GotTimeZone,
                      &elm_time_Time_here),
                  A2(&elm_core_Task_perform,
                      NEW_CLOSURE(0,
                          1,
                          &eval_author_project_Page_Article_init_lambda0,
                          ((void* []){})),
                      &author_project_Loading_slowThreshold),
              }))));
}
Closure author_project_Page_Article_init = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Article_init,
};

void* eval_author_project_Page_Home_CompletedFeedLoad(void* args[]) {
  return ctorCustom(CTOR_CompletedFeedLoad, 1, args);
}
Closure author_project_Page_Home_CompletedFeedLoad = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Home_CompletedFeedLoad,
};

void* eval_author_project_Page_Home_CompletedTagsLoad(void* args[]) {
  return ctorCustom(CTOR_CompletedTagsLoad, 1, args);
}
Closure author_project_Page_Home_CompletedTagsLoad = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Home_CompletedTagsLoad,
};

Custom author_project_Page_Home_GlobalFeed = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_GlobalFeed,
};

void* eval_author_project_Page_Home_GotTimeZone(void* args[]) {
  return ctorCustom(CTOR_GotTimeZone, 1, args);
}
Closure author_project_Page_Home_GotTimeZone = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Home_GotTimeZone,
};

Custom author_project_Page_Home_Loading = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Loading,
};

Custom author_project_Page_Home_PassedSlowLoadThreshold = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_PassedSlowLoadThreshold,
};

void* eval_author_project_Page_Home_YourFeed(void* args[]) {
  return ctorCustom(CTOR_YourFeed, 1, args);
}
Closure author_project_Page_Home_YourFeed = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Home_YourFeed,
};

void* eval_author_project_Api_Endpoint_articles(void* args[]) {
  void* x_params = args[0];
  return A2(&author_project_Api_Endpoint_url,
      List_fromArray(1,
          ((void* []){
              &literal_string_articles,
          })),
      x_params);
}
Closure author_project_Api_Endpoint_articles = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Api_Endpoint_articles,
};

#define author_project_Page_Home_articlesPerPage literal_int_10

void* eval_author_project_PaginatedList_PaginatedList(void* args[]) {
  return ctorCustom(CTOR_PaginatedList, 1, args);
}
Closure author_project_PaginatedList_PaginatedList = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_PaginatedList_PaginatedList,
};
void* eval_author_project_PaginatedList_fromList(void* args[]) {
  void* x_totalCount = args[0];
  void* x_list = args[1];
  return A1(&author_project_PaginatedList_PaginatedList,
      NEW_RECORD(&fg_total_values,
          2,
          ((void* []){
              x_totalCount,
              x_list,
          })));
}
Closure author_project_PaginatedList_fromList = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_PaginatedList_fromList,
};

#define elm_core_Basics_ceiling Basics_ceiling

#define elm_core_Basics_fdiv Basics_fdiv

#define elm_core_Basics_toFloat Basics_toFloat
void* eval_author_project_Article_Feed_pageCountDecoder_lambda0(void* args[]) {
  void* x_resultsPerPage = args[0];
  void* x_total = args[1];
  return A1(&elm_core_Basics_ceiling,
      A2(&elm_core_Basics_fdiv,
          A1(&elm_core_Basics_toFloat, x_total),
          A1(&elm_core_Basics_toFloat, x_resultsPerPage)));
}
void* eval_author_project_Article_Feed_pageCountDecoder(void* args[]) {
  void* x_resultsPerPage = args[0];
  return A2(&elm_core_Basics_apR,
      &elm_json_Json_Decode_int,
      A1(&elm_json_Json_Decode_map,
          NEW_CLOSURE(1,
              2,
              &eval_author_project_Article_Feed_pageCountDecoder_lambda0,
              ((void* []){
                  x_resultsPerPage,
              }))));
}
Closure author_project_Article_Feed_pageCountDecoder = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Article_Feed_pageCountDecoder,
};

Custom author_project_Article_Preview = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Preview,
};

void* eval_elm_core_Basics_composeR(void* args[]) {
  void* x_f = args[0];
  void* x_g = args[1];
  void* x_x = args[2];
  return A1(x_g, A1(x_f, x_x));
}
Closure elm_core_Basics_composeR = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_elm_core_Basics_composeR,
};
#define NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_hardcoded \
  (*ptr_NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_hardcoded)
ElmValue* ptr_NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_hardcoded;
void* init_NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_hardcoded() {
  return A2(&elm_core_Basics_composeR,
      &elm_json_Json_Decode_succeed,
      &NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_custom);
}
void* eval_author_project_Article_previewDecoder(void* args[]) {
  void* x_maybeCred = args[0];
  return A2(&elm_core_Basics_apR,
      A2(&elm_core_Basics_apR,
          A1(&elm_json_Json_Decode_succeed, &author_project_Article_Article),
          A1(&NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_custom,
              A1(&author_project_Article_internalsDecoder, x_maybeCred))),
      A1(&NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_hardcoded,
          &author_project_Article_Preview));
}
Closure author_project_Article_previewDecoder = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Article_previewDecoder,
};
void* eval_author_project_Article_Feed_decoder(void* args[]) {
  void* x_maybeCred = args[0];
  void* x_resultsPerPage = args[1];
  return A2(&elm_core_Basics_apR,
      A2(&elm_core_Basics_apR,
          A1(&elm_json_Json_Decode_succeed, &author_project_PaginatedList_fromList),
          A2(&NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_required,
              &literal_string_articlesCount,
              A1(&author_project_Article_Feed_pageCountDecoder, x_resultsPerPage))),
      A2(&NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_required,
          &literal_string_articles,
          A1(&elm_json_Json_Decode_list,
              A1(&author_project_Article_previewDecoder, x_maybeCred))));
}
Closure author_project_Article_Feed_decoder = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Article_Feed_decoder,
};

void* eval_author_project_Api_Endpoint_feed(void* args[]) {
  void* x_params = args[0];
  return A2(&author_project_Api_Endpoint_url,
      List_fromArray(2,
          ((void* []){
              &literal_string_articles,
              &literal_string_feed,
          })),
      x_params);
}
Closure author_project_Api_Endpoint_feed = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Api_Endpoint_feed,
};

void* eval_author_project_Article_Feed_Model(void* args[]) {
  return ctorCustom(CTOR_Model, 1, args);
}
Closure author_project_Article_Feed_Model = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Article_Feed_Model,
};
void* eval_author_project_Article_Feed_init(void* args[]) {
  void* x_session = args[0];
  void* x_articles = args[1];
  return A1(&author_project_Article_Feed_Model,
      NEW_RECORD(&fg_articles_errors_isLoading_session,
          4,
          ((void* []){
              x_articles,
              &Nil,
              &False,
              x_session,
          })));
}
Closure author_project_Article_Feed_init = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Article_Feed_init,
};

void* eval_elm_url_Url_Builder_QueryParameter(void* args[]) {
  return ctorCustom(CTOR_QueryParameter, 2, args);
}
Closure elm_url_Url_Builder_QueryParameter = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_url_Url_Builder_QueryParameter,
};

#define elm_url_Url_percentEncode Url_percentEncode
void* eval_elm_url_Url_Builder_string(void* args[]) {
  void* x_key = args[0];
  void* x_value = args[1];
  return A2(&elm_url_Url_Builder_QueryParameter,
      A1(&elm_url_Url_percentEncode, x_key),
      A1(&elm_url_Url_percentEncode, x_value));
}
Closure elm_url_Url_Builder_string = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_url_Url_Builder_string,
};
void* eval_author_project_PaginatedList_params(void* args[]) {
  void* x__v0 = args[0];
  void* x_page = Utils_access_eval(((void* []){
      (void*)FIELD_page,
      x__v0,
  }));
  void* x_resultsPerPage = Utils_access_eval(((void* []){
      (void*)FIELD_resultsPerPage,
      x__v0,
  }));
  void* x_offset = A2(&elm_core_Basics_mul,
      A2(&elm_core_Basics_sub, x_page, &literal_int_1),
      x_resultsPerPage);
  return List_fromArray(2,
      ((void* []){
          A2(&elm_url_Url_Builder_string,
              &literal_string_limit,
              A1(&elm_core_String_fromInt, x_resultsPerPage)),
          A2(&elm_url_Url_Builder_string,
              &literal_string_offset,
              A1(&elm_core_String_fromInt, x_offset)),
      }));
}
Closure author_project_PaginatedList_params = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_PaginatedList_params,
};

void* eval_author_project_Article_Tag_toString(void* args[]) {
  void* x__v0 = args[0];
  void* x_slug = ((Custom*)x__v0)->values[0];
  return x_slug;
}
Closure author_project_Article_Tag_toString = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Article_Tag_toString,
};
void* eval_author_project_Page_Home_fetchFeed(void* args[]) {
  void* x_session = args[0];
  void* x_feedTabs = args[1];
  void* x_page = args[2];
  void* x_params = A1(&author_project_PaginatedList_params,
      NEW_RECORD(&fg_page_resultsPerPage,
          2,
          ((void* []){
              x_page,
              &author_project_Page_Home_articlesPerPage,
          })));
  void* x_maybeCred = A1(&author_project_Session_cred, x_session);
  void* x_decoder = A2(&author_project_Article_Feed_decoder,
      x_maybeCred,
      &author_project_Page_Home_articlesPerPage);
  void* tmp0;
  void* tmp1 = ((Custom*)x_feedTabs)->ctor;
  if (tmp1 == CTOR_YourFeed) {
    {
      void* x_cred = Utils_destruct_index(x_feedTabs, 0);
      tmp0 = A3(&author_project_Api_get,
          A1(&author_project_Api_Endpoint_feed, x_params),
          x_maybeCred,
          x_decoder);
    };
  } else if (tmp1 == CTOR_GlobalFeed) {
    {
      tmp0 = A3(&author_project_Api_get,
          A1(&author_project_Api_Endpoint_articles, x_params),
          x_maybeCred,
          x_decoder);
    };
  } else {
    void* x_tag = Utils_destruct_index(x_feedTabs, 0);
    void* x_firstParam = A2(&elm_url_Url_Builder_string,
        &literal_string_tag,
        A1(&author_project_Article_Tag_toString, x_tag));
    tmp0 = A3(&author_project_Api_get,
        A1(&author_project_Api_Endpoint_articles,
            A2(&elm_core_List_cons, x_firstParam, x_params)),
        x_maybeCred,
        x_decoder);
  };
  void* x_request = tmp0;
  return A2(&elm_core_Basics_apR,
      A1(&elm_http_Http_toTask, x_request),
      A1(&elm_core_Task_map, A1(&author_project_Article_Feed_init, x_session)));
}
Closure author_project_Page_Home_fetchFeed = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_author_project_Page_Home_fetchFeed,
};

void* eval_author_project_Article_Tag_Tag(void* args[]) {
  return ctorCustom(CTOR_Tag, 1, args);
}
Closure author_project_Article_Tag_Tag = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Article_Tag_Tag,
};
#define author_project_Article_Tag_decoder (*ptr_author_project_Article_Tag_decoder)
ElmValue* ptr_author_project_Article_Tag_decoder;
void* init_author_project_Article_Tag_decoder() {
  return A2(&elm_json_Json_Decode_map,
      &author_project_Article_Tag_Tag,
      &elm_json_Json_Decode_string);
}

#define author_project_Api_Endpoint_tags (*ptr_author_project_Api_Endpoint_tags)
ElmValue* ptr_author_project_Api_Endpoint_tags;
void* init_author_project_Api_Endpoint_tags() {
  return A2(&author_project_Api_Endpoint_url,
      List_fromArray(1,
          ((void* []){
              &literal_string_tags,
          })),
      &Nil);
}
#define author_project_Article_Tag_list (*ptr_author_project_Article_Tag_list)
ElmValue* ptr_author_project_Article_Tag_list;
void* init_author_project_Article_Tag_list() {
  return A2(&elm_core_Basics_apR,
      A2(&elm_json_Json_Decode_field,
          &literal_string_tags,
          A1(&elm_json_Json_Decode_list, &author_project_Article_Tag_decoder)),
      A2(&author_project_Api_get,
          &author_project_Api_Endpoint_tags,
          &elm_core_Maybe_Nothing));
}
void* eval_author_project_Page_Home_init_lambda2(void* args[]) {
  void* x__v0 = args[0];
  return &author_project_Page_Home_PassedSlowLoadThreshold;
}
void* eval_author_project_Page_Home_init(void* args[]) {
  void* x_session = args[0];
  void* x_loadTags = A1(&elm_http_Http_toTask, &author_project_Article_Tag_list);
  void* x__v1 = A1(&author_project_Session_cred, x_session);
  void* tmp0;
  void* tmp1 = ((Custom*)x__v1)->ctor;
  if (tmp1 == CTOR_Just) {
    void* x_cred = Utils_destruct_index(x__v1, 0);
    tmp0 = A1(&author_project_Page_Home_YourFeed, x_cred);
  } else {
    tmp0 = &author_project_Page_Home_GlobalFeed;
  };
  void* x_feedTab = tmp0;
  return NEW_TUPLE2(NEW_RECORD(&fg_feed_feedPage_feedTab_session_tags_timeZone,
                        6,
                        ((void* []){
                            &author_project_Page_Home_Loading,
                            &literal_int_1,
                            x_feedTab,
                            x_session,
                            &author_project_Page_Home_Loading,
                            &elm_time_Time_utc,
                        })),
      A1(&elm_core_Platform_Cmd_batch,
          List_fromArray(4,
              ((void* []){
                  A2(&elm_core_Basics_apR,
                      A3(&author_project_Page_Home_fetchFeed,
                          x_session,
                          x_feedTab,
                          &literal_int_1),
                      A1(&elm_core_Task_attempt,
                          &author_project_Page_Home_CompletedFeedLoad)),
                  A2(&elm_core_Basics_apR,
                      &author_project_Article_Tag_list,
                      A1(&elm_http_Http_send,
                          &author_project_Page_Home_CompletedTagsLoad)),
                  A2(&elm_core_Task_perform,
                      &author_project_Page_Home_GotTimeZone,
                      &elm_time_Time_here),
                  A2(&elm_core_Task_perform,
                      NEW_CLOSURE(0,
                          1,
                          &eval_author_project_Page_Home_init_lambda2,
                          ((void* []){})),
                      &author_project_Loading_slowThreshold),
              }))));
}
Closure author_project_Page_Home_init = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Home_init,
};

#define elm_core_Platform_Cmd_none (*ptr_elm_core_Platform_Cmd_none)
ElmValue* ptr_elm_core_Platform_Cmd_none;
void* init_elm_core_Platform_Cmd_none() {
  return A1(&elm_core_Platform_Cmd_batch, &Nil);
}
void* eval_author_project_Page_Login_init(void* args[]) {
  void* x_session = args[0];
  return NEW_TUPLE2(NEW_RECORD(&fg_form_problems_session,
                        3,
                        ((void* []){
                            NEW_RECORD(&fg_email_password,
                                2,
                                ((void* []){
                                    &literal_string_,
                                    &literal_string_,
                                })),
                            &Nil,
                            x_session,
                        })),
      &elm_core_Platform_Cmd_none);
}
Closure author_project_Page_Login_init = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Login_init,
};

void* eval_author_project_Page_Profile_CompletedAuthorLoad(void* args[]) {
  return ctorCustom(CTOR_CompletedAuthorLoad, 1, args);
}
Closure author_project_Page_Profile_CompletedAuthorLoad = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Profile_CompletedAuthorLoad,
};

void* eval_author_project_Page_Profile_GotTimeZone(void* args[]) {
  return ctorCustom(CTOR_GotTimeZone, 1, args);
}
Closure author_project_Page_Profile_GotTimeZone = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Profile_GotTimeZone,
};

void* eval_author_project_Page_Profile_Loading(void* args[]) {
  return ctorCustom(CTOR_Loading, 1, args);
}
Closure author_project_Page_Profile_Loading = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Profile_Loading,
};

Custom author_project_Page_Profile_PassedSlowLoadThreshold = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_PassedSlowLoadThreshold,
};

Custom author_project_Page_Profile_MyArticles = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_MyArticles,
};
#define author_project_Page_Profile_defaultFeedTab author_project_Page_Profile_MyArticles

void* eval_author_project_Username_toString(void* args[]) {
  void* x__v0 = args[0];
  void* x_username = ((Custom*)x__v0)->values[0];
  return x_username;
}
Closure author_project_Username_toString = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Username_toString,
};
void* eval_author_project_Api_Endpoint_profiles(void* args[]) {
  void* x_uname = args[0];
  return A2(&author_project_Api_Endpoint_url,
      List_fromArray(2,
          ((void* []){
              &literal_string_profiles,
              A1(&author_project_Username_toString, x_uname),
          })),
      &Nil);
}
Closure author_project_Api_Endpoint_profiles = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Api_Endpoint_profiles,
};
void* eval_author_project_Author_fetch(void* args[]) {
  void* x_uname = args[0];
  void* x_maybeCred = args[1];
  return A2(&elm_core_Basics_apR,
      A2(&elm_json_Json_Decode_field,
          &literal_string_profile,
          A1(&author_project_Author_decoder, x_maybeCred)),
      A2(&author_project_Api_get,
          A1(&author_project_Api_Endpoint_profiles, x_uname),
          x_maybeCred));
}
Closure author_project_Author_fetch = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Author_fetch,
};

void* eval_author_project_Page_Profile_CompletedFeedLoad(void* args[]) {
  return ctorCustom(CTOR_CompletedFeedLoad, 1, args);
}
Closure author_project_Page_Profile_CompletedFeedLoad = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Profile_CompletedFeedLoad,
};

#define author_project_Page_Profile_articlesPerPage literal_int_5

#define elm_core_Task_fail Scheduler_fail
void* eval_elm_core_Task_mapError(void* args[]) {
  void* x_convert = args[0];
  void* x_task = args[1];
  return A2(&elm_core_Basics_apR,
      x_task,
      A1(&elm_core_Task_onError,
          A2(&elm_core_Basics_composeL, &elm_core_Task_fail, x_convert)));
}
Closure elm_core_Task_mapError = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_core_Task_mapError,
};
void* eval_author_project_Page_Profile_fetchFeed(void* args[]) {
  void* x_session = args[0];
  void* x_feedTabs = args[1];
  void* x_username = args[2];
  void* x_page = args[3];
  void* x_maybeCred = A1(&author_project_Session_cred, x_session);
  void* tmp0;
  void* tmp1 = ((Custom*)x_feedTabs)->ctor;
  if (tmp1 == CTOR_MyArticles) {
    tmp0 = A2(&elm_url_Url_Builder_string,
        &literal_string_author,
        A1(&author_project_Username_toString, x_username));
  } else {
    tmp0 = A2(&elm_url_Url_Builder_string,
        &literal_string_favorited,
        A1(&author_project_Username_toString, x_username));
  };
  void* x_firstParam = tmp0;
  void* x_params = A2(&elm_core_List_cons,
      x_firstParam,
      A1(&author_project_PaginatedList_params,
          NEW_RECORD(&fg_page_resultsPerPage,
              2,
              ((void* []){
                  x_page,
                  &author_project_Page_Profile_articlesPerPage,
              }))));
  void* x_expect = A2(&author_project_Article_Feed_decoder,
      x_maybeCred,
      &author_project_Page_Profile_articlesPerPage);
  return A2(&elm_core_Basics_apR,
      A2(&elm_core_Basics_apR,
          A2(&elm_core_Basics_apR,
              A2(&elm_core_Basics_apR,
                  A3(&author_project_Api_get,
                      A1(&author_project_Api_Endpoint_articles, x_params),
                      x_maybeCred,
                      x_expect),
                  &elm_http_Http_toTask),
              A1(&elm_core_Task_map, A1(&author_project_Article_Feed_init, x_session))),
          A1(&elm_core_Task_mapError, A1(&elm_core_Tuple_pair, x_username))),
      A1(&elm_core_Task_attempt, &author_project_Page_Profile_CompletedFeedLoad));
}
Closure author_project_Page_Profile_fetchFeed = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x4,
    .evaluator = &eval_author_project_Page_Profile_fetchFeed,
};
void* eval_author_project_Page_Profile_init_lambda0(void* args[]) {
  void* x__v0 = args[0];
  return &author_project_Page_Profile_PassedSlowLoadThreshold;
}
void* eval_author_project_Page_Profile_init(void* args[]) {
  void* x_session = args[0];
  void* x_username = args[1];
  void* x_maybeCred = A1(&author_project_Session_cred, x_session);
  return NEW_TUPLE2(NEW_RECORD(&fg_author_errors_feed_feedPage_feedTab_session_timeZone,
                        7,
                        ((void* []){
                            A1(&author_project_Page_Profile_Loading, x_username),
                            &Nil,
                            A1(&author_project_Page_Profile_Loading, x_username),
                            &literal_int_1,
                            &author_project_Page_Profile_defaultFeedTab,
                            x_session,
                            &elm_time_Time_utc,
                        })),
      A1(&elm_core_Platform_Cmd_batch,
          List_fromArray(4,
              ((void* []){
                  A2(&elm_core_Basics_apR,
                      A2(&elm_core_Basics_apR,
                          A2(&elm_core_Basics_apR,
                              A2(&author_project_Author_fetch, x_username, x_maybeCred),
                              &elm_http_Http_toTask),
                          A1(&elm_core_Task_mapError,
                              A1(&elm_core_Tuple_pair, x_username))),
                      A1(&elm_core_Task_attempt,
                          &author_project_Page_Profile_CompletedAuthorLoad)),
                  A4(&author_project_Page_Profile_fetchFeed,
                      x_session,
                      &author_project_Page_Profile_defaultFeedTab,
                      x_username,
                      &literal_int_1),
                  A2(&elm_core_Task_perform,
                      &author_project_Page_Profile_GotTimeZone,
                      &elm_time_Time_here),
                  A2(&elm_core_Task_perform,
                      NEW_CLOSURE(0,
                          1,
                          &eval_author_project_Page_Profile_init_lambda0,
                          ((void* []){})),
                      &author_project_Loading_slowThreshold),
              }))));
}
Closure author_project_Page_Profile_init = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Profile_init,
};

void* eval_author_project_Page_Register_init(void* args[]) {
  void* x_session = args[0];
  return NEW_TUPLE2(NEW_RECORD(&fg_form_problems_session,
                        3,
                        ((void* []){
                            NEW_RECORD(&fg_email_password_username,
                                3,
                                ((void* []){
                                    &literal_string_,
                                    &literal_string_,
                                    &literal_string_,
                                })),
                            &Nil,
                            x_session,
                        })),
      &elm_core_Platform_Cmd_none);
}
Closure author_project_Page_Register_init = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Register_init,
};

void* eval_author_project_Page_Settings_CompletedFormLoad(void* args[]) {
  return ctorCustom(CTOR_CompletedFormLoad, 1, args);
}
Closure author_project_Page_Settings_CompletedFormLoad = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Settings_CompletedFormLoad,
};

Custom author_project_Page_Settings_Loading = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Loading,
};

Custom author_project_Page_Settings_PassedSlowLoadThreshold = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_PassedSlowLoadThreshold,
};

void* eval_author_project_Page_Settings_Form(void* args[]) {
  void* x_avatar = args[0];
  void* x_bio = args[1];
  void* x_email = args[2];
  void* x_username = args[3];
  void* x_password = args[4];
  return NEW_RECORD(&fg_avatar_bio_email_password_username,
      5,
      ((void* []){
          x_avatar,
          x_bio,
          x_email,
          x_password,
          x_username,
      }));
}
Closure author_project_Page_Settings_Form = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x5,
    .evaluator = &eval_author_project_Page_Settings_Form,
};
#define author_project_Page_Settings_formDecoder \
  (*ptr_author_project_Page_Settings_formDecoder)
ElmValue* ptr_author_project_Page_Settings_formDecoder;
void* init_author_project_Page_Settings_formDecoder() {
  return A2(&elm_core_Basics_apR,
      A2(&elm_core_Basics_apR,
          A2(&elm_core_Basics_apR,
              A2(&elm_core_Basics_apR,
                  A2(&elm_core_Basics_apR,
                      A1(&elm_json_Json_Decode_succeed,
                          &author_project_Page_Settings_Form),
                      A2(&NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_required,
                          &literal_string_image,
                          A2(&elm_json_Json_Decode_map,
                              A1(&elm_core_Maybe_withDefault, &literal_string_),
                              A1(&elm_json_Json_Decode_nullable,
                                  &elm_json_Json_Decode_string)))),
                  A2(&NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_required,
                      &literal_string_bio,
                      A2(&elm_json_Json_Decode_map,
                          A1(&elm_core_Maybe_withDefault, &literal_string_),
                          A1(&elm_json_Json_Decode_nullable,
                              &elm_json_Json_Decode_string)))),
              A2(&NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_required,
                  &literal_string_email,
                  &elm_json_Json_Decode_string)),
          A2(&NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_required,
              &literal_string_username,
              &elm_json_Json_Decode_string)),
      A1(&NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_hardcoded,
          &literal_string_));
}

#define author_project_Api_Endpoint_user (*ptr_author_project_Api_Endpoint_user)
ElmValue* ptr_author_project_Api_Endpoint_user;
void* init_author_project_Api_Endpoint_user() {
  return A2(&author_project_Api_Endpoint_url,
      List_fromArray(1,
          ((void* []){
              &literal_string_user,
          })),
      &Nil);
}
void* eval_author_project_Page_Settings_init_lambda0(void* args[]) {
  void* x__v0 = args[0];
  return &author_project_Page_Settings_PassedSlowLoadThreshold;
}
void* eval_author_project_Page_Settings_init(void* args[]) {
  void* x_session = args[0];
  return NEW_TUPLE2(NEW_RECORD(&fg_problems_session_status,
                        3,
                        ((void* []){
                            &Nil,
                            x_session,
                            &author_project_Page_Settings_Loading,
                        })),
      A1(&elm_core_Platform_Cmd_batch,
          List_fromArray(2,
              ((void* []){
                  A2(&elm_core_Basics_apR,
                      A3(&author_project_Api_get,
                          &author_project_Api_Endpoint_user,
                          A1(&author_project_Session_cred, x_session),
                          A2(&elm_json_Json_Decode_field,
                              &literal_string_user,
                              &author_project_Page_Settings_formDecoder)),
                      A1(&elm_http_Http_send,
                          &author_project_Page_Settings_CompletedFormLoad)),
                  A2(&elm_core_Task_perform,
                      NEW_CLOSURE(0,
                          1,
                          &eval_author_project_Page_Settings_init_lambda0,
                          ((void* []){})),
                      &author_project_Loading_slowThreshold),
              }))));
}
Closure author_project_Page_Settings_init = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Settings_init,
};

void* eval_author_project_Page_Article_Editor_CompletedArticleLoad(void* args[]) {
  return ctorCustom(CTOR_CompletedArticleLoad, 1, args);
}
Closure author_project_Page_Article_Editor_CompletedArticleLoad = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_Editor_CompletedArticleLoad,
};

void* eval_author_project_Page_Article_Editor_Loading(void* args[]) {
  return ctorCustom(CTOR_Loading, 1, args);
}
Closure author_project_Page_Article_Editor_Loading = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_Editor_Loading,
};

Custom author_project_Page_Article_Editor_PassedSlowLoadThreshold = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_PassedSlowLoadThreshold,
};
void* eval_author_project_Page_Article_Editor_initEdit_lambda0(void* args[]) {
  void* x__v0 = args[0];
  return &author_project_Page_Article_Editor_PassedSlowLoadThreshold;
}
void* eval_author_project_Page_Article_Editor_initEdit_lambda1(void* args[]) {
  void* x_slug = args[0];
  void* x_httpError = args[1];
  return NEW_TUPLE2(x_slug, x_httpError);
}
void* eval_author_project_Page_Article_Editor_initEdit(void* args[]) {
  void* x_session = args[0];
  void* x_slug = args[1];
  return NEW_TUPLE2(NEW_RECORD(&fg_session_status,
                        2,
                        ((void* []){
                            x_session,
                            A1(&author_project_Page_Article_Editor_Loading, x_slug),
                        })),
      A1(&elm_core_Platform_Cmd_batch,
          List_fromArray(2,
              ((void* []){
                  A2(&elm_core_Basics_apR,
                      A2(&elm_core_Basics_apR,
                          A2(&elm_core_Basics_apR,
                              A2(&author_project_Article_fetch,
                                  A1(&author_project_Session_cred, x_session),
                                  x_slug),
                              &elm_http_Http_toTask),
                          A1(&elm_core_Task_mapError,
                              NEW_CLOSURE(1,
                                  2,
                                  &eval_author_project_Page_Article_Editor_initEdit_lambda1,
                                  ((void* []){
                                      x_slug,
                                  })))),
                      A1(&elm_core_Task_attempt,
                          &author_project_Page_Article_Editor_CompletedArticleLoad)),
                  A2(&elm_core_Task_perform,
                      NEW_CLOSURE(0,
                          1,
                          &eval_author_project_Page_Article_Editor_initEdit_lambda0,
                          ((void* []){})),
                      &author_project_Loading_slowThreshold),
              }))));
}
Closure author_project_Page_Article_Editor_initEdit = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Article_Editor_initEdit,
};

void* eval_author_project_Page_Article_Editor_EditingNew(void* args[]) {
  return ctorCustom(CTOR_EditingNew, 2, args);
}
Closure author_project_Page_Article_Editor_EditingNew = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Article_Editor_EditingNew,
};
void* eval_author_project_Page_Article_Editor_initNew(void* args[]) {
  void* x_session = args[0];
  return NEW_TUPLE2(NEW_RECORD(&fg_session_status,
                        2,
                        ((void* []){
                            x_session,
                            A2(&author_project_Page_Article_Editor_EditingNew,
                                &Nil,
                                NEW_RECORD(&fg_body_description_tags_title,
                                    4,
                                    ((void* []){
                                        &literal_string_,
                                        &literal_string_,
                                        &literal_string_,
                                        &literal_string_,
                                    }))),
                        })),
      &elm_core_Platform_Cmd_none);
}
Closure author_project_Page_Article_Editor_initNew = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_Editor_initNew,
};

void* eval_elm_core_Maybe_destruct(void* args[]) {
  void* x_default = args[0];
  void* x_func = args[1];
  void* x_maybe = args[2];
  void* tmp0;
  void* tmp1 = ((Custom*)x_maybe)->ctor;
  if (tmp1 == CTOR_Just) {
    void* x_a = Utils_destruct_index(x_maybe, 0);
    tmp0 = A1(x_func, x_a);
  } else {
    tmp0 = x_default;
  };
  return tmp0;
}
Closure elm_core_Maybe_destruct = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_elm_core_Maybe_destruct,
};

#define elm_json_Json_Encode_null Json_encodeNull
#define author_project_Api_storeCache (*ptr_author_project_Api_storeCache)
Closure* ptr_author_project_Api_storeCache;
void* eval_author_project_Api_storeCache_lambda0(void* args[]) {
  void* x = args[0];
  return A3(
      &elm_core_Maybe_destruct, &elm_json_Json_Encode_null, &elm_core_Basics_identity, x);
}
void* init_author_project_Api_storeCache() {
  return A2(&Platform_outgoingPort,
      &literal_string_storeCache,
      NEW_CLOSURE(0, 1, &eval_author_project_Api_storeCache_lambda0, ((void* []){})));
}
#define author_project_Api_logout (*ptr_author_project_Api_logout)
ElmValue* ptr_author_project_Api_logout;
void* init_author_project_Api_logout() {
  return A1(&author_project_Api_storeCache, &elm_core_Maybe_Nothing);
}

void* eval_author_project_Session_navKey(void* args[]) {
  void* x_session = args[0];
  void* tmp0;
  void* tmp1 = ((Custom*)x_session)->ctor;
  if (tmp1 == CTOR_LoggedIn) {
    void* x_key = Utils_destruct_index(x_session, 0);
    tmp0 = x_key;
  } else {
    void* x_key = Utils_destruct_index(x_session, 0);
    tmp0 = x_key;
  };
  return tmp0;
}
Closure author_project_Session_navKey = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Session_navKey,
};

#define elm_browser_Browser_Navigation_replaceUrl Browser_replaceUrl

void* eval_author_project_Route_routeToPieces(void* args[]) {
  void* x_page = args[0];
  void* tmp0;
  void* tmp1 = ((Custom*)x_page)->ctor;
  if (tmp1 == CTOR_Home) {
    {
      tmp0 = &Nil;
    };
  } else if (tmp1 == CTOR_Root) {
    { tmp0 = &Nil; };
  } else if (tmp1 == CTOR_Login) {
    {
      tmp0 = List_fromArray(1,
          ((void* []){
              &literal_string_login,
          }));
    };
  } else if (tmp1 == CTOR_Logout) {
    {
      tmp0 = List_fromArray(1,
          ((void* []){
              &literal_string_logout,
          }));
    };
  } else if (tmp1 == CTOR_Register) {
    {
      tmp0 = List_fromArray(1,
          ((void* []){
              &literal_string_register,
          }));
    };
  } else if (tmp1 == CTOR_Settings) {
    {
      tmp0 = List_fromArray(1,
          ((void* []){
              &literal_string_settings,
          }));
    };
  } else if (tmp1 == CTOR_Article) {
    {
      void* x_slug = Utils_destruct_index(x_page, 0);
      tmp0 = List_fromArray(2,
          ((void* []){
              &literal_string_article,
              A1(&author_project_Article_Slug_toString, x_slug),
          }));
    };
  } else if (tmp1 == CTOR_Profile) {
    {
      void* x_username = Utils_destruct_index(x_page, 0);
      tmp0 = List_fromArray(2,
          ((void* []){
              &literal_string_profile,
              A1(&author_project_Username_toString, x_username),
          }));
    };
  } else if (tmp1 == CTOR_NewArticle) {
    {
      tmp0 = List_fromArray(1,
          ((void* []){
              &literal_string_editor,
          }));
    };
  } else {
    void* x_slug = Utils_destruct_index(x_page, 0);
    tmp0 = List_fromArray(2,
        ((void* []){
            &literal_string_editor,
            A1(&author_project_Article_Slug_toString, x_slug),
        }));
  };
  return tmp0;
}
Closure author_project_Route_routeToPieces = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Route_routeToPieces,
};
void* eval_author_project_Route_routeToString(void* args[]) {
  void* x_page = args[0];
  return A2(&elm_core_Basics_append,
      &literal_string__23_2f,
      A2(&elm_core_String_join,
          &literal_string__2f,
          A1(&author_project_Route_routeToPieces, x_page)));
}
Closure author_project_Route_routeToString = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Route_routeToString,
};
void* eval_author_project_Route_replaceUrl(void* args[]) {
  void* x_key = args[0];
  void* x_route = args[1];
  return A2(&elm_browser_Browser_Navigation_replaceUrl,
      x_key,
      A1(&author_project_Route_routeToString, x_route));
}
Closure author_project_Route_replaceUrl = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Route_replaceUrl,
};

void* eval_author_project_Page_Article_toSession(void* args[]) {
  void* x_model = args[0];
  return Utils_access_eval(((void* []){
      (void*)FIELD_session,
      x_model,
  }));
}
Closure author_project_Page_Article_toSession = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_toSession,
};

void* eval_author_project_Page_Article_Editor_toSession(void* args[]) {
  void* x_model = args[0];
  return Utils_access_eval(((void* []){
      (void*)FIELD_session,
      x_model,
  }));
}
Closure author_project_Page_Article_Editor_toSession = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_Editor_toSession,
};

void* eval_author_project_Page_Home_toSession(void* args[]) {
  void* x_model = args[0];
  return Utils_access_eval(((void* []){
      (void*)FIELD_session,
      x_model,
  }));
}
Closure author_project_Page_Home_toSession = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Home_toSession,
};

void* eval_author_project_Page_Login_toSession(void* args[]) {
  void* x_model = args[0];
  return Utils_access_eval(((void* []){
      (void*)FIELD_session,
      x_model,
  }));
}
Closure author_project_Page_Login_toSession = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Login_toSession,
};

void* eval_author_project_Page_Profile_toSession(void* args[]) {
  void* x_model = args[0];
  return Utils_access_eval(((void* []){
      (void*)FIELD_session,
      x_model,
  }));
}
Closure author_project_Page_Profile_toSession = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Profile_toSession,
};

void* eval_author_project_Page_Register_toSession(void* args[]) {
  void* x_model = args[0];
  return Utils_access_eval(((void* []){
      (void*)FIELD_session,
      x_model,
  }));
}
Closure author_project_Page_Register_toSession = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Register_toSession,
};

void* eval_author_project_Page_Settings_toSession(void* args[]) {
  void* x_model = args[0];
  return Utils_access_eval(((void* []){
      (void*)FIELD_session,
      x_model,
  }));
}
Closure author_project_Page_Settings_toSession = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Settings_toSession,
};
void* eval_author_project_Main_toSession(void* args[]) {
  void* x_page = args[0];
  void* tmp0;
  void* tmp1 = ((Custom*)x_page)->ctor;
  if (tmp1 == CTOR_Redirect) {
    {
      void* x_session = Utils_destruct_index(x_page, 0);
      tmp0 = x_session;
    };
  } else if (tmp1 == CTOR_NotFound) {
    {
      void* x_session = Utils_destruct_index(x_page, 0);
      tmp0 = x_session;
    };
  } else if (tmp1 == CTOR_Home) {
    {
      void* x_home = Utils_destruct_index(x_page, 0);
      tmp0 = A1(&author_project_Page_Home_toSession, x_home);
    };
  } else if (tmp1 == CTOR_Settings) {
    {
      void* x_settings = Utils_destruct_index(x_page, 0);
      tmp0 = A1(&author_project_Page_Settings_toSession, x_settings);
    };
  } else if (tmp1 == CTOR_Login) {
    {
      void* x_login = Utils_destruct_index(x_page, 0);
      tmp0 = A1(&author_project_Page_Login_toSession, x_login);
    };
  } else if (tmp1 == CTOR_Register) {
    {
      void* x_register = Utils_destruct_index(x_page, 0);
      tmp0 = A1(&author_project_Page_Register_toSession, x_register);
    };
  } else if (tmp1 == CTOR_Profile) {
    {
      void* x_profile = Utils_destruct_index(x_page, 1);
      tmp0 = A1(&author_project_Page_Profile_toSession, x_profile);
    };
  } else if (tmp1 == CTOR_Article) {
    {
      void* x_article = Utils_destruct_index(x_page, 0);
      tmp0 = A1(&author_project_Page_Article_toSession, x_article);
    };
  } else {
    void* x_editor = Utils_destruct_index(x_page, 1);
    tmp0 = A1(&author_project_Page_Article_Editor_toSession, x_editor);
  };
  return tmp0;
}
Closure author_project_Main_toSession = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Main_toSession,
};

#define elm_core_Platform_Cmd_map Platform_map
void* eval_author_project_Main_updateWith(void* args[]) {
  void* x_toModel = args[0];
  void* x_toMsg = args[1];
  void* x_model = args[2];
  void* x__v0 = args[3];
  void* x_subModel = Utils_destruct_index(x__v0, 0);
  void* x_subCmd = Utils_destruct_index(x__v0, 1);
  return NEW_TUPLE2(
      A1(x_toModel, x_subModel), A2(&elm_core_Platform_Cmd_map, x_toMsg, x_subCmd));
}
Closure author_project_Main_updateWith = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x4,
    .evaluator = &eval_author_project_Main_updateWith,
};
void* eval_author_project_Main_changeRouteTo(void* args[]) {
  void* x_maybeRoute = args[0];
  void* x_model = args[1];
  void* x_session = A1(&author_project_Main_toSession, x_model);
  void* tmp0;
  void* tmp1 = ((Custom*)x_maybeRoute)->ctor;
  void* tmp2 = ((Custom*)Utils_destruct_index(x_maybeRoute, 0))->ctor;
  if (tmp1 == CTOR_Nothing) {
    tmp0 = NEW_TUPLE2(
        A1(&author_project_Main_NotFound, x_session), &elm_core_Platform_Cmd_none);
  } else if (tmp2 == CTOR_Root) {
    {
      void* x__v1 = Utils_destruct_index(x_maybeRoute, 0);
      tmp0 = NEW_TUPLE2(x_model,
          A2(&author_project_Route_replaceUrl,
              A1(&author_project_Session_navKey, x_session),
              &author_project_Route_Home));
    };
  } else if (tmp2 == CTOR_Logout) {
    {
      void* x__v2 = Utils_destruct_index(x_maybeRoute, 0);
      tmp0 = NEW_TUPLE2(x_model, &author_project_Api_logout);
    };
  } else if (tmp2 == CTOR_NewArticle) {
    {
      void* x__v3 = Utils_destruct_index(x_maybeRoute, 0);
      tmp0 = A2(&elm_core_Basics_apR,
          A1(&author_project_Page_Article_Editor_initNew, x_session),
          A3(&author_project_Main_updateWith,
              A1(&author_project_Main_Editor, &elm_core_Maybe_Nothing),
              &author_project_Main_GotEditorMsg,
              x_model));
    };
  } else if (tmp2 == CTOR_EditArticle) {
    {
      void* x_slug = Utils_destruct_index(Utils_destruct_index(x_maybeRoute, 0), 0);
      tmp0 = A2(&elm_core_Basics_apR,
          A2(&author_project_Page_Article_Editor_initEdit, x_session, x_slug),
          A3(&author_project_Main_updateWith,
              A1(&author_project_Main_Editor, A1(&elm_core_Maybe_Just, x_slug)),
              &author_project_Main_GotEditorMsg,
              x_model));
    };
  } else if (tmp2 == CTOR_Settings) {
    {
      void* x__v4 = Utils_destruct_index(x_maybeRoute, 0);
      tmp0 = A2(&elm_core_Basics_apR,
          A1(&author_project_Page_Settings_init, x_session),
          A3(&author_project_Main_updateWith,
              &author_project_Main_Settings,
              &author_project_Main_GotSettingsMsg,
              x_model));
    };
  } else if (tmp2 == CTOR_Home) {
    {
      void* x__v5 = Utils_destruct_index(x_maybeRoute, 0);
      tmp0 = A2(&elm_core_Basics_apR,
          A1(&author_project_Page_Home_init, x_session),
          A3(&author_project_Main_updateWith,
              &author_project_Main_Home,
              &author_project_Main_GotHomeMsg,
              x_model));
    };
  } else if (tmp2 == CTOR_Login) {
    {
      void* x__v6 = Utils_destruct_index(x_maybeRoute, 0);
      tmp0 = A2(&elm_core_Basics_apR,
          A1(&author_project_Page_Login_init, x_session),
          A3(&author_project_Main_updateWith,
              &author_project_Main_Login,
              &author_project_Main_GotLoginMsg,
              x_model));
    };
  } else if (tmp2 == CTOR_Register) {
    {
      void* x__v7 = Utils_destruct_index(x_maybeRoute, 0);
      tmp0 = A2(&elm_core_Basics_apR,
          A1(&author_project_Page_Register_init, x_session),
          A3(&author_project_Main_updateWith,
              &author_project_Main_Register,
              &author_project_Main_GotRegisterMsg,
              x_model));
    };
  } else if (tmp2 == CTOR_Profile) {
    {
      void* x_username = Utils_destruct_index(Utils_destruct_index(x_maybeRoute, 0), 0);
      tmp0 = A2(&elm_core_Basics_apR,
          A2(&author_project_Page_Profile_init, x_session, x_username),
          A3(&author_project_Main_updateWith,
              A1(&author_project_Main_Profile, x_username),
              &author_project_Main_GotProfileMsg,
              x_model));
    };
  } else {
    void* x_slug = Utils_destruct_index(Utils_destruct_index(x_maybeRoute, 0), 0);
    tmp0 = A2(&elm_core_Basics_apR,
        A2(&author_project_Page_Article_init, x_session, x_slug),
        A3(&author_project_Main_updateWith,
            &author_project_Main_Article,
            &author_project_Main_GotArticleMsg,
            x_model));
  };
  return tmp0;
}
Closure author_project_Main_changeRouteTo = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Main_changeRouteTo,
};

void* eval_elm_url_Url_Parser_State(void* args[]) {
  void* x_visited = args[0];
  void* x_unvisited = args[1];
  void* x_params = args[2];
  void* x_frag = args[3];
  void* x_value = args[4];
  return NEW_RECORD(&fg_frag_params_unvisited_value_visited,
      5,
      ((void* []){
          x_frag,
          x_params,
          x_unvisited,
          x_value,
          x_visited,
      }));
}
Closure elm_url_Url_Parser_State = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x5,
    .evaluator = &eval_elm_url_Url_Parser_State,
};

void* tce_elm_url_Url_Parser_getFirstMatch(void* args[], void** gc_tce_data) {
tce_loop:;
  void* x_states = args[0];
  void* tmp0;
  if (x_states == &Nil) {
    tmp0 = &elm_core_Maybe_Nothing;
  } else {
    void* x_state = Utils_destruct_index(x_states, 0);
    void* x_rest = Utils_destruct_index(x_states, 1);
    void* x__v1 = Utils_access_eval(((void* []){
        (void*)FIELD_unvisited,
        x_state,
    }));
    void* tmp1;
    if (x__v1 == &Nil) {
      tmp1 = A1(&elm_core_Maybe_Just,
          Utils_access_eval(((void* []){
              (void*)FIELD_value,
              x_state,
          })));
    } else if (A2(Utils_equal, Utils_destruct_index(x__v1, 0), &literal_string_) ==
                   &True &&
               Utils_destruct_index(x__v1, 1) == &Nil) {
      tmp1 = A1(&elm_core_Maybe_Just,
          Utils_access_eval(((void* []){
              (void*)FIELD_value,
              x_state,
          })));
    } else {
      void* tmp2 = x_rest;
      *gc_tce_data = CAN_THROW(GC_tce_iteration(1));
      args[0] = tmp2;
      goto tce_loop;
      tmp1 = NULL;
    };
    tmp0 = tmp1;
  };
  return tmp0;
}
void* eval_elm_url_Url_Parser_getFirstMatch() {
  return GC_tce_eval(&tce_elm_url_Url_Parser_getFirstMatch,
      &eval_elm_url_Url_Parser_getFirstMatch,
      1,
      args);
}
Closure elm_url_Url_Parser_getFirstMatch = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_url_Url_Parser_getFirstMatch,
};

void* eval_elm_url_Url_Parser_removeFinalEmpty(void* args[]) {
  void* x_segments = args[0];
  void* tmp0;
  if (x_segments == &Nil) {
    tmp0 = &Nil;
  } else if (A2(Utils_equal, Utils_destruct_index(x_segments, 0), &literal_string_) ==
                 &True &&
             Utils_destruct_index(x_segments, 1) == &Nil) {
    tmp0 = &Nil;
  } else {
    void* x_segment = Utils_destruct_index(x_segments, 0);
    void* x_rest = Utils_destruct_index(x_segments, 1);
    tmp0 = A2(
        &elm_core_List_cons, x_segment, A1(&elm_url_Url_Parser_removeFinalEmpty, x_rest));
  };
  return tmp0;
}
Closure elm_url_Url_Parser_removeFinalEmpty = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_url_Url_Parser_removeFinalEmpty,
};
void* eval_elm_url_Url_Parser_preparePath(void* args[]) {
  void* x_path = args[0];
  void* x__v0 = A2(&elm_core_String_split, &literal_string__2f, x_path);
  void* tmp0;
  if (x__v0 != &Nil &&
      A2(Utils_equal, Utils_destruct_index(x__v0, 0), &literal_string_) == &True) {
    void* x_segments = Utils_destruct_index(x__v0, 1);
    tmp0 = A1(&elm_url_Url_Parser_removeFinalEmpty, x_segments);
  } else {
    void* x_segments = x__v0;
    tmp0 = A1(&elm_url_Url_Parser_removeFinalEmpty, x_segments);
  };
  return tmp0;
}
Closure elm_url_Url_Parser_preparePath = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_url_Url_Parser_preparePath,
};

void* eval_elm_url_Url_Parser_addToParametersHelp(void* args[]) {
  void* x_value = args[0];
  void* x_maybeList = args[1];
  void* tmp0;
  void* tmp1 = ((Custom*)x_maybeList)->ctor;
  if (tmp1 == CTOR_Nothing) {
    tmp0 = A1(&elm_core_Maybe_Just,
        List_fromArray(1,
            ((void* []){
                x_value,
            })));
  } else {
    void* x_list = Utils_destruct_index(x_maybeList, 0);
    tmp0 = A1(&elm_core_Maybe_Just, A2(&elm_core_List_cons, x_value, x_list));
  };
  return tmp0;
}
Closure elm_url_Url_Parser_addToParametersHelp = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_url_Url_Parser_addToParametersHelp,
};

#define elm_url_Url_percentDecode Url_percentDecode

#define elm_core_Basics_compare Utils_compare
void* tce_elm_core_Dict_get(void* args[], void** gc_tce_data) {
tce_loop:;
  void* x_targetKey = args[0];
  void* x_dict = args[1];
  void* tmp0;
  void* tmp1 = ((Custom*)x_dict)->ctor;
  if (tmp1 == CTOR_RBEmpty_elm_builtin) {
    tmp0 = &elm_core_Maybe_Nothing;
  } else {
    void* x_key = Utils_destruct_index(x_dict, 1);
    void* x_value = Utils_destruct_index(x_dict, 2);
    void* x_left = Utils_destruct_index(x_dict, 3);
    void* x_right = Utils_destruct_index(x_dict, 4);
    void* x__v1 = A2(&elm_core_Basics_compare, x_targetKey, x_key);
    void* tmp2;
    void* tmp3 = ((Custom*)x__v1)->ctor;
    if (tmp3 == CTOR_LT) {
      {
        void* tmp6 = x_targetKey;
        void* tmp7 = x_left;
        *gc_tce_data = CAN_THROW(GC_tce_iteration(2));
        args[1] = tmp7;
        args[0] = tmp6;
        goto tce_loop;
        tmp2 = NULL;
      };
    } else if (tmp3 == CTOR_EQ) {
      { tmp2 = A1(&elm_core_Maybe_Just, x_value); };
    } else {
      void* tmp4 = x_targetKey;
      void* tmp5 = x_right;
      *gc_tce_data = CAN_THROW(GC_tce_iteration(2));
      args[1] = tmp5;
      args[0] = tmp4;
      goto tce_loop;
      tmp2 = NULL;
    };
    tmp0 = tmp2;
  };
  return tmp0;
}
void* eval_elm_core_Dict_get() {
  return GC_tce_eval(&tce_elm_core_Dict_get, &eval_elm_core_Dict_get, 2, args);
}
Closure elm_core_Dict_get = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_core_Dict_get,
};

Custom elm_core_Dict_Black = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Black,
};

void* eval_elm_core_Dict_RBNode_elm_builtin(void* args[]) {
  return ctorCustom(CTOR_RBNode_elm_builtin, 5, args);
}
Closure elm_core_Dict_RBNode_elm_builtin = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x5,
    .evaluator = &eval_elm_core_Dict_RBNode_elm_builtin,
};

Custom elm_core_Dict_RBEmpty_elm_builtin = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_RBEmpty_elm_builtin,
};

Custom elm_core_Dict_Red = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Red,
};

void* eval_elm_core_Dict_balance(void* args[]) {
  void* x_color = args[0];
  void* x_key = args[1];
  void* x_value = args[2];
  void* x_left = args[3];
  void* x_right = args[4];
  void* tmp0;
  void* tmp1 = ((Custom*)Utils_destruct_index(x_right, 0))->ctor;
  void* tmp2 = ((Custom*)x_right)->ctor;
  if (tmp2 == CTOR_RBNode_elm_builtin && tmp1 == CTOR_Red) {
    void* x__v1 = Utils_destruct_index(x_right, 0);
    void* x_rK = Utils_destruct_index(x_right, 1);
    void* x_rV = Utils_destruct_index(x_right, 2);
    void* x_rLeft = Utils_destruct_index(x_right, 3);
    void* x_rRight = Utils_destruct_index(x_right, 4);
    void* tmp3;
    void* tmp4 = ((Custom*)Utils_destruct_index(x_left, 0))->ctor;
    void* tmp5 = ((Custom*)x_left)->ctor;
    if (tmp5 == CTOR_RBNode_elm_builtin && tmp4 == CTOR_Red) {
      void* x__v3 = Utils_destruct_index(x_left, 0);
      void* x_lK = Utils_destruct_index(x_left, 1);
      void* x_lV = Utils_destruct_index(x_left, 2);
      void* x_lLeft = Utils_destruct_index(x_left, 3);
      void* x_lRight = Utils_destruct_index(x_left, 4);
      tmp3 = A5(&elm_core_Dict_RBNode_elm_builtin,
          &elm_core_Dict_Red,
          x_key,
          x_value,
          A5(&elm_core_Dict_RBNode_elm_builtin,
              &elm_core_Dict_Black,
              x_lK,
              x_lV,
              x_lLeft,
              x_lRight),
          A5(&elm_core_Dict_RBNode_elm_builtin,
              &elm_core_Dict_Black,
              x_rK,
              x_rV,
              x_rLeft,
              x_rRight));
    } else {
      tmp3 = A5(&elm_core_Dict_RBNode_elm_builtin,
          x_color,
          x_rK,
          x_rV,
          A5(&elm_core_Dict_RBNode_elm_builtin,
              &elm_core_Dict_Red,
              x_key,
              x_value,
              x_left,
              x_rLeft),
          x_rRight);
    };
    tmp0 = tmp3;
  } else {
    void* tmp6;
    void* tmp7 =
        ((Custom*)Utils_destruct_index(Utils_destruct_index(x_left, 3), 0))->ctor;
    void* tmp8 = ((Custom*)Utils_destruct_index(x_left, 3))->ctor;
    void* tmp9 = ((Custom*)Utils_destruct_index(x_left, 0))->ctor;
    void* tmp10 = ((Custom*)x_left)->ctor;
    if (tmp10 == CTOR_RBNode_elm_builtin && tmp9 == CTOR_Red &&
        tmp8 == CTOR_RBNode_elm_builtin && tmp7 == CTOR_Red) {
      void* x__v5 = Utils_destruct_index(x_left, 0);
      void* x_lK = Utils_destruct_index(x_left, 1);
      void* x_lV = Utils_destruct_index(x_left, 2);
      void* x__v6 = Utils_destruct_index(x_left, 3);
      void* x__v7 = Utils_destruct_index(x__v6, 0);
      void* x_llK = Utils_destruct_index(x__v6, 1);
      void* x_llV = Utils_destruct_index(x__v6, 2);
      void* x_llLeft = Utils_destruct_index(x__v6, 3);
      void* x_llRight = Utils_destruct_index(x__v6, 4);
      void* x_lRight = Utils_destruct_index(x_left, 4);
      tmp6 = A5(&elm_core_Dict_RBNode_elm_builtin,
          &elm_core_Dict_Red,
          x_lK,
          x_lV,
          A5(&elm_core_Dict_RBNode_elm_builtin,
              &elm_core_Dict_Black,
              x_llK,
              x_llV,
              x_llLeft,
              x_llRight),
          A5(&elm_core_Dict_RBNode_elm_builtin,
              &elm_core_Dict_Black,
              x_key,
              x_value,
              x_lRight,
              x_right));
    } else {
      tmp6 =
          A5(&elm_core_Dict_RBNode_elm_builtin, x_color, x_key, x_value, x_left, x_right);
    };
    tmp0 = tmp6;
  };
  return tmp0;
}
Closure elm_core_Dict_balance = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x5,
    .evaluator = &eval_elm_core_Dict_balance,
};
void* eval_elm_core_Dict_insertHelp(void* args[]) {
  void* x_key = args[0];
  void* x_value = args[1];
  void* x_dict = args[2];
  void* tmp0;
  void* tmp1 = ((Custom*)x_dict)->ctor;
  if (tmp1 == CTOR_RBEmpty_elm_builtin) {
    tmp0 = A5(&elm_core_Dict_RBNode_elm_builtin,
        &elm_core_Dict_Red,
        x_key,
        x_value,
        &elm_core_Dict_RBEmpty_elm_builtin,
        &elm_core_Dict_RBEmpty_elm_builtin);
  } else {
    void* x_nColor = Utils_destruct_index(x_dict, 0);
    void* x_nKey = Utils_destruct_index(x_dict, 1);
    void* x_nValue = Utils_destruct_index(x_dict, 2);
    void* x_nLeft = Utils_destruct_index(x_dict, 3);
    void* x_nRight = Utils_destruct_index(x_dict, 4);
    void* x__v1 = A2(&elm_core_Basics_compare, x_key, x_nKey);
    void* tmp2;
    void* tmp3 = ((Custom*)x__v1)->ctor;
    if (tmp3 == CTOR_LT) {
      {
        tmp2 = A5(&elm_core_Dict_balance,
            x_nColor,
            x_nKey,
            x_nValue,
            A3(&elm_core_Dict_insertHelp, x_key, x_value, x_nLeft),
            x_nRight);
      };
    } else if (tmp3 == CTOR_EQ) {
      {
        tmp2 = A5(&elm_core_Dict_RBNode_elm_builtin,
            x_nColor,
            x_nKey,
            x_value,
            x_nLeft,
            x_nRight);
      };
    } else {
      tmp2 = A5(&elm_core_Dict_balance,
          x_nColor,
          x_nKey,
          x_nValue,
          x_nLeft,
          A3(&elm_core_Dict_insertHelp, x_key, x_value, x_nRight));
    };
    tmp0 = tmp2;
  };
  return tmp0;
}
Closure elm_core_Dict_insertHelp = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_elm_core_Dict_insertHelp,
};
void* eval_elm_core_Dict_insert(void* args[]) {
  void* x_key = args[0];
  void* x_value = args[1];
  void* x_dict = args[2];
  void* x__v0 = A3(&elm_core_Dict_insertHelp, x_key, x_value, x_dict);
  void* tmp0;
  void* tmp1 = ((Custom*)Utils_destruct_index(x__v0, 0))->ctor;
  void* tmp2 = ((Custom*)x__v0)->ctor;
  if (tmp2 == CTOR_RBNode_elm_builtin && tmp1 == CTOR_Red) {
    void* x__v1 = Utils_destruct_index(x__v0, 0);
    void* x_k = Utils_destruct_index(x__v0, 1);
    void* x_v = Utils_destruct_index(x__v0, 2);
    void* x_l = Utils_destruct_index(x__v0, 3);
    void* x_r = Utils_destruct_index(x__v0, 4);
    tmp0 =
        A5(&elm_core_Dict_RBNode_elm_builtin, &elm_core_Dict_Black, x_k, x_v, x_l, x_r);
  } else {
    void* x_x = x__v0;
    tmp0 = x_x;
  };
  return tmp0;
}
Closure elm_core_Dict_insert = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_elm_core_Dict_insert,
};

void* tce_elm_core_Dict_getMin(void* args[], void** gc_tce_data) {
tce_loop:;
  void* x_dict = args[0];
  void* tmp0;
  void* tmp1 = ((Custom*)Utils_destruct_index(x_dict, 3))->ctor;
  void* tmp2 = ((Custom*)x_dict)->ctor;
  if (tmp2 == CTOR_RBNode_elm_builtin && tmp1 == CTOR_RBNode_elm_builtin) {
    void* x_left = Utils_destruct_index(x_dict, 3);
    void* tmp3 = x_left;
    *gc_tce_data = CAN_THROW(GC_tce_iteration(1));
    args[0] = tmp3;
    goto tce_loop;
    tmp0 = NULL;
  } else {
    tmp0 = x_dict;
  };
  return tmp0;
}
void* eval_elm_core_Dict_getMin() {
  return GC_tce_eval(&tce_elm_core_Dict_getMin, &eval_elm_core_Dict_getMin, 1, args);
}
Closure elm_core_Dict_getMin = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_core_Dict_getMin,
};

void* eval_elm_core_Dict_moveRedLeft(void* args[]) {
  void* x_dict = args[0];
  void* tmp0;
  void* tmp1 = ((Custom*)Utils_destruct_index(x_dict, 4))->ctor;
  void* tmp2 = ((Custom*)Utils_destruct_index(x_dict, 3))->ctor;
  void* tmp3 = ((Custom*)x_dict)->ctor;
  void* tmp4 = ((Custom*)Utils_destruct_index(
                    Utils_destruct_index(Utils_destruct_index(x_dict, 4), 3), 0))
                   ->ctor;
  void* tmp5 = ((Custom*)Utils_destruct_index(Utils_destruct_index(x_dict, 4), 3))->ctor;
  if (tmp3 == CTOR_RBNode_elm_builtin && tmp2 == CTOR_RBNode_elm_builtin &&
      tmp1 == CTOR_RBNode_elm_builtin)
    if (tmp5 == CTOR_RBNode_elm_builtin && tmp4 == CTOR_Red) {
      void* x_clr = Utils_destruct_index(x_dict, 0);
      void* x_k = Utils_destruct_index(x_dict, 1);
      void* x_v = Utils_destruct_index(x_dict, 2);
      void* x__v1 = Utils_destruct_index(x_dict, 3);
      void* x_lClr = Utils_destruct_index(x__v1, 0);
      void* x_lK = Utils_destruct_index(x__v1, 1);
      void* x_lV = Utils_destruct_index(x__v1, 2);
      void* x_lLeft = Utils_destruct_index(x__v1, 3);
      void* x_lRight = Utils_destruct_index(x__v1, 4);
      void* x__v2 = Utils_destruct_index(x_dict, 4);
      void* x_rClr = Utils_destruct_index(x__v2, 0);
      void* x_rK = Utils_destruct_index(x__v2, 1);
      void* x_rV = Utils_destruct_index(x__v2, 2);
      void* x_rLeft = Utils_destruct_index(x__v2, 3);
      void* x__v3 = Utils_destruct_index(x_rLeft, 0);
      void* x_rlK = Utils_destruct_index(x_rLeft, 1);
      void* x_rlV = Utils_destruct_index(x_rLeft, 2);
      void* x_rlL = Utils_destruct_index(x_rLeft, 3);
      void* x_rlR = Utils_destruct_index(x_rLeft, 4);
      void* x_rRight = Utils_destruct_index(x__v2, 4);
      tmp0 = A5(&elm_core_Dict_RBNode_elm_builtin,
          &elm_core_Dict_Red,
          x_rlK,
          x_rlV,
          A5(&elm_core_Dict_RBNode_elm_builtin,
              &elm_core_Dict_Black,
              x_k,
              x_v,
              A5(&elm_core_Dict_RBNode_elm_builtin,
                  &elm_core_Dict_Red,
                  x_lK,
                  x_lV,
                  x_lLeft,
                  x_lRight),
              x_rlL),
          A5(&elm_core_Dict_RBNode_elm_builtin,
              &elm_core_Dict_Black,
              x_rK,
              x_rV,
              x_rlR,
              x_rRight));
    } else {
      void* x_clr = Utils_destruct_index(x_dict, 0);
      void* x_k = Utils_destruct_index(x_dict, 1);
      void* x_v = Utils_destruct_index(x_dict, 2);
      void* x__v4 = Utils_destruct_index(x_dict, 3);
      void* x_lClr = Utils_destruct_index(x__v4, 0);
      void* x_lK = Utils_destruct_index(x__v4, 1);
      void* x_lV = Utils_destruct_index(x__v4, 2);
      void* x_lLeft = Utils_destruct_index(x__v4, 3);
      void* x_lRight = Utils_destruct_index(x__v4, 4);
      void* x__v5 = Utils_destruct_index(x_dict, 4);
      void* x_rClr = Utils_destruct_index(x__v5, 0);
      void* x_rK = Utils_destruct_index(x__v5, 1);
      void* x_rV = Utils_destruct_index(x__v5, 2);
      void* x_rLeft = Utils_destruct_index(x__v5, 3);
      void* x_rRight = Utils_destruct_index(x__v5, 4);
      void* tmp6;
      void* tmp7 = ((Custom*)x_clr)->ctor;
      if (tmp7 == CTOR_Black) {
        tmp6 = A5(&elm_core_Dict_RBNode_elm_builtin,
            &elm_core_Dict_Black,
            x_k,
            x_v,
            A5(&elm_core_Dict_RBNode_elm_builtin,
                &elm_core_Dict_Red,
                x_lK,
                x_lV,
                x_lLeft,
                x_lRight),
            A5(&elm_core_Dict_RBNode_elm_builtin,
                &elm_core_Dict_Red,
                x_rK,
                x_rV,
                x_rLeft,
                x_rRight));
      } else {
        tmp6 = A5(&elm_core_Dict_RBNode_elm_builtin,
            &elm_core_Dict_Black,
            x_k,
            x_v,
            A5(&elm_core_Dict_RBNode_elm_builtin,
                &elm_core_Dict_Red,
                x_lK,
                x_lV,
                x_lLeft,
                x_lRight),
            A5(&elm_core_Dict_RBNode_elm_builtin,
                &elm_core_Dict_Red,
                x_rK,
                x_rV,
                x_rLeft,
                x_rRight));
      };
      tmp0 = tmp6;
    }
  else {
    tmp0 = x_dict;
  };
  return tmp0;
}
Closure elm_core_Dict_moveRedLeft = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_core_Dict_moveRedLeft,
};

void* eval_elm_core_Dict_moveRedRight(void* args[]) {
  void* x_dict = args[0];
  void* tmp0;
  void* tmp1 = ((Custom*)Utils_destruct_index(x_dict, 4))->ctor;
  void* tmp2 = ((Custom*)Utils_destruct_index(x_dict, 3))->ctor;
  void* tmp3 = ((Custom*)x_dict)->ctor;
  void* tmp4 = ((Custom*)Utils_destruct_index(
                    Utils_destruct_index(Utils_destruct_index(x_dict, 3), 3), 0))
                   ->ctor;
  void* tmp5 = ((Custom*)Utils_destruct_index(Utils_destruct_index(x_dict, 3), 3))->ctor;
  if (tmp3 == CTOR_RBNode_elm_builtin && tmp2 == CTOR_RBNode_elm_builtin &&
      tmp1 == CTOR_RBNode_elm_builtin)
    if (tmp5 == CTOR_RBNode_elm_builtin && tmp4 == CTOR_Red) {
      void* x_clr = Utils_destruct_index(x_dict, 0);
      void* x_k = Utils_destruct_index(x_dict, 1);
      void* x_v = Utils_destruct_index(x_dict, 2);
      void* x__v1 = Utils_destruct_index(x_dict, 3);
      void* x_lClr = Utils_destruct_index(x__v1, 0);
      void* x_lK = Utils_destruct_index(x__v1, 1);
      void* x_lV = Utils_destruct_index(x__v1, 2);
      void* x__v2 = Utils_destruct_index(x__v1, 3);
      void* x__v3 = Utils_destruct_index(x__v2, 0);
      void* x_llK = Utils_destruct_index(x__v2, 1);
      void* x_llV = Utils_destruct_index(x__v2, 2);
      void* x_llLeft = Utils_destruct_index(x__v2, 3);
      void* x_llRight = Utils_destruct_index(x__v2, 4);
      void* x_lRight = Utils_destruct_index(x__v1, 4);
      void* x__v4 = Utils_destruct_index(x_dict, 4);
      void* x_rClr = Utils_destruct_index(x__v4, 0);
      void* x_rK = Utils_destruct_index(x__v4, 1);
      void* x_rV = Utils_destruct_index(x__v4, 2);
      void* x_rLeft = Utils_destruct_index(x__v4, 3);
      void* x_rRight = Utils_destruct_index(x__v4, 4);
      tmp0 = A5(&elm_core_Dict_RBNode_elm_builtin,
          &elm_core_Dict_Red,
          x_lK,
          x_lV,
          A5(&elm_core_Dict_RBNode_elm_builtin,
              &elm_core_Dict_Black,
              x_llK,
              x_llV,
              x_llLeft,
              x_llRight),
          A5(&elm_core_Dict_RBNode_elm_builtin,
              &elm_core_Dict_Black,
              x_k,
              x_v,
              x_lRight,
              A5(&elm_core_Dict_RBNode_elm_builtin,
                  &elm_core_Dict_Red,
                  x_rK,
                  x_rV,
                  x_rLeft,
                  x_rRight)));
    } else {
      void* x_clr = Utils_destruct_index(x_dict, 0);
      void* x_k = Utils_destruct_index(x_dict, 1);
      void* x_v = Utils_destruct_index(x_dict, 2);
      void* x__v5 = Utils_destruct_index(x_dict, 3);
      void* x_lClr = Utils_destruct_index(x__v5, 0);
      void* x_lK = Utils_destruct_index(x__v5, 1);
      void* x_lV = Utils_destruct_index(x__v5, 2);
      void* x_lLeft = Utils_destruct_index(x__v5, 3);
      void* x_lRight = Utils_destruct_index(x__v5, 4);
      void* x__v6 = Utils_destruct_index(x_dict, 4);
      void* x_rClr = Utils_destruct_index(x__v6, 0);
      void* x_rK = Utils_destruct_index(x__v6, 1);
      void* x_rV = Utils_destruct_index(x__v6, 2);
      void* x_rLeft = Utils_destruct_index(x__v6, 3);
      void* x_rRight = Utils_destruct_index(x__v6, 4);
      void* tmp6;
      void* tmp7 = ((Custom*)x_clr)->ctor;
      if (tmp7 == CTOR_Black) {
        tmp6 = A5(&elm_core_Dict_RBNode_elm_builtin,
            &elm_core_Dict_Black,
            x_k,
            x_v,
            A5(&elm_core_Dict_RBNode_elm_builtin,
                &elm_core_Dict_Red,
                x_lK,
                x_lV,
                x_lLeft,
                x_lRight),
            A5(&elm_core_Dict_RBNode_elm_builtin,
                &elm_core_Dict_Red,
                x_rK,
                x_rV,
                x_rLeft,
                x_rRight));
      } else {
        tmp6 = A5(&elm_core_Dict_RBNode_elm_builtin,
            &elm_core_Dict_Black,
            x_k,
            x_v,
            A5(&elm_core_Dict_RBNode_elm_builtin,
                &elm_core_Dict_Red,
                x_lK,
                x_lV,
                x_lLeft,
                x_lRight),
            A5(&elm_core_Dict_RBNode_elm_builtin,
                &elm_core_Dict_Red,
                x_rK,
                x_rV,
                x_rLeft,
                x_rRight));
      };
      tmp0 = tmp6;
    }
  else {
    tmp0 = x_dict;
  };
  return tmp0;
}
Closure elm_core_Dict_moveRedRight = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_core_Dict_moveRedRight,
};
void* eval_elm_core_Dict_removeHelpPrepEQGT(void* args[]) {
  void* x_targetKey = args[0];
  void* x_dict = args[1];
  void* x_color = args[2];
  void* x_key = args[3];
  void* x_value = args[4];
  void* x_left = args[5];
  void* x_right = args[6];
  void* tmp0;
  void* tmp1 = ((Custom*)Utils_destruct_index(x_left, 0))->ctor;
  void* tmp2 = ((Custom*)x_left)->ctor;
  if (tmp2 == CTOR_RBNode_elm_builtin && tmp1 == CTOR_Red) {
    void* x__v1 = Utils_destruct_index(x_left, 0);
    void* x_lK = Utils_destruct_index(x_left, 1);
    void* x_lV = Utils_destruct_index(x_left, 2);
    void* x_lLeft = Utils_destruct_index(x_left, 3);
    void* x_lRight = Utils_destruct_index(x_left, 4);
    tmp0 = A5(&elm_core_Dict_RBNode_elm_builtin,
        x_color,
        x_lK,
        x_lV,
        x_lLeft,
        A5(&elm_core_Dict_RBNode_elm_builtin,
            &elm_core_Dict_Red,
            x_key,
            x_value,
            x_lRight,
            x_right));
  } else {
    void* tmp3;
    void* tmp4 = ((Custom*)Utils_destruct_index(x_right, 0))->ctor;
    void* tmp5 = ((Custom*)x_right)->ctor;
    void* tmp6 = ((Custom*)Utils_destruct_index(x_right, 3))->ctor;
    void* tmp7 =
        ((Custom*)Utils_destruct_index(Utils_destruct_index(x_right, 3), 0))->ctor;
    if (tmp5 == CTOR_RBNode_elm_builtin && tmp4 == CTOR_Black)
      if (tmp6 == CTOR_RBNode_elm_builtin)
        if (tmp7 == CTOR_Black) {
          void* x__v3 = Utils_destruct_index(x_right, 0);
          void* x__v4 = Utils_destruct_index(x_right, 3);
          void* x__v5 = Utils_destruct_index(x__v4, 0);
          tmp3 = A1(&elm_core_Dict_moveRedRight, x_dict);
        } else {
          goto _v2_2;
        }
      else {
        void* x__v6 = Utils_destruct_index(x_right, 0);
        void* x__v7 = Utils_destruct_index(x_right, 3);
        tmp3 = A1(&elm_core_Dict_moveRedRight, x_dict);
      }
    else {
      goto _v2_2;
    };
  _v2_2:
    tmp3 = x_dict;
    tmp0 = tmp3;
  };
  return tmp0;
}
Closure elm_core_Dict_removeHelpPrepEQGT = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x7,
    .evaluator = &eval_elm_core_Dict_removeHelpPrepEQGT,
};

void* eval_elm_core_Dict_removeMin(void* args[]) {
  void* x_dict = args[0];
  void* tmp0;
  void* tmp1 = ((Custom*)Utils_destruct_index(x_dict, 3))->ctor;
  void* tmp2 = ((Custom*)x_dict)->ctor;
  if (tmp2 == CTOR_RBNode_elm_builtin && tmp1 == CTOR_RBNode_elm_builtin) {
    void* x_color = Utils_destruct_index(x_dict, 0);
    void* x_key = Utils_destruct_index(x_dict, 1);
    void* x_value = Utils_destruct_index(x_dict, 2);
    void* x_left = Utils_destruct_index(x_dict, 3);
    void* x_lColor = Utils_destruct_index(x_left, 0);
    void* x_lLeft = Utils_destruct_index(x_left, 3);
    void* x_right = Utils_destruct_index(x_dict, 4);
    void* tmp3;
    void* tmp4 = ((Custom*)x_lColor)->ctor;
    if (tmp4 == CTOR_Black) {
      void* tmp5;
      void* tmp6 = ((Custom*)Utils_destruct_index(x_lLeft, 0))->ctor;
      void* tmp7 = ((Custom*)x_lLeft)->ctor;
      if (tmp7 == CTOR_RBNode_elm_builtin && tmp6 == CTOR_Red) {
        void* x__v3 = Utils_destruct_index(x_lLeft, 0);
        tmp5 = A5(&elm_core_Dict_RBNode_elm_builtin,
            x_color,
            x_key,
            x_value,
            A1(&elm_core_Dict_removeMin, x_left),
            x_right);
      } else {
        void* x__v4 = A1(&elm_core_Dict_moveRedLeft, x_dict);
        void* tmp8;
        void* tmp9 = ((Custom*)x__v4)->ctor;
        if (tmp9 == CTOR_RBNode_elm_builtin) {
          void* x_nColor = Utils_destruct_index(x__v4, 0);
          void* x_nKey = Utils_destruct_index(x__v4, 1);
          void* x_nValue = Utils_destruct_index(x__v4, 2);
          void* x_nLeft = Utils_destruct_index(x__v4, 3);
          void* x_nRight = Utils_destruct_index(x__v4, 4);
          tmp8 = A5(&elm_core_Dict_balance,
              x_nColor,
              x_nKey,
              x_nValue,
              A1(&elm_core_Dict_removeMin, x_nLeft),
              x_nRight);
        } else {
          tmp8 = &elm_core_Dict_RBEmpty_elm_builtin;
        };
        tmp5 = tmp8;
      };
      tmp3 = tmp5;
    } else {
      tmp3 = A5(&elm_core_Dict_RBNode_elm_builtin,
          x_color,
          x_key,
          x_value,
          A1(&elm_core_Dict_removeMin, x_left),
          x_right);
    };
    tmp0 = tmp3;
  } else {
    tmp0 = &elm_core_Dict_RBEmpty_elm_builtin;
  };
  return tmp0;
}
Closure elm_core_Dict_removeMin = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_core_Dict_removeMin,
};
void* eval_elm_core_Dict_removeHelp(void* args[]) {
  void* x_targetKey = args[0];
  void* x_dict = args[1];
  void* tmp0;
  void* tmp1 = ((Custom*)x_dict)->ctor;
  if (tmp1 == CTOR_RBEmpty_elm_builtin) {
    tmp0 = &elm_core_Dict_RBEmpty_elm_builtin;
  } else {
    void* x_color = Utils_destruct_index(x_dict, 0);
    void* x_key = Utils_destruct_index(x_dict, 1);
    void* x_value = Utils_destruct_index(x_dict, 2);
    void* x_left = Utils_destruct_index(x_dict, 3);
    void* x_right = Utils_destruct_index(x_dict, 4);
    void* tmp2;
    if (A2(&elm_core_Basics_lt, x_targetKey, x_key) == &True) {
      void* tmp3;
      void* tmp4 = ((Custom*)Utils_destruct_index(x_left, 0))->ctor;
      void* tmp5 = ((Custom*)x_left)->ctor;
      if (tmp5 == CTOR_RBNode_elm_builtin && tmp4 == CTOR_Black) {
        void* x__v4 = Utils_destruct_index(x_left, 0);
        void* x_lLeft = Utils_destruct_index(x_left, 3);
        void* tmp6;
        void* tmp7 = ((Custom*)Utils_destruct_index(x_lLeft, 0))->ctor;
        void* tmp8 = ((Custom*)x_lLeft)->ctor;
        if (tmp8 == CTOR_RBNode_elm_builtin && tmp7 == CTOR_Red) {
          void* x__v6 = Utils_destruct_index(x_lLeft, 0);
          tmp6 = A5(&elm_core_Dict_RBNode_elm_builtin,
              x_color,
              x_key,
              x_value,
              A2(&elm_core_Dict_removeHelp, x_targetKey, x_left),
              x_right);
        } else {
          void* x__v7 = A1(&elm_core_Dict_moveRedLeft, x_dict);
          void* tmp9;
          void* tmp10 = ((Custom*)x__v7)->ctor;
          if (tmp10 == CTOR_RBNode_elm_builtin) {
            void* x_nColor = Utils_destruct_index(x__v7, 0);
            void* x_nKey = Utils_destruct_index(x__v7, 1);
            void* x_nValue = Utils_destruct_index(x__v7, 2);
            void* x_nLeft = Utils_destruct_index(x__v7, 3);
            void* x_nRight = Utils_destruct_index(x__v7, 4);
            tmp9 = A5(&elm_core_Dict_balance,
                x_nColor,
                x_nKey,
                x_nValue,
                A2(&elm_core_Dict_removeHelp, x_targetKey, x_nLeft),
                x_nRight);
          } else {
            tmp9 = &elm_core_Dict_RBEmpty_elm_builtin;
          };
          tmp6 = tmp9;
        };
        tmp3 = tmp6;
      } else {
        tmp3 = A5(&elm_core_Dict_RBNode_elm_builtin,
            x_color,
            x_key,
            x_value,
            A2(&elm_core_Dict_removeHelp, x_targetKey, x_left),
            x_right);
      };
      tmp2 = tmp3;
    } else {
      tmp2 = A2(&elm_core_Dict_removeHelpEQGT,
          x_targetKey,
          A7(&elm_core_Dict_removeHelpPrepEQGT,
              x_targetKey,
              x_dict,
              x_color,
              x_key,
              x_value,
              x_left,
              x_right));
    };
    tmp0 = tmp2;
  };
  return tmp0;
}
Closure elm_core_Dict_removeHelp = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_core_Dict_removeHelp,
};
void* eval_elm_core_Dict_removeHelpEQGT(void* args[]) {
  void* x_targetKey = args[0];
  void* x_dict = args[1];
  void* tmp0;
  void* tmp1 = ((Custom*)x_dict)->ctor;
  if (tmp1 == CTOR_RBNode_elm_builtin) {
    void* x_color = Utils_destruct_index(x_dict, 0);
    void* x_key = Utils_destruct_index(x_dict, 1);
    void* x_value = Utils_destruct_index(x_dict, 2);
    void* x_left = Utils_destruct_index(x_dict, 3);
    void* x_right = Utils_destruct_index(x_dict, 4);
    void* tmp2;
    if (A2(&elm_core_Basics_eq, x_targetKey, x_key) == &True) {
      void* x__v1 = A1(&elm_core_Dict_getMin, x_right);
      void* tmp3;
      void* tmp4 = ((Custom*)x__v1)->ctor;
      if (tmp4 == CTOR_RBNode_elm_builtin) {
        void* x_minKey = Utils_destruct_index(x__v1, 1);
        void* x_minValue = Utils_destruct_index(x__v1, 2);
        tmp3 = A5(&elm_core_Dict_balance,
            x_color,
            x_minKey,
            x_minValue,
            x_left,
            A1(&elm_core_Dict_removeMin, x_right));
      } else {
        tmp3 = &elm_core_Dict_RBEmpty_elm_builtin;
      };
      tmp2 = tmp3;
    } else {
      tmp2 = A5(&elm_core_Dict_balance,
          x_color,
          x_key,
          x_value,
          x_left,
          A2(&elm_core_Dict_removeHelp, x_targetKey, x_right));
    };
    tmp0 = tmp2;
  } else {
    tmp0 = &elm_core_Dict_RBEmpty_elm_builtin;
  };
  return tmp0;
}
Closure elm_core_Dict_removeHelpEQGT = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_core_Dict_removeHelpEQGT,
};
void* eval_elm_core_Dict_remove(void* args[]) {
  void* x_key = args[0];
  void* x_dict = args[1];
  void* x__v0 = A2(&elm_core_Dict_removeHelp, x_key, x_dict);
  void* tmp0;
  void* tmp1 = ((Custom*)Utils_destruct_index(x__v0, 0))->ctor;
  void* tmp2 = ((Custom*)x__v0)->ctor;
  if (tmp2 == CTOR_RBNode_elm_builtin && tmp1 == CTOR_Red) {
    void* x__v1 = Utils_destruct_index(x__v0, 0);
    void* x_k = Utils_destruct_index(x__v0, 1);
    void* x_v = Utils_destruct_index(x__v0, 2);
    void* x_l = Utils_destruct_index(x__v0, 3);
    void* x_r = Utils_destruct_index(x__v0, 4);
    tmp0 =
        A5(&elm_core_Dict_RBNode_elm_builtin, &elm_core_Dict_Black, x_k, x_v, x_l, x_r);
  } else {
    void* x_x = x__v0;
    tmp0 = x_x;
  };
  return tmp0;
}
Closure elm_core_Dict_remove = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_core_Dict_remove,
};
void* eval_elm_core_Dict_update(void* args[]) {
  void* x_targetKey = args[0];
  void* x_alter = args[1];
  void* x_dictionary = args[2];
  void* x__v0 = A1(x_alter, A2(&elm_core_Dict_get, x_targetKey, x_dictionary));
  void* tmp0;
  void* tmp1 = ((Custom*)x__v0)->ctor;
  if (tmp1 == CTOR_Just) {
    void* x_value = Utils_destruct_index(x__v0, 0);
    tmp0 = A3(&elm_core_Dict_insert, x_targetKey, x_value, x_dictionary);
  } else {
    tmp0 = A2(&elm_core_Dict_remove, x_targetKey, x_dictionary);
  };
  return tmp0;
}
Closure elm_core_Dict_update = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_elm_core_Dict_update,
};
void* eval_elm_url_Url_Parser_addParam(void* args[]) {
  void* x_segment = args[0];
  void* x_dict = args[1];
  void* x__v0 = A2(&elm_core_String_split, &literal_string__3d, x_segment);
  void* tmp0;
  if (x__v0 != &Nil && Utils_destruct_index(x__v0, 1) != &Nil &&
      Utils_destruct_index(Utils_destruct_index(x__v0, 1), 1) == &Nil) {
    void* x_rawKey = Utils_destruct_index(x__v0, 0);
    void* x__v1 = Utils_destruct_index(x__v0, 1);
    void* x_rawValue = Utils_destruct_index(x__v1, 0);
    void* x__v2 = A1(&elm_url_Url_percentDecode, x_rawKey);
    void* tmp1;
    void* tmp2 = ((Custom*)x__v2)->ctor;
    if (tmp2 == CTOR_Nothing) {
      tmp1 = x_dict;
    } else {
      void* x_key = Utils_destruct_index(x__v2, 0);
      void* x__v3 = A1(&elm_url_Url_percentDecode, x_rawValue);
      void* tmp3;
      void* tmp4 = ((Custom*)x__v3)->ctor;
      if (tmp4 == CTOR_Nothing) {
        tmp3 = x_dict;
      } else {
        void* x_value = Utils_destruct_index(x__v3, 0);
        tmp3 = A3(&elm_core_Dict_update,
            x_key,
            A1(&elm_url_Url_Parser_addToParametersHelp, x_value),
            x_dict);
      };
      tmp1 = tmp3;
    };
    tmp0 = tmp1;
  } else {
    tmp0 = x_dict;
  };
  return tmp0;
}
Closure elm_url_Url_Parser_addParam = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_url_Url_Parser_addParam,
};

#define elm_core_Dict_empty elm_core_Dict_RBEmpty_elm_builtin
void* eval_elm_url_Url_Parser_prepareQuery(void* args[]) {
  void* x_maybeQuery = args[0];
  void* tmp0;
  void* tmp1 = ((Custom*)x_maybeQuery)->ctor;
  if (tmp1 == CTOR_Nothing) {
    tmp0 = &elm_core_Dict_empty;
  } else {
    void* x_qry = Utils_destruct_index(x_maybeQuery, 0);
    tmp0 = A3(&elm_core_List_foldr,
        &elm_url_Url_Parser_addParam,
        &elm_core_Dict_empty,
        A2(&elm_core_String_split, &literal_string__26, x_qry));
  };
  return tmp0;
}
Closure elm_url_Url_Parser_prepareQuery = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_url_Url_Parser_prepareQuery,
};
void* eval_elm_url_Url_Parser_parse(void* args[]) {
  void* x__v0 = args[0];
  void* x_url = args[1];
  void* x_parser = ((Custom*)x__v0)->values[0];
  return A2(&elm_core_Basics_apL,
      &elm_url_Url_Parser_getFirstMatch,
      A2(&elm_core_Basics_apL,
          x_parser,
          A5(&elm_url_Url_Parser_State,
              &Nil,
              A1(&elm_url_Url_Parser_preparePath,
                  Utils_access_eval(((void* []){
                      (void*)FIELD_path,
                      x_url,
                  }))),
              A1(&elm_url_Url_Parser_prepareQuery,
                  Utils_access_eval(((void* []){
                      (void*)FIELD_query,
                      x_url,
                  }))),
              Utils_access_eval(((void* []){
                  (void*)FIELD_fragment,
                  x_url,
              })),
              &elm_core_Basics_identity)));
}
Closure elm_url_Url_Parser_parse = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_url_Url_Parser_parse,
};

void* eval_author_project_Route_Article(void* args[]) {
  return ctorCustom(CTOR_Article, 1, args);
}
Closure author_project_Route_Article = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Route_Article,
};

void* eval_author_project_Route_EditArticle(void* args[]) {
  return ctorCustom(CTOR_EditArticle, 1, args);
}
Closure author_project_Route_EditArticle = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Route_EditArticle,
};

Custom author_project_Route_Login = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Login,
};

Custom author_project_Route_Logout = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Logout,
};

Custom author_project_Route_NewArticle = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_NewArticle,
};

void* eval_author_project_Route_Profile(void* args[]) {
  return ctorCustom(CTOR_Profile, 1, args);
}
Closure author_project_Route_Profile = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Route_Profile,
};

Custom author_project_Route_Register = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Register,
};

Custom author_project_Route_Settings = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Settings,
};

void* eval_elm_url_Url_Parser_Parser(void* args[]) {
  return ctorCustom(CTOR_Parser, 1, args);
}
Closure elm_url_Url_Parser_Parser = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_url_Url_Parser_Parser,
};

void* eval_elm_url_Url_Parser_mapState(void* args[]) {
  void* x_func = args[0];
  void* x__v0 = args[1];
  void* x_visited = Utils_access_eval(((void* []){
      (void*)FIELD_visited,
      x__v0,
  }));
  void* x_unvisited = Utils_access_eval(((void* []){
      (void*)FIELD_unvisited,
      x__v0,
  }));
  void* x_params = Utils_access_eval(((void* []){
      (void*)FIELD_params,
      x__v0,
  }));
  void* x_frag = Utils_access_eval(((void* []){
      (void*)FIELD_frag,
      x__v0,
  }));
  void* x_value = Utils_access_eval(((void* []){
      (void*)FIELD_value,
      x__v0,
  }));
  return A5(&elm_url_Url_Parser_State,
      x_visited,
      x_unvisited,
      x_params,
      x_frag,
      A1(x_func, x_value));
}
Closure elm_url_Url_Parser_mapState = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_url_Url_Parser_mapState,
};
void* eval_elm_url_Url_Parser_map_lambda0(void* args[]) {
  void* x_parseArg = args[0];
  void* x_subValue = args[1];
  void* x__v1 = args[2];
  void* x_visited = Utils_access_eval(((void* []){
      (void*)FIELD_visited,
      x__v1,
  }));
  void* x_unvisited = Utils_access_eval(((void* []){
      (void*)FIELD_unvisited,
      x__v1,
  }));
  void* x_params = Utils_access_eval(((void* []){
      (void*)FIELD_params,
      x__v1,
  }));
  void* x_frag = Utils_access_eval(((void* []){
      (void*)FIELD_frag,
      x__v1,
  }));
  void* x_value = Utils_access_eval(((void* []){
      (void*)FIELD_value,
      x__v1,
  }));
  return A2(&elm_core_Basics_apL,
      A1(&elm_core_List_map, A1(&elm_url_Url_Parser_mapState, x_value)),
      A2(&elm_core_Basics_apL,
          x_parseArg,
          A5(&elm_url_Url_Parser_State,
              x_visited,
              x_unvisited,
              x_params,
              x_frag,
              x_subValue)));
}
void* eval_elm_url_Url_Parser_map(void* args[]) {
  void* x_subValue = args[0];
  void* x__v0 = args[1];
  void* x_parseArg = ((Custom*)x__v0)->values[0];
  return A2(&elm_core_Basics_apL,
      &elm_url_Url_Parser_Parser,
      NEW_CLOSURE(2,
          3,
          &eval_elm_url_Url_Parser_map_lambda0,
          ((void* []){
              x_parseArg,
              x_subValue,
          })));
}
Closure elm_url_Url_Parser_map = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_url_Url_Parser_map,
};

void* eval_elm_core_List_append(void* args[]) {
  void* x_xs = args[0];
  void* x_ys = args[1];
  void* tmp0;
  if (x_ys == &Nil) {
    tmp0 = x_xs;
  } else {
    tmp0 = A3(&elm_core_List_foldr, &elm_core_List_cons, x_ys, x_xs);
  };
  return tmp0;
}
Closure elm_core_List_append = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_core_List_append,
};
void* eval_elm_core_List_concat(void* args[]) {
  void* x_lists = args[0];
  return A3(&elm_core_List_foldr, &elm_core_List_append, &Nil, x_lists);
}
Closure elm_core_List_concat = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_core_List_concat,
};
void* eval_elm_core_List_concatMap(void* args[]) {
  void* x_f = args[0];
  void* x_list = args[1];
  return A1(&elm_core_List_concat, A2(&elm_core_List_map, x_f, x_list));
}
Closure elm_core_List_concatMap = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_core_List_concatMap,
};
void* eval_elm_url_Url_Parser_oneOf_lambda1(void* args[]) {
  void* x_state = args[0];
  void* x__v0 = args[1];
  void* x_parser = ((Custom*)x__v0)->values[0];
  return A1(x_parser, x_state);
}
void* eval_elm_url_Url_Parser_oneOf_lambda0(void* args[]) {
  void* x_parsers = args[0];
  void* x_state = args[1];
  return A2(&elm_core_List_concatMap,
      NEW_CLOSURE(1,
          2,
          &eval_elm_url_Url_Parser_oneOf_lambda1,
          ((void* []){
              x_state,
          })),
      x_parsers);
}
void* eval_elm_url_Url_Parser_oneOf(void* args[]) {
  void* x_parsers = args[0];
  return A2(&elm_core_Basics_apL,
      &elm_url_Url_Parser_Parser,
      NEW_CLOSURE(1,
          2,
          &eval_elm_url_Url_Parser_oneOf_lambda0,
          ((void* []){
              x_parsers,
          })));
}
Closure elm_url_Url_Parser_oneOf = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_url_Url_Parser_oneOf,
};

void* eval_elm_url_Url_Parser_s_lambda0(void* args[]) {
  void* x_str = args[0];
  void* x__v0 = args[1];
  void* x_visited = Utils_access_eval(((void* []){
      (void*)FIELD_visited,
      x__v0,
  }));
  void* x_unvisited = Utils_access_eval(((void* []){
      (void*)FIELD_unvisited,
      x__v0,
  }));
  void* x_params = Utils_access_eval(((void* []){
      (void*)FIELD_params,
      x__v0,
  }));
  void* x_frag = Utils_access_eval(((void* []){
      (void*)FIELD_frag,
      x__v0,
  }));
  void* x_value = Utils_access_eval(((void* []){
      (void*)FIELD_value,
      x__v0,
  }));
  void* tmp1;
  if (x_unvisited == &Nil) {
    tmp1 = &Nil;
  } else {
    void* x_next = Utils_destruct_index(x_unvisited, 0);
    void* x_rest = Utils_destruct_index(x_unvisited, 1);
    void* tmp2;
    if (A2(&elm_core_Basics_eq, x_next, x_str) == &True) {
      tmp2 = List_fromArray(1,
          ((void* []){
              A5(&elm_url_Url_Parser_State,
                  A2(&elm_core_List_cons, x_next, x_visited),
                  x_rest,
                  x_params,
                  x_frag,
                  x_value),
          }));
    } else {
      tmp2 = &Nil;
    };
    tmp1 = tmp2;
  };
  return tmp1;
}
void* eval_elm_url_Url_Parser_s(void* args[]) {
  void* x_str = args[0];
  return A2(&elm_core_Basics_apL,
      &elm_url_Url_Parser_Parser,
      NEW_CLOSURE(1,
          2,
          &eval_elm_url_Url_Parser_s_lambda0,
          ((void* []){
              x_str,
          })));
}
Closure elm_url_Url_Parser_s = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_url_Url_Parser_s,
};

void* eval_elm_url_Url_Parser_slash_lambda0(void* args[]) {
  void* x_parseAfter = args[0];
  void* x_parseBefore = args[1];
  void* x_state = args[2];
  return A2(&elm_core_List_concatMap, x_parseAfter, A1(x_parseBefore, x_state));
}
void* eval_elm_url_Url_Parser_slash(void* args[]) {
  void* x__v0 = args[0];
  void* x__v1 = args[1];
  void* x_parseBefore = ((Custom*)x__v0)->values[0];
  void* x_parseAfter = ((Custom*)x__v1)->values[0];
  return A2(&elm_core_Basics_apL,
      &elm_url_Url_Parser_Parser,
      NEW_CLOSURE(2,
          3,
          &eval_elm_url_Url_Parser_slash_lambda0,
          ((void* []){
              x_parseAfter,
              x_parseBefore,
          })));
}
Closure elm_url_Url_Parser_slash = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_url_Url_Parser_slash,
};

#define elm_url_Url_Parser_top (*ptr_elm_url_Url_Parser_top)
ElmValue* ptr_elm_url_Url_Parser_top;
void* eval_elm_url_Url_Parser_top_lambda0(void* args[]) {
  void* x_state = args[0];
  return List_fromArray(1,
      ((void* []){
          x_state,
      }));
}
void* init_elm_url_Url_Parser_top() {
  return A2(&elm_core_Basics_apL,
      &elm_url_Url_Parser_Parser,
      NEW_CLOSURE(0, 1, &eval_elm_url_Url_Parser_top_lambda0, ((void* []){})));
}

void* eval_elm_url_Url_Parser_custom_lambda0(void* args[]) {
  void* x_stringToSomething = args[0];
  void* x__v0 = args[1];
  void* x_visited = Utils_access_eval(((void* []){
      (void*)FIELD_visited,
      x__v0,
  }));
  void* x_unvisited = Utils_access_eval(((void* []){
      (void*)FIELD_unvisited,
      x__v0,
  }));
  void* x_params = Utils_access_eval(((void* []){
      (void*)FIELD_params,
      x__v0,
  }));
  void* x_frag = Utils_access_eval(((void* []){
      (void*)FIELD_frag,
      x__v0,
  }));
  void* x_value = Utils_access_eval(((void* []){
      (void*)FIELD_value,
      x__v0,
  }));
  void* tmp1;
  if (x_unvisited == &Nil) {
    tmp1 = &Nil;
  } else {
    void* x_next = Utils_destruct_index(x_unvisited, 0);
    void* x_rest = Utils_destruct_index(x_unvisited, 1);
    void* x__v2 = A1(x_stringToSomething, x_next);
    void* tmp2;
    void* tmp3 = ((Custom*)x__v2)->ctor;
    if (tmp3 == CTOR_Just) {
      void* x_nextValue = Utils_destruct_index(x__v2, 0);
      tmp2 = List_fromArray(1,
          ((void* []){
              A5(&elm_url_Url_Parser_State,
                  A2(&elm_core_List_cons, x_next, x_visited),
                  x_rest,
                  x_params,
                  x_frag,
                  A1(x_value, x_nextValue)),
          }));
    } else {
      tmp2 = &Nil;
    };
    tmp1 = tmp2;
  };
  return tmp1;
}
void* eval_elm_url_Url_Parser_custom(void* args[]) {
  void* x_tipe = args[0];
  void* x_stringToSomething = args[1];
  return A2(&elm_core_Basics_apL,
      &elm_url_Url_Parser_Parser,
      NEW_CLOSURE(1,
          2,
          &eval_elm_url_Url_Parser_custom_lambda0,
          ((void* []){
              x_stringToSomething,
          })));
}
Closure elm_url_Url_Parser_custom = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_url_Url_Parser_custom,
};
#define author_project_Article_Slug_urlParser (*ptr_author_project_Article_Slug_urlParser)
ElmValue* ptr_author_project_Article_Slug_urlParser;
void* eval_author_project_Article_Slug_urlParser_lambda0(void* args[]) {
  void* x_str = args[0];
  return A1(&elm_core_Maybe_Just, A1(&author_project_Article_Slug_Slug, x_str));
}
void* init_author_project_Article_Slug_urlParser() {
  return A2(&elm_url_Url_Parser_custom,
      &literal_string_SLUG,
      NEW_CLOSURE(
          0, 1, &eval_author_project_Article_Slug_urlParser_lambda0, ((void* []){})));
}

#define author_project_Username_urlParser (*ptr_author_project_Username_urlParser)
ElmValue* ptr_author_project_Username_urlParser;
void* eval_author_project_Username_urlParser_lambda0(void* args[]) {
  void* x_str = args[0];
  return A1(&elm_core_Maybe_Just, A1(&author_project_Username_Username, x_str));
}
void* init_author_project_Username_urlParser() {
  return A2(&elm_url_Url_Parser_custom,
      &literal_string_USERNAME,
      NEW_CLOSURE(0, 1, &eval_author_project_Username_urlParser_lambda0, ((void* []){})));
}
#define author_project_Route_parser (*ptr_author_project_Route_parser)
ElmValue* ptr_author_project_Route_parser;
void* init_author_project_Route_parser() {
  return A1(&elm_url_Url_Parser_oneOf,
      List_fromArray(9,
          ((void* []){
              A2(&elm_url_Url_Parser_map,
                  &author_project_Route_Home,
                  &elm_url_Url_Parser_top),
              A2(&elm_url_Url_Parser_map,
                  &author_project_Route_Login,
                  A1(&elm_url_Url_Parser_s, &literal_string_login)),
              A2(&elm_url_Url_Parser_map,
                  &author_project_Route_Logout,
                  A1(&elm_url_Url_Parser_s, &literal_string_logout)),
              A2(&elm_url_Url_Parser_map,
                  &author_project_Route_Settings,
                  A1(&elm_url_Url_Parser_s, &literal_string_settings)),
              A2(&elm_url_Url_Parser_map,
                  &author_project_Route_Profile,
                  A2(&elm_url_Url_Parser_slash,
                      A1(&elm_url_Url_Parser_s, &literal_string_profile),
                      &author_project_Username_urlParser)),
              A2(&elm_url_Url_Parser_map,
                  &author_project_Route_Register,
                  A1(&elm_url_Url_Parser_s, &literal_string_register)),
              A2(&elm_url_Url_Parser_map,
                  &author_project_Route_Article,
                  A2(&elm_url_Url_Parser_slash,
                      A1(&elm_url_Url_Parser_s, &literal_string_article),
                      &author_project_Article_Slug_urlParser)),
              A2(&elm_url_Url_Parser_map,
                  &author_project_Route_NewArticle,
                  A1(&elm_url_Url_Parser_s, &literal_string_editor)),
              A2(&elm_url_Url_Parser_map,
                  &author_project_Route_EditArticle,
                  A2(&elm_url_Url_Parser_slash,
                      A1(&elm_url_Url_Parser_s, &literal_string_editor),
                      &author_project_Article_Slug_urlParser)),
          })));
}
void* eval_author_project_Route_fromUrl(void* args[]) {
  void* x_url = args[0];
  return A2(&elm_core_Basics_apR,
      Utils_update(x_url,
          2,
          ((u32[]){
              FIELD_fragment,
              FIELD_path,
          }),
          ((void* []){
              &elm_core_Maybe_Nothing,
              A2(&elm_core_Maybe_withDefault,
                  &literal_string_,
                  Utils_access_eval(((void* []){
                      (void*)FIELD_fragment,
                      x_url,
                  }))),
          })),
      A1(&elm_url_Url_Parser_parse, &author_project_Route_parser));
}
Closure author_project_Route_fromUrl = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Route_fromUrl,
};

void* eval_author_project_Session_Guest(void* args[]) {
  return ctorCustom(CTOR_Guest, 1, args);
}
Closure author_project_Session_Guest = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Session_Guest,
};

void* eval_author_project_Session_LoggedIn(void* args[]) {
  return ctorCustom(CTOR_LoggedIn, 2, args);
}
Closure author_project_Session_LoggedIn = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Session_LoggedIn,
};
void* eval_author_project_Session_fromViewer(void* args[]) {
  void* x_key = args[0];
  void* x_maybeViewer = args[1];
  void* tmp0;
  void* tmp1 = ((Custom*)x_maybeViewer)->ctor;
  if (tmp1 == CTOR_Just) {
    void* x_viewerVal = Utils_destruct_index(x_maybeViewer, 0);
    tmp0 = A2(&author_project_Session_LoggedIn, x_key, x_viewerVal);
  } else {
    tmp0 = A1(&author_project_Session_Guest, x_key);
  };
  return tmp0;
}
Closure author_project_Session_fromViewer = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Session_fromViewer,
};
void* eval_author_project_Main_init(void* args[]) {
  void* x_maybeViewer = args[0];
  void* x_url = args[1];
  void* x_navKey = args[2];
  return A2(&author_project_Main_changeRouteTo,
      A1(&author_project_Route_fromUrl, x_url),
      A1(&author_project_Main_Redirect,
          A2(&author_project_Session_fromViewer, x_navKey, x_maybeViewer)));
}
Closure author_project_Main_init = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_author_project_Main_init,
};

void* eval_author_project_Main_GotSession(void* args[]) {
  return ctorCustom(CTOR_GotSession, 1, args);
}
Closure author_project_Main_GotSession = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Main_GotSession,
};

void* eval_author_project_Api_decodeFromChange(void* args[]) {
  void* x_viewerDecoder = args[0];
  void* x_val = args[1];
  return A2(&elm_core_Basics_apR,
      A2(&elm_json_Json_Decode_decodeValue,
          A1(&author_project_Api_storageDecoder, x_viewerDecoder),
          x_val),
      &elm_core_Result_toMaybe);
}
Closure author_project_Api_decodeFromChange = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Api_decodeFromChange,
};

#define author_project_Api_onStoreChange (*ptr_author_project_Api_onStoreChange)
Closure* ptr_author_project_Api_onStoreChange;
void* init_author_project_Api_onStoreChange() {
  return A2(
      &Platform_incomingPort, &literal_string_onStoreChange, &elm_json_Json_Decode_value);
}
void* eval_author_project_Api_viewerChanges_lambda0(void* args[]) {
  void* x_decoder = args[0];
  void* x_toMsg = args[1];
  void* x_value = args[2];
  return A1(x_toMsg, A2(&author_project_Api_decodeFromChange, x_decoder, x_value));
}
void* eval_author_project_Api_viewerChanges(void* args[]) {
  void* x_toMsg = args[0];
  void* x_decoder = args[1];
  return A1(&author_project_Api_onStoreChange,
      NEW_CLOSURE(2,
          3,
          &eval_author_project_Api_viewerChanges_lambda0,
          ((void* []){
              x_decoder,
              x_toMsg,
          })));
}
Closure author_project_Api_viewerChanges = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Api_viewerChanges,
};
void* eval_author_project_Session_changes_lambda0(void* args[]) {
  void* x_key = args[0];
  void* x_toMsg = args[1];
  void* x_maybeViewer = args[2];
  return A1(x_toMsg, A2(&author_project_Session_fromViewer, x_key, x_maybeViewer));
}
void* eval_author_project_Session_changes(void* args[]) {
  void* x_toMsg = args[0];
  void* x_key = args[1];
  return A2(&author_project_Api_viewerChanges,
      NEW_CLOSURE(2,
          3,
          &eval_author_project_Session_changes_lambda0,
          ((void* []){
              x_key,
              x_toMsg,
          })),
      &author_project_Viewer_decoder);
}
Closure author_project_Session_changes = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Session_changes,
};

#define elm_core_Platform_Sub_map Platform_map

#define elm_core_Platform_Sub_batch Platform_batch
#define elm_core_Platform_Sub_none (*ptr_elm_core_Platform_Sub_none)
ElmValue* ptr_elm_core_Platform_Sub_none;
void* init_elm_core_Platform_Sub_none() {
  return A1(&elm_core_Platform_Sub_batch, &Nil);
}

void* eval_author_project_Page_Article_GotSession(void* args[]) {
  return ctorCustom(CTOR_GotSession, 1, args);
}
Closure author_project_Page_Article_GotSession = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_GotSession,
};
void* eval_author_project_Page_Article_subscriptions(void* args[]) {
  void* x_model = args[0];
  return A2(&author_project_Session_changes,
      &author_project_Page_Article_GotSession,
      A1(&author_project_Session_navKey,
          Utils_access_eval(((void* []){
              (void*)FIELD_session,
              x_model,
          }))));
}
Closure author_project_Page_Article_subscriptions = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_subscriptions,
};

void* eval_author_project_Page_Article_Editor_GotSession(void* args[]) {
  return ctorCustom(CTOR_GotSession, 1, args);
}
Closure author_project_Page_Article_Editor_GotSession = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_Editor_GotSession,
};
void* eval_author_project_Page_Article_Editor_subscriptions(void* args[]) {
  void* x_model = args[0];
  return A2(&author_project_Session_changes,
      &author_project_Page_Article_Editor_GotSession,
      A1(&author_project_Session_navKey,
          Utils_access_eval(((void* []){
              (void*)FIELD_session,
              x_model,
          }))));
}
Closure author_project_Page_Article_Editor_subscriptions = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_Editor_subscriptions,
};

void* eval_author_project_Page_Home_GotSession(void* args[]) {
  return ctorCustom(CTOR_GotSession, 1, args);
}
Closure author_project_Page_Home_GotSession = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Home_GotSession,
};
void* eval_author_project_Page_Home_subscriptions(void* args[]) {
  void* x_model = args[0];
  return A2(&author_project_Session_changes,
      &author_project_Page_Home_GotSession,
      A1(&author_project_Session_navKey,
          Utils_access_eval(((void* []){
              (void*)FIELD_session,
              x_model,
          }))));
}
Closure author_project_Page_Home_subscriptions = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Home_subscriptions,
};

void* eval_author_project_Page_Login_GotSession(void* args[]) {
  return ctorCustom(CTOR_GotSession, 1, args);
}
Closure author_project_Page_Login_GotSession = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Login_GotSession,
};
void* eval_author_project_Page_Login_subscriptions(void* args[]) {
  void* x_model = args[0];
  return A2(&author_project_Session_changes,
      &author_project_Page_Login_GotSession,
      A1(&author_project_Session_navKey,
          Utils_access_eval(((void* []){
              (void*)FIELD_session,
              x_model,
          }))));
}
Closure author_project_Page_Login_subscriptions = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Login_subscriptions,
};

void* eval_author_project_Page_Profile_GotSession(void* args[]) {
  return ctorCustom(CTOR_GotSession, 1, args);
}
Closure author_project_Page_Profile_GotSession = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Profile_GotSession,
};
void* eval_author_project_Page_Profile_subscriptions(void* args[]) {
  void* x_model = args[0];
  return A2(&author_project_Session_changes,
      &author_project_Page_Profile_GotSession,
      A1(&author_project_Session_navKey,
          Utils_access_eval(((void* []){
              (void*)FIELD_session,
              x_model,
          }))));
}
Closure author_project_Page_Profile_subscriptions = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Profile_subscriptions,
};

void* eval_author_project_Page_Register_GotSession(void* args[]) {
  return ctorCustom(CTOR_GotSession, 1, args);
}
Closure author_project_Page_Register_GotSession = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Register_GotSession,
};
void* eval_author_project_Page_Register_subscriptions(void* args[]) {
  void* x_model = args[0];
  return A2(&author_project_Session_changes,
      &author_project_Page_Register_GotSession,
      A1(&author_project_Session_navKey,
          Utils_access_eval(((void* []){
              (void*)FIELD_session,
              x_model,
          }))));
}
Closure author_project_Page_Register_subscriptions = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Register_subscriptions,
};

void* eval_author_project_Page_Settings_GotSession(void* args[]) {
  return ctorCustom(CTOR_GotSession, 1, args);
}
Closure author_project_Page_Settings_GotSession = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Settings_GotSession,
};
void* eval_author_project_Page_Settings_subscriptions(void* args[]) {
  void* x_model = args[0];
  return A2(&author_project_Session_changes,
      &author_project_Page_Settings_GotSession,
      A1(&author_project_Session_navKey,
          Utils_access_eval(((void* []){
              (void*)FIELD_session,
              x_model,
          }))));
}
Closure author_project_Page_Settings_subscriptions = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Settings_subscriptions,
};
void* eval_author_project_Main_subscriptions(void* args[]) {
  void* x_model = args[0];
  void* tmp0;
  void* tmp1 = ((Custom*)x_model)->ctor;
  if (tmp1 == CTOR_NotFound) {
    {
      tmp0 = &elm_core_Platform_Sub_none;
    };
  } else if (tmp1 == CTOR_Redirect) {
    {
      tmp0 = A2(&author_project_Session_changes,
          &author_project_Main_GotSession,
          A1(&author_project_Session_navKey,
              A1(&author_project_Main_toSession, x_model)));
    };
  } else if (tmp1 == CTOR_Settings) {
    {
      void* x_settings = Utils_destruct_index(x_model, 0);
      tmp0 = A2(&elm_core_Platform_Sub_map,
          &author_project_Main_GotSettingsMsg,
          A1(&author_project_Page_Settings_subscriptions, x_settings));
    };
  } else if (tmp1 == CTOR_Home) {
    {
      void* x_home = Utils_destruct_index(x_model, 0);
      tmp0 = A2(&elm_core_Platform_Sub_map,
          &author_project_Main_GotHomeMsg,
          A1(&author_project_Page_Home_subscriptions, x_home));
    };
  } else if (tmp1 == CTOR_Login) {
    {
      void* x_login = Utils_destruct_index(x_model, 0);
      tmp0 = A2(&elm_core_Platform_Sub_map,
          &author_project_Main_GotLoginMsg,
          A1(&author_project_Page_Login_subscriptions, x_login));
    };
  } else if (tmp1 == CTOR_Register) {
    {
      void* x_register = Utils_destruct_index(x_model, 0);
      tmp0 = A2(&elm_core_Platform_Sub_map,
          &author_project_Main_GotRegisterMsg,
          A1(&author_project_Page_Register_subscriptions, x_register));
    };
  } else if (tmp1 == CTOR_Profile) {
    {
      void* x_profile = Utils_destruct_index(x_model, 1);
      tmp0 = A2(&elm_core_Platform_Sub_map,
          &author_project_Main_GotProfileMsg,
          A1(&author_project_Page_Profile_subscriptions, x_profile));
    };
  } else if (tmp1 == CTOR_Article) {
    {
      void* x_article = Utils_destruct_index(x_model, 0);
      tmp0 = A2(&elm_core_Platform_Sub_map,
          &author_project_Main_GotArticleMsg,
          A1(&author_project_Page_Article_subscriptions, x_article));
    };
  } else {
    void* x_editor = Utils_destruct_index(x_model, 1);
    tmp0 = A2(&elm_core_Platform_Sub_map,
        &author_project_Main_GotEditorMsg,
        A1(&author_project_Page_Article_Editor_subscriptions, x_editor));
  };
  return tmp0;
}
Closure author_project_Main_subscriptions = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Main_subscriptions,
};

#define elm_browser_Browser_Navigation_load Browser_load

#define elm_browser_Browser_Navigation_pushUrl Browser_pushUrl

void* eval_elm_url_Url_addPort(void* args[]) {
  void* x_maybePort = args[0];
  void* x_starter = args[1];
  void* tmp0;
  void* tmp1 = ((Custom*)x_maybePort)->ctor;
  if (tmp1 == CTOR_Nothing) {
    tmp0 = x_starter;
  } else {
    void* x_port_ = Utils_destruct_index(x_maybePort, 0);
    tmp0 = A2(&elm_core_Basics_append,
        x_starter,
        A2(&elm_core_Basics_append,
            &literal_string__3a,
            A1(&elm_core_String_fromInt, x_port_)));
  };
  return tmp0;
}
Closure elm_url_Url_addPort = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_url_Url_addPort,
};

void* eval_elm_url_Url_addPrefixed(void* args[]) {
  void* x_prefix = args[0];
  void* x_maybeSegment = args[1];
  void* x_starter = args[2];
  void* tmp0;
  void* tmp1 = ((Custom*)x_maybeSegment)->ctor;
  if (tmp1 == CTOR_Nothing) {
    tmp0 = x_starter;
  } else {
    void* x_segment = Utils_destruct_index(x_maybeSegment, 0);
    tmp0 = A2(&elm_core_Basics_append,
        x_starter,
        A2(&elm_core_Basics_append, x_prefix, x_segment));
  };
  return tmp0;
}
Closure elm_url_Url_addPrefixed = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_elm_url_Url_addPrefixed,
};
void* eval_elm_url_Url_toString(void* args[]) {
  void* x_url = args[0];
  void* x__v0 = Utils_access_eval(((void* []){
      (void*)FIELD_protocol,
      x_url,
  }));
  void* tmp0;
  void* tmp1 = ((Custom*)x__v0)->ctor;
  if (tmp1 == CTOR_Http) {
    tmp0 = &literal_string_http_3a_2f_2f;
  } else {
    tmp0 = &literal_string_https_3a_2f_2f;
  };
  void* x_http = tmp0;
  return A2(&elm_core_Basics_apR,
      A2(&elm_core_Basics_apR,
          A2(&elm_core_Basics_append,
              A2(&elm_url_Url_addPort,
                  Utils_access_eval(((void* []){
                      (void*)FIELD_port_,
                      x_url,
                  })),
                  A2(&elm_core_Basics_append,
                      x_http,
                      Utils_access_eval(((void* []){
                          (void*)FIELD_host,
                          x_url,
                      })))),
              Utils_access_eval(((void* []){
                  (void*)FIELD_path,
                  x_url,
              }))),
          A2(&elm_url_Url_addPrefixed,
              &literal_string__3f,
              Utils_access_eval(((void* []){
                  (void*)FIELD_query,
                  x_url,
              })))),
      A2(&elm_url_Url_addPrefixed,
          &literal_string__23,
          Utils_access_eval(((void* []){
              (void*)FIELD_fragment,
              x_url,
          }))));
}
Closure elm_url_Url_toString = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_url_Url_toString,
};

void* eval_author_project_Page_Article_CompletedDeleteArticle(void* args[]) {
  return ctorCustom(CTOR_CompletedDeleteArticle, 1, args);
}
Closure author_project_Page_Article_CompletedDeleteArticle = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_CompletedDeleteArticle,
};

void* eval_author_project_Page_Article_CompletedDeleteComment(void* args[]) {
  return ctorCustom(CTOR_CompletedDeleteComment, 2, args);
}
Closure author_project_Page_Article_CompletedDeleteComment = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Article_CompletedDeleteComment,
};

void* eval_author_project_Page_Article_CompletedFollowChange(void* args[]) {
  return ctorCustom(CTOR_CompletedFollowChange, 1, args);
}
Closure author_project_Page_Article_CompletedFollowChange = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_CompletedFollowChange,
};

void* eval_author_project_Page_Article_CompletedPostComment(void* args[]) {
  return ctorCustom(CTOR_CompletedPostComment, 1, args);
}
Closure author_project_Page_Article_CompletedPostComment = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_CompletedPostComment,
};

void* eval_author_project_Page_Article_Editing(void* args[]) {
  return ctorCustom(CTOR_Editing, 1, args);
}
Closure author_project_Page_Article_Editing = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_Editing,
};

Custom author_project_Page_Article_Failed = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Failed,
};

void* eval_author_project_Page_Article_Loaded(void* args[]) {
  return ctorCustom(CTOR_Loaded, 1, args);
}
Closure author_project_Page_Article_Loaded = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_Loaded,
};

Custom author_project_Page_Article_LoadingSlowly = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_LoadingSlowly,
};

void* eval_author_project_Page_Article_Sending(void* args[]) {
  return ctorCustom(CTOR_Sending, 1, args);
}
Closure author_project_Page_Article_Sending = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_Sending,
};

void* eval_author_project_Api_addServerError(void* args[]) {
  void* x_list = args[0];
  return A2(&elm_core_List_cons, &literal_string_Server_20error, x_list);
}
Closure author_project_Api_addServerError = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Api_addServerError,
};

void* eval_author_project_CommentId_toString(void* args[]) {
  void* x__v0 = args[0];
  void* x_id = ((Custom*)x__v0)->values[0];
  return A1(&elm_core_String_fromInt, x_id);
}
Closure author_project_CommentId_toString = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_CommentId_toString,
};
void* eval_author_project_Api_Endpoint_comment(void* args[]) {
  void* x_slug = args[0];
  void* x_commentId = args[1];
  return A2(&author_project_Api_Endpoint_url,
      List_fromArray(4,
          ((void* []){
              &literal_string_articles,
              A1(&author_project_Article_Slug_toString, x_slug),
              &literal_string_comments,
              A1(&author_project_CommentId_toString, x_commentId),
          })),
      &Nil);
}
Closure author_project_Api_Endpoint_comment = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Api_Endpoint_comment,
};

void* eval_author_project_Api_delete(void* args[]) {
  void* x_url = args[0];
  void* x_cred = args[1];
  void* x_body = args[2];
  void* x_decoder = args[3];
  return A1(&author_project_Api_Endpoint_request,
      NEW_RECORD(&fg_body_expect_headers_method_timeout_url_withCredentials,
          7,
          ((void* []){
              x_body,
              A1(&elm_http_Http_expectJson, x_decoder),
              List_fromArray(1,
                  ((void* []){
                      A1(&author_project_Api_credHeader, x_cred),
                  })),
              &literal_string_DELETE,
              &elm_core_Maybe_Nothing,
              x_url,
              &False,
          })));
}
Closure author_project_Api_delete = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x4,
    .evaluator = &eval_author_project_Api_delete,
};
void* eval_author_project_Article_Comment_delete(void* args[]) {
  void* x_articleSlug = args[0];
  void* x_commentId = args[1];
  void* x_cred = args[2];
  return A4(&author_project_Api_delete,
      A2(&author_project_Api_Endpoint_comment, x_articleSlug, x_commentId),
      x_cred,
      &elm_http_Http_emptyBody,
      A1(&elm_json_Json_Decode_succeed, &Unit));
}
Closure author_project_Article_Comment_delete = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_author_project_Article_Comment_delete,
};

void* eval_author_project_Page_Article_delete(void* args[]) {
  void* x_slug = args[0];
  void* x_cred = args[1];
  return A4(&author_project_Api_delete,
      A1(&author_project_Api_Endpoint_article, x_slug),
      x_cred,
      &elm_http_Http_emptyBody,
      A1(&elm_json_Json_Decode_succeed, &Unit));
}
Closure author_project_Page_Article_delete = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Article_delete,
};

#define author_project_Log_error elm_core_Platform_Cmd_none

void* eval_author_project_Page_Article_CompletedFavoriteChange(void* args[]) {
  return ctorCustom(CTOR_CompletedFavoriteChange, 1, args);
}
Closure author_project_Page_Article_CompletedFavoriteChange = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_CompletedFavoriteChange,
};

void* eval_author_project_Article_fromPreview(void* args[]) {
  void* x_newBody = args[0];
  void* x__v0 = args[1];
  void* x_info = Utils_destruct_index(x__v0, 0);
  void* x__v1 = Utils_destruct_index(x__v0, 1);
  return A2(&author_project_Article_Article,
      x_info,
      A1(&author_project_Article_Full, x_newBody));
}
Closure author_project_Article_fromPreview = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Article_fromPreview,
};
void* eval_author_project_Page_Article_fave(void* args[]) {
  void* x_toRequest = args[0];
  void* x_cred = args[1];
  void* x_slug = args[2];
  void* x_body = args[3];
  return A2(&elm_core_Basics_apR,
      A2(&elm_core_Basics_apR,
          A2(&elm_core_Basics_apR,
              A2(x_toRequest, x_slug, x_cred),
              &elm_http_Http_toTask),
          A1(&elm_core_Task_map, A1(&author_project_Article_fromPreview, x_body))),
      A1(&elm_core_Task_attempt, &author_project_Page_Article_CompletedFavoriteChange));
}
Closure author_project_Page_Article_fave = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x4,
    .evaluator = &eval_author_project_Page_Article_fave,
};

void* eval_author_project_Article_faveDecoder(void* args[]) {
  void* x_cred = args[0];
  return A2(&elm_json_Json_Decode_field,
      &literal_string_article,
      A1(&author_project_Article_previewDecoder, A1(&elm_core_Maybe_Just, x_cred)));
}
Closure author_project_Article_faveDecoder = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Article_faveDecoder,
};

void* eval_author_project_Api_Endpoint_favorite(void* args[]) {
  void* x_slug = args[0];
  return A2(&author_project_Api_Endpoint_url,
      List_fromArray(3,
          ((void* []){
              &literal_string_articles,
              A1(&author_project_Article_Slug_toString, x_slug),
              &literal_string_favorite,
          })),
      &Nil);
}
Closure author_project_Api_Endpoint_favorite = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Api_Endpoint_favorite,
};

void* eval_author_project_Api_post(void* args[]) {
  void* x_url = args[0];
  void* x_maybeCred = args[1];
  void* x_body = args[2];
  void* x_decoder = args[3];
  void* tmp0;
  void* tmp1 = ((Custom*)x_maybeCred)->ctor;
  if (tmp1 == CTOR_Just) {
    void* x_cred = Utils_destruct_index(x_maybeCred, 0);
    tmp0 = List_fromArray(1,
        ((void* []){
            A1(&author_project_Api_credHeader, x_cred),
        }));
  } else {
    tmp0 = &Nil;
  };
  return A1(&author_project_Api_Endpoint_request,
      NEW_RECORD(&fg_body_expect_headers_method_timeout_url_withCredentials,
          7,
          ((void* []){
              x_body,
              A1(&elm_http_Http_expectJson, x_decoder),
              tmp0,
              &literal_string_POST,
              &elm_core_Maybe_Nothing,
              x_url,
              &False,
          })));
}
Closure author_project_Api_post = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x4,
    .evaluator = &eval_author_project_Api_post,
};
void* eval_author_project_Article_favorite(void* args[]) {
  void* x_articleSlug = args[0];
  void* x_cred = args[1];
  return A4(&author_project_Api_post,
      A1(&author_project_Api_Endpoint_favorite, x_articleSlug),
      A1(&elm_core_Maybe_Just, x_cred),
      &elm_http_Http_emptyBody,
      A1(&author_project_Article_faveDecoder, x_cred));
}
Closure author_project_Article_favorite = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Article_favorite,
};

void* eval_author_project_Article_mapAuthor(void* args[]) {
  void* x_transform = args[0];
  void* x__v0 = args[1];
  void* x_info = Utils_destruct_index(x__v0, 0);
  void* x_extras = Utils_destruct_index(x__v0, 1);
  return A2(&author_project_Article_Article,
      Utils_update(x_info,
          1,
          ((u32[]){
              FIELD_author,
          }),
          ((void* []){
              A1(x_transform,
                  Utils_access_eval(((void* []){
                      (void*)FIELD_author,
                      x_info,
                  }))),
          })),
      x_extras);
}
Closure author_project_Article_mapAuthor = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Article_mapAuthor,
};

void* eval_elm_json_Json_Encode_object_lambda0(void* args[]) {
  void* x__v0 = args[0];
  void* x_obj = args[1];
  void* x_k = Utils_destruct_index(x__v0, 0);
  void* x_v = Utils_destruct_index(x__v0, 1);
  return A3(&Json_addField, x_k, x_v, x_obj);
}
void* eval_elm_json_Json_Encode_object(void* args[]) {
  void* x_pairs = args[0];
  return A1(&Json_wrap,
      A3(&elm_core_List_foldl,
          NEW_CLOSURE(0, 2, &eval_elm_json_Json_Encode_object_lambda0, ((void* []){})),
          A1(&Json_emptyObject, &Unit),
          x_pairs));
}
Closure elm_json_Json_Encode_object = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_json_Json_Encode_object,
};

#define elm_json_Json_Encode_string Json_wrap
void* eval_author_project_Article_Comment_encodeCommentBody(void* args[]) {
  void* x_str = args[0];
  return A1(&elm_json_Json_Encode_object,
      List_fromArray(1,
          ((void* []){
              NEW_TUPLE2(&literal_string_comment,
                  A1(&elm_json_Json_Encode_object,
                      List_fromArray(1,
                          ((void* []){
                              NEW_TUPLE2(&literal_string_body,
                                  A1(&elm_json_Json_Encode_string, x_str)),
                          })))),
          })));
}
Closure author_project_Article_Comment_encodeCommentBody = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Article_Comment_encodeCommentBody,
};

void* eval_elm_http_Http_Internal_StringBody(void* args[]) {
  return ctorCustom(CTOR_StringBody, 2, args);
}
Closure elm_http_Http_Internal_StringBody = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_http_Http_Internal_StringBody,
};
void* eval_elm_http_Http_jsonBody(void* args[]) {
  void* x_value = args[0];
  return A2(&elm_http_Http_Internal_StringBody,
      &literal_string_application_2fjson,
      A2(&elm_json_Json_Encode_encode, &literal_int_0, x_value));
}
Closure elm_http_Http_jsonBody = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_http_Http_jsonBody,
};
void* eval_author_project_Article_Comment_post(void* args[]) {
  void* x_articleSlug = args[0];
  void* x_commentBody = args[1];
  void* x_cred = args[2];
  void* x_bod = A2(&elm_core_Basics_apR,
      A1(&author_project_Article_Comment_encodeCommentBody, x_commentBody),
      &elm_http_Http_jsonBody);
  return A2(&elm_core_Basics_apR,
      A2(&elm_json_Json_Decode_field,
          &literal_string_comment,
          A1(&author_project_Article_Comment_decoder, A1(&elm_core_Maybe_Just, x_cred))),
      A3(&author_project_Api_post,
          A1(&author_project_Api_Endpoint_comments, x_articleSlug),
          A1(&elm_core_Maybe_Just, x_cred),
          x_bod));
}
Closure author_project_Article_Comment_post = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_author_project_Article_Comment_post,
};

void* eval_author_project_Api_Endpoint_follow(void* args[]) {
  void* x_uname = args[0];
  return A2(&author_project_Api_Endpoint_url,
      List_fromArray(3,
          ((void* []){
              &literal_string_profiles,
              A1(&author_project_Username_toString, x_uname),
              &literal_string_follow,
          })),
      &Nil);
}
Closure author_project_Api_Endpoint_follow = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Api_Endpoint_follow,
};

void* eval_author_project_Author_followDecoder(void* args[]) {
  void* x_cred = args[0];
  return A2(&elm_json_Json_Decode_field,
      &literal_string_profile,
      A1(&author_project_Author_decoder, A1(&elm_core_Maybe_Just, x_cred)));
}
Closure author_project_Author_followDecoder = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Author_followDecoder,
};
void* eval_author_project_Author_requestFollow(void* args[]) {
  void* x__v0 = args[0];
  void* x_cred = args[1];
  void* x_uname = Utils_destruct_index(x__v0, 0);
  return A4(&author_project_Api_post,
      A1(&author_project_Api_Endpoint_follow, x_uname),
      A1(&elm_core_Maybe_Just, x_cred),
      &elm_http_Http_emptyBody,
      A1(&author_project_Author_followDecoder, x_cred));
}
Closure author_project_Author_requestFollow = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Author_requestFollow,
};

void* eval_author_project_Author_requestUnfollow(void* args[]) {
  void* x__v0 = args[0];
  void* x_cred = args[1];
  void* x_uname = Utils_destruct_index(x__v0, 0);
  return A4(&author_project_Api_delete,
      A1(&author_project_Api_Endpoint_follow, x_uname),
      x_cred,
      &elm_http_Http_emptyBody,
      A1(&author_project_Author_followDecoder, x_cred));
}
Closure author_project_Author_requestUnfollow = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Author_requestUnfollow,
};

void* eval_author_project_Article_unfavorite(void* args[]) {
  void* x_articleSlug = args[0];
  void* x_cred = args[1];
  return A4(&author_project_Api_delete,
      A1(&author_project_Api_Endpoint_favorite, x_articleSlug),
      x_cred,
      &elm_http_Http_emptyBody,
      A1(&author_project_Article_faveDecoder, x_cred));
}
Closure author_project_Article_unfavorite = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Article_unfavorite,
};

void* eval_elm_core_List_filter_lambda0(void* args[]) {
  void* x_isGood = args[0];
  void* x_x = args[1];
  void* x_xs = args[2];
  void* tmp1;
  if (A1(x_isGood, x_x) == &True) {
    tmp1 = A2(&elm_core_List_cons, x_x, x_xs);
  } else {
    tmp1 = x_xs;
  };
  return tmp1;
}
void* eval_elm_core_List_filter(void* args[]) {
  void* x_isGood = args[0];
  void* x_list = args[1];
  return A3(&elm_core_List_foldr,
      NEW_CLOSURE(1,
          3,
          &eval_elm_core_List_filter_lambda0,
          ((void* []){
              x_isGood,
          })),
      &Nil,
      x_list);
}
Closure elm_core_List_filter = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_core_List_filter,
};

void* eval_author_project_Article_Comment_id(void* args[]) {
  void* x__v0 = args[0];
  void* x_comment = ((Custom*)x__v0)->values[0];
  return Utils_access_eval(((void* []){
      (void*)FIELD_id,
      x_comment,
  }));
}
Closure author_project_Article_Comment_id = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Article_Comment_id,
};
void* eval_author_project_Page_Article_withoutComment_lambda0(void* args[]) {
  void* x_id = args[0];
  void* x_comment = args[1];
  return A2(
      &elm_core_Basics_neq, A1(&author_project_Article_Comment_id, x_comment), x_id);
}
void* eval_author_project_Page_Article_withoutComment(void* args[]) {
  void* x_id = args[0];
  void* x_list = args[1];
  return A2(&elm_core_List_filter,
      NEW_CLOSURE(1,
          2,
          &eval_author_project_Page_Article_withoutComment_lambda0,
          ((void* []){
              x_id,
          })),
      x_list);
}
Closure author_project_Page_Article_withoutComment = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Article_withoutComment,
};
void* eval_author_project_Page_Article_update_lambda22(void* args[]) {
  void* x_newAuthor = args[0];
  void* x__v2 = args[1];
  return x_newAuthor;
}
void* eval_author_project_Page_Article_update(void* args[]) {
  void* x_msg = args[0];
  void* x_model = args[1];
  void* tmp0;
  void* tmp1 = ((Custom*)x_msg)->ctor;
  if (tmp1 == CTOR_ClickedDismissErrors) {
    {
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_errors,
                            }),
                            ((void* []){
                                &Nil,
                            })),
          &elm_core_Platform_Cmd_none);
    };
  } else if (tmp1 == CTOR_ClickedFavorite) {
    {
      void* x_cred = Utils_destruct_index(x_msg, 0);
      void* x_slug = Utils_destruct_index(x_msg, 1);
      void* x_body = Utils_destruct_index(x_msg, 2);
      tmp0 = NEW_TUPLE2(x_model,
          A4(&author_project_Page_Article_fave,
              &author_project_Article_favorite,
              x_cred,
              x_slug,
              x_body));
    };
  } else if (tmp1 == CTOR_ClickedUnfavorite) {
    {
      void* x_cred = Utils_destruct_index(x_msg, 0);
      void* x_slug = Utils_destruct_index(x_msg, 1);
      void* x_body = Utils_destruct_index(x_msg, 2);
      tmp0 = NEW_TUPLE2(x_model,
          A4(&author_project_Page_Article_fave,
              &author_project_Article_unfavorite,
              x_cred,
              x_slug,
              x_body));
    };
  } else if (tmp1 == CTOR_CompletedLoadArticle) {
    void* tmp25 = ((Custom*)Utils_destruct_index(x_msg, 0))->ctor;
    if (tmp25 == CTOR_Ok) {
      void* x_article = Utils_destruct_index(Utils_destruct_index(x_msg, 0), 0);
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_article,
                            }),
                            ((void* []){
                                A1(&author_project_Page_Article_Loaded, x_article),
                            })),
          &elm_core_Platform_Cmd_none);
    } else {
      void* x_error = Utils_destruct_index(Utils_destruct_index(x_msg, 0), 0);
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_article,
                            }),
                            ((void* []){
                                &author_project_Page_Article_Failed,
                            })),
          &author_project_Log_error);
    };
  } else if (tmp1 == CTOR_CompletedLoadComments) {
    void* tmp24 = ((Custom*)Utils_destruct_index(x_msg, 0))->ctor;
    if (tmp24 == CTOR_Ok) {
      void* x_comments = Utils_destruct_index(Utils_destruct_index(x_msg, 0), 0);
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_comments,
                            }),
                            ((void* []){
                                A1(&author_project_Page_Article_Loaded,
                                    NEW_TUPLE2(A1(&author_project_Page_Article_Editing,
                                                   &literal_string_),
                                        x_comments)),
                            })),
          &elm_core_Platform_Cmd_none);
    } else {
      void* x_error = Utils_destruct_index(Utils_destruct_index(x_msg, 0), 0);
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_article,
                            }),
                            ((void* []){
                                &author_project_Page_Article_Failed,
                            })),
          &author_project_Log_error);
    };
  } else if (tmp1 == CTOR_CompletedFavoriteChange) {
    void* tmp23 = ((Custom*)Utils_destruct_index(x_msg, 0))->ctor;
    if (tmp23 == CTOR_Ok) {
      void* x_newArticle = Utils_destruct_index(Utils_destruct_index(x_msg, 0), 0);
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_article,
                            }),
                            ((void* []){
                                A1(&author_project_Page_Article_Loaded, x_newArticle),
                            })),
          &elm_core_Platform_Cmd_none);
    } else {
      void* x_error = Utils_destruct_index(Utils_destruct_index(x_msg, 0), 0);
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_errors,
                            }),
                            ((void* []){
                                A1(&author_project_Api_addServerError,
                                    Utils_access_eval(((void* []){
                                        (void*)FIELD_errors,
                                        x_model,
                                    }))),
                            })),
          &author_project_Log_error);
    };
  } else if (tmp1 == CTOR_ClickedUnfollow) {
    {
      void* x_cred = Utils_destruct_index(x_msg, 0);
      void* x_followedAuthor = Utils_destruct_index(x_msg, 1);
      tmp0 = NEW_TUPLE2(x_model,
          A2(&elm_core_Basics_apR,
              A2(&author_project_Author_requestUnfollow, x_followedAuthor, x_cred),
              A1(&elm_http_Http_send,
                  &author_project_Page_Article_CompletedFollowChange)));
    };
  } else if (tmp1 == CTOR_ClickedFollow) {
    {
      void* x_cred = Utils_destruct_index(x_msg, 0);
      void* x_unfollowedAuthor = Utils_destruct_index(x_msg, 1);
      tmp0 = NEW_TUPLE2(x_model,
          A2(&elm_core_Basics_apR,
              A2(&author_project_Author_requestFollow, x_unfollowedAuthor, x_cred),
              A1(&elm_http_Http_send,
                  &author_project_Page_Article_CompletedFollowChange)));
    };
  } else if (tmp1 == CTOR_CompletedFollowChange) {
    void* tmp19 = ((Custom*)Utils_destruct_index(x_msg, 0))->ctor;
    if (tmp19 == CTOR_Ok) {
      void* x_newAuthor = Utils_destruct_index(Utils_destruct_index(x_msg, 0), 0);
      void* x__v1 = Utils_access_eval(((void* []){
          (void*)FIELD_article,
          x_model,
      }));
      void* tmp20;
      void* tmp21 = ((Custom*)x__v1)->ctor;
      if (tmp21 == CTOR_Loaded) {
        void* x_article = Utils_destruct_index(x__v1, 0);
        tmp20 = NEW_TUPLE2(
            Utils_update(x_model,
                1,
                ((u32[]){
                    FIELD_article,
                }),
                ((void* []){
                    A1(&author_project_Page_Article_Loaded,
                        A2(&author_project_Article_mapAuthor,
                            NEW_CLOSURE(1,
                                2,
                                &eval_author_project_Page_Article_update_lambda22,
                                ((void* []){
                                    x_newAuthor,
                                })),
                            x_article)),
                })),
            &elm_core_Platform_Cmd_none);
      } else {
        tmp20 = NEW_TUPLE2(x_model, &author_project_Log_error);
      };
      tmp0 = tmp20;
    } else {
      void* x_error = Utils_destruct_index(Utils_destruct_index(x_msg, 0), 0);
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_errors,
                            }),
                            ((void* []){
                                A1(&author_project_Api_addServerError,
                                    Utils_access_eval(((void* []){
                                        (void*)FIELD_errors,
                                        x_model,
                                    }))),
                            })),
          &author_project_Log_error);
    };
  } else if (tmp1 == CTOR_EnteredCommentText) {
    {
      void* x_str = Utils_destruct_index(x_msg, 0);
      void* x__v3 = Utils_access_eval(((void* []){
          (void*)FIELD_comments,
          x_model,
      }));
      void* tmp16;
      void* tmp17 =
          ((Custom*)Utils_destruct_index(Utils_destruct_index(x__v3, 0), 0))->ctor;
      void* tmp18 = ((Custom*)x__v3)->ctor;
      if (tmp18 == CTOR_Loaded && tmp17 == CTOR_Editing) {
        void* x__v4 = Utils_destruct_index(x__v3, 0);
        void* x_comments = Utils_destruct_index(x__v4, 1);
        tmp16 = NEW_TUPLE2(
            Utils_update(x_model,
                1,
                ((u32[]){
                    FIELD_comments,
                }),
                ((void* []){
                    A1(&author_project_Page_Article_Loaded,
                        NEW_TUPLE2(
                            A1(&author_project_Page_Article_Editing, x_str), x_comments)),
                })),
            &elm_core_Platform_Cmd_none);
      } else {
        tmp16 = NEW_TUPLE2(x_model, &author_project_Log_error);
      };
      tmp0 = tmp16;
    };
  } else if (tmp1 == CTOR_ClickedPostComment) {
    {
      void* x_cred = Utils_destruct_index(x_msg, 0);
      void* x_slug = Utils_destruct_index(x_msg, 1);
      void* x__v5 = Utils_access_eval(((void* []){
          (void*)FIELD_comments,
          x_model,
      }));
      void* tmp13;
      void* tmp14 =
          ((Custom*)Utils_destruct_index(Utils_destruct_index(x__v5, 0), 0))->ctor;
      void* tmp15 = ((Custom*)x__v5)->ctor;
      if (tmp15 == CTOR_Loaded && tmp14 == CTOR_Editing)
        if (A2(Utils_equal,
                Utils_destruct_index(
                    Utils_destruct_index(Utils_destruct_index(x__v5, 0), 0), 0),
                &literal_string_) == &True) {
          void* x__v6 = Utils_destruct_index(x__v5, 0);
          void* x_comments = Utils_destruct_index(x__v6, 1);
          tmp13 = NEW_TUPLE2(x_model, &elm_core_Platform_Cmd_none);
        } else {
          void* x__v7 = Utils_destruct_index(x__v5, 0);
          void* x_str = Utils_destruct_index(Utils_destruct_index(x__v7, 0), 0);
          void* x_comments = Utils_destruct_index(x__v7, 1);
          tmp13 = NEW_TUPLE2(
              Utils_update(x_model,
                  1,
                  ((u32[]){
                      FIELD_comments,
                  }),
                  ((void* []){
                      A1(&author_project_Page_Article_Loaded,
                          NEW_TUPLE2(A1(&author_project_Page_Article_Sending, x_str),
                              x_comments)),
                  })),
              A2(&elm_core_Basics_apR,
                  A2(&elm_core_Basics_apR,
                      x_cred,
                      A2(&author_project_Article_Comment_post, x_slug, x_str)),
                  A1(&elm_http_Http_send,
                      &author_project_Page_Article_CompletedPostComment)));
        }
      else {
        tmp13 = NEW_TUPLE2(x_model, &author_project_Log_error);
      };
      tmp0 = tmp13;
    };
  } else if (tmp1 == CTOR_CompletedPostComment) {
    void* tmp10 = ((Custom*)Utils_destruct_index(x_msg, 0))->ctor;
    if (tmp10 == CTOR_Ok) {
      void* x_comment = Utils_destruct_index(Utils_destruct_index(x_msg, 0), 0);
      void* x__v8 = Utils_access_eval(((void* []){
          (void*)FIELD_comments,
          x_model,
      }));
      void* tmp11;
      void* tmp12 = ((Custom*)x__v8)->ctor;
      if (tmp12 == CTOR_Loaded) {
        void* x__v9 = Utils_destruct_index(x__v8, 0);
        void* x_comments = Utils_destruct_index(x__v9, 1);
        tmp11 = NEW_TUPLE2(
            Utils_update(x_model,
                1,
                ((u32[]){
                    FIELD_comments,
                }),
                ((void* []){
                    A1(&author_project_Page_Article_Loaded,
                        NEW_TUPLE2(
                            A1(&author_project_Page_Article_Editing, &literal_string_),
                            A2(&elm_core_List_cons, x_comment, x_comments))),
                })),
            &elm_core_Platform_Cmd_none);
      } else {
        tmp11 = NEW_TUPLE2(x_model, &author_project_Log_error);
      };
      tmp0 = tmp11;
    } else {
      void* x_error = Utils_destruct_index(Utils_destruct_index(x_msg, 0), 0);
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_errors,
                            }),
                            ((void* []){
                                A1(&author_project_Api_addServerError,
                                    Utils_access_eval(((void* []){
                                        (void*)FIELD_errors,
                                        x_model,
                                    }))),
                            })),
          &author_project_Log_error);
    };
  } else if (tmp1 == CTOR_ClickedDeleteComment) {
    {
      void* x_cred = Utils_destruct_index(x_msg, 0);
      void* x_slug = Utils_destruct_index(x_msg, 1);
      void* x_id = Utils_destruct_index(x_msg, 2);
      tmp0 = NEW_TUPLE2(x_model,
          A2(&elm_core_Basics_apR,
              A2(&elm_core_Basics_apR,
                  x_cred,
                  A2(&author_project_Article_Comment_delete, x_slug, x_id)),
              A1(&elm_http_Http_send,
                  A1(&author_project_Page_Article_CompletedDeleteComment, x_id))));
    };
  } else if (tmp1 == CTOR_CompletedDeleteComment) {
    void* tmp7 = ((Custom*)Utils_destruct_index(x_msg, 1))->ctor;
    if (tmp7 == CTOR_Ok) {
      void* x_id = Utils_destruct_index(x_msg, 0);
      void* x__v10 = Utils_access_eval(((void* []){
          (void*)FIELD_comments,
          x_model,
      }));
      void* tmp8;
      void* tmp9 = ((Custom*)x__v10)->ctor;
      if (tmp9 == CTOR_Loaded) {
        void* x__v11 = Utils_destruct_index(x__v10, 0);
        void* x_commentText = Utils_destruct_index(x__v11, 0);
        void* x_comments = Utils_destruct_index(x__v11, 1);
        tmp8 = NEW_TUPLE2(Utils_update(x_model,
                              1,
                              ((u32[]){
                                  FIELD_comments,
                              }),
                              ((void* []){
                                  A1(&author_project_Page_Article_Loaded,
                                      NEW_TUPLE2(x_commentText,
                                          A2(&author_project_Page_Article_withoutComment,
                                              x_id,
                                              x_comments))),
                              })),
            &elm_core_Platform_Cmd_none);
      } else {
        tmp8 = NEW_TUPLE2(x_model, &author_project_Log_error);
      };
      tmp0 = tmp8;
    } else {
      void* x_id = Utils_destruct_index(x_msg, 0);
      void* x_error = Utils_destruct_index(Utils_destruct_index(x_msg, 1), 0);
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_errors,
                            }),
                            ((void* []){
                                A1(&author_project_Api_addServerError,
                                    Utils_access_eval(((void* []){
                                        (void*)FIELD_errors,
                                        x_model,
                                    }))),
                            })),
          &author_project_Log_error);
    };
  } else if (tmp1 == CTOR_ClickedDeleteArticle) {
    {
      void* x_cred = Utils_destruct_index(x_msg, 0);
      void* x_slug = Utils_destruct_index(x_msg, 1);
      tmp0 = NEW_TUPLE2(x_model,
          A2(&elm_core_Basics_apR,
              A2(&author_project_Page_Article_delete, x_slug, x_cred),
              A1(&elm_http_Http_send,
                  &author_project_Page_Article_CompletedDeleteArticle)));
    };
  } else if (tmp1 == CTOR_CompletedDeleteArticle) {
    void* tmp6 = ((Custom*)Utils_destruct_index(x_msg, 0))->ctor;
    if (tmp6 == CTOR_Ok) {
      tmp0 = NEW_TUPLE2(x_model,
          A2(&author_project_Route_replaceUrl,
              A1(&author_project_Session_navKey,
                  Utils_access_eval(((void* []){
                      (void*)FIELD_session,
                      x_model,
                  }))),
              &author_project_Route_Home));
    } else {
      void* x_error = Utils_destruct_index(Utils_destruct_index(x_msg, 0), 0);
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_errors,
                            }),
                            ((void* []){
                                A1(&author_project_Api_addServerError,
                                    Utils_access_eval(((void* []){
                                        (void*)FIELD_errors,
                                        x_model,
                                    }))),
                            })),
          &author_project_Log_error);
    };
  } else if (tmp1 == CTOR_GotTimeZone) {
    {
      void* x_tz = Utils_destruct_index(x_msg, 0);
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_timeZone,
                            }),
                            ((void* []){
                                x_tz,
                            })),
          &elm_core_Platform_Cmd_none);
    };
  } else if (tmp1 == CTOR_GotSession) {
    {
      void* x_session = Utils_destruct_index(x_msg, 0);
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_session,
                            }),
                            ((void* []){
                                x_session,
                            })),
          A2(&author_project_Route_replaceUrl,
              A1(&author_project_Session_navKey, x_session),
              &author_project_Route_Home));
    };
  } else {
    void* x__v13 = Utils_access_eval(((void* []){
        (void*)FIELD_comments,
        x_model,
    }));
    void* tmp2;
    void* tmp3 = ((Custom*)x__v13)->ctor;
    if (tmp3 == CTOR_Loading) {
      tmp2 = &author_project_Page_Article_LoadingSlowly;
    } else {
      void* x_other = x__v13;
      tmp2 = x_other;
    };
    void* x_comments = tmp2;
    void* x__v12 = Utils_access_eval(((void* []){
        (void*)FIELD_article,
        x_model,
    }));
    void* tmp4;
    void* tmp5 = ((Custom*)x__v12)->ctor;
    if (tmp5 == CTOR_Loading) {
      tmp4 = &author_project_Page_Article_LoadingSlowly;
    } else {
      void* x_other = x__v12;
      tmp4 = x_other;
    };
    void* x_article = tmp4;
    tmp0 = NEW_TUPLE2(Utils_update(x_model,
                          2,
                          ((u32[]){
                              FIELD_article,
                              FIELD_comments,
                          }),
                          ((void* []){
                              x_article,
                              x_comments,
                          })),
        &elm_core_Platform_Cmd_none);
  };
  return tmp0;
}
Closure author_project_Page_Article_update = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Article_update,
};

void* eval_author_project_Page_Article_Editor_Editing(void* args[]) {
  return ctorCustom(CTOR_Editing, 3, args);
}
Closure author_project_Page_Article_Editor_Editing = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_author_project_Page_Article_Editor_Editing,
};

void* eval_author_project_Page_Article_Editor_LoadingFailed(void* args[]) {
  return ctorCustom(CTOR_LoadingFailed, 1, args);
}
Closure author_project_Page_Article_Editor_LoadingFailed = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_Editor_LoadingFailed,
};

void* eval_author_project_Page_Article_Editor_LoadingSlowly(void* args[]) {
  return ctorCustom(CTOR_LoadingSlowly, 1, args);
}
Closure author_project_Page_Article_Editor_LoadingSlowly = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_Editor_LoadingSlowly,
};

void* eval_author_project_Article_body(void* args[]) {
  void* x__v0 = args[0];
  void* x_extraInfo = ((Custom*)Utils_destruct_index(x__v0, 1))->values[0];
  return x_extraInfo;
}
Closure author_project_Article_body = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Article_body,
};

void* eval_elm_core_Tuple_mapFirst(void* args[]) {
  void* x_func = args[0];
  void* x__v0 = args[1];
  void* x_x = Utils_destruct_index(x__v0, 0);
  void* x_y = Utils_destruct_index(x__v0, 1);
  return NEW_TUPLE2(A1(x_func, x_x), x_y);
}
Closure elm_core_Tuple_mapFirst = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_core_Tuple_mapFirst,
};

void* eval_author_project_Article_metadata(void* args[]) {
  void* x__v0 = args[0];
  void* x_internals = Utils_destruct_index(x__v0, 0);
  return Utils_access_eval(((void* []){
      (void*)FIELD_metadata,
      x_internals,
  }));
}
Closure author_project_Article_metadata = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Article_metadata,
};

void* eval_author_project_Page_Article_Editor_CompletedCreate(void* args[]) {
  return ctorCustom(CTOR_CompletedCreate, 1, args);
}
Closure author_project_Page_Article_Editor_CompletedCreate = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_Editor_CompletedCreate,
};

void* eval_author_project_Page_Article_Editor_CompletedEdit(void* args[]) {
  return ctorCustom(CTOR_CompletedEdit, 1, args);
}
Closure author_project_Page_Article_Editor_CompletedEdit = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_Editor_CompletedEdit,
};

void* eval_author_project_Page_Article_Editor_Creating(void* args[]) {
  return ctorCustom(CTOR_Creating, 1, args);
}
Closure author_project_Page_Article_Editor_Creating = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_Editor_Creating,
};

void* eval_author_project_Page_Article_Editor_Saving(void* args[]) {
  return ctorCustom(CTOR_Saving, 2, args);
}
Closure author_project_Page_Article_Editor_Saving = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Article_Editor_Saving,
};

void* eval_elm_json_Json_Encode_list(void* args[]) {
  void* x_func = args[0];
  void* x_entries = args[1];
  return A1(&Json_wrap,
      A3(&elm_core_List_foldl,
          A1(&Json_addEntry, x_func),
          A1(&Json_emptyArray, &Unit),
          x_entries));
}
Closure elm_json_Json_Encode_list = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_json_Json_Encode_list,
};

#define elm_core_String_trim String_trim
void* eval_author_project_Page_Article_Editor_tagsFromString(void* args[]) {
  void* x_str = args[0];
  return A2(&elm_core_Basics_apR,
      A2(&elm_core_Basics_apR,
          A2(&elm_core_String_split, &literal_string__20, x_str),
          A1(&elm_core_List_map, &elm_core_String_trim)),
      A1(&elm_core_List_filter,
          A2(&elm_core_Basics_composeL, &elm_core_Basics_not, &elm_core_String_isEmpty)));
}
Closure author_project_Page_Article_Editor_tagsFromString = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_Editor_tagsFromString,
};
void* eval_author_project_Page_Article_Editor_create(void* args[]) {
  void* x__v0 = args[0];
  void* x_cred = args[1];
  void* x_form = ((Custom*)x__v0)->values[0];
  void* x_article = A1(&elm_json_Json_Encode_object,
      List_fromArray(4,
          ((void* []){
              NEW_TUPLE2(&literal_string_title,
                  A1(&elm_json_Json_Encode_string,
                      Utils_access_eval(((void* []){
                          (void*)FIELD_title,
                          x_form,
                      })))),
              NEW_TUPLE2(&literal_string_description,
                  A1(&elm_json_Json_Encode_string,
                      Utils_access_eval(((void* []){
                          (void*)FIELD_description,
                          x_form,
                      })))),
              NEW_TUPLE2(&literal_string_body,
                  A1(&elm_json_Json_Encode_string,
                      Utils_access_eval(((void* []){
                          (void*)FIELD_body,
                          x_form,
                      })))),
              NEW_TUPLE2(&literal_string_tagList,
                  A2(&elm_json_Json_Encode_list,
                      &elm_json_Json_Encode_string,
                      A1(&author_project_Page_Article_Editor_tagsFromString,
                          Utils_access_eval(((void* []){
                              (void*)FIELD_tags,
                              x_form,
                          }))))),
          })));
  void* x_body = A2(&elm_core_Basics_apR,
      A1(&elm_json_Json_Encode_object,
          List_fromArray(1,
              ((void* []){
                  NEW_TUPLE2(&literal_string_article, x_article),
              }))),
      &elm_http_Http_jsonBody);
  return A2(&elm_core_Basics_apR,
      A2(&elm_json_Json_Decode_field,
          &literal_string_article,
          A1(&author_project_Article_fullDecoder, A1(&elm_core_Maybe_Just, x_cred))),
      A3(&author_project_Api_post,
          A1(&author_project_Api_Endpoint_articles, &Nil),
          A1(&elm_core_Maybe_Just, x_cred),
          x_body));
}
Closure author_project_Page_Article_Editor_create = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Article_Editor_create,
};

void* eval_author_project_Api_put(void* args[]) {
  void* x_url = args[0];
  void* x_cred = args[1];
  void* x_body = args[2];
  void* x_decoder = args[3];
  return A1(&author_project_Api_Endpoint_request,
      NEW_RECORD(&fg_body_expect_headers_method_timeout_url_withCredentials,
          7,
          ((void* []){
              x_body,
              A1(&elm_http_Http_expectJson, x_decoder),
              List_fromArray(1,
                  ((void* []){
                      A1(&author_project_Api_credHeader, x_cred),
                  })),
              &literal_string_PUT,
              &elm_core_Maybe_Nothing,
              x_url,
              &False,
          })));
}
Closure author_project_Api_put = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x4,
    .evaluator = &eval_author_project_Api_put,
};
void* eval_author_project_Page_Article_Editor_edit(void* args[]) {
  void* x_articleSlug = args[0];
  void* x__v0 = args[1];
  void* x_cred = args[2];
  void* x_form = ((Custom*)x__v0)->values[0];
  void* x_article = A1(&elm_json_Json_Encode_object,
      List_fromArray(3,
          ((void* []){
              NEW_TUPLE2(&literal_string_title,
                  A1(&elm_json_Json_Encode_string,
                      Utils_access_eval(((void* []){
                          (void*)FIELD_title,
                          x_form,
                      })))),
              NEW_TUPLE2(&literal_string_description,
                  A1(&elm_json_Json_Encode_string,
                      Utils_access_eval(((void* []){
                          (void*)FIELD_description,
                          x_form,
                      })))),
              NEW_TUPLE2(&literal_string_body,
                  A1(&elm_json_Json_Encode_string,
                      Utils_access_eval(((void* []){
                          (void*)FIELD_body,
                          x_form,
                      })))),
          })));
  void* x_body = A2(&elm_core_Basics_apR,
      A1(&elm_json_Json_Encode_object,
          List_fromArray(1,
              ((void* []){
                  NEW_TUPLE2(&literal_string_article, x_article),
              }))),
      &elm_http_Http_jsonBody);
  return A2(&elm_core_Basics_apR,
      A2(&elm_json_Json_Decode_field,
          &literal_string_article,
          A1(&author_project_Article_fullDecoder, A1(&elm_core_Maybe_Just, x_cred))),
      A3(&author_project_Api_put,
          A1(&author_project_Api_Endpoint_article, x_articleSlug),
          x_cred,
          x_body));
}
Closure author_project_Page_Article_Editor_edit = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_author_project_Page_Article_Editor_edit,
};

Custom author_project_Page_Article_Editor_Body = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Body,
};

Custom author_project_Page_Article_Editor_Title = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Title,
};
#define author_project_Page_Article_Editor_fieldsToValidate \
  (*ptr_author_project_Page_Article_Editor_fieldsToValidate)
Cons* ptr_author_project_Page_Article_Editor_fieldsToValidate;
void* init_author_project_Page_Article_Editor_fieldsToValidate() {
  return List_fromArray(2,
      ((void* []){
          &author_project_Page_Article_Editor_Title,
          &author_project_Page_Article_Editor_Body,
      }));
}

void* eval_author_project_Page_Article_Editor_Trimmed(void* args[]) {
  return ctorCustom(CTOR_Trimmed, 1, args);
}
Closure author_project_Page_Article_Editor_Trimmed = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_Editor_Trimmed,
};
void* eval_author_project_Page_Article_Editor_trimFields(void* args[]) {
  void* x_form = args[0];
  return A1(&author_project_Page_Article_Editor_Trimmed,
      NEW_RECORD(&fg_body_description_tags_title,
          4,
          ((void* []){
              A1(&elm_core_String_trim,
                  Utils_access_eval(((void* []){
                      (void*)FIELD_body,
                      x_form,
                  }))),
              A1(&elm_core_String_trim,
                  Utils_access_eval(((void* []){
                      (void*)FIELD_description,
                      x_form,
                  }))),
              A1(&elm_core_String_trim,
                  Utils_access_eval(((void* []){
                      (void*)FIELD_tags,
                      x_form,
                  }))),
              A1(&elm_core_String_trim,
                  Utils_access_eval(((void* []){
                      (void*)FIELD_title,
                      x_form,
                  }))),
          })));
}
Closure author_project_Page_Article_Editor_trimFields = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_Editor_trimFields,
};

void* eval_author_project_Page_Article_Editor_InvalidEntry(void* args[]) {
  return ctorCustom(CTOR_InvalidEntry, 2, args);
}
Closure author_project_Page_Article_Editor_InvalidEntry = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Article_Editor_InvalidEntry,
};
void* eval_author_project_Page_Article_Editor_validateField(void* args[]) {
  void* x__v0 = args[0];
  void* x_field = args[1];
  void* x_form = ((Custom*)x__v0)->values[0];
  void* tmp0;
  void* tmp1 = ((Custom*)x_field)->ctor;
  if (tmp1 == CTOR_Title) {
    void* tmp2;
    if (A1(&elm_core_String_isEmpty,
            Utils_access_eval(((void* []){
                (void*)FIELD_title,
                x_form,
            }))) == &True) {
      tmp2 = List_fromArray(1,
          ((void* []){
              &literal_string_title_20can_5c_27t_20be_20blank_2e,
          }));
    } else {
      tmp2 = &Nil;
    };
    tmp0 = tmp2;
  } else {
    void* tmp3;
    if (A1(&elm_core_String_isEmpty,
            Utils_access_eval(((void* []){
                (void*)FIELD_body,
                x_form,
            }))) == &True) {
      tmp3 = List_fromArray(1,
          ((void* []){
              &literal_string_body_20can_5c_27t_20be_20blank_2e,
          }));
    } else {
      tmp3 = &Nil;
    };
    tmp0 = tmp3;
  };
  return A2(&elm_core_Basics_apL,
      A1(&elm_core_List_map,
          A1(&author_project_Page_Article_Editor_InvalidEntry, x_field)),
      tmp0);
}
Closure author_project_Page_Article_Editor_validateField = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Article_Editor_validateField,
};
void* eval_author_project_Page_Article_Editor_validate(void* args[]) {
  void* x_form = args[0];
  void* x_trimmedForm = A1(&author_project_Page_Article_Editor_trimFields, x_form);
  void* x__v0 = A2(&elm_core_List_concatMap,
      A1(&author_project_Page_Article_Editor_validateField, x_trimmedForm),
      &author_project_Page_Article_Editor_fieldsToValidate);
  void* tmp0;
  if (x__v0 == &Nil) {
    tmp0 = A1(&elm_core_Result_Ok, x_trimmedForm);
  } else {
    void* x_problems = x__v0;
    tmp0 = A1(&elm_core_Result_Err, x_problems);
  };
  return tmp0;
}
Closure author_project_Page_Article_Editor_validate = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_Editor_validate,
};
void* eval_author_project_Page_Article_Editor_save(void* args[]) {
  void* x_cred = args[0];
  void* x_status = args[1];
  void* tmp0;
  void* tmp1 = ((Custom*)x_status)->ctor;
  if (tmp1 == CTOR_Editing) {
    {
      void* x_slug = Utils_destruct_index(x_status, 0);
      void* x_form = Utils_destruct_index(x_status, 2);
      void* x__v1 = A1(&author_project_Page_Article_Editor_validate, x_form);
      void* tmp4;
      void* tmp5 = ((Custom*)x__v1)->ctor;
      if (tmp5 == CTOR_Ok) {
        void* x_validForm = Utils_destruct_index(x__v1, 0);
        tmp4 = NEW_TUPLE2(A2(&author_project_Page_Article_Editor_Saving, x_slug, x_form),
            A2(&elm_core_Basics_apR,
                A3(&author_project_Page_Article_Editor_edit, x_slug, x_validForm, x_cred),
                A1(&elm_http_Http_send,
                    &author_project_Page_Article_Editor_CompletedEdit)));
      } else {
        void* x_problems = Utils_destruct_index(x__v1, 0);
        tmp4 = NEW_TUPLE2(
            A3(&author_project_Page_Article_Editor_Editing, x_slug, x_problems, x_form),
            &elm_core_Platform_Cmd_none);
      };
      tmp0 = tmp4;
    };
  } else if (tmp1 == CTOR_EditingNew) {
    {
      void* x_form = Utils_destruct_index(x_status, 1);
      void* x__v2 = A1(&author_project_Page_Article_Editor_validate, x_form);
      void* tmp2;
      void* tmp3 = ((Custom*)x__v2)->ctor;
      if (tmp3 == CTOR_Ok) {
        void* x_validForm = Utils_destruct_index(x__v2, 0);
        tmp2 = NEW_TUPLE2(A1(&author_project_Page_Article_Editor_Creating, x_form),
            A2(&elm_core_Basics_apR,
                A2(&author_project_Page_Article_Editor_create, x_validForm, x_cred),
                A1(&elm_http_Http_send,
                    &author_project_Page_Article_Editor_CompletedCreate)));
      } else {
        void* x_problems = Utils_destruct_index(x__v2, 0);
        tmp2 = NEW_TUPLE2(
            A2(&author_project_Page_Article_Editor_EditingNew, x_problems, x_form),
            &elm_core_Platform_Cmd_none);
      };
      tmp0 = tmp2;
    };
  } else {
    tmp0 = NEW_TUPLE2(x_status, &elm_core_Platform_Cmd_none);
  };
  return tmp0;
}
Closure author_project_Page_Article_Editor_save = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Article_Editor_save,
};

void* eval_author_project_Page_Article_Editor_ServerError(void* args[]) {
  return ctorCustom(CTOR_ServerError, 1, args);
}
Closure author_project_Page_Article_Editor_ServerError = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_Editor_ServerError,
};
void* eval_author_project_Page_Article_Editor_savingError(void* args[]) {
  void* x_error = args[0];
  void* x_status = args[1];
  void* x_problems = List_fromArray(1,
      ((void* []){
          A1(&author_project_Page_Article_Editor_ServerError,
              &literal_string_Error_20saving_20article),
      }));
  void* tmp0;
  void* tmp1 = ((Custom*)x_status)->ctor;
  if (tmp1 == CTOR_Saving) {
    {
      void* x_slug = Utils_destruct_index(x_status, 0);
      void* x_form = Utils_destruct_index(x_status, 1);
      tmp0 = A3(&author_project_Page_Article_Editor_Editing, x_slug, x_problems, x_form);
    };
  } else if (tmp1 == CTOR_Creating) {
    {
      void* x_form = Utils_destruct_index(x_status, 0);
      tmp0 = A2(&author_project_Page_Article_Editor_EditingNew, x_problems, x_form);
    };
  } else {
    tmp0 = x_status;
  };
  return tmp0;
}
Closure author_project_Page_Article_Editor_savingError = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Article_Editor_savingError,
};

void* eval_author_project_Article_slug(void* args[]) {
  void* x__v0 = args[0];
  void* x_internals = Utils_destruct_index(x__v0, 0);
  return Utils_access_eval(((void* []){
      (void*)FIELD_slug,
      x_internals,
  }));
}
Closure author_project_Article_slug = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Article_slug,
};

void* eval_author_project_Article_Body_toMarkdownString(void* args[]) {
  void* x__v0 = args[0];
  void* x_markdown = ((Custom*)x__v0)->values[0];
  return x_markdown;
}
Closure author_project_Article_Body_toMarkdownString = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Article_Body_toMarkdownString,
};

void* eval_author_project_Page_Article_Editor_updateForm(void* args[]) {
  void* x_transform = args[0];
  void* x_model = args[1];
  void* x__v0 = Utils_access_eval(((void* []){
      (void*)FIELD_status,
      x_model,
  }));
  void* tmp0;
  void* tmp1 = ((Custom*)x__v0)->ctor;
  if (tmp1 == CTOR_Loading) {
    {
      tmp0 = x_model;
    };
  } else if (tmp1 == CTOR_LoadingSlowly) {
    { tmp0 = x_model; };
  } else if (tmp1 == CTOR_LoadingFailed) {
    { tmp0 = x_model; };
  } else if (tmp1 == CTOR_Saving) {
    {
      void* x_slug = Utils_destruct_index(x__v0, 0);
      void* x_form = Utils_destruct_index(x__v0, 1);
      tmp0 = Utils_update(x_model,
          1,
          ((u32[]){
              FIELD_status,
          }),
          ((void* []){
              A2(&author_project_Page_Article_Editor_Saving,
                  x_slug,
                  A1(x_transform, x_form)),
          }));
    };
  } else if (tmp1 == CTOR_Editing) {
    {
      void* x_slug = Utils_destruct_index(x__v0, 0);
      void* x_errors = Utils_destruct_index(x__v0, 1);
      void* x_form = Utils_destruct_index(x__v0, 2);
      tmp0 = Utils_update(x_model,
          1,
          ((u32[]){
              FIELD_status,
          }),
          ((void* []){
              A3(&author_project_Page_Article_Editor_Editing,
                  x_slug,
                  x_errors,
                  A1(x_transform, x_form)),
          }));
    };
  } else if (tmp1 == CTOR_EditingNew) {
    {
      void* x_errors = Utils_destruct_index(x__v0, 0);
      void* x_form = Utils_destruct_index(x__v0, 1);
      tmp0 = Utils_update(x_model,
          1,
          ((u32[]){
              FIELD_status,
          }),
          ((void* []){
              A2(&author_project_Page_Article_Editor_EditingNew,
                  x_errors,
                  A1(x_transform, x_form)),
          }));
    };
  } else {
    void* x_form = Utils_destruct_index(x__v0, 0);
    tmp0 = Utils_update(x_model,
        1,
        ((u32[]){
            FIELD_status,
        }),
        ((void* []){
            A1(&author_project_Page_Article_Editor_Creating, A1(x_transform, x_form)),
        }));
  };
  void* x_newModel = tmp0;
  return NEW_TUPLE2(x_newModel, &elm_core_Platform_Cmd_none);
}
Closure author_project_Page_Article_Editor_updateForm = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Article_Editor_updateForm,
};
void* eval_author_project_Page_Article_Editor_update_lambda7(void* args[]) {
  void* x_body = args[0];
  void* x_form = args[1];
  return Utils_update(x_form,
      1,
      ((u32[]){
          FIELD_body,
      }),
      ((void* []){
          x_body,
      }));
}
void* eval_author_project_Page_Article_Editor_update_lambda8(void* args[]) {
  void* x_tags = args[0];
  void* x_form = args[1];
  return Utils_update(x_form,
      1,
      ((u32[]){
          FIELD_tags,
      }),
      ((void* []){
          x_tags,
      }));
}
void* eval_author_project_Page_Article_Editor_update_lambda9(void* args[]) {
  void* x_description = args[0];
  void* x_form = args[1];
  return Utils_update(x_form,
      1,
      ((u32[]){
          FIELD_description,
      }),
      ((void* []){
          x_description,
      }));
}
void* eval_author_project_Page_Article_Editor_update_lambda10(void* args[]) {
  void* x_title = args[0];
  void* x_form = args[1];
  return Utils_update(x_form,
      1,
      ((u32[]){
          FIELD_title,
      }),
      ((void* []){
          x_title,
      }));
}
void* eval_author_project_Page_Article_Editor_update_lambda11(void* args[]) {
  void* x_model = args[0];
  void* x_status = args[1];
  return Utils_update(x_model,
      1,
      ((u32[]){
          FIELD_status,
      }),
      ((void* []){
          x_status,
      }));
}
void* eval_author_project_Page_Article_Editor_update(void* args[]) {
  void* x_msg = args[0];
  void* x_model = args[1];
  void* tmp0;
  void* tmp1 = ((Custom*)x_msg)->ctor;
  if (tmp1 == CTOR_ClickedSave) {
    {
      void* x_cred = Utils_destruct_index(x_msg, 0);
      tmp0 = A2(&elm_core_Basics_apR,
          A2(&elm_core_Basics_apR,
              Utils_access_eval(((void* []){
                  (void*)FIELD_status,
                  x_model,
              })),
              A1(&author_project_Page_Article_Editor_save, x_cred)),
          A1(&elm_core_Tuple_mapFirst,
              NEW_CLOSURE(1,
                  2,
                  &eval_author_project_Page_Article_Editor_update_lambda11,
                  ((void* []){
                      x_model,
                  }))));
    };
  } else if (tmp1 == CTOR_EnteredTitle) {
    {
      void* x_title = Utils_destruct_index(x_msg, 0);
      tmp0 = A2(&author_project_Page_Article_Editor_updateForm,
          NEW_CLOSURE(1,
              2,
              &eval_author_project_Page_Article_Editor_update_lambda10,
              ((void* []){
                  x_title,
              })),
          x_model);
    };
  } else if (tmp1 == CTOR_EnteredDescription) {
    {
      void* x_description = Utils_destruct_index(x_msg, 0);
      tmp0 = A2(&author_project_Page_Article_Editor_updateForm,
          NEW_CLOSURE(1,
              2,
              &eval_author_project_Page_Article_Editor_update_lambda9,
              ((void* []){
                  x_description,
              })),
          x_model);
    };
  } else if (tmp1 == CTOR_EnteredTags) {
    {
      void* x_tags = Utils_destruct_index(x_msg, 0);
      tmp0 = A2(&author_project_Page_Article_Editor_updateForm,
          NEW_CLOSURE(1,
              2,
              &eval_author_project_Page_Article_Editor_update_lambda8,
              ((void* []){
                  x_tags,
              })),
          x_model);
    };
  } else if (tmp1 == CTOR_EnteredBody) {
    {
      void* x_body = Utils_destruct_index(x_msg, 0);
      tmp0 = A2(&author_project_Page_Article_Editor_updateForm,
          NEW_CLOSURE(1,
              2,
              &eval_author_project_Page_Article_Editor_update_lambda7,
              ((void* []){
                  x_body,
              })),
          x_model);
    };
  } else if (tmp1 == CTOR_CompletedCreate) {
    void* tmp6 = ((Custom*)Utils_destruct_index(x_msg, 0))->ctor;
    if (tmp6 == CTOR_Ok) {
      void* x_article = Utils_destruct_index(Utils_destruct_index(x_msg, 0), 0);
      tmp0 = NEW_TUPLE2(x_model,
          A2(&elm_core_Basics_apR,
              A1(&author_project_Route_Article,
                  A1(&author_project_Article_slug, x_article)),
              A1(&author_project_Route_replaceUrl,
                  A1(&author_project_Session_navKey,
                      Utils_access_eval(((void* []){
                          (void*)FIELD_session,
                          x_model,
                      }))))));
    } else {
      void* x_error = Utils_destruct_index(Utils_destruct_index(x_msg, 0), 0);
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_status,
                            }),
                            ((void* []){
                                A2(&author_project_Page_Article_Editor_savingError,
                                    x_error,
                                    Utils_access_eval(((void* []){
                                        (void*)FIELD_status,
                                        x_model,
                                    }))),
                            })),
          &elm_core_Platform_Cmd_none);
    };
  } else if (tmp1 == CTOR_CompletedEdit) {
    void* tmp5 = ((Custom*)Utils_destruct_index(x_msg, 0))->ctor;
    if (tmp5 == CTOR_Ok) {
      void* x_article = Utils_destruct_index(Utils_destruct_index(x_msg, 0), 0);
      tmp0 = NEW_TUPLE2(x_model,
          A2(&elm_core_Basics_apR,
              A1(&author_project_Route_Article,
                  A1(&author_project_Article_slug, x_article)),
              A1(&author_project_Route_replaceUrl,
                  A1(&author_project_Session_navKey,
                      Utils_access_eval(((void* []){
                          (void*)FIELD_session,
                          x_model,
                      }))))));
    } else {
      void* x_error = Utils_destruct_index(Utils_destruct_index(x_msg, 0), 0);
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_status,
                            }),
                            ((void* []){
                                A2(&author_project_Page_Article_Editor_savingError,
                                    x_error,
                                    Utils_access_eval(((void* []){
                                        (void*)FIELD_status,
                                        x_model,
                                    }))),
                            })),
          &elm_core_Platform_Cmd_none);
    };
  } else if (tmp1 == CTOR_CompletedArticleLoad) {
    void* tmp4 = ((Custom*)Utils_destruct_index(x_msg, 0))->ctor;
    if (tmp4 == CTOR_Err) {
      void* x__v1 = Utils_destruct_index(Utils_destruct_index(x_msg, 0), 0);
      void* x_slug = Utils_destruct_index(x__v1, 0);
      void* x_error = Utils_destruct_index(x__v1, 1);
      tmp0 = NEW_TUPLE2(
          Utils_update(x_model,
              1,
              ((u32[]){
                  FIELD_status,
              }),
              ((void* []){
                  A1(&author_project_Page_Article_Editor_LoadingFailed, x_slug),
              })),
          &elm_core_Platform_Cmd_none);
    } else {
      void* x_article = Utils_destruct_index(Utils_destruct_index(x_msg, 0), 0);
      void* x__v2 = A1(&author_project_Article_metadata, x_article);
      void* x_title = Utils_access_eval(((void* []){
          (void*)FIELD_title,
          x__v2,
      }));
      void* x_description = Utils_access_eval(((void* []){
          (void*)FIELD_description,
          x__v2,
      }));
      void* x_tags = Utils_access_eval(((void* []){
          (void*)FIELD_tags,
          x__v2,
      }));
      void* x_status = A3(&author_project_Page_Article_Editor_Editing,
          A1(&author_project_Article_slug, x_article),
          &Nil,
          NEW_RECORD(&fg_body_description_tags_title,
              4,
              ((void* []){
                  A1(&author_project_Article_Body_toMarkdownString,
                      A1(&author_project_Article_body, x_article)),
                  x_description,
                  A2(&elm_core_String_join, &literal_string__20, x_tags),
                  x_title,
              })));
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_status,
                            }),
                            ((void* []){
                                x_status,
                            })),
          &elm_core_Platform_Cmd_none);
    };
  } else if (tmp1 == CTOR_GotSession) {
    {
      void* x_session = Utils_destruct_index(x_msg, 0);
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_session,
                            }),
                            ((void* []){
                                x_session,
                            })),
          A2(&author_project_Route_replaceUrl,
              A1(&author_project_Session_navKey, x_session),
              &author_project_Route_Home));
    };
  } else {
    void* x__v3 = Utils_access_eval(((void* []){
        (void*)FIELD_status,
        x_model,
    }));
    void* tmp2;
    void* tmp3 = ((Custom*)x__v3)->ctor;
    if (tmp3 == CTOR_Loading) {
      void* x_slug = Utils_destruct_index(x__v3, 0);
      tmp2 = A1(&author_project_Page_Article_Editor_LoadingSlowly, x_slug);
    } else {
      void* x_other = x__v3;
      tmp2 = x_other;
    };
    void* x_status = tmp2;
    tmp0 = NEW_TUPLE2(Utils_update(x_model,
                          1,
                          ((u32[]){
                              FIELD_status,
                          }),
                          ((void* []){
                              x_status,
                          })),
        &elm_core_Platform_Cmd_none);
  };
  return tmp0;
}
Closure author_project_Page_Article_Editor_update = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Article_Editor_update,
};

Custom author_project_Page_Home_Failed = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Failed,
};

void* eval_author_project_Page_Home_GotFeedMsg(void* args[]) {
  return ctorCustom(CTOR_GotFeedMsg, 1, args);
}
Closure author_project_Page_Home_GotFeedMsg = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Home_GotFeedMsg,
};

void* eval_author_project_Page_Home_Loaded(void* args[]) {
  return ctorCustom(CTOR_Loaded, 1, args);
}
Closure author_project_Page_Home_Loaded = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Home_Loaded,
};

Custom author_project_Page_Home_LoadingSlowly = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_LoadingSlowly,
};

void* eval_author_project_Page_Home_TagFeed(void* args[]) {
  return ctorCustom(CTOR_TagFeed, 1, args);
}
Closure author_project_Page_Home_TagFeed = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Home_TagFeed,
};

#define elm_browser_Browser_Dom_setViewport Browser_setViewport
#define author_project_Page_Home_scrollToTop (*ptr_author_project_Page_Home_scrollToTop)
ElmValue* ptr_author_project_Page_Home_scrollToTop;
void* eval_author_project_Page_Home_scrollToTop_lambda0(void* args[]) {
  void* x__v0 = args[0];
  return A1(&elm_core_Task_succeed, &Unit);
}
void* init_author_project_Page_Home_scrollToTop() {
  return A2(&elm_core_Basics_apR,
      A2(&elm_browser_Browser_Dom_setViewport, &literal_int_0, &literal_int_0),
      A1(&elm_core_Task_onError,
          NEW_CLOSURE(
              0, 1, &eval_author_project_Page_Home_scrollToTop_lambda0, ((void* []){}))));
}

void* eval_author_project_Article_Feed_CompletedFavorite(void* args[]) {
  return ctorCustom(CTOR_CompletedFavorite, 1, args);
}
Closure author_project_Article_Feed_CompletedFavorite = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Article_Feed_CompletedFavorite,
};
void* eval_author_project_Article_Feed_fave(void* args[]) {
  void* x_toRequest = args[0];
  void* x_cred = args[1];
  void* x_slug = args[2];
  void* x_model = args[3];
  return NEW_TUPLE2(A1(&author_project_Article_Feed_Model, x_model),
      A2(&elm_core_Basics_apR,
          A2(&elm_core_Basics_apR,
              A2(x_toRequest, x_slug, x_cred),
              &elm_http_Http_toTask),
          A1(&elm_core_Task_attempt, &author_project_Article_Feed_CompletedFavorite)));
}
Closure author_project_Article_Feed_fave = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x4,
    .evaluator = &eval_author_project_Article_Feed_fave,
};

void* eval_author_project_PaginatedList_map(void* args[]) {
  void* x_transform = args[0];
  void* x__v0 = args[1];
  void* x_info = ((Custom*)x__v0)->values[0];
  return A1(&author_project_PaginatedList_PaginatedList,
      Utils_update(x_info,
          1,
          ((u32[]){
              FIELD_values,
          }),
          ((void* []){
              A2(&elm_core_List_map,
                  x_transform,
                  Utils_access_eval(((void* []){
                      (void*)FIELD_values,
                      x_info,
                  }))),
          })));
}
Closure author_project_PaginatedList_map = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_PaginatedList_map,
};

void* eval_author_project_Article_Feed_replaceArticle(void* args[]) {
  void* x_newArticle = args[0];
  void* x_oldArticle = args[1];
  void* tmp0;
  if (A2(&elm_core_Basics_eq,
          A1(&author_project_Article_slug, x_newArticle),
          A1(&author_project_Article_slug, x_oldArticle)) == &True) {
    tmp0 = x_newArticle;
  } else {
    tmp0 = x_oldArticle;
  };
  return tmp0;
}
Closure author_project_Article_Feed_replaceArticle = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Article_Feed_replaceArticle,
};
void* eval_author_project_Article_Feed_update(void* args[]) {
  void* x_maybeCred = args[0];
  void* x_msg = args[1];
  void* x__v0 = args[2];
  void* x_model = ((Custom*)x__v0)->values[0];
  void* tmp0;
  void* tmp1 = ((Custom*)x_msg)->ctor;
  void* tmp2 = ((Custom*)Utils_destruct_index(x_msg, 0))->ctor;
  if (tmp1 == CTOR_ClickedDismissErrors) {
    {
      tmp0 = NEW_TUPLE2(A1(&author_project_Article_Feed_Model,
                            Utils_update(x_model,
                                1,
                                ((u32[]){
                                    FIELD_errors,
                                }),
                                ((void* []){
                                    &Nil,
                                }))),
          &elm_core_Platform_Cmd_none);
    };
  } else if (tmp1 == CTOR_ClickedFavorite) {
    {
      void* x_cred = Utils_destruct_index(x_msg, 0);
      void* x_slug = Utils_destruct_index(x_msg, 1);
      tmp0 = A4(&author_project_Article_Feed_fave,
          &author_project_Article_favorite,
          x_cred,
          x_slug,
          x_model);
    };
  } else if (tmp1 == CTOR_ClickedUnfavorite) {
    {
      void* x_cred = Utils_destruct_index(x_msg, 0);
      void* x_slug = Utils_destruct_index(x_msg, 1);
      tmp0 = A4(&author_project_Article_Feed_fave,
          &author_project_Article_unfavorite,
          x_cred,
          x_slug,
          x_model);
    };
  } else if (tmp2 == CTOR_Ok) {
    void* x_article = Utils_destruct_index(Utils_destruct_index(x_msg, 0), 0);
    tmp0 = NEW_TUPLE2(
        A1(&author_project_Article_Feed_Model,
            Utils_update(x_model,
                1,
                ((u32[]){
                    FIELD_articles,
                }),
                ((void* []){
                    A2(&author_project_PaginatedList_map,
                        A1(&author_project_Article_Feed_replaceArticle, x_article),
                        Utils_access_eval(((void* []){
                            (void*)FIELD_articles,
                            x_model,
                        }))),
                }))),
        &elm_core_Platform_Cmd_none);
  } else {
    void* x_error = Utils_destruct_index(Utils_destruct_index(x_msg, 0), 0);
    tmp0 = NEW_TUPLE2(A1(&author_project_Article_Feed_Model,
                          Utils_update(x_model,
                              1,
                              ((u32[]){
                                  FIELD_errors,
                              }),
                              ((void* []){
                                  A1(&author_project_Api_addServerError,
                                      Utils_access_eval(((void* []){
                                          (void*)FIELD_errors,
                                          x_model,
                                      }))),
                              }))),
        &elm_core_Platform_Cmd_none);
  };
  return tmp0;
}
Closure author_project_Article_Feed_update = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_author_project_Article_Feed_update,
};
void* eval_author_project_Page_Home_update_lambda11(void* args[]) {
  void* x_feed = args[0];
  void* x__v1 = args[1];
  return x_feed;
}
void* eval_author_project_Page_Home_update_lambda10(void* args[]) {
  void* x_feed = args[0];
  return A2(&elm_core_Task_map,
      NEW_CLOSURE(1,
          2,
          &eval_author_project_Page_Home_update_lambda11,
          ((void* []){
              x_feed,
          })),
      &author_project_Page_Home_scrollToTop);
}
void* eval_author_project_Page_Home_update(void* args[]) {
  void* x_msg = args[0];
  void* x_model = args[1];
  void* tmp0;
  void* tmp1 = ((Custom*)x_msg)->ctor;
  if (tmp1 == CTOR_ClickedTag) {
    {
      void* x_tag = Utils_destruct_index(x_msg, 0);
      void* x_feedTab = A1(&author_project_Page_Home_TagFeed, x_tag);
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_feedTab,
                            }),
                            ((void* []){
                                x_feedTab,
                            })),
          A2(&elm_core_Basics_apR,
              A3(&author_project_Page_Home_fetchFeed,
                  Utils_access_eval(((void* []){
                      (void*)FIELD_session,
                      x_model,
                  })),
                  x_feedTab,
                  &literal_int_1),
              A1(&elm_core_Task_attempt, &author_project_Page_Home_CompletedFeedLoad)));
    };
  } else if (tmp1 == CTOR_ClickedTab) {
    {
      void* x_tab = Utils_destruct_index(x_msg, 0);
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_feedTab,
                            }),
                            ((void* []){
                                x_tab,
                            })),
          A2(&elm_core_Basics_apR,
              A3(&author_project_Page_Home_fetchFeed,
                  Utils_access_eval(((void* []){
                      (void*)FIELD_session,
                      x_model,
                  })),
                  x_tab,
                  &literal_int_1),
              A1(&elm_core_Task_attempt, &author_project_Page_Home_CompletedFeedLoad)));
    };
  } else if (tmp1 == CTOR_ClickedFeedPage) {
    {
      void* x_page = Utils_destruct_index(x_msg, 0);
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_feedPage,
                            }),
                            ((void* []){
                                x_page,
                            })),
          A2(&elm_core_Basics_apR,
              A2(&elm_core_Basics_apR,
                  A3(&author_project_Page_Home_fetchFeed,
                      Utils_access_eval(((void* []){
                          (void*)FIELD_session,
                          x_model,
                      })),
                      Utils_access_eval(((void* []){
                          (void*)FIELD_feedTab,
                          x_model,
                      })),
                      x_page),
                  A1(&elm_core_Task_andThen,
                      NEW_CLOSURE(0,
                          1,
                          &eval_author_project_Page_Home_update_lambda10,
                          ((void* []){})))),
              A1(&elm_core_Task_attempt, &author_project_Page_Home_CompletedFeedLoad)));
    };
  } else if (tmp1 == CTOR_CompletedFeedLoad) {
    void* tmp9 = ((Custom*)Utils_destruct_index(x_msg, 0))->ctor;
    if (tmp9 == CTOR_Ok) {
      void* x_feed = Utils_destruct_index(Utils_destruct_index(x_msg, 0), 0);
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_feed,
                            }),
                            ((void* []){
                                A1(&author_project_Page_Home_Loaded, x_feed),
                            })),
          &elm_core_Platform_Cmd_none);
    } else {
      void* x_error = Utils_destruct_index(Utils_destruct_index(x_msg, 0), 0);
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_feed,
                            }),
                            ((void* []){
                                &author_project_Page_Home_Failed,
                            })),
          &elm_core_Platform_Cmd_none);
    };
  } else if (tmp1 == CTOR_CompletedTagsLoad) {
    void* tmp8 = ((Custom*)Utils_destruct_index(x_msg, 0))->ctor;
    if (tmp8 == CTOR_Ok) {
      void* x_tags = Utils_destruct_index(Utils_destruct_index(x_msg, 0), 0);
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_tags,
                            }),
                            ((void* []){
                                A1(&author_project_Page_Home_Loaded, x_tags),
                            })),
          &elm_core_Platform_Cmd_none);
    } else {
      void* x_error = Utils_destruct_index(Utils_destruct_index(x_msg, 0), 0);
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_tags,
                            }),
                            ((void* []){
                                &author_project_Page_Home_Failed,
                            })),
          &author_project_Log_error);
    };
  } else if (tmp1 == CTOR_GotFeedMsg) {
    {
      void* x_subMsg = Utils_destruct_index(x_msg, 0);
      void* x__v2 = Utils_access_eval(((void* []){
          (void*)FIELD_feed,
          x_model,
      }));
      void* tmp6;
      void* tmp7 = ((Custom*)x__v2)->ctor;
      if (tmp7 == CTOR_Loaded) {
        {
          void* x_feed = Utils_destruct_index(x__v2, 0);
          void* x__v3 = A3(&author_project_Article_Feed_update,
              A1(&author_project_Session_cred,
                  Utils_access_eval(((void* []){
                      (void*)FIELD_session,
                      x_model,
                  }))),
              x_subMsg,
              x_feed);
          void* x_newFeed = Utils_destruct_index(x__v3, 0);
          void* x_subCmd = Utils_destruct_index(x__v3, 1);
          tmp6 = NEW_TUPLE2(Utils_update(x_model,
                                1,
                                ((u32[]){
                                    FIELD_feed,
                                }),
                                ((void* []){
                                    A1(&author_project_Page_Home_Loaded, x_newFeed),
                                })),
              A2(&elm_core_Platform_Cmd_map,
                  &author_project_Page_Home_GotFeedMsg,
                  x_subCmd));
        };
      } else if (tmp7 == CTOR_Loading) {
        { tmp6 = NEW_TUPLE2(x_model, &author_project_Log_error); };
      } else if (tmp7 == CTOR_LoadingSlowly) {
        { tmp6 = NEW_TUPLE2(x_model, &author_project_Log_error); };
      } else {
        tmp6 = NEW_TUPLE2(x_model, &author_project_Log_error);
      };
      tmp0 = tmp6;
    };
  } else if (tmp1 == CTOR_GotTimeZone) {
    {
      void* x_tz = Utils_destruct_index(x_msg, 0);
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_timeZone,
                            }),
                            ((void* []){
                                x_tz,
                            })),
          &elm_core_Platform_Cmd_none);
    };
  } else if (tmp1 == CTOR_GotSession) {
    {
      void* x_session = Utils_destruct_index(x_msg, 0);
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_session,
                            }),
                            ((void* []){
                                x_session,
                            })),
          &elm_core_Platform_Cmd_none);
    };
  } else {
    void* x__v5 = Utils_access_eval(((void* []){
        (void*)FIELD_tags,
        x_model,
    }));
    void* tmp2;
    void* tmp3 = ((Custom*)x__v5)->ctor;
    if (tmp3 == CTOR_Loading) {
      tmp2 = &author_project_Page_Home_LoadingSlowly;
    } else {
      void* x_other = x__v5;
      tmp2 = x_other;
    };
    void* x_tags = tmp2;
    void* x__v4 = Utils_access_eval(((void* []){
        (void*)FIELD_feed,
        x_model,
    }));
    void* tmp4;
    void* tmp5 = ((Custom*)x__v4)->ctor;
    if (tmp5 == CTOR_Loading) {
      tmp4 = &author_project_Page_Home_LoadingSlowly;
    } else {
      void* x_other = x__v4;
      tmp4 = x_other;
    };
    void* x_feed = tmp4;
    tmp0 = NEW_TUPLE2(Utils_update(x_model,
                          2,
                          ((u32[]){
                              FIELD_feed,
                              FIELD_tags,
                          }),
                          ((void* []){
                              x_feed,
                              x_tags,
                          })),
        &elm_core_Platform_Cmd_none);
  };
  return tmp0;
}
Closure author_project_Page_Home_update = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Home_update,
};

void* eval_author_project_Page_Login_CompletedLogin(void* args[]) {
  return ctorCustom(CTOR_CompletedLogin, 1, args);
}
Closure author_project_Page_Login_CompletedLogin = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Login_CompletedLogin,
};

void* eval_author_project_Page_Login_ServerError(void* args[]) {
  return ctorCustom(CTOR_ServerError, 1, args);
}
Closure author_project_Page_Login_ServerError = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Login_ServerError,
};

void* eval_author_project_Api_fromPair_lambda0(void* args[]) {
  void* x_field = args[0];
  void* x_error = args[1];
  return A2(&elm_core_Basics_append,
      x_field,
      A2(&elm_core_Basics_append, &literal_string__20, x_error));
}
void* eval_author_project_Api_fromPair(void* args[]) {
  void* x__v0 = args[0];
  void* x_field = Utils_destruct_index(x__v0, 0);
  void* x_errors = Utils_destruct_index(x__v0, 1);
  return A2(&elm_core_List_map,
      NEW_CLOSURE(1,
          2,
          &eval_author_project_Api_fromPair_lambda0,
          ((void* []){
              x_field,
          })),
      x_errors);
}
Closure author_project_Api_fromPair = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Api_fromPair,
};

#define elm_json_Json_Decode_keyValuePairs Json_decodeKeyValuePairs
#define author_project_Api_errorsDecoder (*ptr_author_project_Api_errorsDecoder)
ElmValue* ptr_author_project_Api_errorsDecoder;
void* init_author_project_Api_errorsDecoder() {
  return A2(&elm_core_Basics_apR,
      A1(&elm_json_Json_Decode_keyValuePairs,
          A1(&elm_json_Json_Decode_list, &elm_json_Json_Decode_string)),
      A1(&elm_json_Json_Decode_map,
          A1(&elm_core_List_concatMap, &author_project_Api_fromPair)));
}

void* eval_elm_core_Result_withDefault(void* args[]) {
  void* x_def = args[0];
  void* x_result = args[1];
  void* tmp0;
  void* tmp1 = ((Custom*)x_result)->ctor;
  if (tmp1 == CTOR_Ok) {
    void* x_a = Utils_destruct_index(x_result, 0);
    tmp0 = x_a;
  } else {
    tmp0 = x_def;
  };
  return tmp0;
}
Closure elm_core_Result_withDefault = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_core_Result_withDefault,
};
void* eval_author_project_Api_decodeErrors(void* args[]) {
  void* x_error = args[0];
  void* tmp0;
  void* tmp1 = ((Custom*)x_error)->ctor;
  if (tmp1 == CTOR_BadStatus) {
    void* x_response = Utils_destruct_index(x_error, 0);
    tmp0 = A2(&elm_core_Basics_apR,
        A2(&elm_core_Basics_apR,
            Utils_access_eval(((void* []){
                (void*)FIELD_body,
                x_response,
            })),
            A1(&elm_json_Json_Decode_decodeString,
                A2(&elm_json_Json_Decode_field,
                    &literal_string_errors,
                    &author_project_Api_errorsDecoder))),
        A1(&elm_core_Result_withDefault,
            List_fromArray(1,
                ((void* []){
                    &literal_string_Server_20error,
                }))));
  } else {
    void* x_err = x_error;
    tmp0 = List_fromArray(1,
        ((void* []){
            &literal_string_Server_20error,
        }));
  };
  return tmp0;
}
Closure author_project_Api_decodeErrors = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Api_decodeErrors,
};

#define author_project_Api_Endpoint_login (*ptr_author_project_Api_Endpoint_login)
ElmValue* ptr_author_project_Api_Endpoint_login;
void* init_author_project_Api_Endpoint_login() {
  return A2(&author_project_Api_Endpoint_url,
      List_fromArray(2,
          ((void* []){
              &literal_string_users,
              &literal_string_login,
          })),
      &Nil);
}
void* eval_author_project_Api_login(void* args[]) {
  void* x_body = args[0];
  void* x_decoder = args[1];
  return A4(&author_project_Api_post,
      &author_project_Api_Endpoint_login,
      &elm_core_Maybe_Nothing,
      x_body,
      A2(&elm_json_Json_Decode_field,
          &literal_string_user,
          A1(&author_project_Api_decoderFromCred, x_decoder)));
}
Closure author_project_Api_login = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Api_login,
};
void* eval_author_project_Page_Login_login(void* args[]) {
  void* x__v0 = args[0];
  void* x_form = ((Custom*)x__v0)->values[0];
  void* x_user = A1(&elm_json_Json_Encode_object,
      List_fromArray(2,
          ((void* []){
              NEW_TUPLE2(&literal_string_email,
                  A1(&elm_json_Json_Encode_string,
                      Utils_access_eval(((void* []){
                          (void*)FIELD_email,
                          x_form,
                      })))),
              NEW_TUPLE2(&literal_string_password,
                  A1(&elm_json_Json_Encode_string,
                      Utils_access_eval(((void* []){
                          (void*)FIELD_password,
                          x_form,
                      })))),
          })));
  void* x_body = A2(&elm_core_Basics_apR,
      A1(&elm_json_Json_Encode_object,
          List_fromArray(1,
              ((void* []){
                  NEW_TUPLE2(&literal_string_user, x_user),
              }))),
      &elm_http_Http_jsonBody);
  return A2(&author_project_Api_login, x_body, &author_project_Viewer_decoder);
}
Closure author_project_Page_Login_login = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Login_login,
};

void* eval_author_project_Avatar_encode(void* args[]) {
  void* x__v0 = args[0];
  void* x_maybeUrl = ((Custom*)x__v0)->values[0];
  void* tmp0;
  void* tmp1 = ((Custom*)x_maybeUrl)->ctor;
  if (tmp1 == CTOR_Just) {
    void* x_url = Utils_destruct_index(x_maybeUrl, 0);
    tmp0 = A1(&elm_json_Json_Encode_string, x_url);
  } else {
    tmp0 = &elm_json_Json_Encode_null;
  };
  return tmp0;
}
Closure author_project_Avatar_encode = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Avatar_encode,
};

void* eval_author_project_Username_encode(void* args[]) {
  void* x__v0 = args[0];
  void* x_username = ((Custom*)x__v0)->values[0];
  return A1(&elm_json_Json_Encode_string, x_username);
}
Closure author_project_Username_encode = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Username_encode,
};
void* eval_author_project_Api_storeCredWith(void* args[]) {
  void* x__v0 = args[0];
  void* x_avatar = args[1];
  void* x_uname = Utils_destruct_index(x__v0, 0);
  void* x_token = Utils_destruct_index(x__v0, 1);
  void* x_json = A1(&elm_json_Json_Encode_object,
      List_fromArray(1,
          ((void* []){
              NEW_TUPLE2(&literal_string_user,
                  A1(&elm_json_Json_Encode_object,
                      List_fromArray(3,
                          ((void* []){
                              NEW_TUPLE2(&literal_string_username,
                                  A1(&author_project_Username_encode, x_uname)),
                              NEW_TUPLE2(&literal_string_token,
                                  A1(&elm_json_Json_Encode_string, x_token)),
                              NEW_TUPLE2(&literal_string_image,
                                  A1(&author_project_Avatar_encode, x_avatar)),
                          })))),
          })));
  return A1(&author_project_Api_storeCache, A1(&elm_core_Maybe_Just, x_json));
}
Closure author_project_Api_storeCredWith = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Api_storeCredWith,
};
void* eval_author_project_Viewer_store(void* args[]) {
  void* x__v0 = args[0];
  void* x_avatarVal = Utils_destruct_index(x__v0, 0);
  void* x_credVal = Utils_destruct_index(x__v0, 1);
  return A2(&author_project_Api_storeCredWith, x_credVal, x_avatarVal);
}
Closure author_project_Viewer_store = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Viewer_store,
};

void* eval_author_project_Page_Login_updateForm(void* args[]) {
  void* x_transform = args[0];
  void* x_model = args[1];
  return NEW_TUPLE2(Utils_update(x_model,
                        1,
                        ((u32[]){
                            FIELD_form,
                        }),
                        ((void* []){
                            A1(x_transform,
                                Utils_access_eval(((void* []){
                                    (void*)FIELD_form,
                                    x_model,
                                }))),
                        })),
      &elm_core_Platform_Cmd_none);
}
Closure author_project_Page_Login_updateForm = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Login_updateForm,
};

Custom author_project_Page_Login_Email = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Email,
};

Custom author_project_Page_Login_Password = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Password,
};
#define author_project_Page_Login_fieldsToValidate \
  (*ptr_author_project_Page_Login_fieldsToValidate)
Cons* ptr_author_project_Page_Login_fieldsToValidate;
void* init_author_project_Page_Login_fieldsToValidate() {
  return List_fromArray(2,
      ((void* []){
          &author_project_Page_Login_Email,
          &author_project_Page_Login_Password,
      }));
}

void* eval_author_project_Page_Login_Trimmed(void* args[]) {
  return ctorCustom(CTOR_Trimmed, 1, args);
}
Closure author_project_Page_Login_Trimmed = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Login_Trimmed,
};
void* eval_author_project_Page_Login_trimFields(void* args[]) {
  void* x_form = args[0];
  return A1(&author_project_Page_Login_Trimmed,
      NEW_RECORD(&fg_email_password,
          2,
          ((void* []){
              A1(&elm_core_String_trim,
                  Utils_access_eval(((void* []){
                      (void*)FIELD_email,
                      x_form,
                  }))),
              A1(&elm_core_String_trim,
                  Utils_access_eval(((void* []){
                      (void*)FIELD_password,
                      x_form,
                  }))),
          })));
}
Closure author_project_Page_Login_trimFields = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Login_trimFields,
};

void* eval_author_project_Page_Login_InvalidEntry(void* args[]) {
  return ctorCustom(CTOR_InvalidEntry, 2, args);
}
Closure author_project_Page_Login_InvalidEntry = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Login_InvalidEntry,
};
void* eval_author_project_Page_Login_validateField(void* args[]) {
  void* x__v0 = args[0];
  void* x_field = args[1];
  void* x_form = ((Custom*)x__v0)->values[0];
  void* tmp0;
  void* tmp1 = ((Custom*)x_field)->ctor;
  if (tmp1 == CTOR_Email) {
    void* tmp2;
    if (A1(&elm_core_String_isEmpty,
            Utils_access_eval(((void* []){
                (void*)FIELD_email,
                x_form,
            }))) == &True) {
      tmp2 = List_fromArray(1,
          ((void* []){
              &literal_string_email_20can_5c_27t_20be_20blank_2e,
          }));
    } else {
      tmp2 = &Nil;
    };
    tmp0 = tmp2;
  } else {
    void* tmp3;
    if (A1(&elm_core_String_isEmpty,
            Utils_access_eval(((void* []){
                (void*)FIELD_password,
                x_form,
            }))) == &True) {
      tmp3 = List_fromArray(1,
          ((void* []){
              &literal_string_password_20can_5c_27t_20be_20blank_2e,
          }));
    } else {
      tmp3 = &Nil;
    };
    tmp0 = tmp3;
  };
  return A2(&elm_core_Basics_apL,
      A1(&elm_core_List_map, A1(&author_project_Page_Login_InvalidEntry, x_field)),
      tmp0);
}
Closure author_project_Page_Login_validateField = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Login_validateField,
};
void* eval_author_project_Page_Login_validate(void* args[]) {
  void* x_form = args[0];
  void* x_trimmedForm = A1(&author_project_Page_Login_trimFields, x_form);
  void* x__v0 = A2(&elm_core_List_concatMap,
      A1(&author_project_Page_Login_validateField, x_trimmedForm),
      &author_project_Page_Login_fieldsToValidate);
  void* tmp0;
  if (x__v0 == &Nil) {
    tmp0 = A1(&elm_core_Result_Ok, x_trimmedForm);
  } else {
    void* x_problems = x__v0;
    tmp0 = A1(&elm_core_Result_Err, x_problems);
  };
  return tmp0;
}
Closure author_project_Page_Login_validate = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Login_validate,
};
void* eval_author_project_Page_Login_update_lambda3(void* args[]) {
  void* x_password = args[0];
  void* x_form = args[1];
  return Utils_update(x_form,
      1,
      ((u32[]){
          FIELD_password,
      }),
      ((void* []){
          x_password,
      }));
}
void* eval_author_project_Page_Login_update_lambda4(void* args[]) {
  void* x_email = args[0];
  void* x_form = args[1];
  return Utils_update(x_form,
      1,
      ((u32[]){
          FIELD_email,
      }),
      ((void* []){
          x_email,
      }));
}
void* eval_author_project_Page_Login_update(void* args[]) {
  void* x_msg = args[0];
  void* x_model = args[1];
  void* tmp0;
  void* tmp1 = ((Custom*)x_msg)->ctor;
  if (tmp1 == CTOR_SubmittedForm) {
    {
      void* x__v1 = A1(&author_project_Page_Login_validate,
          Utils_access_eval(((void* []){
              (void*)FIELD_form,
              x_model,
          })));
      void* tmp5;
      void* tmp6 = ((Custom*)x__v1)->ctor;
      if (tmp6 == CTOR_Ok) {
        void* x_validForm = Utils_destruct_index(x__v1, 0);
        tmp5 = NEW_TUPLE2(Utils_update(x_model,
                              1,
                              ((u32[]){
                                  FIELD_problems,
                              }),
                              ((void* []){
                                  &Nil,
                              })),
            A2(&elm_http_Http_send,
                &author_project_Page_Login_CompletedLogin,
                A1(&author_project_Page_Login_login, x_validForm)));
      } else {
        void* x_problems = Utils_destruct_index(x__v1, 0);
        tmp5 = NEW_TUPLE2(Utils_update(x_model,
                              1,
                              ((u32[]){
                                  FIELD_problems,
                              }),
                              ((void* []){
                                  x_problems,
                              })),
            &elm_core_Platform_Cmd_none);
      };
      tmp0 = tmp5;
    };
  } else if (tmp1 == CTOR_EnteredEmail) {
    {
      void* x_email = Utils_destruct_index(x_msg, 0);
      tmp0 = A2(&author_project_Page_Login_updateForm,
          NEW_CLOSURE(1,
              2,
              &eval_author_project_Page_Login_update_lambda4,
              ((void* []){
                  x_email,
              })),
          x_model);
    };
  } else if (tmp1 == CTOR_EnteredPassword) {
    {
      void* x_password = Utils_destruct_index(x_msg, 0);
      tmp0 = A2(&author_project_Page_Login_updateForm,
          NEW_CLOSURE(1,
              2,
              &eval_author_project_Page_Login_update_lambda3,
              ((void* []){
                  x_password,
              })),
          x_model);
    };
  } else if (tmp1 == CTOR_CompletedLogin) {
    void* tmp2 = ((Custom*)Utils_destruct_index(x_msg, 0))->ctor;
    if (tmp2 == CTOR_Err) {
      void* x_error = Utils_destruct_index(Utils_destruct_index(x_msg, 0), 0);
      void* x_serverErrors = A2(&elm_core_Basics_apR,
          A1(&author_project_Api_decodeErrors, x_error),
          A1(&elm_core_List_map, &author_project_Page_Login_ServerError));
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_problems,
                            }),
                            ((void* []){
                                A2(&elm_core_List_append,
                                    Utils_access_eval(((void* []){
                                        (void*)FIELD_problems,
                                        x_model,
                                    })),
                                    x_serverErrors),
                            })),
          &elm_core_Platform_Cmd_none);
    } else {
      void* x_viewer = Utils_destruct_index(Utils_destruct_index(x_msg, 0), 0);
      tmp0 = NEW_TUPLE2(x_model, A1(&author_project_Viewer_store, x_viewer));
    };
  } else {
    void* x_session = Utils_destruct_index(x_msg, 0);
    tmp0 = NEW_TUPLE2(Utils_update(x_model,
                          1,
                          ((u32[]){
                              FIELD_session,
                          }),
                          ((void* []){
                              x_session,
                          })),
        A2(&author_project_Route_replaceUrl,
            A1(&author_project_Session_navKey, x_session),
            &author_project_Route_Home));
  };
  return tmp0;
}
Closure author_project_Page_Login_update = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Login_update,
};

void* eval_author_project_Page_Profile_CompletedFollowChange(void* args[]) {
  return ctorCustom(CTOR_CompletedFollowChange, 1, args);
}
Closure author_project_Page_Profile_CompletedFollowChange = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Profile_CompletedFollowChange,
};

void* eval_author_project_Page_Profile_Failed(void* args[]) {
  return ctorCustom(CTOR_Failed, 1, args);
}
Closure author_project_Page_Profile_Failed = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Profile_Failed,
};

void* eval_author_project_Page_Profile_GotFeedMsg(void* args[]) {
  return ctorCustom(CTOR_GotFeedMsg, 1, args);
}
Closure author_project_Page_Profile_GotFeedMsg = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Profile_GotFeedMsg,
};

void* eval_author_project_Page_Profile_Loaded(void* args[]) {
  return ctorCustom(CTOR_Loaded, 1, args);
}
Closure author_project_Page_Profile_Loaded = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Profile_Loaded,
};

void* eval_author_project_Page_Profile_LoadingSlowly(void* args[]) {
  return ctorCustom(CTOR_LoadingSlowly, 1, args);
}
Closure author_project_Page_Profile_LoadingSlowly = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Profile_LoadingSlowly,
};

void* eval_author_project_Author_username(void* args[]) {
  void* x_author = args[0];
  void* tmp0;
  void* tmp1 = ((Custom*)x_author)->ctor;
  if (tmp1 == CTOR_IsViewer) {
    {
      void* x_cred = Utils_destruct_index(x_author, 0);
      tmp0 = A1(&author_project_Api_username, x_cred);
    };
  } else if (tmp1 == CTOR_IsFollowing) {
    {
      void* x__v1 = Utils_destruct_index(x_author, 0);
      void* x_val = Utils_destruct_index(x__v1, 0);
      tmp0 = x_val;
    };
  } else {
    void* x__v2 = Utils_destruct_index(x_author, 0);
    void* x_val = Utils_destruct_index(x__v2, 0);
    tmp0 = x_val;
  };
  return tmp0;
}
Closure author_project_Author_username = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Author_username,
};
void* eval_author_project_Page_Profile_currentUsername(void* args[]) {
  void* x_model = args[0];
  void* x__v0 = Utils_access_eval(((void* []){
      (void*)FIELD_author,
      x_model,
  }));
  void* tmp0;
  void* tmp1 = ((Custom*)x__v0)->ctor;
  if (tmp1 == CTOR_Loading) {
    {
      void* x_username = Utils_destruct_index(x__v0, 0);
      tmp0 = x_username;
    };
  } else if (tmp1 == CTOR_LoadingSlowly) {
    {
      void* x_username = Utils_destruct_index(x__v0, 0);
      tmp0 = x_username;
    };
  } else if (tmp1 == CTOR_Loaded) {
    {
      void* x_author = Utils_destruct_index(x__v0, 0);
      tmp0 = A1(&author_project_Author_username, x_author);
    };
  } else {
    void* x_username = Utils_destruct_index(x__v0, 0);
    tmp0 = x_username;
  };
  return tmp0;
}
Closure author_project_Page_Profile_currentUsername = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Profile_currentUsername,
};
void* eval_author_project_Page_Profile_update(void* args[]) {
  void* x_msg = args[0];
  void* x_model = args[1];
  void* tmp0;
  void* tmp1 = ((Custom*)x_msg)->ctor;
  if (tmp1 == CTOR_ClickedDismissErrors) {
    {
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_errors,
                            }),
                            ((void* []){
                                &Nil,
                            })),
          &elm_core_Platform_Cmd_none);
    };
  } else if (tmp1 == CTOR_ClickedUnfollow) {
    {
      void* x_cred = Utils_destruct_index(x_msg, 0);
      void* x_followedAuthor = Utils_destruct_index(x_msg, 1);
      tmp0 = NEW_TUPLE2(x_model,
          A2(&elm_core_Basics_apR,
              A2(&author_project_Author_requestUnfollow, x_followedAuthor, x_cred),
              A1(&elm_http_Http_send,
                  &author_project_Page_Profile_CompletedFollowChange)));
    };
  } else if (tmp1 == CTOR_ClickedFollow) {
    {
      void* x_cred = Utils_destruct_index(x_msg, 0);
      void* x_unfollowedAuthor = Utils_destruct_index(x_msg, 1);
      tmp0 = NEW_TUPLE2(x_model,
          A2(&elm_core_Basics_apR,
              A2(&author_project_Author_requestFollow, x_unfollowedAuthor, x_cred),
              A1(&elm_http_Http_send,
                  &author_project_Page_Profile_CompletedFollowChange)));
    };
  } else if (tmp1 == CTOR_ClickedTab) {
    {
      void* x_tab = Utils_destruct_index(x_msg, 0);
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_feedTab,
                            }),
                            ((void* []){
                                x_tab,
                            })),
          A4(&author_project_Page_Profile_fetchFeed,
              Utils_access_eval(((void* []){
                  (void*)FIELD_session,
                  x_model,
              })),
              x_tab,
              A1(&author_project_Page_Profile_currentUsername, x_model),
              &literal_int_1));
    };
  } else if (tmp1 == CTOR_ClickedFeedPage) {
    {
      void* x_page = Utils_destruct_index(x_msg, 0);
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_feedPage,
                            }),
                            ((void* []){
                                x_page,
                            })),
          A4(&author_project_Page_Profile_fetchFeed,
              Utils_access_eval(((void* []){
                  (void*)FIELD_session,
                  x_model,
              })),
              Utils_access_eval(((void* []){
                  (void*)FIELD_feedTab,
                  x_model,
              })),
              A1(&author_project_Page_Profile_currentUsername, x_model),
              x_page));
    };
  } else if (tmp1 == CTOR_CompletedFollowChange) {
    void* tmp8 = ((Custom*)Utils_destruct_index(x_msg, 0))->ctor;
    if (tmp8 == CTOR_Ok) {
      void* x_newAuthor = Utils_destruct_index(Utils_destruct_index(x_msg, 0), 0);
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_author,
                            }),
                            ((void* []){
                                A1(&author_project_Page_Profile_Loaded, x_newAuthor),
                            })),
          &elm_core_Platform_Cmd_none);
    } else {
      void* x_error = Utils_destruct_index(Utils_destruct_index(x_msg, 0), 0);
      tmp0 = NEW_TUPLE2(x_model, &author_project_Log_error);
    };
  } else if (tmp1 == CTOR_CompletedAuthorLoad) {
    void* tmp7 = ((Custom*)Utils_destruct_index(x_msg, 0))->ctor;
    if (tmp7 == CTOR_Ok) {
      void* x_author = Utils_destruct_index(Utils_destruct_index(x_msg, 0), 0);
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_author,
                            }),
                            ((void* []){
                                A1(&author_project_Page_Profile_Loaded, x_author),
                            })),
          &elm_core_Platform_Cmd_none);
    } else {
      void* x__v1 = Utils_destruct_index(Utils_destruct_index(x_msg, 0), 0);
      void* x_username = Utils_destruct_index(x__v1, 0);
      void* x_err = Utils_destruct_index(x__v1, 1);
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_author,
                            }),
                            ((void* []){
                                A1(&author_project_Page_Profile_Failed, x_username),
                            })),
          &author_project_Log_error);
    };
  } else if (tmp1 == CTOR_CompletedFeedLoad) {
    void* tmp6 = ((Custom*)Utils_destruct_index(x_msg, 0))->ctor;
    if (tmp6 == CTOR_Ok) {
      void* x_feed = Utils_destruct_index(Utils_destruct_index(x_msg, 0), 0);
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_feed,
                            }),
                            ((void* []){
                                A1(&author_project_Page_Profile_Loaded, x_feed),
                            })),
          &elm_core_Platform_Cmd_none);
    } else {
      void* x__v2 = Utils_destruct_index(Utils_destruct_index(x_msg, 0), 0);
      void* x_username = Utils_destruct_index(x__v2, 0);
      void* x_err = Utils_destruct_index(x__v2, 1);
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_feed,
                            }),
                            ((void* []){
                                A1(&author_project_Page_Profile_Failed, x_username),
                            })),
          &author_project_Log_error);
    };
  } else if (tmp1 == CTOR_GotFeedMsg) {
    {
      void* x_subMsg = Utils_destruct_index(x_msg, 0);
      void* x__v3 = Utils_access_eval(((void* []){
          (void*)FIELD_feed,
          x_model,
      }));
      void* tmp4;
      void* tmp5 = ((Custom*)x__v3)->ctor;
      if (tmp5 == CTOR_Loaded) {
        {
          void* x_feed = Utils_destruct_index(x__v3, 0);
          void* x__v4 = A3(&author_project_Article_Feed_update,
              A1(&author_project_Session_cred,
                  Utils_access_eval(((void* []){
                      (void*)FIELD_session,
                      x_model,
                  }))),
              x_subMsg,
              x_feed);
          void* x_newFeed = Utils_destruct_index(x__v4, 0);
          void* x_subCmd = Utils_destruct_index(x__v4, 1);
          tmp4 = NEW_TUPLE2(Utils_update(x_model,
                                1,
                                ((u32[]){
                                    FIELD_feed,
                                }),
                                ((void* []){
                                    A1(&author_project_Page_Profile_Loaded, x_newFeed),
                                })),
              A2(&elm_core_Platform_Cmd_map,
                  &author_project_Page_Profile_GotFeedMsg,
                  x_subCmd));
        };
      } else if (tmp5 == CTOR_Loading) {
        { tmp4 = NEW_TUPLE2(x_model, &author_project_Log_error); };
      } else if (tmp5 == CTOR_LoadingSlowly) {
        { tmp4 = NEW_TUPLE2(x_model, &author_project_Log_error); };
      } else {
        tmp4 = NEW_TUPLE2(x_model, &author_project_Log_error);
      };
      tmp0 = tmp4;
    };
  } else if (tmp1 == CTOR_GotTimeZone) {
    {
      void* x_tz = Utils_destruct_index(x_msg, 0);
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_timeZone,
                            }),
                            ((void* []){
                                x_tz,
                            })),
          &elm_core_Platform_Cmd_none);
    };
  } else if (tmp1 == CTOR_GotSession) {
    {
      void* x_session = Utils_destruct_index(x_msg, 0);
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_session,
                            }),
                            ((void* []){
                                x_session,
                            })),
          A2(&author_project_Route_replaceUrl,
              A1(&author_project_Session_navKey, x_session),
              &author_project_Route_Home));
    };
  } else {
    void* x__v5 = Utils_access_eval(((void* []){
        (void*)FIELD_feed,
        x_model,
    }));
    void* tmp2;
    void* tmp3 = ((Custom*)x__v5)->ctor;
    if (tmp3 == CTOR_Loading) {
      void* x_username = Utils_destruct_index(x__v5, 0);
      tmp2 = A1(&author_project_Page_Profile_LoadingSlowly, x_username);
    } else {
      void* x_other = x__v5;
      tmp2 = x_other;
    };
    void* x_feed = tmp2;
    tmp0 = NEW_TUPLE2(Utils_update(x_model,
                          1,
                          ((u32[]){
                              FIELD_feed,
                          }),
                          ((void* []){
                              x_feed,
                          })),
        &elm_core_Platform_Cmd_none);
  };
  return tmp0;
}
Closure author_project_Page_Profile_update = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Profile_update,
};

void* eval_author_project_Page_Register_CompletedRegister(void* args[]) {
  return ctorCustom(CTOR_CompletedRegister, 1, args);
}
Closure author_project_Page_Register_CompletedRegister = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Register_CompletedRegister,
};

void* eval_author_project_Page_Register_ServerError(void* args[]) {
  return ctorCustom(CTOR_ServerError, 1, args);
}
Closure author_project_Page_Register_ServerError = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Register_ServerError,
};

#define author_project_Api_Endpoint_users (*ptr_author_project_Api_Endpoint_users)
ElmValue* ptr_author_project_Api_Endpoint_users;
void* init_author_project_Api_Endpoint_users() {
  return A2(&author_project_Api_Endpoint_url,
      List_fromArray(1,
          ((void* []){
              &literal_string_users,
          })),
      &Nil);
}
void* eval_author_project_Api_register(void* args[]) {
  void* x_body = args[0];
  void* x_decoder = args[1];
  return A4(&author_project_Api_post,
      &author_project_Api_Endpoint_users,
      &elm_core_Maybe_Nothing,
      x_body,
      A2(&elm_json_Json_Decode_field,
          &literal_string_user,
          A1(&author_project_Api_decoderFromCred, x_decoder)));
}
Closure author_project_Api_register = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Api_register,
};
void* eval_author_project_Page_Register_register(void* args[]) {
  void* x__v0 = args[0];
  void* x_form = ((Custom*)x__v0)->values[0];
  void* x_user = A1(&elm_json_Json_Encode_object,
      List_fromArray(3,
          ((void* []){
              NEW_TUPLE2(&literal_string_username,
                  A1(&elm_json_Json_Encode_string,
                      Utils_access_eval(((void* []){
                          (void*)FIELD_username,
                          x_form,
                      })))),
              NEW_TUPLE2(&literal_string_email,
                  A1(&elm_json_Json_Encode_string,
                      Utils_access_eval(((void* []){
                          (void*)FIELD_email,
                          x_form,
                      })))),
              NEW_TUPLE2(&literal_string_password,
                  A1(&elm_json_Json_Encode_string,
                      Utils_access_eval(((void* []){
                          (void*)FIELD_password,
                          x_form,
                      })))),
          })));
  void* x_body = A2(&elm_core_Basics_apR,
      A1(&elm_json_Json_Encode_object,
          List_fromArray(1,
              ((void* []){
                  NEW_TUPLE2(&literal_string_user, x_user),
              }))),
      &elm_http_Http_jsonBody);
  return A2(&author_project_Api_register, x_body, &author_project_Viewer_decoder);
}
Closure author_project_Page_Register_register = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Register_register,
};

void* eval_author_project_Page_Register_updateForm(void* args[]) {
  void* x_transform = args[0];
  void* x_model = args[1];
  return NEW_TUPLE2(Utils_update(x_model,
                        1,
                        ((u32[]){
                            FIELD_form,
                        }),
                        ((void* []){
                            A1(x_transform,
                                Utils_access_eval(((void* []){
                                    (void*)FIELD_form,
                                    x_model,
                                }))),
                        })),
      &elm_core_Platform_Cmd_none);
}
Closure author_project_Page_Register_updateForm = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Register_updateForm,
};

Custom author_project_Page_Register_Email = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Email,
};

Custom author_project_Page_Register_Password = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Password,
};

Custom author_project_Page_Register_Username = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Username,
};
#define author_project_Page_Register_fieldsToValidate \
  (*ptr_author_project_Page_Register_fieldsToValidate)
Cons* ptr_author_project_Page_Register_fieldsToValidate;
void* init_author_project_Page_Register_fieldsToValidate() {
  return List_fromArray(3,
      ((void* []){
          &author_project_Page_Register_Username,
          &author_project_Page_Register_Email,
          &author_project_Page_Register_Password,
      }));
}

void* eval_author_project_Page_Register_Trimmed(void* args[]) {
  return ctorCustom(CTOR_Trimmed, 1, args);
}
Closure author_project_Page_Register_Trimmed = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Register_Trimmed,
};
void* eval_author_project_Page_Register_trimFields(void* args[]) {
  void* x_form = args[0];
  return A1(&author_project_Page_Register_Trimmed,
      NEW_RECORD(&fg_email_password_username,
          3,
          ((void* []){
              A1(&elm_core_String_trim,
                  Utils_access_eval(((void* []){
                      (void*)FIELD_email,
                      x_form,
                  }))),
              A1(&elm_core_String_trim,
                  Utils_access_eval(((void* []){
                      (void*)FIELD_password,
                      x_form,
                  }))),
              A1(&elm_core_String_trim,
                  Utils_access_eval(((void* []){
                      (void*)FIELD_username,
                      x_form,
                  }))),
          })));
}
Closure author_project_Page_Register_trimFields = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Register_trimFields,
};

void* eval_author_project_Page_Register_InvalidEntry(void* args[]) {
  return ctorCustom(CTOR_InvalidEntry, 2, args);
}
Closure author_project_Page_Register_InvalidEntry = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Register_InvalidEntry,
};

#define author_project_Viewer_minPasswordChars literal_int_6
void* eval_author_project_Page_Register_validateField(void* args[]) {
  void* x__v0 = args[0];
  void* x_field = args[1];
  void* x_form = ((Custom*)x__v0)->values[0];
  void* tmp0;
  void* tmp1 = ((Custom*)x_field)->ctor;
  if (tmp1 == CTOR_Username) {
    {
      void* tmp4;
      if (A1(&elm_core_String_isEmpty,
              Utils_access_eval(((void* []){
                  (void*)FIELD_username,
                  x_form,
              }))) == &True) {
        tmp4 = List_fromArray(1,
            ((void* []){
                &literal_string_username_20can_5c_27t_20be_20blank_2e,
            }));
      } else {
        tmp4 = &Nil;
      };
      tmp0 = tmp4;
    };
  } else if (tmp1 == CTOR_Email) {
    {
      void* tmp3;
      if (A1(&elm_core_String_isEmpty,
              Utils_access_eval(((void* []){
                  (void*)FIELD_email,
                  x_form,
              }))) == &True) {
        tmp3 = List_fromArray(1,
            ((void* []){
                &literal_string_email_20can_5c_27t_20be_20blank_2e,
            }));
      } else {
        tmp3 = &Nil;
      };
      tmp0 = tmp3;
    };
  } else {
    void* tmp2;
    if (A1(&elm_core_String_isEmpty,
            Utils_access_eval(((void* []){
                (void*)FIELD_password,
                x_form,
            }))) == &True) {
      tmp2 = List_fromArray(1,
          ((void* []){
              &literal_string_password_20can_5c_27t_20be_20blank_2e,
          }));
    } else if (A2(&elm_core_Basics_lt,
                   A1(&elm_core_String_length,
                       Utils_access_eval(((void* []){
                           (void*)FIELD_password,
                           x_form,
                       }))),
                   &author_project_Viewer_minPasswordChars) == &True) {
      tmp2 = List_fromArray(1,
          ((void* []){
              A2(&elm_core_Basics_append,
                  &literal_string_password_20must_20be_20at_20least_20,
                  A2(&elm_core_Basics_append,
                      A1(&elm_core_String_fromInt,
                          &author_project_Viewer_minPasswordChars),
                      &literal_string__20characters_20long_2e)),
          }));
    } else {
      tmp2 = &Nil;
    };
    tmp0 = tmp2;
  };
  return A2(&elm_core_Basics_apL,
      A1(&elm_core_List_map, A1(&author_project_Page_Register_InvalidEntry, x_field)),
      tmp0);
}
Closure author_project_Page_Register_validateField = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Register_validateField,
};
void* eval_author_project_Page_Register_validate(void* args[]) {
  void* x_form = args[0];
  void* x_trimmedForm = A1(&author_project_Page_Register_trimFields, x_form);
  void* x__v0 = A2(&elm_core_List_concatMap,
      A1(&author_project_Page_Register_validateField, x_trimmedForm),
      &author_project_Page_Register_fieldsToValidate);
  void* tmp0;
  if (x__v0 == &Nil) {
    tmp0 = A1(&elm_core_Result_Ok, x_trimmedForm);
  } else {
    void* x_problems = x__v0;
    tmp0 = A1(&elm_core_Result_Err, x_problems);
  };
  return tmp0;
}
Closure author_project_Page_Register_validate = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Register_validate,
};
void* eval_author_project_Page_Register_update_lambda3(void* args[]) {
  void* x_password = args[0];
  void* x_form = args[1];
  return Utils_update(x_form,
      1,
      ((u32[]){
          FIELD_password,
      }),
      ((void* []){
          x_password,
      }));
}
void* eval_author_project_Page_Register_update_lambda4(void* args[]) {
  void* x_email = args[0];
  void* x_form = args[1];
  return Utils_update(x_form,
      1,
      ((u32[]){
          FIELD_email,
      }),
      ((void* []){
          x_email,
      }));
}
void* eval_author_project_Page_Register_update_lambda5(void* args[]) {
  void* x_username = args[0];
  void* x_form = args[1];
  return Utils_update(x_form,
      1,
      ((u32[]){
          FIELD_username,
      }),
      ((void* []){
          x_username,
      }));
}
void* eval_author_project_Page_Register_update(void* args[]) {
  void* x_msg = args[0];
  void* x_model = args[1];
  void* tmp0;
  void* tmp1 = ((Custom*)x_msg)->ctor;
  if (tmp1 == CTOR_SubmittedForm) {
    {
      void* x__v1 = A1(&author_project_Page_Register_validate,
          Utils_access_eval(((void* []){
              (void*)FIELD_form,
              x_model,
          })));
      void* tmp6;
      void* tmp7 = ((Custom*)x__v1)->ctor;
      if (tmp7 == CTOR_Ok) {
        void* x_validForm = Utils_destruct_index(x__v1, 0);
        tmp6 = NEW_TUPLE2(Utils_update(x_model,
                              1,
                              ((u32[]){
                                  FIELD_problems,
                              }),
                              ((void* []){
                                  &Nil,
                              })),
            A2(&elm_http_Http_send,
                &author_project_Page_Register_CompletedRegister,
                A1(&author_project_Page_Register_register, x_validForm)));
      } else {
        void* x_problems = Utils_destruct_index(x__v1, 0);
        tmp6 = NEW_TUPLE2(Utils_update(x_model,
                              1,
                              ((u32[]){
                                  FIELD_problems,
                              }),
                              ((void* []){
                                  x_problems,
                              })),
            &elm_core_Platform_Cmd_none);
      };
      tmp0 = tmp6;
    };
  } else if (tmp1 == CTOR_EnteredUsername) {
    {
      void* x_username = Utils_destruct_index(x_msg, 0);
      tmp0 = A2(&author_project_Page_Register_updateForm,
          NEW_CLOSURE(1,
              2,
              &eval_author_project_Page_Register_update_lambda5,
              ((void* []){
                  x_username,
              })),
          x_model);
    };
  } else if (tmp1 == CTOR_EnteredEmail) {
    {
      void* x_email = Utils_destruct_index(x_msg, 0);
      tmp0 = A2(&author_project_Page_Register_updateForm,
          NEW_CLOSURE(1,
              2,
              &eval_author_project_Page_Register_update_lambda4,
              ((void* []){
                  x_email,
              })),
          x_model);
    };
  } else if (tmp1 == CTOR_EnteredPassword) {
    {
      void* x_password = Utils_destruct_index(x_msg, 0);
      tmp0 = A2(&author_project_Page_Register_updateForm,
          NEW_CLOSURE(1,
              2,
              &eval_author_project_Page_Register_update_lambda3,
              ((void* []){
                  x_password,
              })),
          x_model);
    };
  } else if (tmp1 == CTOR_CompletedRegister) {
    void* tmp2 = ((Custom*)Utils_destruct_index(x_msg, 0))->ctor;
    if (tmp2 == CTOR_Err) {
      void* x_error = Utils_destruct_index(Utils_destruct_index(x_msg, 0), 0);
      void* x_serverErrors = A2(&elm_core_Basics_apR,
          A1(&author_project_Api_decodeErrors, x_error),
          A1(&elm_core_List_map, &author_project_Page_Register_ServerError));
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_problems,
                            }),
                            ((void* []){
                                A2(&elm_core_List_append,
                                    Utils_access_eval(((void* []){
                                        (void*)FIELD_problems,
                                        x_model,
                                    })),
                                    x_serverErrors),
                            })),
          &elm_core_Platform_Cmd_none);
    } else {
      void* x_viewer = Utils_destruct_index(Utils_destruct_index(x_msg, 0), 0);
      tmp0 = NEW_TUPLE2(x_model, A1(&author_project_Viewer_store, x_viewer));
    };
  } else {
    void* x_session = Utils_destruct_index(x_msg, 0);
    tmp0 = NEW_TUPLE2(Utils_update(x_model,
                          1,
                          ((u32[]){
                              FIELD_session,
                          }),
                          ((void* []){
                              x_session,
                          })),
        A2(&author_project_Route_replaceUrl,
            A1(&author_project_Session_navKey, x_session),
            &author_project_Route_Home));
  };
  return tmp0;
}
Closure author_project_Page_Register_update = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Register_update,
};

void* eval_author_project_Page_Settings_CompletedSave(void* args[]) {
  return ctorCustom(CTOR_CompletedSave, 1, args);
}
Closure author_project_Page_Settings_CompletedSave = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Settings_CompletedSave,
};

Custom author_project_Page_Settings_Failed = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Failed,
};

void* eval_author_project_Page_Settings_Loaded(void* args[]) {
  return ctorCustom(CTOR_Loaded, 1, args);
}
Closure author_project_Page_Settings_Loaded = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Settings_Loaded,
};

Custom author_project_Page_Settings_LoadingSlowly = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_LoadingSlowly,
};

void* eval_author_project_Page_Settings_ServerError(void* args[]) {
  return ctorCustom(CTOR_ServerError, 1, args);
}
Closure author_project_Page_Settings_ServerError = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Settings_ServerError,
};

void* eval_author_project_Api_settings(void* args[]) {
  void* x_cred = args[0];
  void* x_body = args[1];
  void* x_decoder = args[2];
  return A4(&author_project_Api_put,
      &author_project_Api_Endpoint_user,
      x_cred,
      x_body,
      A2(&elm_json_Json_Decode_field,
          &literal_string_user,
          A1(&author_project_Api_decoderFromCred, x_decoder)));
}
Closure author_project_Api_settings = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_author_project_Api_settings,
};
void* eval_author_project_Page_Settings_edit(void* args[]) {
  void* x_cred = args[0];
  void* x__v0 = args[1];
  void* x_form = ((Custom*)x__v0)->values[0];
  void* x__v2 = Utils_access_eval(((void* []){
      (void*)FIELD_avatar,
      x_form,
  }));
  void* tmp0;
  if (A2(Utils_equal, x__v2, &literal_string_) == &True) {
    tmp0 = &elm_json_Json_Encode_null;
  } else {
    void* x_avatar = x__v2;
    tmp0 = A1(&elm_json_Json_Encode_string, x_avatar);
  };
  void* x_encodedAvatar = tmp0;
  void* x_updates = List_fromArray(4,
      ((void* []){
          NEW_TUPLE2(&literal_string_username,
              A1(&elm_json_Json_Encode_string,
                  Utils_access_eval(((void* []){
                      (void*)FIELD_username,
                      x_form,
                  })))),
          NEW_TUPLE2(&literal_string_email,
              A1(&elm_json_Json_Encode_string,
                  Utils_access_eval(((void* []){
                      (void*)FIELD_email,
                      x_form,
                  })))),
          NEW_TUPLE2(&literal_string_bio,
              A1(&elm_json_Json_Encode_string,
                  Utils_access_eval(((void* []){
                      (void*)FIELD_bio,
                      x_form,
                  })))),
          NEW_TUPLE2(&literal_string_image, x_encodedAvatar),
      }));
  void* x__v1 = Utils_access_eval(((void* []){
      (void*)FIELD_password,
      x_form,
  }));
  void* tmp1;
  if (A2(Utils_equal, x__v1, &literal_string_) == &True) {
    tmp1 = x_updates;
  } else {
    void* x_password = x__v1;
    tmp1 = A2(&elm_core_List_cons,
        NEW_TUPLE2(
            &literal_string_password, A1(&elm_json_Json_Encode_string, x_password)),
        x_updates);
  };
  void* x_encodedUser = A2(&elm_core_Basics_apL, &elm_json_Json_Encode_object, tmp1);
  void* x_body = A2(&elm_core_Basics_apR,
      A1(&elm_json_Json_Encode_object,
          List_fromArray(1,
              ((void* []){
                  NEW_TUPLE2(&literal_string_user, x_encodedUser),
              }))),
      &elm_http_Http_jsonBody);
  return A3(&author_project_Api_settings, x_cred, x_body, &author_project_Viewer_decoder);
}
Closure author_project_Page_Settings_edit = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Settings_edit,
};

void* eval_author_project_Page_Settings_updateForm(void* args[]) {
  void* x_transform = args[0];
  void* x_model = args[1];
  void* x__v0 = Utils_access_eval(((void* []){
      (void*)FIELD_status,
      x_model,
  }));
  void* tmp0;
  void* tmp1 = ((Custom*)x__v0)->ctor;
  if (tmp1 == CTOR_Loaded) {
    void* x_form = Utils_destruct_index(x__v0, 0);
    tmp0 = NEW_TUPLE2(
        Utils_update(x_model,
            1,
            ((u32[]){
                FIELD_status,
            }),
            ((void* []){
                A1(&author_project_Page_Settings_Loaded, A1(x_transform, x_form)),
            })),
        &elm_core_Platform_Cmd_none);
  } else {
    tmp0 = NEW_TUPLE2(x_model, &author_project_Log_error);
  };
  return tmp0;
}
Closure author_project_Page_Settings_updateForm = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Settings_updateForm,
};

Custom author_project_Page_Settings_Email = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Email,
};

Custom author_project_Page_Settings_Password = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Password,
};

Custom author_project_Page_Settings_Username = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Username,
};
#define author_project_Page_Settings_fieldsToValidate \
  (*ptr_author_project_Page_Settings_fieldsToValidate)
Cons* ptr_author_project_Page_Settings_fieldsToValidate;
void* init_author_project_Page_Settings_fieldsToValidate() {
  return List_fromArray(3,
      ((void* []){
          &author_project_Page_Settings_Username,
          &author_project_Page_Settings_Email,
          &author_project_Page_Settings_Password,
      }));
}

void* eval_author_project_Page_Settings_Trimmed(void* args[]) {
  return ctorCustom(CTOR_Trimmed, 1, args);
}
Closure author_project_Page_Settings_Trimmed = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Settings_Trimmed,
};
void* eval_author_project_Page_Settings_trimFields(void* args[]) {
  void* x_form = args[0];
  return A1(&author_project_Page_Settings_Trimmed,
      NEW_RECORD(&fg_avatar_bio_email_password_username,
          5,
          ((void* []){
              A1(&elm_core_String_trim,
                  Utils_access_eval(((void* []){
                      (void*)FIELD_avatar,
                      x_form,
                  }))),
              A1(&elm_core_String_trim,
                  Utils_access_eval(((void* []){
                      (void*)FIELD_bio,
                      x_form,
                  }))),
              A1(&elm_core_String_trim,
                  Utils_access_eval(((void* []){
                      (void*)FIELD_email,
                      x_form,
                  }))),
              A1(&elm_core_String_trim,
                  Utils_access_eval(((void* []){
                      (void*)FIELD_password,
                      x_form,
                  }))),
              A1(&elm_core_String_trim,
                  Utils_access_eval(((void* []){
                      (void*)FIELD_username,
                      x_form,
                  }))),
          })));
}
Closure author_project_Page_Settings_trimFields = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Settings_trimFields,
};

void* eval_author_project_Page_Settings_InvalidEntry(void* args[]) {
  return ctorCustom(CTOR_InvalidEntry, 2, args);
}
Closure author_project_Page_Settings_InvalidEntry = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Settings_InvalidEntry,
};
void* eval_author_project_Page_Settings_validateField(void* args[]) {
  void* x__v0 = args[0];
  void* x_field = args[1];
  void* x_form = ((Custom*)x__v0)->values[0];
  void* tmp0;
  void* tmp1 = ((Custom*)x_field)->ctor;
  if (tmp1 == CTOR_Username) {
    {
      void* tmp4;
      if (A1(&elm_core_String_isEmpty,
              Utils_access_eval(((void* []){
                  (void*)FIELD_username,
                  x_form,
              }))) == &True) {
        tmp4 = List_fromArray(1,
            ((void* []){
                &literal_string_username_20can_5c_27t_20be_20blank_2e,
            }));
      } else {
        tmp4 = &Nil;
      };
      tmp0 = tmp4;
    };
  } else if (tmp1 == CTOR_Email) {
    {
      void* tmp3;
      if (A1(&elm_core_String_isEmpty,
              Utils_access_eval(((void* []){
                  (void*)FIELD_email,
                  x_form,
              }))) == &True) {
        tmp3 = List_fromArray(1,
            ((void* []){
                &literal_string_email_20can_5c_27t_20be_20blank_2e,
            }));
      } else {
        tmp3 = &Nil;
      };
      tmp0 = tmp3;
    };
  } else {
    void* x_passwordLength = A1(&elm_core_String_length,
        Utils_access_eval(((void* []){
            (void*)FIELD_password,
            x_form,
        })));
    void* tmp2;
    if (A2(&elm_core_Basics_and,
            A2(&elm_core_Basics_gt, x_passwordLength, &literal_int_0),
            A2(&elm_core_Basics_lt,
                x_passwordLength,
                &author_project_Viewer_minPasswordChars)) == &True) {
      tmp2 = List_fromArray(1,
          ((void* []){
              A2(&elm_core_Basics_append,
                  &literal_string_password_20must_20be_20at_20least_20,
                  A2(&elm_core_Basics_append,
                      A1(&elm_core_String_fromInt,
                          &author_project_Viewer_minPasswordChars),
                      &literal_string__20characters_20long_2e)),
          }));
    } else {
      tmp2 = &Nil;
    };
    tmp0 = tmp2;
  };
  return A2(&elm_core_Basics_apL,
      A1(&elm_core_List_map, A1(&author_project_Page_Settings_InvalidEntry, x_field)),
      tmp0);
}
Closure author_project_Page_Settings_validateField = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Settings_validateField,
};
void* eval_author_project_Page_Settings_validate(void* args[]) {
  void* x_form = args[0];
  void* x_trimmedForm = A1(&author_project_Page_Settings_trimFields, x_form);
  void* x__v0 = A2(&elm_core_List_concatMap,
      A1(&author_project_Page_Settings_validateField, x_trimmedForm),
      &author_project_Page_Settings_fieldsToValidate);
  void* tmp0;
  if (x__v0 == &Nil) {
    tmp0 = A1(&elm_core_Result_Ok, x_trimmedForm);
  } else {
    void* x_problems = x__v0;
    tmp0 = A1(&elm_core_Result_Err, x_problems);
  };
  return tmp0;
}
Closure author_project_Page_Settings_validate = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Settings_validate,
};
void* eval_author_project_Page_Settings_update_lambda5(void* args[]) {
  void* x_avatar = args[0];
  void* x_form = args[1];
  return Utils_update(x_form,
      1,
      ((u32[]){
          FIELD_avatar,
      }),
      ((void* []){
          x_avatar,
      }));
}
void* eval_author_project_Page_Settings_update_lambda6(void* args[]) {
  void* x_bio = args[0];
  void* x_form = args[1];
  return Utils_update(x_form,
      1,
      ((u32[]){
          FIELD_bio,
      }),
      ((void* []){
          x_bio,
      }));
}
void* eval_author_project_Page_Settings_update_lambda7(void* args[]) {
  void* x_password = args[0];
  void* x_form = args[1];
  return Utils_update(x_form,
      1,
      ((u32[]){
          FIELD_password,
      }),
      ((void* []){
          x_password,
      }));
}
void* eval_author_project_Page_Settings_update_lambda8(void* args[]) {
  void* x_username = args[0];
  void* x_form = args[1];
  return Utils_update(x_form,
      1,
      ((u32[]){
          FIELD_username,
      }),
      ((void* []){
          x_username,
      }));
}
void* eval_author_project_Page_Settings_update_lambda9(void* args[]) {
  void* x_email = args[0];
  void* x_form = args[1];
  return Utils_update(x_form,
      1,
      ((u32[]){
          FIELD_email,
      }),
      ((void* []){
          x_email,
      }));
}
void* eval_author_project_Page_Settings_update(void* args[]) {
  void* x_msg = args[0];
  void* x_model = args[1];
  void* tmp0;
  void* tmp1 = ((Custom*)x_msg)->ctor;
  if (tmp1 == CTOR_CompletedFormLoad) {
    void* tmp12 = ((Custom*)Utils_destruct_index(x_msg, 0))->ctor;
    if (tmp12 == CTOR_Ok) {
      void* x_form = Utils_destruct_index(Utils_destruct_index(x_msg, 0), 0);
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_status,
                            }),
                            ((void* []){
                                A1(&author_project_Page_Settings_Loaded, x_form),
                            })),
          &elm_core_Platform_Cmd_none);
    } else {
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_status,
                            }),
                            ((void* []){
                                &author_project_Page_Settings_Failed,
                            })),
          &elm_core_Platform_Cmd_none);
    };
  } else if (tmp1 == CTOR_SubmittedForm) {
    {
      void* x_cred = Utils_destruct_index(x_msg, 0);
      void* x_form = Utils_destruct_index(x_msg, 1);
      void* x__v1 = A1(&author_project_Page_Settings_validate, x_form);
      void* tmp10;
      void* tmp11 = ((Custom*)x__v1)->ctor;
      if (tmp11 == CTOR_Ok) {
        void* x_validForm = Utils_destruct_index(x__v1, 0);
        tmp10 = NEW_TUPLE2(Utils_update(x_model,
                               1,
                               ((u32[]){
                                   FIELD_status,
                               }),
                               ((void* []){
                                   A1(&author_project_Page_Settings_Loaded, x_form),
                               })),
            A2(&elm_core_Basics_apR,
                A2(&author_project_Page_Settings_edit, x_cred, x_validForm),
                A1(&elm_http_Http_send, &author_project_Page_Settings_CompletedSave)));
      } else {
        void* x_problems = Utils_destruct_index(x__v1, 0);
        tmp10 = NEW_TUPLE2(Utils_update(x_model,
                               1,
                               ((u32[]){
                                   FIELD_problems,
                               }),
                               ((void* []){
                                   x_problems,
                               })),
            &elm_core_Platform_Cmd_none);
      };
      tmp0 = tmp10;
    };
  } else if (tmp1 == CTOR_EnteredEmail) {
    {
      void* x_email = Utils_destruct_index(x_msg, 0);
      tmp0 = A2(&author_project_Page_Settings_updateForm,
          NEW_CLOSURE(1,
              2,
              &eval_author_project_Page_Settings_update_lambda9,
              ((void* []){
                  x_email,
              })),
          x_model);
    };
  } else if (tmp1 == CTOR_EnteredUsername) {
    {
      void* x_username = Utils_destruct_index(x_msg, 0);
      tmp0 = A2(&author_project_Page_Settings_updateForm,
          NEW_CLOSURE(1,
              2,
              &eval_author_project_Page_Settings_update_lambda8,
              ((void* []){
                  x_username,
              })),
          x_model);
    };
  } else if (tmp1 == CTOR_EnteredPassword) {
    {
      void* x_password = Utils_destruct_index(x_msg, 0);
      tmp0 = A2(&author_project_Page_Settings_updateForm,
          NEW_CLOSURE(1,
              2,
              &eval_author_project_Page_Settings_update_lambda7,
              ((void* []){
                  x_password,
              })),
          x_model);
    };
  } else if (tmp1 == CTOR_EnteredBio) {
    {
      void* x_bio = Utils_destruct_index(x_msg, 0);
      tmp0 = A2(&author_project_Page_Settings_updateForm,
          NEW_CLOSURE(1,
              2,
              &eval_author_project_Page_Settings_update_lambda6,
              ((void* []){
                  x_bio,
              })),
          x_model);
    };
  } else if (tmp1 == CTOR_EnteredAvatar) {
    {
      void* x_avatar = Utils_destruct_index(x_msg, 0);
      tmp0 = A2(&author_project_Page_Settings_updateForm,
          NEW_CLOSURE(1,
              2,
              &eval_author_project_Page_Settings_update_lambda5,
              ((void* []){
                  x_avatar,
              })),
          x_model);
    };
  } else if (tmp1 == CTOR_CompletedSave) {
    void* tmp4 = ((Custom*)Utils_destruct_index(x_msg, 0))->ctor;
    if (tmp4 == CTOR_Err) {
      void* x_error = Utils_destruct_index(Utils_destruct_index(x_msg, 0), 0);
      void* x_serverErrors = A2(&elm_core_Basics_apR,
          A1(&author_project_Api_decodeErrors, x_error),
          A1(&elm_core_List_map, &author_project_Page_Settings_ServerError));
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_problems,
                            }),
                            ((void* []){
                                A2(&elm_core_List_append,
                                    Utils_access_eval(((void* []){
                                        (void*)FIELD_problems,
                                        x_model,
                                    })),
                                    x_serverErrors),
                            })),
          &elm_core_Platform_Cmd_none);
    } else {
      void* x_viewer = Utils_destruct_index(Utils_destruct_index(x_msg, 0), 0);
      tmp0 = NEW_TUPLE2(x_model, A1(&author_project_Viewer_store, x_viewer));
    };
  } else if (tmp1 == CTOR_GotSession) {
    {
      void* x_session = Utils_destruct_index(x_msg, 0);
      tmp0 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_session,
                            }),
                            ((void* []){
                                x_session,
                            })),
          A2(&author_project_Route_replaceUrl,
              A1(&author_project_Session_navKey, x_session),
              &author_project_Route_Home));
    };
  } else {
    void* x__v2 = Utils_access_eval(((void* []){
        (void*)FIELD_status,
        x_model,
    }));
    void* tmp2;
    void* tmp3 = ((Custom*)x__v2)->ctor;
    if (tmp3 == CTOR_Loading) {
      tmp2 = NEW_TUPLE2(Utils_update(x_model,
                            1,
                            ((u32[]){
                                FIELD_status,
                            }),
                            ((void* []){
                                &author_project_Page_Settings_LoadingSlowly,
                            })),
          &elm_core_Platform_Cmd_none);
    } else {
      tmp2 = NEW_TUPLE2(x_model, &elm_core_Platform_Cmd_none);
    };
    tmp0 = tmp2;
  };
  return tmp0;
}
Closure author_project_Page_Settings_update = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Settings_update,
};
void* eval_author_project_Main_update(void* args[]) {
  void* x_msg = args[0];
  void* x_model = args[1];
  void* x__v0 = NEW_TUPLE2(x_msg, x_model);
  void* tmp0;
  void* tmp1 = ((Custom*)Utils_destruct_index(x__v0, 0))->ctor;
  void* tmp2 = ((Custom*)Utils_destruct_index(x__v0, 1))->ctor;
  if (tmp1 == CTOR_ClickedLink) {
    {
      void* x_urlRequest = Utils_destruct_index(Utils_destruct_index(x__v0, 0), 0);
      void* tmp10;
      void* tmp11 = ((Custom*)x_urlRequest)->ctor;
      if (tmp11 == CTOR_Internal) {
        void* x_url = Utils_destruct_index(x_urlRequest, 0);
        void* x__v2 = Utils_access_eval(((void* []){
            (void*)FIELD_fragment,
            x_url,
        }));
        void* tmp12;
        void* tmp13 = ((Custom*)x__v2)->ctor;
        if (tmp13 == CTOR_Nothing) {
          tmp12 = NEW_TUPLE2(x_model, &elm_core_Platform_Cmd_none);
        } else {
          tmp12 = NEW_TUPLE2(x_model,
              A2(&elm_browser_Browser_Navigation_pushUrl,
                  A1(&author_project_Session_navKey,
                      A1(&author_project_Main_toSession, x_model)),
                  A1(&elm_url_Url_toString, x_url)));
        };
        tmp10 = tmp12;
      } else {
        void* x_href = Utils_destruct_index(x_urlRequest, 0);
        tmp10 = NEW_TUPLE2(x_model, A1(&elm_browser_Browser_Navigation_load, x_href));
      };
      tmp0 = tmp10;
    };
  } else if (tmp1 == CTOR_ChangedUrl) {
    {
      void* x_url = Utils_destruct_index(Utils_destruct_index(x__v0, 0), 0);
      tmp0 = A2(&author_project_Main_changeRouteTo,
          A1(&author_project_Route_fromUrl, x_url),
          x_model);
    };
  } else if (tmp1 == CTOR_GotSettingsMsg) {
    void* tmp9 = ((Custom*)Utils_destruct_index(x__v0, 1))->ctor;
    if (tmp9 == CTOR_Settings) {
      void* x_subMsg = Utils_destruct_index(Utils_destruct_index(x__v0, 0), 0);
      void* x_settings = Utils_destruct_index(Utils_destruct_index(x__v0, 1), 0);
      tmp0 = A2(&elm_core_Basics_apR,
          A2(&author_project_Page_Settings_update, x_subMsg, x_settings),
          A3(&author_project_Main_updateWith,
              &author_project_Main_Settings,
              &author_project_Main_GotSettingsMsg,
              x_model));
    } else {
      goto _v0_10;
    };
  } else if (tmp1 == CTOR_GotLoginMsg) {
    void* tmp8 = ((Custom*)Utils_destruct_index(x__v0, 1))->ctor;
    if (tmp8 == CTOR_Login) {
      void* x_subMsg = Utils_destruct_index(Utils_destruct_index(x__v0, 0), 0);
      void* x_login = Utils_destruct_index(Utils_destruct_index(x__v0, 1), 0);
      tmp0 = A2(&elm_core_Basics_apR,
          A2(&author_project_Page_Login_update, x_subMsg, x_login),
          A3(&author_project_Main_updateWith,
              &author_project_Main_Login,
              &author_project_Main_GotLoginMsg,
              x_model));
    } else {
      goto _v0_10;
    };
  } else if (tmp1 == CTOR_GotRegisterMsg) {
    void* tmp7 = ((Custom*)Utils_destruct_index(x__v0, 1))->ctor;
    if (tmp7 == CTOR_Register) {
      void* x_subMsg = Utils_destruct_index(Utils_destruct_index(x__v0, 0), 0);
      void* x_register = Utils_destruct_index(Utils_destruct_index(x__v0, 1), 0);
      tmp0 = A2(&elm_core_Basics_apR,
          A2(&author_project_Page_Register_update, x_subMsg, x_register),
          A3(&author_project_Main_updateWith,
              &author_project_Main_Register,
              &author_project_Main_GotRegisterMsg,
              x_model));
    } else {
      goto _v0_10;
    };
  } else if (tmp1 == CTOR_GotHomeMsg) {
    void* tmp6 = ((Custom*)Utils_destruct_index(x__v0, 1))->ctor;
    if (tmp6 == CTOR_Home) {
      void* x_subMsg = Utils_destruct_index(Utils_destruct_index(x__v0, 0), 0);
      void* x_home = Utils_destruct_index(Utils_destruct_index(x__v0, 1), 0);
      tmp0 = A2(&elm_core_Basics_apR,
          A2(&author_project_Page_Home_update, x_subMsg, x_home),
          A3(&author_project_Main_updateWith,
              &author_project_Main_Home,
              &author_project_Main_GotHomeMsg,
              x_model));
    } else {
      goto _v0_10;
    };
  } else if (tmp1 == CTOR_GotProfileMsg) {
    void* tmp5 = ((Custom*)Utils_destruct_index(x__v0, 1))->ctor;
    if (tmp5 == CTOR_Profile) {
      void* x_subMsg = Utils_destruct_index(Utils_destruct_index(x__v0, 0), 0);
      void* x__v3 = Utils_destruct_index(x__v0, 1);
      void* x_username = Utils_destruct_index(x__v3, 0);
      void* x_profile = Utils_destruct_index(x__v3, 1);
      tmp0 = A2(&elm_core_Basics_apR,
          A2(&author_project_Page_Profile_update, x_subMsg, x_profile),
          A3(&author_project_Main_updateWith,
              A1(&author_project_Main_Profile, x_username),
              &author_project_Main_GotProfileMsg,
              x_model));
    } else {
      goto _v0_10;
    };
  } else if (tmp1 == CTOR_GotArticleMsg) {
    void* tmp4 = ((Custom*)Utils_destruct_index(x__v0, 1))->ctor;
    if (tmp4 == CTOR_Article) {
      void* x_subMsg = Utils_destruct_index(Utils_destruct_index(x__v0, 0), 0);
      void* x_article = Utils_destruct_index(Utils_destruct_index(x__v0, 1), 0);
      tmp0 = A2(&elm_core_Basics_apR,
          A2(&author_project_Page_Article_update, x_subMsg, x_article),
          A3(&author_project_Main_updateWith,
              &author_project_Main_Article,
              &author_project_Main_GotArticleMsg,
              x_model));
    } else {
      goto _v0_10;
    };
  } else if (tmp1 == CTOR_GotEditorMsg) {
    void* tmp3 = ((Custom*)Utils_destruct_index(x__v0, 1))->ctor;
    if (tmp3 == CTOR_Editor) {
      void* x_subMsg = Utils_destruct_index(Utils_destruct_index(x__v0, 0), 0);
      void* x__v4 = Utils_destruct_index(x__v0, 1);
      void* x_slug = Utils_destruct_index(x__v4, 0);
      void* x_editor = Utils_destruct_index(x__v4, 1);
      tmp0 = A2(&elm_core_Basics_apR,
          A2(&author_project_Page_Article_Editor_update, x_subMsg, x_editor),
          A3(&author_project_Main_updateWith,
              A1(&author_project_Main_Editor, x_slug),
              &author_project_Main_GotEditorMsg,
              x_model));
    } else {
      goto _v0_10;
    };
  } else if (tmp2 == CTOR_Redirect) {
    void* x_session = Utils_destruct_index(Utils_destruct_index(x__v0, 0), 0);
    tmp0 = NEW_TUPLE2(A1(&author_project_Main_Redirect, x_session),
        A2(&author_project_Route_replaceUrl,
            A1(&author_project_Session_navKey, x_session),
            &author_project_Route_Home));
  } else {
    goto _v0_10;
  };
_v0_10:
  tmp0 = NEW_TUPLE2(x_model, &elm_core_Platform_Cmd_none);
  return tmp0;
}
Closure author_project_Main_update = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Main_update,
};

Custom author_project_Page_Home = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Home,
};

Custom author_project_Page_NewArticle = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_NewArticle,
};

Custom author_project_Page_Other = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Other,
};

void* eval_author_project_Page_Profile(void* args[]) {
  return ctorCustom(CTOR_Profile, 1, args);
}
Closure author_project_Page_Profile = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Profile,
};

#define elm_virtual_dom_VirtualDom_map VirtualDom_map
#define elm_html_Html_map elm_virtual_dom_VirtualDom_map

#define elm_html_Html_a (*ptr_elm_html_Html_a)
ElmValue* ptr_elm_html_Html_a;
void* init_elm_html_Html_a() {
  return A1(&VirtualDom_node, &literal_string_a);
}

void* eval_elm_html_Html_Attributes_stringProperty(void* args[]) {
  void* x_key = args[0];
  void* x_string = args[1];
  return A2(&VirtualDom_property, x_key, A1(&elm_json_Json_Encode_string, x_string));
}
Closure elm_html_Html_Attributes_stringProperty = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_html_Html_Attributes_stringProperty,
};
#define elm_html_Html_Attributes_class (*ptr_elm_html_Html_Attributes_class)
ElmValue* ptr_elm_html_Html_Attributes_class;
void* init_elm_html_Html_Attributes_class() {
  return A1(&elm_html_Html_Attributes_stringProperty, &literal_string_className);
}

#define elm_html_Html_div (*ptr_elm_html_Html_div)
ElmValue* ptr_elm_html_Html_div;
void* init_elm_html_Html_div() {
  return A1(&VirtualDom_node, &literal_string_div);
}

#define elm_html_Html_footer (*ptr_elm_html_Html_footer)
ElmValue* ptr_elm_html_Html_footer;
void* init_elm_html_Html_footer() {
  return A1(&VirtualDom_node, &literal_string_footer);
}

void* eval_elm_html_Html_Attributes_href(void* args[]) {
  void* x_url = args[0];
  return A2(&elm_html_Html_Attributes_stringProperty,
      &literal_string_href,
      A1(&VirtualDom_noJavaScriptUri, x_url));
}
Closure elm_html_Html_Attributes_href = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_html_Html_Attributes_href,
};

#define elm_html_Html_span (*ptr_elm_html_Html_span)
ElmValue* ptr_elm_html_Html_span;
void* init_elm_html_Html_span() {
  return A1(&VirtualDom_node, &literal_string_span);
}

#define elm_virtual_dom_VirtualDom_text VirtualDom_text
#define elm_html_Html_text elm_virtual_dom_VirtualDom_text
#define author_project_Page_viewFooter (*ptr_author_project_Page_viewFooter)
ElmValue* ptr_author_project_Page_viewFooter;
void* init_author_project_Page_viewFooter() {
  return A2(&elm_html_Html_footer,
      &Nil,
      List_fromArray(1,
          ((void* []){
              A2(&elm_html_Html_div,
                  List_fromArray(1,
                      ((void* []){
                          A1(&elm_html_Html_Attributes_class, &literal_string_container),
                      })),
                  List_fromArray(2,
                      ((void* []){
                          A2(&elm_html_Html_a,
                              List_fromArray(2,
                                  ((void* []){
                                      A1(&elm_html_Html_Attributes_class,
                                          &literal_string_logo_2dfont),
                                      A1(&elm_html_Html_Attributes_href,
                                          &literal_string__2f),
                                  })),
                              List_fromArray(1,
                                  ((void* []){
                                      A1(&elm_html_Html_text, &literal_string_conduit),
                                  }))),
                          A2(&elm_html_Html_span,
                              List_fromArray(1,
                                  ((void* []){
                                      A1(&elm_html_Html_Attributes_class,
                                          &literal_string_attribution),
                                  })),
                              List_fromArray(3,
                                  ((void* []){
                                      A1(&elm_html_Html_text,
                                          &literal_string_An_20interactive_20learning_20project_20from_20),
                                      A2(&elm_html_Html_a,
                                          List_fromArray(1,
                                              ((void* []){
                                                  A1(&elm_html_Html_Attributes_href,
                                                      &literal_string_https_3a_2f_2fthinkster_2eio),
                                              })),
                                          List_fromArray(1,
                                              ((void* []){
                                                  A1(&elm_html_Html_text,
                                                      &literal_string_Thinkster),
                                              }))),
                                      A1(&elm_html_Html_text,
                                          &literal_string__2e_20Code_20_26_20design_20licensed_20under_20MIT_2e),
                                  }))),
                      }))),
          })));
}

void* eval_author_project_Route_href(void* args[]) {
  void* x_targetRoute = args[0];
  return A1(&elm_html_Html_Attributes_href,
      A1(&author_project_Route_routeToString, x_targetRoute));
}
Closure author_project_Route_href = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Route_href,
};

#define elm_html_Html_nav (*ptr_elm_html_Html_nav)
ElmValue* ptr_elm_html_Html_nav;
void* init_elm_html_Html_nav() {
  return A1(&VirtualDom_node, &literal_string_nav);
}

Custom elm_core_Basics_True = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_True,
};

void* eval_elm_core_Tuple_first(void* args[]) {
  void* x__v0 = args[0];
  void* x_x = Utils_destruct_index(x__v0, 0);
  return x_x;
}
Closure elm_core_Tuple_first = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_core_Tuple_first,
};

void* eval_elm_core_Tuple_second(void* args[]) {
  void* x__v0 = args[0];
  void* x_y = Utils_destruct_index(x__v0, 1);
  return x_y;
}
Closure elm_core_Tuple_second = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_core_Tuple_second,
};
void* eval_elm_html_Html_Attributes_classList(void* args[]) {
  void* x_classes = args[0];
  return A2(&elm_core_Basics_apL,
      &elm_html_Html_Attributes_class,
      A2(&elm_core_Basics_apL,
          A1(&elm_core_String_join, &literal_string__20),
          A2(&elm_core_Basics_apL,
              A1(&elm_core_List_map, &elm_core_Tuple_first),
              A2(&elm_core_List_filter, &elm_core_Tuple_second, x_classes))));
}
Closure elm_html_Html_Attributes_classList = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_html_Html_Attributes_classList,
};

void* eval_author_project_Page_isActive(void* args[]) {
  void* x_page = args[0];
  void* x_route = args[1];
  void* x__v0 = NEW_TUPLE2(x_page, x_route);
  void* tmp0;
  void* tmp1 = ((Custom*)Utils_destruct_index(x__v0, 0))->ctor;
  if (tmp1 == CTOR_Home) {
    void* tmp7 = ((Custom*)Utils_destruct_index(x__v0, 1))->ctor;
    if (tmp7 == CTOR_Home) {
      void* x__v1 = Utils_destruct_index(x__v0, 0);
      void* x__v2 = Utils_destruct_index(x__v0, 1);
      tmp0 = &True;
    } else {
      goto _v0_6;
    };
  } else if (tmp1 == CTOR_Login) {
    void* tmp6 = ((Custom*)Utils_destruct_index(x__v0, 1))->ctor;
    if (tmp6 == CTOR_Login) {
      void* x__v3 = Utils_destruct_index(x__v0, 0);
      void* x__v4 = Utils_destruct_index(x__v0, 1);
      tmp0 = &True;
    } else {
      goto _v0_6;
    };
  } else if (tmp1 == CTOR_Register) {
    void* tmp5 = ((Custom*)Utils_destruct_index(x__v0, 1))->ctor;
    if (tmp5 == CTOR_Register) {
      void* x__v5 = Utils_destruct_index(x__v0, 0);
      void* x__v6 = Utils_destruct_index(x__v0, 1);
      tmp0 = &True;
    } else {
      goto _v0_6;
    };
  } else if (tmp1 == CTOR_Settings) {
    void* tmp4 = ((Custom*)Utils_destruct_index(x__v0, 1))->ctor;
    if (tmp4 == CTOR_Settings) {
      void* x__v7 = Utils_destruct_index(x__v0, 0);
      void* x__v8 = Utils_destruct_index(x__v0, 1);
      tmp0 = &True;
    } else {
      goto _v0_6;
    };
  } else if (tmp1 == CTOR_Profile) {
    void* tmp3 = ((Custom*)Utils_destruct_index(x__v0, 1))->ctor;
    if (tmp3 == CTOR_Profile) {
      void* x_pageUsername = Utils_destruct_index(Utils_destruct_index(x__v0, 0), 0);
      void* x_routeUsername = Utils_destruct_index(Utils_destruct_index(x__v0, 1), 0);
      tmp0 = A2(&elm_core_Basics_eq, x_pageUsername, x_routeUsername);
    } else {
      goto _v0_6;
    };
  } else if (tmp1 == CTOR_NewArticle) {
    void* tmp2 = ((Custom*)Utils_destruct_index(x__v0, 1))->ctor;
    if (tmp2 == CTOR_NewArticle) {
      void* x__v9 = Utils_destruct_index(x__v0, 0);
      void* x__v10 = Utils_destruct_index(x__v0, 1);
      tmp0 = &True;
    } else {
      goto _v0_6;
    };
  } else {
    goto _v0_6;
  };
_v0_6:
  tmp0 = &False;
  return tmp0;
}
Closure author_project_Page_isActive = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_isActive,
};

#define elm_html_Html_li (*ptr_elm_html_Html_li)
ElmValue* ptr_elm_html_Html_li;
void* init_elm_html_Html_li() {
  return A1(&VirtualDom_node, &literal_string_li);
}
void* eval_author_project_Page_navbarLink(void* args[]) {
  void* x_page = args[0];
  void* x_route = args[1];
  void* x_linkContent = args[2];
  return A2(&elm_html_Html_li,
      List_fromArray(1,
          ((void* []){
              A1(&elm_html_Html_Attributes_classList,
                  List_fromArray(2,
                      ((void* []){
                          NEW_TUPLE2(&literal_string_nav_2ditem, &True),
                          NEW_TUPLE2(&literal_string_active,
                              A2(&author_project_Page_isActive, x_page, x_route)),
                      }))),
          })),
      List_fromArray(1,
          ((void* []){
              A2(&elm_html_Html_a,
                  List_fromArray(2,
                      ((void* []){
                          A1(&elm_html_Html_Attributes_class, &literal_string_nav_2dlink),
                          A1(&author_project_Route_href, x_route),
                      })),
                  x_linkContent),
          })));
}
Closure author_project_Page_navbarLink = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_author_project_Page_navbarLink,
};

#define elm_html_Html_ul (*ptr_elm_html_Html_ul)
ElmValue* ptr_elm_html_Html_ul;
void* init_elm_html_Html_ul() {
  return A1(&VirtualDom_node, &literal_string_ul);
}

void* eval_author_project_Viewer_avatar(void* args[]) {
  void* x__v0 = args[0];
  void* x_val = Utils_destruct_index(x__v0, 0);
  return x_val;
}
Closure author_project_Viewer_avatar = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Viewer_avatar,
};

#define elm_html_Html_i (*ptr_elm_html_Html_i)
ElmValue* ptr_elm_html_Html_i;
void* init_elm_html_Html_i() {
  return A1(&VirtualDom_node, &literal_string_i);
}

#define elm_html_Html_img (*ptr_elm_html_Html_img)
ElmValue* ptr_elm_html_Html_img;
void* init_elm_html_Html_img() {
  return A1(&VirtualDom_node, &literal_string_img);
}

void* eval_author_project_Asset_Image(void* args[]) {
  return ctorCustom(CTOR_Image, 1, args);
}
Closure author_project_Asset_Image = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Asset_Image,
};
void* eval_author_project_Asset_image(void* args[]) {
  void* x_filename = args[0];
  return A1(&author_project_Asset_Image,
      A2(&elm_core_Basics_append, &literal_string__2fassets_2fimages_2f, x_filename));
}
Closure author_project_Asset_image = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Asset_image,
};
#define author_project_Asset_defaultAvatar (*ptr_author_project_Asset_defaultAvatar)
ElmValue* ptr_author_project_Asset_defaultAvatar;
void* init_author_project_Asset_defaultAvatar() {
  return A1(&author_project_Asset_image, &literal_string_smiley_2dcyrus_2ejpg);
}

void* eval_elm_html_Html_Attributes_src(void* args[]) {
  void* x_url = args[0];
  return A2(&elm_html_Html_Attributes_stringProperty,
      &literal_string_src,
      A1(&VirtualDom_noJavaScriptOrHtmlUri, x_url));
}
Closure elm_html_Html_Attributes_src = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_html_Html_Attributes_src,
};
void* eval_author_project_Asset_src(void* args[]) {
  void* x__v0 = args[0];
  void* x_url = ((Custom*)x__v0)->values[0];
  return A1(&elm_html_Html_Attributes_src, x_url);
}
Closure author_project_Asset_src = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Asset_src,
};
void* eval_author_project_Avatar_src(void* args[]) {
  void* x__v0 = args[0];
  void* x_maybeUrl = ((Custom*)x__v0)->values[0];
  void* tmp0;
  void* tmp1 = ((Custom*)x_maybeUrl)->ctor;
  if (tmp1 == CTOR_Nothing) {
    tmp0 = A1(&author_project_Asset_src, &author_project_Asset_defaultAvatar);
  } else if (A2(Utils_equal, Utils_destruct_index(x_maybeUrl, 0), &literal_string_) ==
             &True) {
    tmp0 = A1(&author_project_Asset_src, &author_project_Asset_defaultAvatar);
  } else {
    void* x_url = Utils_destruct_index(x_maybeUrl, 0);
    tmp0 = A1(&elm_html_Html_Attributes_src, x_url);
  };
  return tmp0;
}
Closure author_project_Avatar_src = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Avatar_src,
};

void* eval_author_project_Username_toHtml(void* args[]) {
  void* x__v0 = args[0];
  void* x_username = ((Custom*)x__v0)->values[0];
  return A1(&elm_html_Html_text, x_username);
}
Closure author_project_Username_toHtml = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Username_toHtml,
};

void* eval_author_project_Viewer_username(void* args[]) {
  void* x__v0 = args[0];
  void* x_val = Utils_destruct_index(x__v0, 1);
  return A1(&author_project_Api_username, x_val);
}
Closure author_project_Viewer_username = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Viewer_username,
};
void* eval_author_project_Page_viewMenu(void* args[]) {
  void* x_page = args[0];
  void* x_maybeViewer = args[1];
  void* x_linkTo = A1(&author_project_Page_navbarLink, x_page);
  void* tmp0;
  void* tmp1 = ((Custom*)x_maybeViewer)->ctor;
  if (tmp1 == CTOR_Just) {
    void* x_viewer = Utils_destruct_index(x_maybeViewer, 0);
    void* x_username = A1(&author_project_Viewer_username, x_viewer);
    void* x_avatar = A1(&author_project_Viewer_avatar, x_viewer);
    tmp0 = List_fromArray(4,
        ((void* []){
            A2(x_linkTo,
                &author_project_Route_NewArticle,
                List_fromArray(2,
                    ((void* []){
                        A2(&elm_html_Html_i,
                            List_fromArray(1,
                                ((void* []){
                                    A1(&elm_html_Html_Attributes_class,
                                        &literal_string_ion_2dcompose),
                                })),
                            &Nil),
                        A1(&elm_html_Html_text, &literal_string__5cu00A0New_20Post),
                    }))),
            A2(x_linkTo,
                &author_project_Route_Settings,
                List_fromArray(2,
                    ((void* []){
                        A2(&elm_html_Html_i,
                            List_fromArray(1,
                                ((void* []){
                                    A1(&elm_html_Html_Attributes_class,
                                        &literal_string_ion_2dgear_2da),
                                })),
                            &Nil),
                        A1(&elm_html_Html_text, &literal_string__5cu00A0Settings),
                    }))),
            A2(x_linkTo,
                A1(&author_project_Route_Profile, x_username),
                List_fromArray(2,
                    ((void* []){
                        A2(&elm_html_Html_img,
                            List_fromArray(2,
                                ((void* []){
                                    A1(&elm_html_Html_Attributes_class,
                                        &literal_string_user_2dpic),
                                    A1(&author_project_Avatar_src, x_avatar),
                                })),
                            &Nil),
                        A1(&author_project_Username_toHtml, x_username),
                    }))),
            A2(x_linkTo,
                &author_project_Route_Logout,
                List_fromArray(1,
                    ((void* []){
                        A1(&elm_html_Html_text, &literal_string_Sign_20out),
                    }))),
        }));
  } else {
    tmp0 = List_fromArray(2,
        ((void* []){
            A2(x_linkTo,
                &author_project_Route_Login,
                List_fromArray(1,
                    ((void* []){
                        A1(&elm_html_Html_text, &literal_string_Sign_20in),
                    }))),
            A2(x_linkTo,
                &author_project_Route_Register,
                List_fromArray(1,
                    ((void* []){
                        A1(&elm_html_Html_text, &literal_string_Sign_20up),
                    }))),
        }));
  };
  return tmp0;
}
Closure author_project_Page_viewMenu = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_viewMenu,
};
void* eval_author_project_Page_viewHeader(void* args[]) {
  void* x_page = args[0];
  void* x_maybeViewer = args[1];
  return A2(&elm_html_Html_nav,
      List_fromArray(1,
          ((void* []){
              A1(&elm_html_Html_Attributes_class,
                  &literal_string_navbar_20navbar_2dlight),
          })),
      List_fromArray(1,
          ((void* []){
              A2(&elm_html_Html_div,
                  List_fromArray(1,
                      ((void* []){
                          A1(&elm_html_Html_Attributes_class, &literal_string_container),
                      })),
                  List_fromArray(2,
                      ((void* []){
                          A2(&elm_html_Html_a,
                              List_fromArray(2,
                                  ((void* []){
                                      A1(&elm_html_Html_Attributes_class,
                                          &literal_string_navbar_2dbrand),
                                      A1(&author_project_Route_href,
                                          &author_project_Route_Home),
                                  })),
                              List_fromArray(1,
                                  ((void* []){
                                      A1(&elm_html_Html_text, &literal_string_conduit),
                                  }))),
                          A2(&elm_core_Basics_apL,
                              A1(&elm_html_Html_ul,
                                  List_fromArray(1,
                                      ((void* []){
                                          A1(&elm_html_Html_Attributes_class,
                                              &literal_string_nav_20navbar_2dnav_20pull_2dxs_2dright),
                                      }))),
                              A2(&elm_core_List_cons,
                                  A3(&author_project_Page_navbarLink,
                                      x_page,
                                      &author_project_Route_Home,
                                      List_fromArray(1,
                                          ((void* []){
                                              A1(&elm_html_Html_text,
                                                  &literal_string_Home),
                                          }))),
                                  A2(&author_project_Page_viewMenu,
                                      x_page,
                                      x_maybeViewer))),
                      }))),
          })));
}
Closure author_project_Page_viewHeader = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_viewHeader,
};
void* eval_author_project_Page_view(void* args[]) {
  void* x_maybeViewer = args[0];
  void* x_page = args[1];
  void* x__v0 = args[2];
  void* x_title = Utils_access_eval(((void* []){
      (void*)FIELD_title,
      x__v0,
  }));
  void* x_content = Utils_access_eval(((void* []){
      (void*)FIELD_content,
      x__v0,
  }));
  return NEW_RECORD(&fg_body_title,
      2,
      ((void* []){
          A2(&elm_core_List_cons,
              A2(&author_project_Page_viewHeader, x_page, x_maybeViewer),
              A2(&elm_core_List_cons,
                  x_content,
                  List_fromArray(1,
                      ((void* []){
                          &author_project_Page_viewFooter,
                      })))),
          A2(&elm_core_Basics_append, x_title, &literal_string__20_2d_20Conduit),
      }));
}
Closure author_project_Page_view = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_author_project_Page_view,
};

Custom author_project_Page_Article_ClickedDismissErrors = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_ClickedDismissErrors,
};

void* eval_author_project_Article_author(void* args[]) {
  void* x__v0 = args[0];
  void* x_internals = Utils_destruct_index(x__v0, 0);
  return Utils_access_eval(((void* []){
      (void*)FIELD_author,
      x_internals,
  }));
}
Closure author_project_Article_author = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Article_author,
};

void* eval_author_project_Profile_avatar(void* args[]) {
  void* x__v0 = args[0];
  void* x_info = ((Custom*)x__v0)->values[0];
  return Utils_access_eval(((void* []){
      (void*)FIELD_avatar,
      x_info,
  }));
}
Closure author_project_Profile_avatar = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Profile_avatar,
};

void* eval_author_project_Loading_error(void* args[]) {
  void* x_str = args[0];
  return A1(&elm_html_Html_text,
      A2(&elm_core_Basics_append,
          &literal_string_Error_20loading_20,
          A2(&elm_core_Basics_append, x_str, &literal_string__2e)));
}
Closure author_project_Loading_error = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Loading_error,
};

#define elm_html_Html_h1 (*ptr_elm_html_Html_h1)
ElmValue* ptr_elm_html_Html_h1;
void* init_elm_html_Html_h1() {
  return A1(&VirtualDom_node, &literal_string_h1);
}

#define elm_html_Html_hr (*ptr_elm_html_Html_hr)
ElmValue* ptr_elm_html_Html_hr;
void* init_elm_html_Html_hr() {
  return A1(&VirtualDom_node, &literal_string_hr);
}

#define elm_html_Html_Attributes_alt (*ptr_elm_html_Html_Attributes_alt)
ElmValue* ptr_elm_html_Html_Attributes_alt;
void* init_elm_html_Html_Attributes_alt() {
  return A1(&elm_html_Html_Attributes_stringProperty, &literal_string_alt);
}

void* eval_elm_html_Html_Attributes_height(void* args[]) {
  void* x_n = args[0];
  return A2(
      &VirtualDom_attribute, &literal_string_height, A1(&elm_core_String_fromInt, x_n));
}
Closure elm_html_Html_Attributes_height = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_html_Html_Attributes_height,
};

#define author_project_Asset_loading (*ptr_author_project_Asset_loading)
ElmValue* ptr_author_project_Asset_loading;
void* init_author_project_Asset_loading() {
  return A1(&author_project_Asset_image, &literal_string_loading_2esvg);
}

void* eval_elm_html_Html_Attributes_width(void* args[]) {
  void* x_n = args[0];
  return A2(
      &VirtualDom_attribute, &literal_string_width, A1(&elm_core_String_fromInt, x_n));
}
Closure elm_html_Html_Attributes_width = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_html_Html_Attributes_width,
};
#define author_project_Loading_icon (*ptr_author_project_Loading_icon)
ElmValue* ptr_author_project_Loading_icon;
void* init_author_project_Loading_icon() {
  return A2(&elm_html_Html_img,
      List_fromArray(4,
          ((void* []){
              A1(&author_project_Asset_src, &author_project_Asset_loading),
              A1(&elm_html_Html_Attributes_width, &literal_int_64),
              A1(&elm_html_Html_Attributes_height, &literal_int_64),
              A1(&elm_html_Html_Attributes_alt, &literal_string_Loading_2e_2e_2e),
          })),
      &Nil);
}

void* eval_author_project_Author_profile(void* args[]) {
  void* x_author = args[0];
  void* tmp0;
  void* tmp1 = ((Custom*)x_author)->ctor;
  if (tmp1 == CTOR_IsViewer) {
    {
      void* x_val = Utils_destruct_index(x_author, 1);
      tmp0 = x_val;
    };
  } else if (tmp1 == CTOR_IsFollowing) {
    {
      void* x__v1 = Utils_destruct_index(x_author, 0);
      void* x_val = Utils_destruct_index(x__v1, 1);
      tmp0 = x_val;
    };
  } else {
    void* x__v2 = Utils_destruct_index(x_author, 0);
    void* x_val = Utils_destruct_index(x__v2, 1);
    tmp0 = x_val;
  };
  return tmp0;
}
Closure author_project_Author_profile = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Author_profile,
};

#define elm_explorations_markdown_Markdown_defaultOptions \
  (*ptr_elm_explorations_markdown_Markdown_defaultOptions)
Record* ptr_elm_explorations_markdown_Markdown_defaultOptions;
void* init_elm_explorations_markdown_Markdown_defaultOptions() {
  return NEW_RECORD(&fg_defaultHighlighting_githubFlavored_sanitize_smartypants,
      4,
      ((void* []){
          &elm_core_Maybe_Nothing,
          A1(&elm_core_Maybe_Just,
              NEW_RECORD(&fg_breaks_tables,
                  2,
                  ((void* []){
                      &False,
                      &False,
                  }))),
          &True,
          &False,
      }));
}

#define elm_explorations_markdown_Markdown_toHtmlWith Markdown_toHtml
#define elm_explorations_markdown_Markdown_toHtml \
  (*ptr_elm_explorations_markdown_Markdown_toHtml)
ElmValue* ptr_elm_explorations_markdown_Markdown_toHtml;
void* init_elm_explorations_markdown_Markdown_toHtml() {
  return A1(&elm_explorations_markdown_Markdown_toHtmlWith,
      &elm_explorations_markdown_Markdown_defaultOptions);
}
void* eval_author_project_Article_Body_toHtml(void* args[]) {
  void* x__v0 = args[0];
  void* x_attributes = args[1];
  void* x_markdown = ((Custom*)x__v0)->values[0];
  return A2(&elm_explorations_markdown_Markdown_toHtml, x_attributes, x_markdown);
}
Closure author_project_Article_Body_toHtml = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Article_Body_toHtml,
};

void* eval_author_project_Author_view(void* args[]) {
  void* x_uname = args[0];
  return A2(&elm_html_Html_a,
      List_fromArray(2,
          ((void* []){
              A1(&elm_html_Html_Attributes_class, &literal_string_author),
              A1(&author_project_Route_href, A1(&author_project_Route_Profile, x_uname)),
          })),
      List_fromArray(1,
          ((void* []){
              A1(&author_project_Username_toHtml, x_uname),
          })));
}
Closure author_project_Author_view = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Author_view,
};

#define elm_core_Basics_floor Basics_floor
void* eval_elm_time_Time_flooredDiv(void* args[]) {
  void* x_numerator = args[0];
  void* x_denominator = args[1];
  return A1(&elm_core_Basics_floor,
      A2(&elm_core_Basics_fdiv,
          A1(&elm_core_Basics_toFloat, x_numerator),
          x_denominator));
}
Closure elm_time_Time_flooredDiv = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_time_Time_flooredDiv,
};

void* eval_elm_time_Time_posixToMillis(void* args[]) {
  void* x__v0 = args[0];
  void* x_millis = ((Custom*)x__v0)->values[0];
  return x_millis;
}
Closure elm_time_Time_posixToMillis = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_time_Time_posixToMillis,
};

void* tce_elm_time_Time_toAdjustedMinutesHelp(void* args[], void** gc_tce_data) {
tce_loop:;
  void* x_defaultOffset = args[0];
  void* x_posixMinutes = args[1];
  void* x_eras = args[2];
  void* tmp0;
  if (x_eras == &Nil) {
    tmp0 = A2(&elm_core_Basics_add, x_posixMinutes, x_defaultOffset);
  } else {
    void* x_era = Utils_destruct_index(x_eras, 0);
    void* x_olderEras = Utils_destruct_index(x_eras, 1);
    void* tmp1;
    if (A2(&elm_core_Basics_lt,
            Utils_access_eval(((void* []){
                (void*)FIELD_start,
                x_era,
            })),
            x_posixMinutes) == &True) {
      tmp1 = A2(&elm_core_Basics_add,
          x_posixMinutes,
          Utils_access_eval(((void* []){
              (void*)FIELD_offset,
              x_era,
          })));
    } else {
      void* tmp2 = x_defaultOffset;
      void* tmp3 = x_posixMinutes;
      void* tmp4 = x_olderEras;
      *gc_tce_data = CAN_THROW(GC_tce_iteration(3));
      args[2] = tmp4;
      args[1] = tmp3;
      args[0] = tmp2;
      goto tce_loop;
      tmp1 = NULL;
    };
    tmp0 = tmp1;
  };
  return tmp0;
}
void* eval_elm_time_Time_toAdjustedMinutesHelp() {
  return GC_tce_eval(&tce_elm_time_Time_toAdjustedMinutesHelp,
      &eval_elm_time_Time_toAdjustedMinutesHelp,
      3,
      args);
}
Closure elm_time_Time_toAdjustedMinutesHelp = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_elm_time_Time_toAdjustedMinutesHelp,
};
void* eval_elm_time_Time_toAdjustedMinutes(void* args[]) {
  void* x__v0 = args[0];
  void* x_time = args[1];
  void* x_defaultOffset = Utils_destruct_index(x__v0, 0);
  void* x_eras = Utils_destruct_index(x__v0, 1);
  return A3(&elm_time_Time_toAdjustedMinutesHelp,
      x_defaultOffset,
      A2(&elm_time_Time_flooredDiv,
          A1(&elm_time_Time_posixToMillis, x_time),
          &literal_int_60000),
      x_eras);
}
Closure elm_time_Time_toAdjustedMinutes = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_time_Time_toAdjustedMinutes,
};

#define elm_core_Basics_ge Utils_ge
void* eval_elm_time_Time_toCivil(void* args[]) {
  void* x_minutes = args[0];
  void* x_rawDay = A2(&elm_core_Basics_add,
      A2(&elm_time_Time_flooredDiv,
          x_minutes,
          A2(&elm_core_Basics_mul, &literal_int_60, &literal_int_24)),
      &literal_int_719468);
  void* tmp0;
  if (A2(&elm_core_Basics_ge, x_rawDay, &literal_int_0) == &True) {
    tmp0 = x_rawDay;
  } else {
    tmp0 = A2(&elm_core_Basics_sub, x_rawDay, &literal_int_146096);
  };
  void* x_era = A2(&elm_core_Basics_idiv, tmp0, &literal_int_146097);
  void* x_dayOfEra = A2(&elm_core_Basics_sub,
      x_rawDay,
      A2(&elm_core_Basics_mul, x_era, &literal_int_146097));
  void* x_yearOfEra = A2(&elm_core_Basics_idiv,
      A2(&elm_core_Basics_sub,
          A2(&elm_core_Basics_add,
              A2(&elm_core_Basics_sub,
                  x_dayOfEra,
                  A2(&elm_core_Basics_idiv, x_dayOfEra, &literal_int_1460)),
              A2(&elm_core_Basics_idiv, x_dayOfEra, &literal_int_36524)),
          A2(&elm_core_Basics_idiv, x_dayOfEra, &literal_int_146096)),
      &literal_int_365);
  void* x_dayOfYear = A2(&elm_core_Basics_sub,
      x_dayOfEra,
      A2(&elm_core_Basics_sub,
          A2(&elm_core_Basics_add,
              A2(&elm_core_Basics_mul, &literal_int_365, x_yearOfEra),
              A2(&elm_core_Basics_idiv, x_yearOfEra, &literal_int_4)),
          A2(&elm_core_Basics_idiv, x_yearOfEra, &literal_int_100)));
  void* x_mp = A2(&elm_core_Basics_idiv,
      A2(&elm_core_Basics_add,
          A2(&elm_core_Basics_mul, &literal_int_5, x_dayOfYear),
          &literal_int_2),
      &literal_int_153);
  void* tmp1;
  if (A2(&elm_core_Basics_lt, x_mp, &literal_int_10) == &True) {
    tmp1 = &literal_int_3;
  } else {
    tmp1 = A1(&elm_core_Basics_negate, &literal_int_9);
  };
  void* x_month = A2(&elm_core_Basics_add, x_mp, tmp1);
  void* x_year = A2(&elm_core_Basics_add,
      x_yearOfEra,
      A2(&elm_core_Basics_mul, x_era, &literal_int_400));
  void* tmp2;
  if (A2(&elm_core_Basics_le, x_month, &literal_int_2) == &True) {
    tmp2 = &literal_int_1;
  } else {
    tmp2 = &literal_int_0;
  };
  return NEW_RECORD(&fg_day_month_year,
      3,
      ((void* []){
          A2(&elm_core_Basics_add,
              A2(&elm_core_Basics_sub,
                  x_dayOfYear,
                  A2(&elm_core_Basics_idiv,
                      A2(&elm_core_Basics_add,
                          A2(&elm_core_Basics_mul, &literal_int_153, x_mp),
                          &literal_int_2),
                      &literal_int_5)),
              &literal_int_1),
          x_month,
          A2(&elm_core_Basics_add, x_year, tmp2),
      }));
}
Closure elm_time_Time_toCivil = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_time_Time_toCivil,
};
void* eval_elm_time_Time_toDay(void* args[]) {
  void* x_zone = args[0];
  void* x_time = args[1];
  return Utils_access_eval(((void* []){
      (void*)FIELD_day,
      A1(&elm_time_Time_toCivil, A2(&elm_time_Time_toAdjustedMinutes, x_zone, x_time)),
  }));
}
Closure elm_time_Time_toDay = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_time_Time_toDay,
};

Custom elm_time_Time_Apr = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Apr,
};

Custom elm_time_Time_Aug = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Aug,
};

Custom elm_time_Time_Dec = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Dec,
};

Custom elm_time_Time_Feb = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Feb,
};

Custom elm_time_Time_Jan = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Jan,
};

Custom elm_time_Time_Jul = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Jul,
};

Custom elm_time_Time_Jun = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Jun,
};

Custom elm_time_Time_Mar = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Mar,
};

Custom elm_time_Time_May = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_May,
};

Custom elm_time_Time_Nov = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Nov,
};

Custom elm_time_Time_Oct = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Oct,
};

Custom elm_time_Time_Sep = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Sep,
};
void* eval_elm_time_Time_toMonth(void* args[]) {
  void* x_zone = args[0];
  void* x_time = args[1];
  void* x__v0 = Utils_access_eval(((void* []){
      (void*)FIELD_month,
      A1(&elm_time_Time_toCivil, A2(&elm_time_Time_toAdjustedMinutes, x_zone, x_time)),
  }));
  void* tmp0;
  void* tmp1 = ((ElmInt*)x__v0)->value;
  if (tmp1 == 1) {
    {
      tmp0 = &elm_time_Time_Jan;
    };
  } else if (tmp1 == 2) {
    { tmp0 = &elm_time_Time_Feb; };
  } else if (tmp1 == 3) {
    { tmp0 = &elm_time_Time_Mar; };
  } else if (tmp1 == 4) {
    { tmp0 = &elm_time_Time_Apr; };
  } else if (tmp1 == 5) {
    { tmp0 = &elm_time_Time_May; };
  } else if (tmp1 == 6) {
    { tmp0 = &elm_time_Time_Jun; };
  } else if (tmp1 == 7) {
    { tmp0 = &elm_time_Time_Jul; };
  } else if (tmp1 == 8) {
    { tmp0 = &elm_time_Time_Aug; };
  } else if (tmp1 == 9) {
    { tmp0 = &elm_time_Time_Sep; };
  } else if (tmp1 == 10) {
    { tmp0 = &elm_time_Time_Oct; };
  } else if (tmp1 == 11) {
    { tmp0 = &elm_time_Time_Nov; };
  } else {
    tmp0 = &elm_time_Time_Dec;
  };
  return tmp0;
}
Closure elm_time_Time_toMonth = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_time_Time_toMonth,
};

void* eval_elm_time_Time_toYear(void* args[]) {
  void* x_zone = args[0];
  void* x_time = args[1];
  return Utils_access_eval(((void* []){
      (void*)FIELD_year,
      A1(&elm_time_Time_toCivil, A2(&elm_time_Time_toAdjustedMinutes, x_zone, x_time)),
  }));
}
Closure elm_time_Time_toYear = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_time_Time_toYear,
};
void* eval_author_project_Timestamp_format(void* args[]) {
  void* x_zone = args[0];
  void* x_time = args[1];
  void* x_year = A1(&elm_core_String_fromInt, A2(&elm_time_Time_toYear, x_zone, x_time));
  void* x__v0 = A2(&elm_time_Time_toMonth, x_zone, x_time);
  void* tmp0;
  void* tmp1 = ((Custom*)x__v0)->ctor;
  if (tmp1 == CTOR_Jan) {
    {
      tmp0 = &literal_string_January;
    };
  } else if (tmp1 == CTOR_Feb) {
    { tmp0 = &literal_string_February; };
  } else if (tmp1 == CTOR_Mar) {
    { tmp0 = &literal_string_March; };
  } else if (tmp1 == CTOR_Apr) {
    { tmp0 = &literal_string_April; };
  } else if (tmp1 == CTOR_May) {
    { tmp0 = &literal_string_May; };
  } else if (tmp1 == CTOR_Jun) {
    { tmp0 = &literal_string_June; };
  } else if (tmp1 == CTOR_Jul) {
    { tmp0 = &literal_string_July; };
  } else if (tmp1 == CTOR_Aug) {
    { tmp0 = &literal_string_August; };
  } else if (tmp1 == CTOR_Sep) {
    { tmp0 = &literal_string_September; };
  } else if (tmp1 == CTOR_Oct) {
    { tmp0 = &literal_string_October; };
  } else if (tmp1 == CTOR_Nov) {
    { tmp0 = &literal_string_November; };
  } else {
    tmp0 = &literal_string_December;
  };
  void* x_month = tmp0;
  void* x_day = A1(&elm_core_String_fromInt, A2(&elm_time_Time_toDay, x_zone, x_time));
  return A2(&elm_core_Basics_append,
      x_month,
      A2(&elm_core_Basics_append,
          &literal_string__20,
          A2(&elm_core_Basics_append,
              x_day,
              A2(&elm_core_Basics_append, &literal_string__2c_20, x_year))));
}
Closure author_project_Timestamp_format = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Timestamp_format,
};
void* eval_author_project_Timestamp_view(void* args[]) {
  void* x_timeZone = args[0];
  void* x_timestamp = args[1];
  return A2(&elm_html_Html_span,
      List_fromArray(1,
          ((void* []){
              A1(&elm_html_Html_Attributes_class, &literal_string_date),
          })),
      List_fromArray(1,
          ((void* []){
              A1(&elm_html_Html_text,
                  A2(&author_project_Timestamp_format, x_timeZone, x_timestamp)),
          })));
}
Closure author_project_Timestamp_view = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Timestamp_view,
};

void* eval_author_project_Page_Article_ClickedPostComment(void* args[]) {
  return ctorCustom(CTOR_ClickedPostComment, 2, args);
}
Closure author_project_Page_Article_ClickedPostComment = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Article_ClickedPostComment,
};

void* eval_author_project_Page_Article_EnteredCommentText(void* args[]) {
  return ctorCustom(CTOR_EnteredCommentText, 1, args);
}
Closure author_project_Page_Article_EnteredCommentText = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_EnteredCommentText,
};

void* eval_elm_virtual_dom_VirtualDom_attribute(void* args[]) {
  void* x_key = args[0];
  void* x_value = args[1];
  return A2(&VirtualDom_attribute,
      A1(&VirtualDom_noOnOrFormAction, x_key),
      A1(&VirtualDom_noJavaScriptOrHtmlUri, x_value));
}
Closure elm_virtual_dom_VirtualDom_attribute = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_virtual_dom_VirtualDom_attribute,
};
#define elm_html_Html_Attributes_attribute elm_virtual_dom_VirtualDom_attribute

#define elm_html_Html_button (*ptr_elm_html_Html_button)
ElmValue* ptr_elm_html_Html_button;
void* init_elm_html_Html_button() {
  return A1(&VirtualDom_node, &literal_string_button);
}

#define elm_json_Json_Encode_bool Json_wrap
void* eval_elm_html_Html_Attributes_boolProperty(void* args[]) {
  void* x_key = args[0];
  void* x_bool = args[1];
  return A2(&VirtualDom_property, x_key, A1(&elm_json_Json_Encode_bool, x_bool));
}
Closure elm_html_Html_Attributes_boolProperty = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_html_Html_Attributes_boolProperty,
};
#define elm_html_Html_Attributes_disabled (*ptr_elm_html_Html_Attributes_disabled)
ElmValue* ptr_elm_html_Html_Attributes_disabled;
void* init_elm_html_Html_Attributes_disabled() {
  return A1(&elm_html_Html_Attributes_boolProperty, &literal_string_disabled);
}

#define elm_html_Html_form (*ptr_elm_html_Html_form)
ElmValue* ptr_elm_html_Html_form;
void* init_elm_html_Html_form() {
  return A1(&VirtualDom_node, &literal_string_form);
}

void* eval_elm_html_Html_Events_alwaysStop(void* args[]) {
  void* x_x = args[0];
  return NEW_TUPLE2(x_x, &True);
}
Closure elm_html_Html_Events_alwaysStop = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_html_Html_Events_alwaysStop,
};

void* eval_elm_virtual_dom_VirtualDom_MayStopPropagation(void* args[]) {
  return ctorCustom(CTOR_MayStopPropagation, 1, args);
}
Closure elm_virtual_dom_VirtualDom_MayStopPropagation = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_virtual_dom_VirtualDom_MayStopPropagation,
};

#define elm_virtual_dom_VirtualDom_on VirtualDom_on
void* eval_elm_html_Html_Events_stopPropagationOn(void* args[]) {
  void* x_event = args[0];
  void* x_decoder = args[1];
  return A2(&elm_virtual_dom_VirtualDom_on,
      x_event,
      A1(&elm_virtual_dom_VirtualDom_MayStopPropagation, x_decoder));
}
Closure elm_html_Html_Events_stopPropagationOn = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_html_Html_Events_stopPropagationOn,
};

void* eval_elm_json_Json_Decode_at(void* args[]) {
  void* x_fields = args[0];
  void* x_decoder = args[1];
  return A3(&elm_core_List_foldr, &elm_json_Json_Decode_field, x_decoder, x_fields);
}
Closure elm_json_Json_Decode_at = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_json_Json_Decode_at,
};
#define elm_html_Html_Events_targetValue (*ptr_elm_html_Html_Events_targetValue)
ElmValue* ptr_elm_html_Html_Events_targetValue;
void* init_elm_html_Html_Events_targetValue() {
  return A2(&elm_json_Json_Decode_at,
      List_fromArray(2,
          ((void* []){
              &literal_string_target,
              &literal_string_value,
          })),
      &elm_json_Json_Decode_string);
}
void* eval_elm_html_Html_Events_onInput(void* args[]) {
  void* x_tagger = args[0];
  return A2(&elm_html_Html_Events_stopPropagationOn,
      &literal_string_input,
      A2(&elm_json_Json_Decode_map,
          &elm_html_Html_Events_alwaysStop,
          A2(&elm_json_Json_Decode_map, x_tagger, &elm_html_Html_Events_targetValue)));
}
Closure elm_html_Html_Events_onInput = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_html_Html_Events_onInput,
};

void* eval_elm_html_Html_Events_alwaysPreventDefault(void* args[]) {
  void* x_msg = args[0];
  return NEW_TUPLE2(x_msg, &True);
}
Closure elm_html_Html_Events_alwaysPreventDefault = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_html_Html_Events_alwaysPreventDefault,
};

void* eval_elm_virtual_dom_VirtualDom_MayPreventDefault(void* args[]) {
  return ctorCustom(CTOR_MayPreventDefault, 1, args);
}
Closure elm_virtual_dom_VirtualDom_MayPreventDefault = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_virtual_dom_VirtualDom_MayPreventDefault,
};
void* eval_elm_html_Html_Events_preventDefaultOn(void* args[]) {
  void* x_event = args[0];
  void* x_decoder = args[1];
  return A2(&elm_virtual_dom_VirtualDom_on,
      x_event,
      A1(&elm_virtual_dom_VirtualDom_MayPreventDefault, x_decoder));
}
Closure elm_html_Html_Events_preventDefaultOn = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_html_Html_Events_preventDefaultOn,
};
void* eval_elm_html_Html_Events_onSubmit(void* args[]) {
  void* x_msg = args[0];
  return A2(&elm_html_Html_Events_preventDefaultOn,
      &literal_string_submit,
      A2(&elm_json_Json_Decode_map,
          &elm_html_Html_Events_alwaysPreventDefault,
          A1(&elm_json_Json_Decode_succeed, x_msg)));
}
Closure elm_html_Html_Events_onSubmit = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_html_Html_Events_onSubmit,
};

#define elm_html_Html_p (*ptr_elm_html_Html_p)
ElmValue* ptr_elm_html_Html_p;
void* init_elm_html_Html_p() {
  return A1(&VirtualDom_node, &literal_string_p);
}

#define elm_html_Html_Attributes_placeholder (*ptr_elm_html_Html_Attributes_placeholder)
ElmValue* ptr_elm_html_Html_Attributes_placeholder;
void* init_elm_html_Html_Attributes_placeholder() {
  return A1(&elm_html_Html_Attributes_stringProperty, &literal_string_placeholder);
}

#define elm_html_Html_textarea (*ptr_elm_html_Html_textarea)
ElmValue* ptr_elm_html_Html_textarea;
void* init_elm_html_Html_textarea() {
  return A1(&VirtualDom_node, &literal_string_textarea);
}

#define elm_html_Html_Attributes_value (*ptr_elm_html_Html_Attributes_value)
ElmValue* ptr_elm_html_Html_Attributes_value;
void* init_elm_html_Html_Attributes_value() {
  return A1(&elm_html_Html_Attributes_stringProperty, &literal_string_value);
}
void* eval_author_project_Page_Article_viewAddComment(void* args[]) {
  void* x_slug = args[0];
  void* x_commentText = args[1];
  void* x_maybeViewer = args[2];
  void* tmp0;
  void* tmp1 = ((Custom*)x_maybeViewer)->ctor;
  if (tmp1 == CTOR_Just) {
    void* x_viewer = Utils_destruct_index(x_maybeViewer, 0);
    void* x_cred = A1(&author_project_Viewer_cred, x_viewer);
    void* x_avatar = A1(&author_project_Viewer_avatar, x_viewer);
    void* tmp2;
    void* tmp3 = ((Custom*)x_commentText)->ctor;
    if (tmp3 == CTOR_Editing) {
      void* x_str = Utils_destruct_index(x_commentText, 0);
      tmp2 = NEW_TUPLE2(x_str, &Nil);
    } else {
      void* x_str = Utils_destruct_index(x_commentText, 0);
      tmp2 = NEW_TUPLE2(x_str,
          List_fromArray(1,
              ((void* []){
                  A1(&elm_html_Html_Attributes_disabled, &True),
              })));
    };
    void* x__v1 = tmp2;
    void* x_commentStr = Utils_destruct_index(x__v1, 0);
    void* x_buttonAttrs = Utils_destruct_index(x__v1, 1);
    tmp0 = A2(&elm_html_Html_form,
        List_fromArray(2,
            ((void* []){
                A1(&elm_html_Html_Attributes_class,
                    &literal_string_card_20comment_2dform),
                A1(&elm_html_Html_Events_onSubmit,
                    A2(&author_project_Page_Article_ClickedPostComment, x_cred, x_slug)),
            })),
        List_fromArray(2,
            ((void* []){
                A2(&elm_html_Html_div,
                    List_fromArray(1,
                        ((void* []){
                            A1(&elm_html_Html_Attributes_class,
                                &literal_string_card_2dblock),
                        })),
                    List_fromArray(1,
                        ((void* []){
                            A2(&elm_html_Html_textarea,
                                List_fromArray(5,
                                    ((void* []){
                                        A1(&elm_html_Html_Attributes_class,
                                            &literal_string_form_2dcontrol),
                                        A1(&elm_html_Html_Attributes_placeholder,
                                            &literal_string_Write_20a_20comment_2e_2e_2e),
                                        A2(&elm_html_Html_Attributes_attribute,
                                            &literal_string_rows,
                                            &literal_string_3),
                                        A1(&elm_html_Html_Events_onInput,
                                            &author_project_Page_Article_EnteredCommentText),
                                        A1(&elm_html_Html_Attributes_value, x_commentStr),
                                    })),
                                &Nil),
                        }))),
                A2(&elm_html_Html_div,
                    List_fromArray(1,
                        ((void* []){
                            A1(&elm_html_Html_Attributes_class,
                                &literal_string_card_2dfooter),
                        })),
                    List_fromArray(2,
                        ((void* []){
                            A2(&elm_html_Html_img,
                                List_fromArray(2,
                                    ((void* []){
                                        A1(&elm_html_Html_Attributes_class,
                                            &literal_string_comment_2dauthor_2dimg),
                                        A1(&author_project_Avatar_src, x_avatar),
                                    })),
                                &Nil),
                            A2(&elm_html_Html_button,
                                A2(&elm_core_List_cons,
                                    A1(&elm_html_Html_Attributes_class,
                                        &literal_string_btn_20btn_2dsm_20btn_2dprimary),
                                    x_buttonAttrs),
                                List_fromArray(1,
                                    ((void* []){
                                        A1(&elm_html_Html_text,
                                            &literal_string_Post_20Comment),
                                    }))),
                        }))),
            })));
  } else {
    tmp0 = A2(&elm_html_Html_p,
        &Nil,
        List_fromArray(4,
            ((void* []){
                A2(&elm_html_Html_a,
                    List_fromArray(1,
                        ((void* []){
                            A1(&author_project_Route_href, &author_project_Route_Login),
                        })),
                    List_fromArray(1,
                        ((void* []){
                            A1(&elm_html_Html_text, &literal_string_Sign_20in),
                        }))),
                A1(&elm_html_Html_text, &literal_string__20or_20),
                A2(&elm_html_Html_a,
                    List_fromArray(1,
                        ((void* []){
                            A1(&author_project_Route_href,
                                &author_project_Route_Register),
                        })),
                    List_fromArray(1,
                        ((void* []){
                            A1(&elm_html_Html_text, &literal_string_sign_20up),
                        }))),
                A1(&elm_html_Html_text, &literal_string__20to_20comment_2e),
            })));
  };
  return tmp0;
}
Closure author_project_Page_Article_viewAddComment = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_author_project_Page_Article_viewAddComment,
};

void* eval_author_project_Page_Article_ClickedFollow(void* args[]) {
  return ctorCustom(CTOR_ClickedFollow, 2, args);
}
Closure author_project_Page_Article_ClickedFollow = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Article_ClickedFollow,
};

void* eval_author_project_Page_Article_ClickedUnfollow(void* args[]) {
  return ctorCustom(CTOR_ClickedUnfollow, 2, args);
}
Closure author_project_Page_Article_ClickedUnfollow = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Article_ClickedUnfollow,
};

void* eval_author_project_Page_Article_ClickedDeleteArticle(void* args[]) {
  return ctorCustom(CTOR_ClickedDeleteArticle, 2, args);
}
Closure author_project_Page_Article_ClickedDeleteArticle = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Article_ClickedDeleteArticle,
};

void* eval_elm_virtual_dom_VirtualDom_Normal(void* args[]) {
  return ctorCustom(CTOR_Normal, 1, args);
}
Closure elm_virtual_dom_VirtualDom_Normal = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_virtual_dom_VirtualDom_Normal,
};
void* eval_elm_html_Html_Events_on(void* args[]) {
  void* x_event = args[0];
  void* x_decoder = args[1];
  return A2(&elm_virtual_dom_VirtualDom_on,
      x_event,
      A1(&elm_virtual_dom_VirtualDom_Normal, x_decoder));
}
Closure elm_html_Html_Events_on = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_html_Html_Events_on,
};
void* eval_elm_html_Html_Events_onClick(void* args[]) {
  void* x_msg = args[0];
  return A2(&elm_html_Html_Events_on,
      &literal_string_click,
      A1(&elm_json_Json_Decode_succeed, x_msg));
}
Closure elm_html_Html_Events_onClick = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_html_Html_Events_onClick,
};
void* eval_author_project_Page_Article_deleteButton(void* args[]) {
  void* x_cred = args[0];
  void* x_article = args[1];
  void* x_msg = A2(&author_project_Page_Article_ClickedDeleteArticle,
      x_cred,
      A1(&author_project_Article_slug, x_article));
  return A2(&elm_html_Html_button,
      List_fromArray(2,
          ((void* []){
              A1(&elm_html_Html_Attributes_class,
                  &literal_string_btn_20btn_2doutline_2ddanger_20btn_2dsm),
              A1(&elm_html_Html_Events_onClick, x_msg),
          })),
      List_fromArray(2,
          ((void* []){
              A2(&elm_html_Html_i,
                  List_fromArray(1,
                      ((void* []){
                          A1(&elm_html_Html_Attributes_class,
                              &literal_string_ion_2dtrash_2da),
                      })),
                  &Nil),
              A1(&elm_html_Html_text, &literal_string__20Delete_20Article),
          })));
}
Closure author_project_Page_Article_deleteButton = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Article_deleteButton,
};

void* eval_author_project_Page_Article_editButton(void* args[]) {
  void* x_article = args[0];
  return A2(&elm_html_Html_a,
      List_fromArray(2,
          ((void* []){
              A1(&elm_html_Html_Attributes_class,
                  &literal_string_btn_20btn_2doutline_2dsecondary_20btn_2dsm),
              A1(&author_project_Route_href,
                  A1(&author_project_Route_EditArticle,
                      A1(&author_project_Article_slug, x_article))),
          })),
      List_fromArray(2,
          ((void* []){
              A2(&elm_html_Html_i,
                  List_fromArray(1,
                      ((void* []){
                          A1(&elm_html_Html_Attributes_class, &literal_string_ion_2dedit),
                      })),
                  &Nil),
              A1(&elm_html_Html_text, &literal_string__20Edit_20Article),
          })));
}
Closure author_project_Page_Article_editButton = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_editButton,
};

void* eval_author_project_Page_Article_ClickedFavorite(void* args[]) {
  return ctorCustom(CTOR_ClickedFavorite, 3, args);
}
Closure author_project_Page_Article_ClickedFavorite = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_author_project_Page_Article_ClickedFavorite,
};

void* eval_author_project_Page_Article_ClickedUnfavorite(void* args[]) {
  return ctorCustom(CTOR_ClickedUnfavorite, 3, args);
}
Closure author_project_Page_Article_ClickedUnfavorite = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_author_project_Page_Article_ClickedUnfavorite,
};

void* eval_author_project_Article_onClickStopPropagation(void* args[]) {
  void* x_msg = args[0];
  return A2(&elm_html_Html_Events_stopPropagationOn,
      &literal_string_click,
      A1(&elm_json_Json_Decode_succeed, NEW_TUPLE2(x_msg, &True)));
}
Closure author_project_Article_onClickStopPropagation = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Article_onClickStopPropagation,
};
void* eval_author_project_Article_toggleFavoriteButton(void* args[]) {
  void* x_classStr = args[0];
  void* x_msg = args[1];
  void* x_attrs = args[2];
  void* x_kids = args[3];
  return A2(&elm_html_Html_button,
      A2(&elm_core_List_cons,
          A1(&elm_html_Html_Attributes_class, x_classStr),
          A2(&elm_core_List_cons,
              A1(&author_project_Article_onClickStopPropagation, x_msg),
              x_attrs)),
      A2(&elm_core_List_cons,
          A2(&elm_html_Html_i,
              List_fromArray(1,
                  ((void* []){
                      A1(&elm_html_Html_Attributes_class, &literal_string_ion_2dheart),
                  })),
              &Nil),
          x_kids));
}
Closure author_project_Article_toggleFavoriteButton = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x4,
    .evaluator = &eval_author_project_Article_toggleFavoriteButton,
};
void* eval_author_project_Article_favoriteButton(void* args[]) {
  void* x__v0 = args[0];
  void* x_msg = args[1];
  void* x_attrs = args[2];
  void* x_kids = args[3];
  return A4(&author_project_Article_toggleFavoriteButton,
      &literal_string_btn_20btn_2dsm_20btn_2doutline_2dprimary,
      x_msg,
      x_attrs,
      x_kids);
}
Closure author_project_Article_favoriteButton = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x4,
    .evaluator = &eval_author_project_Article_favoriteButton,
};

void* eval_author_project_Article_unfavoriteButton(void* args[]) {
  void* x__v0 = args[0];
  void* x_msg = args[1];
  void* x_attrs = args[2];
  void* x_kids = args[3];
  return A4(&author_project_Article_toggleFavoriteButton,
      &literal_string_btn_20btn_2dsm_20btn_2dprimary,
      x_msg,
      x_attrs,
      x_kids);
}
Closure author_project_Article_unfavoriteButton = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x4,
    .evaluator = &eval_author_project_Article_unfavoriteButton,
};
void* eval_author_project_Page_Article_favoriteButton(void* args[]) {
  void* x_cred = args[0];
  void* x_article = args[1];
  void* x_slug = A1(&author_project_Article_slug, x_article);
  void* x_body = A1(&author_project_Article_body, x_article);
  void* x__v0 = A1(&author_project_Article_metadata, x_article);
  void* x_favoritesCount = Utils_access_eval(((void* []){
      (void*)FIELD_favoritesCount,
      x__v0,
  }));
  void* x_favorited = Utils_access_eval(((void* []){
      (void*)FIELD_favorited,
      x__v0,
  }));
  void* x_kids = List_fromArray(1,
      ((void* []){
          A1(&elm_html_Html_text,
              A2(&elm_core_Basics_append,
                  &literal_string__20Favorite_20Article_20_28,
                  A2(&elm_core_Basics_append,
                      A1(&elm_core_String_fromInt, x_favoritesCount),
                      &literal_string__29))),
      }));
  void* tmp0;
  if (x_favorited == &True) {
    tmp0 = A4(&author_project_Article_unfavoriteButton,
        x_cred,
        A3(&author_project_Page_Article_ClickedUnfavorite, x_cred, x_slug, x_body),
        &Nil,
        x_kids);
  } else {
    tmp0 = A4(&author_project_Article_favoriteButton,
        x_cred,
        A3(&author_project_Page_Article_ClickedFavorite, x_cred, x_slug, x_body),
        &Nil,
        x_kids);
  };
  return tmp0;
}
Closure author_project_Page_Article_favoriteButton = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Article_favoriteButton,
};

void* eval_author_project_Author_toggleFollowButton(void* args[]) {
  void* x_txt = args[0];
  void* x_extraClasses = args[1];
  void* x_msgWhenClicked = args[2];
  void* x_uname = args[3];
  void* x_classStr = A2(&elm_core_Basics_append,
      &literal_string_btn_20btn_2dsm_20,
      A2(&elm_core_Basics_append,
          A2(&elm_core_String_join, &literal_string__20, x_extraClasses),
          &literal_string__20action_2dbtn));
  void* x_caption = A2(&elm_core_Basics_append,
      &literal_string__5cu00A0,
      A2(&elm_core_Basics_append,
          x_txt,
          A2(&elm_core_Basics_append,
              &literal_string__20,
              A1(&author_project_Username_toString, x_uname))));
  return A2(&elm_html_Html_button,
      List_fromArray(2,
          ((void* []){
              A1(&elm_html_Html_Attributes_class, x_classStr),
              A1(&elm_html_Html_Events_onClick, x_msgWhenClicked),
          })),
      List_fromArray(2,
          ((void* []){
              A2(&elm_html_Html_i,
                  List_fromArray(1,
                      ((void* []){
                          A1(&elm_html_Html_Attributes_class,
                              &literal_string_ion_2dplus_2dround),
                      })),
                  &Nil),
              A1(&elm_html_Html_text, x_caption),
          })));
}
Closure author_project_Author_toggleFollowButton = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x4,
    .evaluator = &eval_author_project_Author_toggleFollowButton,
};
void* eval_author_project_Author_followButton(void* args[]) {
  void* x_toMsg = args[0];
  void* x_cred = args[1];
  void* x_author = args[2];
  void* x_uname = Utils_destruct_index(x_author, 0);
  return A4(&author_project_Author_toggleFollowButton,
      &literal_string_Follow,
      List_fromArray(1,
          ((void* []){
              &literal_string_btn_2doutline_2dsecondary,
          })),
      A2(x_toMsg, x_cred, x_author),
      x_uname);
}
Closure author_project_Author_followButton = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_author_project_Author_followButton,
};

void* eval_author_project_Author_unfollowButton(void* args[]) {
  void* x_toMsg = args[0];
  void* x_cred = args[1];
  void* x_author = args[2];
  void* x_uname = Utils_destruct_index(x_author, 0);
  return A4(&author_project_Author_toggleFollowButton,
      &literal_string_Unfollow,
      List_fromArray(1,
          ((void* []){
              &literal_string_btn_2dsecondary,
          })),
      A2(x_toMsg, x_cred, x_author),
      x_uname);
}
Closure author_project_Author_unfollowButton = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_author_project_Author_unfollowButton,
};
void* eval_author_project_Page_Article_viewButtons(void* args[]) {
  void* x_cred = args[0];
  void* x_article = args[1];
  void* x_author = args[2];
  void* tmp0;
  void* tmp1 = ((Custom*)x_author)->ctor;
  if (tmp1 == CTOR_IsFollowing) {
    {
      void* x_followedAuthor = Utils_destruct_index(x_author, 0);
      tmp0 = List_fromArray(3,
          ((void* []){
              A3(&author_project_Author_unfollowButton,
                  &author_project_Page_Article_ClickedUnfollow,
                  x_cred,
                  x_followedAuthor),
              A1(&elm_html_Html_text, &literal_string__20),
              A2(&author_project_Page_Article_favoriteButton, x_cred, x_article),
          }));
    };
  } else if (tmp1 == CTOR_IsNotFollowing) {
    {
      void* x_unfollowedAuthor = Utils_destruct_index(x_author, 0);
      tmp0 = List_fromArray(3,
          ((void* []){
              A3(&author_project_Author_followButton,
                  &author_project_Page_Article_ClickedFollow,
                  x_cred,
                  x_unfollowedAuthor),
              A1(&elm_html_Html_text, &literal_string__20),
              A2(&author_project_Page_Article_favoriteButton, x_cred, x_article),
          }));
    };
  } else {
    tmp0 = List_fromArray(3,
        ((void* []){
            A1(&author_project_Page_Article_editButton, x_article),
            A1(&elm_html_Html_text, &literal_string__20),
            A2(&author_project_Page_Article_deleteButton, x_cred, x_article),
        }));
  };
  return tmp0;
}
Closure author_project_Page_Article_viewButtons = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_author_project_Page_Article_viewButtons,
};

void* eval_author_project_Page_Article_ClickedDeleteComment(void* args[]) {
  return ctorCustom(CTOR_ClickedDeleteComment, 3, args);
}
Closure author_project_Page_Article_ClickedDeleteComment = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_author_project_Page_Article_ClickedDeleteComment,
};

void* eval_author_project_Article_Comment_author(void* args[]) {
  void* x__v0 = args[0];
  void* x_comment = ((Custom*)x__v0)->values[0];
  return Utils_access_eval(((void* []){
      (void*)FIELD_author,
      x_comment,
  }));
}
Closure author_project_Article_Comment_author = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Article_Comment_author,
};

void* eval_author_project_Article_Comment_body(void* args[]) {
  void* x__v0 = args[0];
  void* x_comment = ((Custom*)x__v0)->values[0];
  return Utils_access_eval(((void* []){
      (void*)FIELD_body,
      x_comment,
  }));
}
Closure author_project_Article_Comment_body = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Article_Comment_body,
};

void* eval_author_project_Article_Comment_createdAt(void* args[]) {
  void* x__v0 = args[0];
  void* x_comment = ((Custom*)x__v0)->values[0];
  return Utils_access_eval(((void* []){
      (void*)FIELD_createdAt,
      x_comment,
  }));
}
Closure author_project_Article_Comment_createdAt = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Article_Comment_createdAt,
};
void* eval_author_project_Page_Article_viewComment(void* args[]) {
  void* x_timeZone = args[0];
  void* x_slug = args[1];
  void* x_comment = args[2];
  void* x_timestamp = A2(&author_project_Timestamp_format,
      x_timeZone,
      A1(&author_project_Article_Comment_createdAt, x_comment));
  void* x_author = A1(&author_project_Article_Comment_author, x_comment);
  void* x_authorUsername = A1(&author_project_Author_username, x_author);
  void* tmp0;
  void* tmp1 = ((Custom*)x_author)->ctor;
  if (tmp1 == CTOR_IsViewer) {
    void* x_cred = Utils_destruct_index(x_author, 0);
    void* x_msg = A3(&author_project_Page_Article_ClickedDeleteComment,
        x_cred,
        x_slug,
        A1(&author_project_Article_Comment_id, x_comment));
    tmp0 = A2(&elm_html_Html_span,
        List_fromArray(2,
            ((void* []){
                A1(&elm_html_Html_Attributes_class, &literal_string_mod_2doptions),
                A1(&elm_html_Html_Events_onClick, x_msg),
            })),
        List_fromArray(1,
            ((void* []){
                A2(&elm_html_Html_i,
                    List_fromArray(1,
                        ((void* []){
                            A1(&elm_html_Html_Attributes_class,
                                &literal_string_ion_2dtrash_2da),
                        })),
                    &Nil),
            })));
  } else {
    tmp0 = A1(&elm_html_Html_text, &literal_string_);
  };
  void* x_deleteCommentButton = tmp0;
  void* x_profile = A1(&author_project_Author_profile, x_author);
  return A2(&elm_html_Html_div,
      List_fromArray(1,
          ((void* []){
              A1(&elm_html_Html_Attributes_class, &literal_string_card),
          })),
      List_fromArray(2,
          ((void* []){
              A2(&elm_html_Html_div,
                  List_fromArray(1,
                      ((void* []){
                          A1(&elm_html_Html_Attributes_class,
                              &literal_string_card_2dblock),
                      })),
                  List_fromArray(1,
                      ((void* []){
                          A2(&elm_html_Html_p,
                              List_fromArray(1,
                                  ((void* []){
                                      A1(&elm_html_Html_Attributes_class,
                                          &literal_string_card_2dtext),
                                  })),
                              List_fromArray(1,
                                  ((void* []){
                                      A1(&elm_html_Html_text,
                                          A1(&author_project_Article_Comment_body,
                                              x_comment)),
                                  }))),
                      }))),
              A2(&elm_html_Html_div,
                  List_fromArray(1,
                      ((void* []){
                          A1(&elm_html_Html_Attributes_class,
                              &literal_string_card_2dfooter),
                      })),
                  List_fromArray(5,
                      ((void* []){
                          A2(&elm_html_Html_a,
                              List_fromArray(2,
                                  ((void* []){
                                      A1(&elm_html_Html_Attributes_class,
                                          &literal_string_comment_2dauthor),
                                      A1(&elm_html_Html_Attributes_href,
                                          &literal_string_),
                                  })),
                              List_fromArray(2,
                                  ((void* []){
                                      A2(&elm_html_Html_img,
                                          List_fromArray(2,
                                              ((void* []){
                                                  A1(&elm_html_Html_Attributes_class,
                                                      &literal_string_comment_2dauthor_2dimg),
                                                  A1(&author_project_Avatar_src,
                                                      A1(&author_project_Profile_avatar,
                                                          x_profile)),
                                              })),
                                          &Nil),
                                      A1(&elm_html_Html_text, &literal_string__20),
                                  }))),
                          A1(&elm_html_Html_text, &literal_string__20),
                          A2(&elm_html_Html_a,
                              List_fromArray(2,
                                  ((void* []){
                                      A1(&elm_html_Html_Attributes_class,
                                          &literal_string_comment_2dauthor),
                                      A1(&author_project_Route_href,
                                          A1(&author_project_Route_Profile,
                                              x_authorUsername)),
                                  })),
                              List_fromArray(1,
                                  ((void* []){
                                      A1(&elm_html_Html_text,
                                          A1(&author_project_Username_toString,
                                              x_authorUsername)),
                                  }))),
                          A2(&elm_html_Html_span,
                              List_fromArray(1,
                                  ((void* []){
                                      A1(&elm_html_Html_Attributes_class,
                                          &literal_string_date_2dposted),
                                  })),
                              List_fromArray(1,
                                  ((void* []){
                                      A1(&elm_html_Html_text, x_timestamp),
                                  }))),
                          x_deleteCommentButton,
                      }))),
          })));
}
Closure author_project_Page_Article_viewComment = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_author_project_Page_Article_viewComment,
};

void* eval_elm_core_List_isEmpty(void* args[]) {
  void* x_xs = args[0];
  void* tmp0;
  if (x_xs == &Nil) {
    tmp0 = &True;
  } else {
    tmp0 = &False;
  };
  return tmp0;
}
Closure elm_core_List_isEmpty = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_core_List_isEmpty,
};

#define elm_virtual_dom_VirtualDom_style VirtualDom_style
#define elm_html_Html_Attributes_style elm_virtual_dom_VirtualDom_style
void* eval_author_project_Page_viewErrors_lambda1(void* args[]) {
  void* x_error = args[0];
  return A2(&elm_html_Html_p,
      &Nil,
      List_fromArray(1,
          ((void* []){
              A1(&elm_html_Html_text, x_error),
          })));
}
void* eval_author_project_Page_viewErrors(void* args[]) {
  void* x_dismissErrors = args[0];
  void* x_errors = args[1];
  void* tmp0;
  if (A1(&elm_core_List_isEmpty, x_errors) == &True) {
    tmp0 = A1(&elm_html_Html_text, &literal_string_);
  } else {
    tmp0 = A2(&elm_core_Basics_apL,
        A1(&elm_html_Html_div,
            List_fromArray(6,
                ((void* []){
                    A1(&elm_html_Html_Attributes_class, &literal_string_error_2dmessages),
                    A2(&elm_html_Html_Attributes_style,
                        &literal_string_position,
                        &literal_string_fixed),
                    A2(&elm_html_Html_Attributes_style,
                        &literal_string_top,
                        &literal_string_0),
                    A2(&elm_html_Html_Attributes_style,
                        &literal_string_background,
                        &literal_string_rgb_28250_2c_20250_2c_20250_29),
                    A2(&elm_html_Html_Attributes_style,
                        &literal_string_padding,
                        &literal_string_20px),
                    A2(&elm_html_Html_Attributes_style,
                        &literal_string_border,
                        &literal_string_1px_20solid),
                }))),
        A2(&elm_core_Basics_append,
            A2(&elm_core_List_map,
                NEW_CLOSURE(
                    0, 1, &eval_author_project_Page_viewErrors_lambda1, ((void* []){})),
                x_errors),
            List_fromArray(1,
                ((void* []){
                    A2(&elm_html_Html_button,
                        List_fromArray(1,
                            ((void* []){
                                A1(&elm_html_Html_Events_onClick, x_dismissErrors),
                            })),
                        List_fromArray(1,
                            ((void* []){
                                A1(&elm_html_Html_text, &literal_string_Ok),
                            }))),
                }))));
  };
  return tmp0;
}
Closure author_project_Page_viewErrors = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_viewErrors,
};

void* eval_author_project_Session_viewer(void* args[]) {
  void* x_session = args[0];
  void* tmp0;
  void* tmp1 = ((Custom*)x_session)->ctor;
  if (tmp1 == CTOR_LoggedIn) {
    void* x_val = Utils_destruct_index(x_session, 1);
    tmp0 = A1(&elm_core_Maybe_Just, x_val);
  } else {
    tmp0 = &elm_core_Maybe_Nothing;
  };
  return tmp0;
}
Closure author_project_Session_viewer = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Session_viewer,
};
void* eval_author_project_Page_Article_view(void* args[]) {
  void* x_model = args[0];
  void* x__v0 = Utils_access_eval(((void* []){
      (void*)FIELD_article,
      x_model,
  }));
  void* tmp0;
  void* tmp1 = ((Custom*)x__v0)->ctor;
  if (tmp1 == CTOR_Loaded) {
    {
      void* x_article = Utils_destruct_index(x__v0, 0);
      void* x_slug = A1(&author_project_Article_slug, x_article);
      void* x_author = A1(&author_project_Article_author, x_article);
      void* x_avatar = A1(
          &author_project_Profile_avatar, A1(&author_project_Author_profile, x_author));
      void* x__v4 = A1(&author_project_Session_cred,
          Utils_access_eval(((void* []){
              (void*)FIELD_session,
              x_model,
          })));
      void* tmp2;
      void* tmp3 = ((Custom*)x__v4)->ctor;
      if (tmp3 == CTOR_Just) {
        void* x_cred = Utils_destruct_index(x__v4, 0);
        tmp2 = A3(&author_project_Page_Article_viewButtons, x_cred, x_article, x_author);
      } else {
        tmp2 = &Nil;
      };
      void* x_buttons = tmp2;
      void* x_profile = A1(&author_project_Author_profile, x_author);
      void* x__v1 = A1(&author_project_Article_metadata, x_article);
      void* x_title = Utils_access_eval(((void* []){
          (void*)FIELD_title,
          x__v1,
      }));
      void* x__v2 = Utils_access_eval(((void* []){
          (void*)FIELD_comments,
          x_model,
      }));
      void* tmp4;
      void* tmp5 = ((Custom*)x__v2)->ctor;
      if (tmp5 == CTOR_Loading) {
        {
          tmp4 = &Nil;
        };
      } else if (tmp5 == CTOR_LoadingSlowly) {
        {
          tmp4 = List_fromArray(1,
              ((void* []){
                  &author_project_Loading_icon,
              }));
        };
      } else if (tmp5 == CTOR_Loaded) {
        {
          void* x__v3 = Utils_destruct_index(x__v2, 0);
          void* x_commentText = Utils_destruct_index(x__v3, 0);
          void* x_comments = Utils_destruct_index(x__v3, 1);
          tmp4 = A2(&elm_core_List_cons,
              A3(&author_project_Page_Article_viewAddComment,
                  x_slug,
                  x_commentText,
                  A1(&author_project_Session_viewer,
                      Utils_access_eval(((void* []){
                          (void*)FIELD_session,
                          x_model,
                      })))),
              A2(&elm_core_List_map,
                  A2(&author_project_Page_Article_viewComment,
                      Utils_access_eval(((void* []){
                          (void*)FIELD_timeZone,
                          x_model,
                      })),
                      x_slug),
                  x_comments));
        };
      } else {
        tmp4 = List_fromArray(1,
            ((void* []){
                A1(&author_project_Loading_error, &literal_string_comments),
            }));
      };
      tmp0 = NEW_RECORD(&fg_content_title,
          2,
          ((void* []){
              A2(&elm_html_Html_div,
                  List_fromArray(1,
                      ((void* []){
                          A1(&elm_html_Html_Attributes_class,
                              &literal_string_article_2dpage),
                      })),
                  List_fromArray(2,
                      ((void* []){
                          A2(&elm_html_Html_div,
                              List_fromArray(1,
                                  ((void* []){
                                      A1(&elm_html_Html_Attributes_class,
                                          &literal_string_banner),
                                  })),
                              List_fromArray(1,
                                  ((void* []){
                                      A2(&elm_html_Html_div,
                                          List_fromArray(1,
                                              ((void* []){
                                                  A1(&elm_html_Html_Attributes_class,
                                                      &literal_string_container),
                                              })),
                                          List_fromArray(3,
                                              ((void* []){
                                                  A2(&elm_html_Html_h1,
                                                      &Nil,
                                                      List_fromArray(1,
                                                          ((void* []){
                                                              A1(&elm_html_Html_text,
                                                                  x_title),
                                                          }))),
                                                  A2(&elm_core_Basics_apL,
                                                      A1(&elm_html_Html_div,
                                                          List_fromArray(1,
                                                              ((void* []){
                                                                  A1(&elm_html_Html_Attributes_class,
                                                                      &literal_string_article_2dmeta),
                                                              }))),
                                                      A2(&elm_core_List_append,
                                                          List_fromArray(2,
                                                              ((void* []){
                                                                  A2(&elm_html_Html_a,
                                                                      List_fromArray(1,
                                                                          ((void* []){
                                                                              A1(&author_project_Route_href,
                                                                                  A1(&author_project_Route_Profile,
                                                                                      A1(&author_project_Author_username,
                                                                                          x_author))),
                                                                          })),
                                                                      List_fromArray(1,
                                                                          ((void* []){
                                                                              A2(&elm_html_Html_img,
                                                                                  List_fromArray(
                                                                                      1,
                                                                                      ((void* []){
                                                                                          A1(&author_project_Avatar_src,
                                                                                              A1(&author_project_Profile_avatar,
                                                                                                  x_profile)),
                                                                                      })),
                                                                                  &Nil),
                                                                          }))),
                                                                  A2(&elm_html_Html_div,
                                                                      List_fromArray(1,
                                                                          ((void* []){
                                                                              A1(&elm_html_Html_Attributes_class,
                                                                                  &literal_string_info),
                                                                          })),
                                                                      List_fromArray(2,
                                                                          ((void* []){
                                                                              A1(&author_project_Author_view,
                                                                                  A1(&author_project_Author_username,
                                                                                      x_author)),
                                                                              A2(&author_project_Timestamp_view,
                                                                                  Utils_access_eval(((
                                                                                      void* []){
                                                                                      (void*)
                                                                                          FIELD_timeZone,
                                                                                      x_model,
                                                                                  })),
                                                                                  Utils_access_eval(((
                                                                                      void* []){
                                                                                      (void*)
                                                                                          FIELD_createdAt,
                                                                                      A1(&author_project_Article_metadata,
                                                                                          x_article),
                                                                                  }))),
                                                                          }))),
                                                              })),
                                                          x_buttons)),
                                                  A2(&author_project_Page_viewErrors,
                                                      &author_project_Page_Article_ClickedDismissErrors,
                                                      Utils_access_eval(((void* []){
                                                          (void*)FIELD_errors,
                                                          x_model,
                                                      }))),
                                              }))),
                                  }))),
                          A2(&elm_html_Html_div,
                              List_fromArray(1,
                                  ((void* []){
                                      A1(&elm_html_Html_Attributes_class,
                                          &literal_string_container_20page),
                                  })),
                              List_fromArray(4,
                                  ((void* []){
                                      A2(&elm_html_Html_div,
                                          List_fromArray(1,
                                              ((void* []){
                                                  A1(&elm_html_Html_Attributes_class,
                                                      &literal_string_row_20article_2dcontent),
                                              })),
                                          List_fromArray(1,
                                              ((void* []){
                                                  A2(&elm_html_Html_div,
                                                      List_fromArray(1,
                                                          ((void* []){
                                                              A1(&elm_html_Html_Attributes_class,
                                                                  &literal_string_col_2dmd_2d12),
                                                          })),
                                                      List_fromArray(1,
                                                          ((void* []){
                                                              A2(&author_project_Article_Body_toHtml,
                                                                  A1(&author_project_Article_body,
                                                                      x_article),
                                                                  &Nil),
                                                          }))),
                                              }))),
                                      A2(&elm_html_Html_hr, &Nil, &Nil),
                                      A2(&elm_html_Html_div,
                                          List_fromArray(1,
                                              ((void* []){
                                                  A1(&elm_html_Html_Attributes_class,
                                                      &literal_string_article_2dactions),
                                              })),
                                          List_fromArray(1,
                                              ((void* []){
                                                  A2(&elm_core_Basics_apL,
                                                      A1(&elm_html_Html_div,
                                                          List_fromArray(1,
                                                              ((void* []){
                                                                  A1(&elm_html_Html_Attributes_class,
                                                                      &literal_string_article_2dmeta),
                                                              }))),
                                                      A2(&elm_core_List_append,
                                                          List_fromArray(2,
                                                              ((void* []){
                                                                  A2(&elm_html_Html_a,
                                                                      List_fromArray(1,
                                                                          ((void* []){
                                                                              A1(&author_project_Route_href,
                                                                                  A1(&author_project_Route_Profile,
                                                                                      A1(&author_project_Author_username,
                                                                                          x_author))),
                                                                          })),
                                                                      List_fromArray(1,
                                                                          ((void* []){
                                                                              A2(&elm_html_Html_img,
                                                                                  List_fromArray(
                                                                                      1,
                                                                                      ((void* []){
                                                                                          A1(&author_project_Avatar_src,
                                                                                              x_avatar),
                                                                                      })),
                                                                                  &Nil),
                                                                          }))),
                                                                  A2(&elm_html_Html_div,
                                                                      List_fromArray(1,
                                                                          ((void* []){
                                                                              A1(&elm_html_Html_Attributes_class,
                                                                                  &literal_string_info),
                                                                          })),
                                                                      List_fromArray(2,
                                                                          ((void* []){
                                                                              A1(&author_project_Author_view,
                                                                                  A1(&author_project_Author_username,
                                                                                      x_author)),
                                                                              A2(&author_project_Timestamp_view,
                                                                                  Utils_access_eval(((
                                                                                      void* []){
                                                                                      (void*)
                                                                                          FIELD_timeZone,
                                                                                      x_model,
                                                                                  })),
                                                                                  Utils_access_eval(((
                                                                                      void* []){
                                                                                      (void*)
                                                                                          FIELD_createdAt,
                                                                                      A1(&author_project_Article_metadata,
                                                                                          x_article),
                                                                                  }))),
                                                                          }))),
                                                              })),
                                                          x_buttons)),
                                              }))),
                                      A2(&elm_html_Html_div,
                                          List_fromArray(1,
                                              ((void* []){
                                                  A1(&elm_html_Html_Attributes_class,
                                                      &literal_string_row),
                                              })),
                                          List_fromArray(1,
                                              ((void* []){
                                                  A2(&elm_core_Basics_apL,
                                                      A1(&elm_html_Html_div,
                                                          List_fromArray(1,
                                                              ((void* []){
                                                                  A1(&elm_html_Html_Attributes_class,
                                                                      &literal_string_col_2dxs_2d12_20col_2dmd_2d8_20offset_2dmd_2d2),
                                                              }))),
                                                      tmp4),
                                              }))),
                                  }))),
                      }))),
              x_title,
          }));
    };
  } else if (tmp1 == CTOR_Loading) {
    {
      tmp0 = NEW_RECORD(&fg_content_title,
          2,
          ((void* []){
              A1(&elm_html_Html_text, &literal_string_),
              &literal_string_Article,
          }));
    };
  } else if (tmp1 == CTOR_LoadingSlowly) {
    {
      tmp0 = NEW_RECORD(&fg_content_title,
          2,
          ((void* []){
              &author_project_Loading_icon,
              &literal_string_Article,
          }));
    };
  } else {
    tmp0 = NEW_RECORD(&fg_content_title,
        2,
        ((void* []){
            A1(&author_project_Loading_error, &literal_string_article),
            &literal_string_Article,
        }));
  };
  return tmp0;
}
Closure author_project_Page_Article_view = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_view,
};

void* eval_author_project_Page_Article_Editor_getSlug(void* args[]) {
  void* x_status = args[0];
  void* tmp0;
  void* tmp1 = ((Custom*)x_status)->ctor;
  if (tmp1 == CTOR_Loading) {
    {
      void* x_slug = Utils_destruct_index(x_status, 0);
      tmp0 = A1(&elm_core_Maybe_Just, x_slug);
    };
  } else if (tmp1 == CTOR_LoadingSlowly) {
    {
      void* x_slug = Utils_destruct_index(x_status, 0);
      tmp0 = A1(&elm_core_Maybe_Just, x_slug);
    };
  } else if (tmp1 == CTOR_LoadingFailed) {
    {
      void* x_slug = Utils_destruct_index(x_status, 0);
      tmp0 = A1(&elm_core_Maybe_Just, x_slug);
    };
  } else if (tmp1 == CTOR_Saving) {
    {
      void* x_slug = Utils_destruct_index(x_status, 0);
      tmp0 = A1(&elm_core_Maybe_Just, x_slug);
    };
  } else if (tmp1 == CTOR_Editing) {
    {
      void* x_slug = Utils_destruct_index(x_status, 0);
      tmp0 = A1(&elm_core_Maybe_Just, x_slug);
    };
  } else if (tmp1 == CTOR_EditingNew) {
    { tmp0 = &elm_core_Maybe_Nothing; };
  } else {
    tmp0 = &elm_core_Maybe_Nothing;
  };
  return tmp0;
}
Closure author_project_Page_Article_Editor_getSlug = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_Editor_getSlug,
};

void* eval_author_project_Page_Article_Editor_saveArticleButton(void* args[]) {
  void* x_caption = args[0];
  void* x_extraAttrs = args[1];
  return A2(&elm_html_Html_button,
      A2(&elm_core_List_cons,
          A1(&elm_html_Html_Attributes_class,
              &literal_string_btn_20btn_2dlg_20pull_2dxs_2dright_20btn_2dprimary),
          x_extraAttrs),
      List_fromArray(1,
          ((void* []){
              A1(&elm_html_Html_text, x_caption),
          })));
}
Closure author_project_Page_Article_Editor_saveArticleButton = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Article_Editor_saveArticleButton,
};
void* eval_author_project_Page_Article_Editor_editArticleSaveButton(void* args[]) {
  void* x_extraAttrs = args[0];
  return A2(&author_project_Page_Article_Editor_saveArticleButton,
      &literal_string_Update_20Article,
      x_extraAttrs);
}
Closure author_project_Page_Article_Editor_editArticleSaveButton = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_Editor_editArticleSaveButton,
};

void* eval_author_project_Page_Article_Editor_newArticleSaveButton(void* args[]) {
  void* x_extraAttrs = args[0];
  return A2(&author_project_Page_Article_Editor_saveArticleButton,
      &literal_string_Publish_20Article,
      x_extraAttrs);
}
Closure author_project_Page_Article_Editor_newArticleSaveButton = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_Editor_newArticleSaveButton,
};

void* eval_author_project_Page_Article_Editor_ClickedSave(void* args[]) {
  return ctorCustom(CTOR_ClickedSave, 1, args);
}
Closure author_project_Page_Article_Editor_ClickedSave = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_Editor_ClickedSave,
};

void* eval_author_project_Page_Article_Editor_EnteredBody(void* args[]) {
  return ctorCustom(CTOR_EnteredBody, 1, args);
}
Closure author_project_Page_Article_Editor_EnteredBody = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_Editor_EnteredBody,
};

void* eval_author_project_Page_Article_Editor_EnteredDescription(void* args[]) {
  return ctorCustom(CTOR_EnteredDescription, 1, args);
}
Closure author_project_Page_Article_Editor_EnteredDescription = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_Editor_EnteredDescription,
};

void* eval_author_project_Page_Article_Editor_EnteredTags(void* args[]) {
  return ctorCustom(CTOR_EnteredTags, 1, args);
}
Closure author_project_Page_Article_Editor_EnteredTags = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_Editor_EnteredTags,
};

void* eval_author_project_Page_Article_Editor_EnteredTitle(void* args[]) {
  return ctorCustom(CTOR_EnteredTitle, 1, args);
}
Closure author_project_Page_Article_Editor_EnteredTitle = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_Editor_EnteredTitle,
};

#define elm_html_Html_fieldset (*ptr_elm_html_Html_fieldset)
ElmValue* ptr_elm_html_Html_fieldset;
void* init_elm_html_Html_fieldset() {
  return A1(&VirtualDom_node, &literal_string_fieldset);
}

#define elm_html_Html_input (*ptr_elm_html_Html_input)
ElmValue* ptr_elm_html_Html_input;
void* init_elm_html_Html_input() {
  return A1(&VirtualDom_node, &literal_string_input);
}
void* eval_author_project_Page_Article_Editor_viewForm(void* args[]) {
  void* x_cred = args[0];
  void* x_fields = args[1];
  void* x_saveButton = args[2];
  return A2(&elm_html_Html_form,
      List_fromArray(1,
          ((void* []){
              A1(&elm_html_Html_Events_onSubmit,
                  A1(&author_project_Page_Article_Editor_ClickedSave, x_cred)),
          })),
      List_fromArray(1,
          ((void* []){
              A2(&elm_html_Html_fieldset,
                  &Nil,
                  List_fromArray(5,
                      ((void* []){
                          A2(&elm_html_Html_fieldset,
                              List_fromArray(1,
                                  ((void* []){
                                      A1(&elm_html_Html_Attributes_class,
                                          &literal_string_form_2dgroup),
                                  })),
                              List_fromArray(1,
                                  ((void* []){
                                      A2(&elm_html_Html_input,
                                          List_fromArray(4,
                                              ((void* []){
                                                  A1(&elm_html_Html_Attributes_class,
                                                      &literal_string_form_2dcontrol_20form_2dcontrol_2dlg),
                                                  A1(&elm_html_Html_Attributes_placeholder,
                                                      &literal_string_Article_20Title),
                                                  A1(&elm_html_Html_Events_onInput,
                                                      &author_project_Page_Article_Editor_EnteredTitle),
                                                  A1(&elm_html_Html_Attributes_value,
                                                      Utils_access_eval(((void* []){
                                                          (void*)FIELD_title,
                                                          x_fields,
                                                      }))),
                                              })),
                                          &Nil),
                                  }))),
                          A2(&elm_html_Html_fieldset,
                              List_fromArray(1,
                                  ((void* []){
                                      A1(&elm_html_Html_Attributes_class,
                                          &literal_string_form_2dgroup),
                                  })),
                              List_fromArray(1,
                                  ((void* []){
                                      A2(&elm_html_Html_input,
                                          List_fromArray(4,
                                              ((void* []){
                                                  A1(&elm_html_Html_Attributes_class,
                                                      &literal_string_form_2dcontrol),
                                                  A1(&elm_html_Html_Attributes_placeholder,
                                                      &literal_string_What_5c_27s_20this_20article_20about_3f),
                                                  A1(&elm_html_Html_Events_onInput,
                                                      &author_project_Page_Article_Editor_EnteredDescription),
                                                  A1(&elm_html_Html_Attributes_value,
                                                      Utils_access_eval(((void* []){
                                                          (void*)FIELD_description,
                                                          x_fields,
                                                      }))),
                                              })),
                                          &Nil),
                                  }))),
                          A2(&elm_html_Html_fieldset,
                              List_fromArray(1,
                                  ((void* []){
                                      A1(&elm_html_Html_Attributes_class,
                                          &literal_string_form_2dgroup),
                                  })),
                              List_fromArray(1,
                                  ((void* []){
                                      A2(&elm_html_Html_textarea,
                                          List_fromArray(5,
                                              ((void* []){
                                                  A1(&elm_html_Html_Attributes_class,
                                                      &literal_string_form_2dcontrol),
                                                  A1(&elm_html_Html_Attributes_placeholder,
                                                      &literal_string_Write_20your_20article_20_28in_20markdown_29),
                                                  A2(&elm_html_Html_Attributes_attribute,
                                                      &literal_string_rows,
                                                      &literal_string_8),
                                                  A1(&elm_html_Html_Events_onInput,
                                                      &author_project_Page_Article_Editor_EnteredBody),
                                                  A1(&elm_html_Html_Attributes_value,
                                                      Utils_access_eval(((void* []){
                                                          (void*)FIELD_body,
                                                          x_fields,
                                                      }))),
                                              })),
                                          &Nil),
                                  }))),
                          A2(&elm_html_Html_fieldset,
                              List_fromArray(1,
                                  ((void* []){
                                      A1(&elm_html_Html_Attributes_class,
                                          &literal_string_form_2dgroup),
                                  })),
                              List_fromArray(1,
                                  ((void* []){
                                      A2(&elm_html_Html_input,
                                          List_fromArray(4,
                                              ((void* []){
                                                  A1(&elm_html_Html_Attributes_class,
                                                      &literal_string_form_2dcontrol),
                                                  A1(&elm_html_Html_Attributes_placeholder,
                                                      &literal_string_Enter_20tags),
                                                  A1(&elm_html_Html_Events_onInput,
                                                      &author_project_Page_Article_Editor_EnteredTags),
                                                  A1(&elm_html_Html_Attributes_value,
                                                      Utils_access_eval(((void* []){
                                                          (void*)FIELD_tags,
                                                          x_fields,
                                                      }))),
                                              })),
                                          &Nil),
                                  }))),
                          x_saveButton,
                      }))),
          })));
}
Closure author_project_Page_Article_Editor_viewForm = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_author_project_Page_Article_Editor_viewForm,
};

void* eval_author_project_Page_Article_Editor_viewProblem(void* args[]) {
  void* x_problem = args[0];
  void* tmp0;
  void* tmp1 = ((Custom*)x_problem)->ctor;
  if (tmp1 == CTOR_InvalidEntry) {
    void* x_message = Utils_destruct_index(x_problem, 1);
    tmp0 = x_message;
  } else {
    void* x_message = Utils_destruct_index(x_problem, 0);
    tmp0 = x_message;
  };
  void* x_errorMessage = tmp0;
  return A2(&elm_html_Html_li,
      &Nil,
      List_fromArray(1,
          ((void* []){
              A1(&elm_html_Html_text, x_errorMessage),
          })));
}
Closure author_project_Page_Article_Editor_viewProblem = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_Editor_viewProblem,
};
void* eval_author_project_Page_Article_Editor_viewProblems(void* args[]) {
  void* x_problems = args[0];
  return A2(&elm_html_Html_ul,
      List_fromArray(1,
          ((void* []){
              A1(&elm_html_Html_Attributes_class, &literal_string_error_2dmessages),
          })),
      A2(&elm_core_List_map,
          &author_project_Page_Article_Editor_viewProblem,
          x_problems));
}
Closure author_project_Page_Article_Editor_viewProblems = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_Editor_viewProblems,
};
void* eval_author_project_Page_Article_Editor_viewAuthenticated(void* args[]) {
  void* x_cred = args[0];
  void* x_model = args[1];
  void* x__v0 = Utils_access_eval(((void* []){
      (void*)FIELD_status,
      x_model,
  }));
  void* tmp0;
  void* tmp1 = ((Custom*)x__v0)->ctor;
  if (tmp1 == CTOR_Loading) {
    {
      tmp0 = &Nil;
    };
  } else if (tmp1 == CTOR_LoadingSlowly) {
    {
      tmp0 = List_fromArray(1,
          ((void* []){
              &author_project_Loading_icon,
          }));
    };
  } else if (tmp1 == CTOR_Saving) {
    {
      void* x_slug = Utils_destruct_index(x__v0, 0);
      void* x_form = Utils_destruct_index(x__v0, 1);
      tmp0 = List_fromArray(1,
          ((void* []){
              A3(&author_project_Page_Article_Editor_viewForm,
                  x_cred,
                  x_form,
                  A1(&author_project_Page_Article_Editor_editArticleSaveButton,
                      List_fromArray(1,
                          ((void* []){
                              A1(&elm_html_Html_Attributes_disabled, &True),
                          })))),
          }));
    };
  } else if (tmp1 == CTOR_Creating) {
    {
      void* x_form = Utils_destruct_index(x__v0, 0);
      tmp0 = List_fromArray(1,
          ((void* []){
              A3(&author_project_Page_Article_Editor_viewForm,
                  x_cred,
                  x_form,
                  A1(&author_project_Page_Article_Editor_newArticleSaveButton,
                      List_fromArray(1,
                          ((void* []){
                              A1(&elm_html_Html_Attributes_disabled, &True),
                          })))),
          }));
    };
  } else if (tmp1 == CTOR_Editing) {
    {
      void* x_slug = Utils_destruct_index(x__v0, 0);
      void* x_problems = Utils_destruct_index(x__v0, 1);
      void* x_form = Utils_destruct_index(x__v0, 2);
      tmp0 = List_fromArray(2,
          ((void* []){
              A1(&author_project_Page_Article_Editor_viewProblems, x_problems),
              A3(&author_project_Page_Article_Editor_viewForm,
                  x_cred,
                  x_form,
                  A1(&author_project_Page_Article_Editor_editArticleSaveButton, &Nil)),
          }));
    };
  } else if (tmp1 == CTOR_EditingNew) {
    {
      void* x_problems = Utils_destruct_index(x__v0, 0);
      void* x_form = Utils_destruct_index(x__v0, 1);
      tmp0 = List_fromArray(2,
          ((void* []){
              A1(&author_project_Page_Article_Editor_viewProblems, x_problems),
              A3(&author_project_Page_Article_Editor_viewForm,
                  x_cred,
                  x_form,
                  A1(&author_project_Page_Article_Editor_newArticleSaveButton, &Nil)),
          }));
    };
  } else {
    tmp0 = List_fromArray(1,
        ((void* []){
            A1(&elm_html_Html_text, &literal_string_Article_20failed_20to_20load_2e),
        }));
  };
  void* x_formHtml = tmp0;
  return A2(&elm_html_Html_div,
      List_fromArray(1,
          ((void* []){
              A1(&elm_html_Html_Attributes_class, &literal_string_editor_2dpage),
          })),
      List_fromArray(1,
          ((void* []){
              A2(&elm_html_Html_div,
                  List_fromArray(1,
                      ((void* []){
                          A1(&elm_html_Html_Attributes_class,
                              &literal_string_container_20page),
                      })),
                  List_fromArray(1,
                      ((void* []){
                          A2(&elm_html_Html_div,
                              List_fromArray(1,
                                  ((void* []){
                                      A1(&elm_html_Html_Attributes_class,
                                          &literal_string_row),
                                  })),
                              List_fromArray(1,
                                  ((void* []){
                                      A2(&elm_html_Html_div,
                                          List_fromArray(1,
                                              ((void* []){
                                                  A1(&elm_html_Html_Attributes_class,
                                                      &literal_string_col_2dmd_2d10_20offset_2dmd_2d1_20col_2dxs_2d12),
                                              })),
                                          x_formHtml),
                                  }))),
                      }))),
          })));
}
Closure author_project_Page_Article_Editor_viewAuthenticated = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Article_Editor_viewAuthenticated,
};
void* eval_author_project_Page_Article_Editor_view(void* args[]) {
  void* x_model = args[0];
  void* x__v1 = A1(&author_project_Page_Article_Editor_getSlug,
      Utils_access_eval(((void* []){
          (void*)FIELD_status,
          x_model,
      })));
  void* tmp0;
  void* tmp1 = ((Custom*)x__v1)->ctor;
  if (tmp1 == CTOR_Just) {
    void* x_slug = Utils_destruct_index(x__v1, 0);
    tmp0 = A2(&elm_core_Basics_append,
        &literal_string_Edit_20Article_20_2d_20,
        A1(&author_project_Article_Slug_toString, x_slug));
  } else {
    tmp0 = &literal_string_New_20Article;
  };
  void* x__v0 = A1(&author_project_Session_cred,
      Utils_access_eval(((void* []){
          (void*)FIELD_session,
          x_model,
      })));
  void* tmp2;
  void* tmp3 = ((Custom*)x__v0)->ctor;
  if (tmp3 == CTOR_Just) {
    void* x_cred = Utils_destruct_index(x__v0, 0);
    tmp2 = A2(&author_project_Page_Article_Editor_viewAuthenticated, x_cred, x_model);
  } else {
    tmp2 = A1(
        &elm_html_Html_text, &literal_string_Sign_20in_20to_20edit_20this_20article_2e);
  };
  return NEW_RECORD(&fg_content_title,
      2,
      ((void* []){
          tmp2,
          tmp0,
      }));
}
Closure author_project_Page_Article_Editor_view = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Article_Editor_view,
};

#define author_project_Page_Blank_view (*ptr_author_project_Page_Blank_view)
Record* ptr_author_project_Page_Blank_view;
void* init_author_project_Page_Blank_view() {
  return NEW_RECORD(&fg_content_title,
      2,
      ((void* []){
          A1(&elm_html_Html_text, &literal_string_),
          &literal_string_,
      }));
}

void* eval_author_project_Page_Home_ClickedFeedPage(void* args[]) {
  return ctorCustom(CTOR_ClickedFeedPage, 1, args);
}
Closure author_project_Page_Home_ClickedFeedPage = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Home_ClickedFeedPage,
};

Custom author_project_Article_Feed_ClickedDismissErrors = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_ClickedDismissErrors,
};

void* eval_author_project_PaginatedList_values(void* args[]) {
  void* x__v0 = args[0];
  void* x_info = ((Custom*)x__v0)->values[0];
  return Utils_access_eval(((void* []){
      (void*)FIELD_values,
      x_info,
  }));
}
Closure author_project_PaginatedList_values = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_PaginatedList_values,
};

void* eval_author_project_Article_Feed_ClickedFavorite(void* args[]) {
  return ctorCustom(CTOR_ClickedFavorite, 2, args);
}
Closure author_project_Article_Feed_ClickedFavorite = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Article_Feed_ClickedFavorite,
};

void* eval_author_project_Article_Feed_ClickedUnfavorite(void* args[]) {
  return ctorCustom(CTOR_ClickedUnfavorite, 2, args);
}
Closure author_project_Article_Feed_ClickedUnfavorite = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Article_Feed_ClickedUnfavorite,
};

void* eval_author_project_Article_Feed_viewTag(void* args[]) {
  void* x_tagName = args[0];
  return A2(&elm_html_Html_li,
      List_fromArray(1,
          ((void* []){
              A1(&elm_html_Html_Attributes_class,
                  &literal_string_tag_2ddefault_20tag_2dpill_20tag_2doutline),
          })),
      List_fromArray(1,
          ((void* []){
              A1(&elm_html_Html_text, x_tagName),
          })));
}
Closure author_project_Article_Feed_viewTag = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Article_Feed_viewTag,
};
void* eval_author_project_Article_Feed_viewPreview(void* args[]) {
  void* x_maybeCred = args[0];
  void* x_timeZone = args[1];
  void* x_article = args[2];
  void* x_slug = A1(&author_project_Article_slug, x_article);
  void* tmp0;
  void* tmp1 = ((Custom*)x_maybeCred)->ctor;
  if (tmp1 == CTOR_Just) {
    void* x_cred = Utils_destruct_index(x_maybeCred, 0);
    void* x__v2 = A1(&author_project_Article_metadata, x_article);
    void* x_favoritesCount = Utils_access_eval(((void* []){
        (void*)FIELD_favoritesCount,
        x__v2,
    }));
    void* x_favorited = Utils_access_eval(((void* []){
        (void*)FIELD_favorited,
        x__v2,
    }));
    void* tmp2;
    if (x_favorited == &True) {
      tmp2 = A2(&author_project_Article_unfavoriteButton,
          x_cred,
          A2(&author_project_Article_Feed_ClickedUnfavorite, x_cred, x_slug));
    } else {
      tmp2 = A2(&author_project_Article_favoriteButton,
          x_cred,
          A2(&author_project_Article_Feed_ClickedFavorite, x_cred, x_slug));
    };
    void* x_viewButton = tmp2;
    tmp0 = A2(x_viewButton,
        List_fromArray(1,
            ((void* []){
                A1(&elm_html_Html_Attributes_class, &literal_string_pull_2dxs_2dright),
            })),
        List_fromArray(1,
            ((void* []){
                A1(&elm_html_Html_text,
                    A2(&elm_core_Basics_append,
                        &literal_string__20,
                        A1(&elm_core_String_fromInt, x_favoritesCount))),
            })));
  } else {
    tmp0 = A1(&elm_html_Html_text, &literal_string_);
  };
  void* x_faveButton = tmp0;
  void* x_author = A1(&author_project_Article_author, x_article);
  void* x_profile = A1(&author_project_Author_profile, x_author);
  void* x_username = A1(&author_project_Author_username, x_author);
  void* x__v0 = A1(&author_project_Article_metadata, x_article);
  void* x_title = Utils_access_eval(((void* []){
      (void*)FIELD_title,
      x__v0,
  }));
  void* x_description = Utils_access_eval(((void* []){
      (void*)FIELD_description,
      x__v0,
  }));
  void* x_createdAt = Utils_access_eval(((void* []){
      (void*)FIELD_createdAt,
      x__v0,
  }));
  return A2(&elm_html_Html_div,
      List_fromArray(1,
          ((void* []){
              A1(&elm_html_Html_Attributes_class, &literal_string_article_2dpreview),
          })),
      List_fromArray(2,
          ((void* []){
              A2(&elm_html_Html_div,
                  List_fromArray(1,
                      ((void* []){
                          A1(&elm_html_Html_Attributes_class,
                              &literal_string_article_2dmeta),
                      })),
                  List_fromArray(3,
                      ((void* []){
                          A2(&elm_html_Html_a,
                              List_fromArray(1,
                                  ((void* []){
                                      A1(&author_project_Route_href,
                                          A1(&author_project_Route_Profile, x_username)),
                                  })),
                              List_fromArray(1,
                                  ((void* []){
                                      A2(&elm_html_Html_img,
                                          List_fromArray(1,
                                              ((void* []){
                                                  A1(&author_project_Avatar_src,
                                                      A1(&author_project_Profile_avatar,
                                                          x_profile)),
                                              })),
                                          &Nil),
                                  }))),
                          A2(&elm_html_Html_div,
                              List_fromArray(1,
                                  ((void* []){
                                      A1(&elm_html_Html_Attributes_class,
                                          &literal_string_info),
                                  })),
                              List_fromArray(2,
                                  ((void* []){
                                      A1(&author_project_Author_view, x_username),
                                      A2(&author_project_Timestamp_view,
                                          x_timeZone,
                                          x_createdAt),
                                  }))),
                          x_faveButton,
                      }))),
              A2(&elm_html_Html_a,
                  List_fromArray(2,
                      ((void* []){
                          A1(&elm_html_Html_Attributes_class,
                              &literal_string_preview_2dlink),
                          A1(&author_project_Route_href,
                              A1(&author_project_Route_Article,
                                  A1(&author_project_Article_slug, x_article))),
                      })),
                  List_fromArray(4,
                      ((void* []){
                          A2(&elm_html_Html_h1,
                              &Nil,
                              List_fromArray(1,
                                  ((void* []){
                                      A1(&elm_html_Html_text, x_title),
                                  }))),
                          A2(&elm_html_Html_p,
                              &Nil,
                              List_fromArray(1,
                                  ((void* []){
                                      A1(&elm_html_Html_text, x_description),
                                  }))),
                          A2(&elm_html_Html_span,
                              &Nil,
                              List_fromArray(1,
                                  ((void* []){
                                      A1(&elm_html_Html_text,
                                          &literal_string_Read_20more_2e_2e_2e),
                                  }))),
                          A2(&elm_html_Html_ul,
                              List_fromArray(1,
                                  ((void* []){
                                      A1(&elm_html_Html_Attributes_class,
                                          &literal_string_tag_2dlist),
                                  })),
                              A2(&elm_core_List_map,
                                  &author_project_Article_Feed_viewTag,
                                  Utils_access_eval(((void* []){
                                      (void*)FIELD_tags,
                                      A1(&author_project_Article_metadata, x_article),
                                  })))),
                      }))),
          })));
}
Closure author_project_Article_Feed_viewPreview = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_author_project_Article_Feed_viewPreview,
};
void* eval_author_project_Article_Feed_viewArticles(void* args[]) {
  void* x_timeZone = args[0];
  void* x__v0 = args[1];
  void* x_articles = Utils_access_eval(((void* []){
      (void*)FIELD_articles,
      ((Custom*)x__v0)->values[0],
  }));
  void* x_session = Utils_access_eval(((void* []){
      (void*)FIELD_session,
      ((Custom*)x__v0)->values[0],
  }));
  void* x_errors = Utils_access_eval(((void* []){
      (void*)FIELD_errors,
      ((Custom*)x__v0)->values[0],
  }));
  void* x_maybeCred = A1(&author_project_Session_cred, x_session);
  void* x_articlesHtml = A2(&elm_core_Basics_apR,
      A1(&author_project_PaginatedList_values, x_articles),
      A1(&elm_core_List_map,
          A2(&author_project_Article_Feed_viewPreview, x_maybeCred, x_timeZone)));
  return A2(&elm_core_List_cons,
      A2(&author_project_Page_viewErrors,
          &author_project_Article_Feed_ClickedDismissErrors,
          x_errors),
      x_articlesHtml);
}
Closure author_project_Article_Feed_viewArticles = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Article_Feed_viewArticles,
};

#define author_project_Page_Home_viewBanner (*ptr_author_project_Page_Home_viewBanner)
ElmValue* ptr_author_project_Page_Home_viewBanner;
void* init_author_project_Page_Home_viewBanner() {
  return A2(&elm_html_Html_div,
      List_fromArray(1,
          ((void* []){
              A1(&elm_html_Html_Attributes_class, &literal_string_banner),
          })),
      List_fromArray(1,
          ((void* []){
              A2(&elm_html_Html_div,
                  List_fromArray(1,
                      ((void* []){
                          A1(&elm_html_Html_Attributes_class, &literal_string_container),
                      })),
                  List_fromArray(2,
                      ((void* []){
                          A2(&elm_html_Html_h1,
                              List_fromArray(1,
                                  ((void* []){
                                      A1(&elm_html_Html_Attributes_class,
                                          &literal_string_logo_2dfont),
                                  })),
                              List_fromArray(1,
                                  ((void* []){
                                      A1(&elm_html_Html_text, &literal_string_conduit),
                                  }))),
                          A2(&elm_html_Html_p,
                              &Nil,
                              List_fromArray(1,
                                  ((void* []){
                                      A1(&elm_html_Html_text,
                                          &literal_string_A_20place_20to_20share_20your_20knowledge_2e),
                                  }))),
                      }))),
          })));
}

void* eval_author_project_Article_Feed_pageLink(void* args[]) {
  void* x_toMsg = args[0];
  void* x_targetPage = args[1];
  void* x_isActive = args[2];
  return A2(&elm_html_Html_li,
      List_fromArray(1,
          ((void* []){
              A1(&elm_html_Html_Attributes_classList,
                  List_fromArray(2,
                      ((void* []){
                          NEW_TUPLE2(&literal_string_page_2ditem, &True),
                          NEW_TUPLE2(&literal_string_active, x_isActive),
                      }))),
          })),
      List_fromArray(1,
          ((void* []){
              A2(&elm_html_Html_a,
                  List_fromArray(3,
                      ((void* []){
                          A1(&elm_html_Html_Attributes_class,
                              &literal_string_page_2dlink),
                          A1(&elm_html_Html_Events_onClick, A1(x_toMsg, x_targetPage)),
                          A1(&elm_html_Html_Attributes_href, &literal_string_),
                      })),
                  List_fromArray(1,
                      ((void* []){
                          A1(&elm_html_Html_text,
                              A1(&elm_core_String_fromInt, x_targetPage)),
                      }))),
          })));
}
Closure author_project_Article_Feed_pageLink = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_author_project_Article_Feed_pageLink,
};

void* eval_author_project_PaginatedList_total(void* args[]) {
  void* x__v0 = args[0];
  void* x_info = ((Custom*)x__v0)->values[0];
  return Utils_access_eval(((void* []){
      (void*)FIELD_total,
      x_info,
  }));
}
Closure author_project_PaginatedList_total = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_PaginatedList_total,
};
void* eval_author_project_Article_Feed_viewPagination_lambda0(void* args[]) {
  void* x_page = args[0];
  void* x_toMsg = args[1];
  void* x_currentPage = args[2];
  return A3(&author_project_Article_Feed_pageLink,
      x_toMsg,
      x_currentPage,
      A2(&elm_core_Basics_eq, x_currentPage, x_page));
}
void* eval_author_project_Article_Feed_viewPagination(void* args[]) {
  void* x_toMsg = args[0];
  void* x_page = args[1];
  void* x__v0 = args[2];
  void* x_feed = ((Custom*)x__v0)->values[0];
  void* x_viewPageLink = NEW_CLOSURE(2,
      3,
      &eval_author_project_Article_Feed_viewPagination_lambda0,
      ((void* []){
          x_page,
          x_toMsg,
      }));
  void* x_totalPages = A1(&author_project_PaginatedList_total,
      Utils_access_eval(((void* []){
          (void*)FIELD_articles,
          x_feed,
      })));
  void* tmp1;
  if (A2(&elm_core_Basics_gt, x_totalPages, &literal_int_1) == &True) {
    tmp1 = A2(&elm_core_Basics_apR,
        A2(&elm_core_Basics_apR,
            A2(&elm_core_List_range, &literal_int_1, x_totalPages),
            A1(&elm_core_List_map, x_viewPageLink)),
        A1(&elm_html_Html_ul,
            List_fromArray(1,
                ((void* []){
                    A1(&elm_html_Html_Attributes_class, &literal_string_pagination),
                }))));
  } else {
    tmp1 = A1(&elm_html_Html_text, &literal_string_);
  };
  return tmp1;
}
Closure author_project_Article_Feed_viewPagination = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_author_project_Article_Feed_viewPagination,
};

void* eval_author_project_Page_Home_ClickedTab(void* args[]) {
  return ctorCustom(CTOR_ClickedTab, 1, args);
}
Closure author_project_Page_Home_ClickedTab = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Home_ClickedTab,
};
#define author_project_Page_Home_globalFeed (*ptr_author_project_Page_Home_globalFeed)
Tuple2* ptr_author_project_Page_Home_globalFeed;
void* init_author_project_Page_Home_globalFeed() {
  return NEW_TUPLE2(&literal_string_Global_20Feed,
      A1(&author_project_Page_Home_ClickedTab, &author_project_Page_Home_GlobalFeed));
}

void* eval_author_project_Page_Home_tagFeed(void* args[]) {
  void* x_tag = args[0];
  return NEW_TUPLE2(A2(&elm_core_Basics_append,
                        &literal_string__23,
                        A1(&author_project_Article_Tag_toString, x_tag)),
      A1(&author_project_Page_Home_ClickedTab,
          A1(&author_project_Page_Home_TagFeed, x_tag)));
}
Closure author_project_Page_Home_tagFeed = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Home_tagFeed,
};

void* eval_author_project_Article_Feed_viewTab(void* args[]) {
  void* x_attrs = args[0];
  void* x__v0 = args[1];
  void* x_name = Utils_destruct_index(x__v0, 0);
  void* x_msg = Utils_destruct_index(x__v0, 1);
  return A2(&elm_html_Html_li,
      List_fromArray(1,
          ((void* []){
              A1(&elm_html_Html_Attributes_class, &literal_string_nav_2ditem),
          })),
      List_fromArray(1,
          ((void* []){
              A2(&elm_html_Html_a,
                  A2(&elm_core_List_cons,
                      A1(&elm_html_Html_Attributes_class, &literal_string_nav_2dlink),
                      A2(&elm_core_List_cons,
                          A1(&elm_html_Html_Events_onClick, x_msg),
                          A2(&elm_core_List_cons,
                              A1(&elm_html_Html_Attributes_href, &literal_string_),
                              x_attrs))),
                  List_fromArray(1,
                      ((void* []){
                          A1(&elm_html_Html_text, x_name),
                      }))),
          })));
}
Closure author_project_Article_Feed_viewTab = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Article_Feed_viewTab,
};
void* eval_author_project_Article_Feed_viewTabs(void* args[]) {
  void* x_before = args[0];
  void* x_selected = args[1];
  void* x_after = args[2];
  return A2(&elm_core_Basics_apL,
      A1(&elm_html_Html_ul,
          List_fromArray(1,
              ((void* []){
                  A1(&elm_html_Html_Attributes_class,
                      &literal_string_nav_20nav_2dpills_20outline_2dactive),
              }))),
      A1(&elm_core_List_concat,
          List_fromArray(3,
              ((void* []){
                  A2(&elm_core_List_map,
                      A1(&author_project_Article_Feed_viewTab, &Nil),
                      x_before),
                  List_fromArray(1,
                      ((void* []){
                          A2(&author_project_Article_Feed_viewTab,
                              List_fromArray(1,
                                  ((void* []){
                                      A1(&elm_html_Html_Attributes_class,
                                          &literal_string_active),
                                  })),
                              x_selected),
                      })),
                  A2(&elm_core_List_map,
                      A1(&author_project_Article_Feed_viewTab, &Nil),
                      x_after),
              }))));
}
Closure author_project_Article_Feed_viewTabs = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_author_project_Article_Feed_viewTabs,
};

void* eval_author_project_Page_Home_yourFeed(void* args[]) {
  void* x_cred = args[0];
  return NEW_TUPLE2(&literal_string_Your_20Feed,
      A1(&author_project_Page_Home_ClickedTab,
          A1(&author_project_Page_Home_YourFeed, x_cred)));
}
Closure author_project_Page_Home_yourFeed = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Home_yourFeed,
};
void* eval_author_project_Page_Home_viewTabs(void* args[]) {
  void* x_maybeCred = args[0];
  void* x_tab = args[1];
  void* tmp0;
  void* tmp1 = ((Custom*)x_tab)->ctor;
  if (tmp1 == CTOR_YourFeed) {
    {
      void* x_cred = Utils_destruct_index(x_tab, 0);
      tmp0 = A3(&author_project_Article_Feed_viewTabs,
          &Nil,
          A1(&author_project_Page_Home_yourFeed, x_cred),
          List_fromArray(1,
              ((void* []){
                  &author_project_Page_Home_globalFeed,
              })));
    };
  } else if (tmp1 == CTOR_GlobalFeed) {
    {
      void* tmp4;
      void* tmp5 = ((Custom*)x_maybeCred)->ctor;
      if (tmp5 == CTOR_Just) {
        void* x_cred = Utils_destruct_index(x_maybeCred, 0);
        tmp4 = List_fromArray(1,
            ((void* []){
                A1(&author_project_Page_Home_yourFeed, x_cred),
            }));
      } else {
        tmp4 = &Nil;
      };
      void* x_otherTabs = tmp4;
      tmp0 = A3(&author_project_Article_Feed_viewTabs,
          x_otherTabs,
          &author_project_Page_Home_globalFeed,
          &Nil);
    };
  } else {
    void* x_tag = Utils_destruct_index(x_tab, 0);
    void* tmp2;
    void* tmp3 = ((Custom*)x_maybeCred)->ctor;
    if (tmp3 == CTOR_Just) {
      void* x_cred = Utils_destruct_index(x_maybeCred, 0);
      tmp2 = List_fromArray(2,
          ((void* []){
              A1(&author_project_Page_Home_yourFeed, x_cred),
              &author_project_Page_Home_globalFeed,
          }));
    } else {
      tmp2 = List_fromArray(1,
          ((void* []){
              &author_project_Page_Home_globalFeed,
          }));
    };
    void* x_otherTabs = tmp2;
    tmp0 = A3(&author_project_Article_Feed_viewTabs,
        x_otherTabs,
        A1(&author_project_Page_Home_tagFeed, x_tag),
        &Nil);
  };
  return tmp0;
}
Closure author_project_Page_Home_viewTabs = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Home_viewTabs,
};

void* eval_author_project_Page_Home_ClickedTag(void* args[]) {
  return ctorCustom(CTOR_ClickedTag, 1, args);
}
Closure author_project_Page_Home_ClickedTag = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Home_ClickedTag,
};
void* eval_author_project_Page_Home_viewTag(void* args[]) {
  void* x_tagName = args[0];
  return A2(&elm_html_Html_a,
      List_fromArray(3,
          ((void* []){
              A1(&elm_html_Html_Attributes_class,
                  &literal_string_tag_2dpill_20tag_2ddefault),
              A1(&elm_html_Html_Events_onClick,
                  A1(&author_project_Page_Home_ClickedTag, x_tagName)),
              A1(&elm_html_Html_Attributes_href, &literal_string_),
          })),
      List_fromArray(1,
          ((void* []){
              A1(&elm_html_Html_text,
                  A1(&author_project_Article_Tag_toString, x_tagName)),
          })));
}
Closure author_project_Page_Home_viewTag = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Home_viewTag,
};
void* eval_author_project_Page_Home_viewTags(void* args[]) {
  void* x_tags = args[0];
  return A2(&elm_html_Html_div,
      List_fromArray(1,
          ((void* []){
              A1(&elm_html_Html_Attributes_class, &literal_string_tag_2dlist),
          })),
      A2(&elm_core_List_map, &author_project_Page_Home_viewTag, x_tags));
}
Closure author_project_Page_Home_viewTags = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Home_viewTags,
};
void* eval_author_project_Page_Home_view(void* args[]) {
  void* x_model = args[0];
  void* x__v1 = Utils_access_eval(((void* []){
      (void*)FIELD_tags,
      x_model,
  }));
  void* tmp0;
  void* tmp1 = ((Custom*)x__v1)->ctor;
  if (tmp1 == CTOR_Loaded) {
    {
      void* x_tags = Utils_destruct_index(x__v1, 0);
      tmp0 = List_fromArray(1,
          ((void* []){
              A2(&elm_core_Basics_apL,
                  A1(&elm_html_Html_div,
                      List_fromArray(1,
                          ((void* []){
                              A1(&elm_html_Html_Attributes_class,
                                  &literal_string_sidebar),
                          }))),
                  List_fromArray(2,
                      ((void* []){
                          A2(&elm_html_Html_p,
                              &Nil,
                              List_fromArray(1,
                                  ((void* []){
                                      A1(&elm_html_Html_text,
                                          &literal_string_Popular_20Tags),
                                  }))),
                          A1(&author_project_Page_Home_viewTags, x_tags),
                      }))),
          }));
    };
  } else if (tmp1 == CTOR_Loading) {
    { tmp0 = &Nil; };
  } else if (tmp1 == CTOR_LoadingSlowly) {
    {
      tmp0 = List_fromArray(1,
          ((void* []){
              &author_project_Loading_icon,
          }));
    };
  } else {
    tmp0 = List_fromArray(1,
        ((void* []){
            A1(&author_project_Loading_error, &literal_string_tags),
        }));
  };
  void* x__v0 = Utils_access_eval(((void* []){
      (void*)FIELD_feed,
      x_model,
  }));
  void* tmp2;
  void* tmp3 = ((Custom*)x__v0)->ctor;
  if (tmp3 == CTOR_Loaded) {
    {
      void* x_feed = Utils_destruct_index(x__v0, 0);
      tmp2 = List_fromArray(1,
          ((void* []){
              A2(&elm_core_Basics_apL,
                  A1(&elm_html_Html_div,
                      List_fromArray(1,
                          ((void* []){
                              A1(&elm_html_Html_Attributes_class,
                                  &literal_string_feed_2dtoggle),
                          }))),
                  A1(&elm_core_List_concat,
                      List_fromArray(3,
                          ((void* []){
                              List_fromArray(1,
                                  ((void* []){
                                      A2(&author_project_Page_Home_viewTabs,
                                          A1(&author_project_Session_cred,
                                              Utils_access_eval(((void* []){
                                                  (void*)FIELD_session,
                                                  x_model,
                                              }))),
                                          Utils_access_eval(((void* []){
                                              (void*)FIELD_feedTab,
                                              x_model,
                                          }))),
                                  })),
                              A2(&elm_core_Basics_apR,
                                  A2(&author_project_Article_Feed_viewArticles,
                                      Utils_access_eval(((void* []){
                                          (void*)FIELD_timeZone,
                                          x_model,
                                      })),
                                      x_feed),
                                  A1(&elm_core_List_map,
                                      A1(&elm_html_Html_map,
                                          &author_project_Page_Home_GotFeedMsg))),
                              List_fromArray(1,
                                  ((void* []){
                                      A3(&author_project_Article_Feed_viewPagination,
                                          &author_project_Page_Home_ClickedFeedPage,
                                          Utils_access_eval(((void* []){
                                              (void*)FIELD_feedPage,
                                              x_model,
                                          })),
                                          x_feed),
                                  })),
                          })))),
          }));
    };
  } else if (tmp3 == CTOR_Loading) {
    { tmp2 = &Nil; };
  } else if (tmp3 == CTOR_LoadingSlowly) {
    {
      tmp2 = List_fromArray(1,
          ((void* []){
              &author_project_Loading_icon,
          }));
    };
  } else {
    tmp2 = List_fromArray(1,
        ((void* []){
            A1(&author_project_Loading_error, &literal_string_feed),
        }));
  };
  return NEW_RECORD(&fg_content_title,
      2,
      ((void* []){
          A2(&elm_html_Html_div,
              List_fromArray(1,
                  ((void* []){
                      A1(&elm_html_Html_Attributes_class, &literal_string_home_2dpage),
                  })),
              List_fromArray(2,
                  ((void* []){
                      &author_project_Page_Home_viewBanner,
                      A2(&elm_html_Html_div,
                          List_fromArray(1,
                              ((void* []){
                                  A1(&elm_html_Html_Attributes_class,
                                      &literal_string_container_20page),
                              })),
                          List_fromArray(1,
                              ((void* []){
                                  A2(&elm_html_Html_div,
                                      List_fromArray(1,
                                          ((void* []){
                                              A1(&elm_html_Html_Attributes_class,
                                                  &literal_string_row),
                                          })),
                                      List_fromArray(2,
                                          ((void* []){
                                              A2(&elm_core_Basics_apL,
                                                  A1(&elm_html_Html_div,
                                                      List_fromArray(1,
                                                          ((void* []){
                                                              A1(&elm_html_Html_Attributes_class,
                                                                  &literal_string_col_2dmd_2d9),
                                                          }))),
                                                  tmp2),
                                              A2(&elm_core_Basics_apL,
                                                  A1(&elm_html_Html_div,
                                                      List_fromArray(1,
                                                          ((void* []){
                                                              A1(&elm_html_Html_Attributes_class,
                                                                  &literal_string_col_2dmd_2d3),
                                                          }))),
                                                  tmp0),
                                          }))),
                              }))),
                  }))),
          &literal_string_Conduit,
      }));
}
Closure author_project_Page_Home_view = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Home_view,
};

void* eval_author_project_Page_Login_EnteredEmail(void* args[]) {
  return ctorCustom(CTOR_EnteredEmail, 1, args);
}
Closure author_project_Page_Login_EnteredEmail = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Login_EnteredEmail,
};

void* eval_author_project_Page_Login_EnteredPassword(void* args[]) {
  return ctorCustom(CTOR_EnteredPassword, 1, args);
}
Closure author_project_Page_Login_EnteredPassword = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Login_EnteredPassword,
};

Custom author_project_Page_Login_SubmittedForm = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_SubmittedForm,
};

#define elm_html_Html_Attributes_type_ (*ptr_elm_html_Html_Attributes_type_)
ElmValue* ptr_elm_html_Html_Attributes_type_;
void* init_elm_html_Html_Attributes_type_() {
  return A1(&elm_html_Html_Attributes_stringProperty, &literal_string_type);
}
void* eval_author_project_Page_Login_viewForm(void* args[]) {
  void* x_form = args[0];
  return A2(&elm_html_Html_form,
      List_fromArray(1,
          ((void* []){
              A1(&elm_html_Html_Events_onSubmit,
                  &author_project_Page_Login_SubmittedForm),
          })),
      List_fromArray(3,
          ((void* []){
              A2(&elm_html_Html_fieldset,
                  List_fromArray(1,
                      ((void* []){
                          A1(&elm_html_Html_Attributes_class,
                              &literal_string_form_2dgroup),
                      })),
                  List_fromArray(1,
                      ((void* []){
                          A2(&elm_html_Html_input,
                              List_fromArray(4,
                                  ((void* []){
                                      A1(&elm_html_Html_Attributes_class,
                                          &literal_string_form_2dcontrol_20form_2dcontrol_2dlg),
                                      A1(&elm_html_Html_Attributes_placeholder,
                                          &literal_string_Email),
                                      A1(&elm_html_Html_Events_onInput,
                                          &author_project_Page_Login_EnteredEmail),
                                      A1(&elm_html_Html_Attributes_value,
                                          Utils_access_eval(((void* []){
                                              (void*)FIELD_email,
                                              x_form,
                                          }))),
                                  })),
                              &Nil),
                      }))),
              A2(&elm_html_Html_fieldset,
                  List_fromArray(1,
                      ((void* []){
                          A1(&elm_html_Html_Attributes_class,
                              &literal_string_form_2dgroup),
                      })),
                  List_fromArray(1,
                      ((void* []){
                          A2(&elm_html_Html_input,
                              List_fromArray(5,
                                  ((void* []){
                                      A1(&elm_html_Html_Attributes_class,
                                          &literal_string_form_2dcontrol_20form_2dcontrol_2dlg),
                                      A1(&elm_html_Html_Attributes_type_,
                                          &literal_string_password),
                                      A1(&elm_html_Html_Attributes_placeholder,
                                          &literal_string_Password),
                                      A1(&elm_html_Html_Events_onInput,
                                          &author_project_Page_Login_EnteredPassword),
                                      A1(&elm_html_Html_Attributes_value,
                                          Utils_access_eval(((void* []){
                                              (void*)FIELD_password,
                                              x_form,
                                          }))),
                                  })),
                              &Nil),
                      }))),
              A2(&elm_html_Html_button,
                  List_fromArray(1,
                      ((void* []){
                          A1(&elm_html_Html_Attributes_class,
                              &literal_string_btn_20btn_2dlg_20btn_2dprimary_20pull_2dxs_2dright),
                      })),
                  List_fromArray(1,
                      ((void* []){
                          A1(&elm_html_Html_text, &literal_string_Sign_20in),
                      }))),
          })));
}
Closure author_project_Page_Login_viewForm = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Login_viewForm,
};

void* eval_author_project_Page_Login_viewProblem(void* args[]) {
  void* x_problem = args[0];
  void* tmp0;
  void* tmp1 = ((Custom*)x_problem)->ctor;
  if (tmp1 == CTOR_InvalidEntry) {
    void* x_str = Utils_destruct_index(x_problem, 1);
    tmp0 = x_str;
  } else {
    void* x_str = Utils_destruct_index(x_problem, 0);
    tmp0 = x_str;
  };
  void* x_errorMessage = tmp0;
  return A2(&elm_html_Html_li,
      &Nil,
      List_fromArray(1,
          ((void* []){
              A1(&elm_html_Html_text, x_errorMessage),
          })));
}
Closure author_project_Page_Login_viewProblem = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Login_viewProblem,
};
void* eval_author_project_Page_Login_view(void* args[]) {
  void* x_model = args[0];
  return NEW_RECORD(&fg_content_title,
      2,
      ((void* []){
          A2(&elm_html_Html_div,
              List_fromArray(1,
                  ((void* []){
                      A1(&elm_html_Html_Attributes_class, &literal_string_cred_2dpage),
                  })),
              List_fromArray(1,
                  ((void* []){
                      A2(&elm_html_Html_div,
                          List_fromArray(1,
                              ((void* []){
                                  A1(&elm_html_Html_Attributes_class,
                                      &literal_string_container_20page),
                              })),
                          List_fromArray(1,
                              ((void* []){
                                  A2(&elm_html_Html_div,
                                      List_fromArray(1,
                                          ((void* []){
                                              A1(&elm_html_Html_Attributes_class,
                                                  &literal_string_row),
                                          })),
                                      List_fromArray(1,
                                          ((void* []){
                                              A2(&elm_html_Html_div,
                                                  List_fromArray(1,
                                                      ((void* []){
                                                          A1(&elm_html_Html_Attributes_class,
                                                              &literal_string_col_2dmd_2d6_20offset_2dmd_2d3_20col_2dxs_2d12),
                                                      })),
                                                  List_fromArray(4,
                                                      ((void* []){
                                                          A2(&elm_html_Html_h1,
                                                              List_fromArray(1,
                                                                  ((void* []){
                                                                      A1(&elm_html_Html_Attributes_class,
                                                                          &literal_string_text_2dxs_2dcenter),
                                                                  })),
                                                              List_fromArray(1,
                                                                  ((void* []){
                                                                      A1(&elm_html_Html_text,
                                                                          &literal_string_Sign_20in),
                                                                  }))),
                                                          A2(&elm_html_Html_p,
                                                              List_fromArray(1,
                                                                  ((void* []){
                                                                      A1(&elm_html_Html_Attributes_class,
                                                                          &literal_string_text_2dxs_2dcenter),
                                                                  })),
                                                              List_fromArray(1,
                                                                  ((void* []){
                                                                      A2(&elm_html_Html_a,
                                                                          List_fromArray(
                                                                              1,
                                                                              ((void* []){
                                                                                  A1(&author_project_Route_href,
                                                                                      &author_project_Route_Register),
                                                                              })),
                                                                          List_fromArray(
                                                                              1,
                                                                              ((void* []){
                                                                                  A1(&elm_html_Html_text,
                                                                                      &literal_string_Need_20an_20account_3f),
                                                                              }))),
                                                                  }))),
                                                          A2(&elm_html_Html_ul,
                                                              List_fromArray(1,
                                                                  ((void* []){
                                                                      A1(&elm_html_Html_Attributes_class,
                                                                          &literal_string_error_2dmessages),
                                                                  })),
                                                              A2(&elm_core_List_map,
                                                                  &author_project_Page_Login_viewProblem,
                                                                  Utils_access_eval(((
                                                                      void* []){
                                                                      (void*)
                                                                          FIELD_problems,
                                                                      x_model,
                                                                  })))),
                                                          A1(&author_project_Page_Login_viewForm,
                                                              Utils_access_eval(
                                                                  ((void* []){
                                                                      (void*)FIELD_form,
                                                                      x_model,
                                                                  }))),
                                                      }))),
                                          }))),
                              }))),
                  }))),
          &literal_string_Login,
      }));
}
Closure author_project_Page_Login_view = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Login_view,
};

#define author_project_Asset_error (*ptr_author_project_Asset_error)
ElmValue* ptr_author_project_Asset_error;
void* init_author_project_Asset_error() {
  return A1(&author_project_Asset_image, &literal_string_error_2ejpg);
}

#define elm_html_Html_Attributes_id (*ptr_elm_html_Html_Attributes_id)
ElmValue* ptr_elm_html_Html_Attributes_id;
void* init_elm_html_Html_Attributes_id() {
  return A1(&elm_html_Html_Attributes_stringProperty, &literal_string_id);
}

#define elm_html_Html_main_ (*ptr_elm_html_Html_main_)
ElmValue* ptr_elm_html_Html_main_;
void* init_elm_html_Html_main_() {
  return A1(&VirtualDom_node, &literal_string_main);
}

void* eval_elm_html_Html_Attributes_tabindex(void* args[]) {
  void* x_n = args[0];
  return A2(
      &VirtualDom_attribute, &literal_string_tabIndex, A1(&elm_core_String_fromInt, x_n));
}
Closure elm_html_Html_Attributes_tabindex = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_html_Html_Attributes_tabindex,
};
#define author_project_Page_NotFound_view (*ptr_author_project_Page_NotFound_view)
Record* ptr_author_project_Page_NotFound_view;
void* init_author_project_Page_NotFound_view() {
  return NEW_RECORD(&fg_content_title,
      2,
      ((void* []){
          A2(&elm_html_Html_main_,
              List_fromArray(3,
                  ((void* []){
                      A1(&elm_html_Html_Attributes_id, &literal_string_content),
                      A1(&elm_html_Html_Attributes_class, &literal_string_container),
                      A1(&elm_html_Html_Attributes_tabindex,
                          A1(&elm_core_Basics_negate, &literal_int_1)),
                  })),
              List_fromArray(2,
                  ((void* []){
                      A2(&elm_html_Html_h1,
                          &Nil,
                          List_fromArray(1,
                              ((void* []){
                                  A1(&elm_html_Html_text, &literal_string_Not_20Found),
                              }))),
                      A2(&elm_html_Html_div,
                          List_fromArray(1,
                              ((void* []){
                                  A1(&elm_html_Html_Attributes_class,
                                      &literal_string_row),
                              })),
                          List_fromArray(1,
                              ((void* []){
                                  A2(&elm_html_Html_img,
                                      List_fromArray(1,
                                          ((void* []){
                                              A1(&author_project_Asset_src,
                                                  &author_project_Asset_error),
                                          })),
                                      &Nil),
                              }))),
                  }))),
          &literal_string_Page_20Not_20Found,
      }));
}

Custom author_project_Page_Profile_ClickedDismissErrors = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_ClickedDismissErrors,
};

void* eval_author_project_Page_Profile_ClickedFeedPage(void* args[]) {
  return ctorCustom(CTOR_ClickedFeedPage, 1, args);
}
Closure author_project_Page_Profile_ClickedFeedPage = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Profile_ClickedFeedPage,
};

void* eval_author_project_Page_Profile_ClickedFollow(void* args[]) {
  return ctorCustom(CTOR_ClickedFollow, 2, args);
}
Closure author_project_Page_Profile_ClickedFollow = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Profile_ClickedFollow,
};

void* eval_author_project_Page_Profile_ClickedUnfollow(void* args[]) {
  return ctorCustom(CTOR_ClickedUnfollow, 2, args);
}
Closure author_project_Page_Profile_ClickedUnfollow = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Profile_ClickedUnfollow,
};

void* eval_author_project_Profile_bio(void* args[]) {
  void* x__v0 = args[0];
  void* x_info = ((Custom*)x__v0)->values[0];
  return Utils_access_eval(((void* []){
      (void*)FIELD_bio,
      x_info,
  }));
}
Closure author_project_Profile_bio = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Profile_bio,
};

#define elm_html_Html_h4 (*ptr_elm_html_Html_h4)
ElmValue* ptr_elm_html_Html_h4;
void* init_elm_html_Html_h4() {
  return A1(&VirtualDom_node, &literal_string_h4);
}

#define author_project_Page_Profile_myProfileTitle literal_string_My_20Profile

#define author_project_Page_Profile_defaultTitle literal_string_Profile
void* eval_author_project_Page_Profile_titleForMe(void* args[]) {
  void* x_maybeCred = args[0];
  void* x_username = args[1];
  void* tmp0;
  void* tmp1 = ((Custom*)x_maybeCred)->ctor;
  if (tmp1 == CTOR_Just) {
    void* x_cred = Utils_destruct_index(x_maybeCred, 0);
    void* tmp2;
    if (A2(&elm_core_Basics_eq, x_username, A1(&author_project_Api_username, x_cred)) ==
        &True) {
      tmp2 = &author_project_Page_Profile_myProfileTitle;
    } else {
      tmp2 = &author_project_Page_Profile_defaultTitle;
    };
    tmp0 = tmp2;
  } else {
    tmp0 = &author_project_Page_Profile_defaultTitle;
  };
  return tmp0;
}
Closure author_project_Page_Profile_titleForMe = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Profile_titleForMe,
};

void* eval_author_project_Page_Profile_titleForOther(void* args[]) {
  void* x_otherUsername = args[0];
  return A2(&elm_core_Basics_append,
      &literal_string_Profile_20_2014_20,
      A1(&author_project_Username_toString, x_otherUsername));
}
Closure author_project_Page_Profile_titleForOther = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Profile_titleForOther,
};

void* eval_author_project_Page_Profile_ClickedTab(void* args[]) {
  return ctorCustom(CTOR_ClickedTab, 1, args);
}
Closure author_project_Page_Profile_ClickedTab = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Profile_ClickedTab,
};

Custom author_project_Page_Profile_FavoritedArticles = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_FavoritedArticles,
};
#define author_project_Page_Profile_favoritedArticles \
  (*ptr_author_project_Page_Profile_favoritedArticles)
Tuple2* ptr_author_project_Page_Profile_favoritedArticles;
void* init_author_project_Page_Profile_favoritedArticles() {
  return NEW_TUPLE2(&literal_string_Favorited_20Articles,
      A1(&author_project_Page_Profile_ClickedTab,
          &author_project_Page_Profile_FavoritedArticles));
}

#define author_project_Page_Profile_myArticles \
  (*ptr_author_project_Page_Profile_myArticles)
Tuple2* ptr_author_project_Page_Profile_myArticles;
void* init_author_project_Page_Profile_myArticles() {
  return NEW_TUPLE2(&literal_string_My_20Articles,
      A1(&author_project_Page_Profile_ClickedTab,
          &author_project_Page_Profile_MyArticles));
}
void* eval_author_project_Page_Profile_viewTabs(void* args[]) {
  void* x_tab = args[0];
  void* tmp0;
  void* tmp1 = ((Custom*)x_tab)->ctor;
  if (tmp1 == CTOR_MyArticles) {
    tmp0 = A3(&author_project_Article_Feed_viewTabs,
        &Nil,
        &author_project_Page_Profile_myArticles,
        List_fromArray(1,
            ((void* []){
                &author_project_Page_Profile_favoritedArticles,
            })));
  } else {
    tmp0 = A3(&author_project_Article_Feed_viewTabs,
        List_fromArray(1,
            ((void* []){
                &author_project_Page_Profile_myArticles,
            })),
        &author_project_Page_Profile_favoritedArticles,
        &Nil);
  };
  return tmp0;
}
Closure author_project_Page_Profile_viewTabs = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Profile_viewTabs,
};
void* eval_author_project_Page_Profile_view(void* args[]) {
  void* x_model = args[0];
  void* x__v4 = Utils_access_eval(((void* []){
      (void*)FIELD_author,
      x_model,
  }));
  void* tmp0;
  void* tmp1 = ((Custom*)x__v4)->ctor;
  if (tmp1 == CTOR_Loaded) {
    void* tmp2 = ((Custom*)Utils_destruct_index(x__v4, 0))->ctor;
    if (tmp2 == CTOR_IsViewer) {
      {
        void* x__v5 = Utils_destruct_index(x__v4, 0);
        tmp0 = &author_project_Page_Profile_myProfileTitle;
      };
    } else if (tmp2 == CTOR_IsFollowing) {
      {
        void* x_author = Utils_destruct_index(x__v4, 0);
        void* x_followedAuthor = Utils_destruct_index(x_author, 0);
        tmp0 = A1(&author_project_Page_Profile_titleForOther,
            A1(&author_project_Author_username, x_author));
      };
    } else {
      void* x_author = Utils_destruct_index(x__v4, 0);
      void* x_unfollowedAuthor = Utils_destruct_index(x_author, 0);
      tmp0 = A1(&author_project_Page_Profile_titleForOther,
          A1(&author_project_Author_username, x_author));
    };
  } else if (tmp1 == CTOR_Loading) {
    {
      void* x_username = Utils_destruct_index(x__v4, 0);
      tmp0 = A2(&author_project_Page_Profile_titleForMe,
          A1(&author_project_Session_cred,
              Utils_access_eval(((void* []){
                  (void*)FIELD_session,
                  x_model,
              }))),
          x_username);
    };
  } else if (tmp1 == CTOR_LoadingSlowly) {
    {
      void* x_username = Utils_destruct_index(x__v4, 0);
      tmp0 = A2(&author_project_Page_Profile_titleForMe,
          A1(&author_project_Session_cred,
              Utils_access_eval(((void* []){
                  (void*)FIELD_session,
                  x_model,
              }))),
          x_username);
    };
  } else {
    void* x_username = Utils_destruct_index(x__v4, 0);
    tmp0 = A2(&author_project_Page_Profile_titleForMe,
        A1(&author_project_Session_cred,
            Utils_access_eval(((void* []){
                (void*)FIELD_session,
                x_model,
            }))),
        x_username);
  };
  void* x_title = tmp0;
  void* x__v0 = Utils_access_eval(((void* []){
      (void*)FIELD_author,
      x_model,
  }));
  void* tmp3;
  void* tmp4 = ((Custom*)x__v0)->ctor;
  if (tmp4 == CTOR_Loaded) {
    {
      void* x_author = Utils_destruct_index(x__v0, 0);
      void* x_username = A1(&author_project_Author_username, x_author);
      void* x_profile = A1(&author_project_Author_profile, x_author);
      void* x__v2 = A1(&author_project_Session_cred,
          Utils_access_eval(((void* []){
              (void*)FIELD_session,
              x_model,
          })));
      void* tmp5;
      void* tmp6 = ((Custom*)x__v2)->ctor;
      if (tmp6 == CTOR_Just) {
        void* x_cred = Utils_destruct_index(x__v2, 0);
        void* tmp7;
        void* tmp8 = ((Custom*)x_author)->ctor;
        if (tmp8 == CTOR_IsViewer) {
          {
            tmp7 = A1(&elm_html_Html_text, &literal_string_);
          };
        } else if (tmp8 == CTOR_IsFollowing) {
          {
            void* x_followedAuthor = Utils_destruct_index(x_author, 0);
            tmp7 = A3(&author_project_Author_unfollowButton,
                &author_project_Page_Profile_ClickedUnfollow,
                x_cred,
                x_followedAuthor);
          };
        } else {
          void* x_unfollowedAuthor = Utils_destruct_index(x_author, 0);
          tmp7 = A3(&author_project_Author_followButton,
              &author_project_Page_Profile_ClickedFollow,
              x_cred,
              x_unfollowedAuthor);
        };
        tmp5 = tmp7;
      } else {
        tmp5 = A1(&elm_html_Html_text, &literal_string_);
      };
      void* x_followButton = tmp5;
      void* x__v1 = Utils_access_eval(((void* []){
          (void*)FIELD_feed,
          x_model,
      }));
      void* tmp9;
      void* tmp10 = ((Custom*)x__v1)->ctor;
      if (tmp10 == CTOR_Loaded) {
        {
          void* x_feed = Utils_destruct_index(x__v1, 0);
          tmp9 = A2(&elm_html_Html_div,
              List_fromArray(1,
                  ((void* []){
                      A1(&elm_html_Html_Attributes_class, &literal_string_container),
                  })),
              List_fromArray(1,
                  ((void* []){
                      A2(&elm_html_Html_div,
                          List_fromArray(1,
                              ((void* []){
                                  A1(&elm_html_Html_Attributes_class,
                                      &literal_string_row),
                              })),
                          List_fromArray(1,
                              ((void* []){
                                  A2(&elm_html_Html_div,
                                      List_fromArray(1,
                                          ((void* []){
                                              A1(&elm_html_Html_Attributes_class,
                                                  &literal_string_col_2dxs_2d12_20col_2dmd_2d10_20offset_2dmd_2d1),
                                          })),
                                      List_fromArray(1,
                                          ((void* []){
                                              A2(&elm_core_Basics_apL,
                                                  A1(&elm_html_Html_div,
                                                      List_fromArray(1,
                                                          ((void* []){
                                                              A1(&elm_html_Html_Attributes_class,
                                                                  &literal_string_articles_2dtoggle),
                                                          }))),
                                                  A1(&elm_core_List_concat,
                                                      List_fromArray(3,
                                                          ((void* []){
                                                              List_fromArray(1,
                                                                  ((void* []){
                                                                      A1(&author_project_Page_Profile_viewTabs,
                                                                          Utils_access_eval(((
                                                                              void* []){
                                                                              (void*)
                                                                                  FIELD_feedTab,
                                                                              x_model,
                                                                          }))),
                                                                  })),
                                                              A2(&elm_core_Basics_apR,
                                                                  A2(&author_project_Article_Feed_viewArticles,
                                                                      Utils_access_eval(((
                                                                          void* []){
                                                                          (void*)
                                                                              FIELD_timeZone,
                                                                          x_model,
                                                                      })),
                                                                      x_feed),
                                                                  A1(&elm_core_List_map,
                                                                      A1(&elm_html_Html_map,
                                                                          &author_project_Page_Profile_GotFeedMsg))),
                                                              List_fromArray(1,
                                                                  ((void* []){
                                                                      A3(&author_project_Article_Feed_viewPagination,
                                                                          &author_project_Page_Profile_ClickedFeedPage,
                                                                          Utils_access_eval(((
                                                                              void* []){
                                                                              (void*)
                                                                                  FIELD_feedPage,
                                                                              x_model,
                                                                          })),
                                                                          x_feed),
                                                                  })),
                                                          })))),
                                          }))),
                              }))),
                  })));
        };
      } else if (tmp10 == CTOR_Loading) {
        { tmp9 = A1(&elm_html_Html_text, &literal_string_); };
      } else if (tmp10 == CTOR_LoadingSlowly) {
        { tmp9 = &author_project_Loading_icon; };
      } else {
        tmp9 = A1(&author_project_Loading_error, &literal_string_feed);
      };
      tmp3 = A2(&elm_html_Html_div,
          List_fromArray(1,
              ((void* []){
                  A1(&elm_html_Html_Attributes_class, &literal_string_profile_2dpage),
              })),
          List_fromArray(3,
              ((void* []){
                  A2(&author_project_Page_viewErrors,
                      &author_project_Page_Profile_ClickedDismissErrors,
                      Utils_access_eval(((void* []){
                          (void*)FIELD_errors,
                          x_model,
                      }))),
                  A2(&elm_html_Html_div,
                      List_fromArray(1,
                          ((void* []){
                              A1(&elm_html_Html_Attributes_class,
                                  &literal_string_user_2dinfo),
                          })),
                      List_fromArray(1,
                          ((void* []){
                              A2(&elm_html_Html_div,
                                  List_fromArray(1,
                                      ((void* []){
                                          A1(&elm_html_Html_Attributes_class,
                                              &literal_string_container),
                                      })),
                                  List_fromArray(1,
                                      ((void* []){
                                          A2(&elm_html_Html_div,
                                              List_fromArray(1,
                                                  ((void* []){
                                                      A1(&elm_html_Html_Attributes_class,
                                                          &literal_string_row),
                                                  })),
                                              List_fromArray(1,
                                                  ((void* []){
                                                      A2(&elm_html_Html_div,
                                                          List_fromArray(1,
                                                              ((void* []){
                                                                  A1(&elm_html_Html_Attributes_class,
                                                                      &literal_string_col_2dxs_2d12_20col_2dmd_2d10_20offset_2dmd_2d1),
                                                              })),
                                                          List_fromArray(4,
                                                              ((void* []){
                                                                  A2(&elm_html_Html_img,
                                                                      List_fromArray(2,
                                                                          ((void* []){
                                                                              A1(&elm_html_Html_Attributes_class,
                                                                                  &literal_string_user_2dimg),
                                                                              A1(&author_project_Avatar_src,
                                                                                  A1(&author_project_Profile_avatar,
                                                                                      x_profile)),
                                                                          })),
                                                                      &Nil),
                                                                  A2(&elm_html_Html_h4,
                                                                      &Nil,
                                                                      List_fromArray(1,
                                                                          ((void* []){
                                                                              A1(&author_project_Username_toHtml,
                                                                                  x_username),
                                                                          }))),
                                                                  A2(&elm_html_Html_p,
                                                                      &Nil,
                                                                      List_fromArray(1,
                                                                          ((void* []){
                                                                              A1(&elm_html_Html_text,
                                                                                  A2(&elm_core_Maybe_withDefault,
                                                                                      &literal_string_,
                                                                                      A1(&author_project_Profile_bio,
                                                                                          x_profile))),
                                                                          }))),
                                                                  x_followButton,
                                                              }))),
                                                  }))),
                                      }))),
                          }))),
                  tmp9,
              })));
    };
  } else if (tmp4 == CTOR_Loading) {
    { tmp3 = A1(&elm_html_Html_text, &literal_string_); };
  } else if (tmp4 == CTOR_LoadingSlowly) {
    { tmp3 = &author_project_Loading_icon; };
  } else {
    tmp3 = A1(&author_project_Loading_error, &literal_string_profile);
  };
  return NEW_RECORD(&fg_content_title,
      2,
      ((void* []){
          tmp3,
          x_title,
      }));
}
Closure author_project_Page_Profile_view = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Profile_view,
};

void* eval_author_project_Page_Register_EnteredEmail(void* args[]) {
  return ctorCustom(CTOR_EnteredEmail, 1, args);
}
Closure author_project_Page_Register_EnteredEmail = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Register_EnteredEmail,
};

void* eval_author_project_Page_Register_EnteredPassword(void* args[]) {
  return ctorCustom(CTOR_EnteredPassword, 1, args);
}
Closure author_project_Page_Register_EnteredPassword = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Register_EnteredPassword,
};

void* eval_author_project_Page_Register_EnteredUsername(void* args[]) {
  return ctorCustom(CTOR_EnteredUsername, 1, args);
}
Closure author_project_Page_Register_EnteredUsername = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Register_EnteredUsername,
};

Custom author_project_Page_Register_SubmittedForm = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_SubmittedForm,
};
void* eval_author_project_Page_Register_viewForm(void* args[]) {
  void* x_form = args[0];
  return A2(&elm_html_Html_form,
      List_fromArray(1,
          ((void* []){
              A1(&elm_html_Html_Events_onSubmit,
                  &author_project_Page_Register_SubmittedForm),
          })),
      List_fromArray(4,
          ((void* []){
              A2(&elm_html_Html_fieldset,
                  List_fromArray(1,
                      ((void* []){
                          A1(&elm_html_Html_Attributes_class,
                              &literal_string_form_2dgroup),
                      })),
                  List_fromArray(1,
                      ((void* []){
                          A2(&elm_html_Html_input,
                              List_fromArray(4,
                                  ((void* []){
                                      A1(&elm_html_Html_Attributes_class,
                                          &literal_string_form_2dcontrol_20form_2dcontrol_2dlg),
                                      A1(&elm_html_Html_Attributes_placeholder,
                                          &literal_string_Username),
                                      A1(&elm_html_Html_Events_onInput,
                                          &author_project_Page_Register_EnteredUsername),
                                      A1(&elm_html_Html_Attributes_value,
                                          Utils_access_eval(((void* []){
                                              (void*)FIELD_username,
                                              x_form,
                                          }))),
                                  })),
                              &Nil),
                      }))),
              A2(&elm_html_Html_fieldset,
                  List_fromArray(1,
                      ((void* []){
                          A1(&elm_html_Html_Attributes_class,
                              &literal_string_form_2dgroup),
                      })),
                  List_fromArray(1,
                      ((void* []){
                          A2(&elm_html_Html_input,
                              List_fromArray(4,
                                  ((void* []){
                                      A1(&elm_html_Html_Attributes_class,
                                          &literal_string_form_2dcontrol_20form_2dcontrol_2dlg),
                                      A1(&elm_html_Html_Attributes_placeholder,
                                          &literal_string_Email),
                                      A1(&elm_html_Html_Events_onInput,
                                          &author_project_Page_Register_EnteredEmail),
                                      A1(&elm_html_Html_Attributes_value,
                                          Utils_access_eval(((void* []){
                                              (void*)FIELD_email,
                                              x_form,
                                          }))),
                                  })),
                              &Nil),
                      }))),
              A2(&elm_html_Html_fieldset,
                  List_fromArray(1,
                      ((void* []){
                          A1(&elm_html_Html_Attributes_class,
                              &literal_string_form_2dgroup),
                      })),
                  List_fromArray(1,
                      ((void* []){
                          A2(&elm_html_Html_input,
                              List_fromArray(5,
                                  ((void* []){
                                      A1(&elm_html_Html_Attributes_class,
                                          &literal_string_form_2dcontrol_20form_2dcontrol_2dlg),
                                      A1(&elm_html_Html_Attributes_type_,
                                          &literal_string_password),
                                      A1(&elm_html_Html_Attributes_placeholder,
                                          &literal_string_Password),
                                      A1(&elm_html_Html_Events_onInput,
                                          &author_project_Page_Register_EnteredPassword),
                                      A1(&elm_html_Html_Attributes_value,
                                          Utils_access_eval(((void* []){
                                              (void*)FIELD_password,
                                              x_form,
                                          }))),
                                  })),
                              &Nil),
                      }))),
              A2(&elm_html_Html_button,
                  List_fromArray(1,
                      ((void* []){
                          A1(&elm_html_Html_Attributes_class,
                              &literal_string_btn_20btn_2dlg_20btn_2dprimary_20pull_2dxs_2dright),
                      })),
                  List_fromArray(1,
                      ((void* []){
                          A1(&elm_html_Html_text, &literal_string_Sign_20up),
                      }))),
          })));
}
Closure author_project_Page_Register_viewForm = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Register_viewForm,
};

void* eval_author_project_Page_Register_viewProblem(void* args[]) {
  void* x_problem = args[0];
  void* tmp0;
  void* tmp1 = ((Custom*)x_problem)->ctor;
  if (tmp1 == CTOR_InvalidEntry) {
    void* x_str = Utils_destruct_index(x_problem, 1);
    tmp0 = x_str;
  } else {
    void* x_str = Utils_destruct_index(x_problem, 0);
    tmp0 = x_str;
  };
  void* x_errorMessage = tmp0;
  return A2(&elm_html_Html_li,
      &Nil,
      List_fromArray(1,
          ((void* []){
              A1(&elm_html_Html_text, x_errorMessage),
          })));
}
Closure author_project_Page_Register_viewProblem = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Register_viewProblem,
};
void* eval_author_project_Page_Register_view(void* args[]) {
  void* x_model = args[0];
  return NEW_RECORD(&fg_content_title,
      2,
      ((void* []){
          A2(&elm_html_Html_div,
              List_fromArray(1,
                  ((void* []){
                      A1(&elm_html_Html_Attributes_class, &literal_string_cred_2dpage),
                  })),
              List_fromArray(1,
                  ((void* []){
                      A2(&elm_html_Html_div,
                          List_fromArray(1,
                              ((void* []){
                                  A1(&elm_html_Html_Attributes_class,
                                      &literal_string_container_20page),
                              })),
                          List_fromArray(1,
                              ((void* []){
                                  A2(&elm_html_Html_div,
                                      List_fromArray(1,
                                          ((void* []){
                                              A1(&elm_html_Html_Attributes_class,
                                                  &literal_string_row),
                                          })),
                                      List_fromArray(1,
                                          ((void* []){
                                              A2(&elm_html_Html_div,
                                                  List_fromArray(1,
                                                      ((void* []){
                                                          A1(&elm_html_Html_Attributes_class,
                                                              &literal_string_col_2dmd_2d6_20offset_2dmd_2d3_20col_2dxs_2d12),
                                                      })),
                                                  List_fromArray(4,
                                                      ((void* []){
                                                          A2(&elm_html_Html_h1,
                                                              List_fromArray(1,
                                                                  ((void* []){
                                                                      A1(&elm_html_Html_Attributes_class,
                                                                          &literal_string_text_2dxs_2dcenter),
                                                                  })),
                                                              List_fromArray(1,
                                                                  ((void* []){
                                                                      A1(&elm_html_Html_text,
                                                                          &literal_string_Sign_20up),
                                                                  }))),
                                                          A2(&elm_html_Html_p,
                                                              List_fromArray(1,
                                                                  ((void* []){
                                                                      A1(&elm_html_Html_Attributes_class,
                                                                          &literal_string_text_2dxs_2dcenter),
                                                                  })),
                                                              List_fromArray(1,
                                                                  ((void* []){
                                                                      A2(&elm_html_Html_a,
                                                                          List_fromArray(
                                                                              1,
                                                                              ((void* []){
                                                                                  A1(&author_project_Route_href,
                                                                                      &author_project_Route_Login),
                                                                              })),
                                                                          List_fromArray(
                                                                              1,
                                                                              ((void* []){
                                                                                  A1(&elm_html_Html_text,
                                                                                      &literal_string_Have_20an_20account_3f),
                                                                              }))),
                                                                  }))),
                                                          A2(&elm_html_Html_ul,
                                                              List_fromArray(1,
                                                                  ((void* []){
                                                                      A1(&elm_html_Html_Attributes_class,
                                                                          &literal_string_error_2dmessages),
                                                                  })),
                                                              A2(&elm_core_List_map,
                                                                  &author_project_Page_Register_viewProblem,
                                                                  Utils_access_eval(((
                                                                      void* []){
                                                                      (void*)
                                                                          FIELD_problems,
                                                                      x_model,
                                                                  })))),
                                                          A1(&author_project_Page_Register_viewForm,
                                                              Utils_access_eval(
                                                                  ((void* []){
                                                                      (void*)FIELD_form,
                                                                      x_model,
                                                                  }))),
                                                      }))),
                                          }))),
                              }))),
                  }))),
          &literal_string_Register,
      }));
}
Closure author_project_Page_Register_view = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Register_view,
};

void* eval_author_project_Page_Settings_EnteredAvatar(void* args[]) {
  return ctorCustom(CTOR_EnteredAvatar, 1, args);
}
Closure author_project_Page_Settings_EnteredAvatar = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Settings_EnteredAvatar,
};

void* eval_author_project_Page_Settings_EnteredBio(void* args[]) {
  return ctorCustom(CTOR_EnteredBio, 1, args);
}
Closure author_project_Page_Settings_EnteredBio = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Settings_EnteredBio,
};

void* eval_author_project_Page_Settings_EnteredEmail(void* args[]) {
  return ctorCustom(CTOR_EnteredEmail, 1, args);
}
Closure author_project_Page_Settings_EnteredEmail = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Settings_EnteredEmail,
};

void* eval_author_project_Page_Settings_EnteredPassword(void* args[]) {
  return ctorCustom(CTOR_EnteredPassword, 1, args);
}
Closure author_project_Page_Settings_EnteredPassword = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Settings_EnteredPassword,
};

void* eval_author_project_Page_Settings_EnteredUsername(void* args[]) {
  return ctorCustom(CTOR_EnteredUsername, 1, args);
}
Closure author_project_Page_Settings_EnteredUsername = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Settings_EnteredUsername,
};

void* eval_author_project_Page_Settings_SubmittedForm(void* args[]) {
  return ctorCustom(CTOR_SubmittedForm, 2, args);
}
Closure author_project_Page_Settings_SubmittedForm = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Settings_SubmittedForm,
};
void* eval_author_project_Page_Settings_viewForm(void* args[]) {
  void* x_cred = args[0];
  void* x_form = args[1];
  return A2(&elm_html_Html_form,
      List_fromArray(1,
          ((void* []){
              A1(&elm_html_Html_Events_onSubmit,
                  A2(&author_project_Page_Settings_SubmittedForm, x_cred, x_form)),
          })),
      List_fromArray(1,
          ((void* []){
              A2(&elm_html_Html_fieldset,
                  &Nil,
                  List_fromArray(6,
                      ((void* []){
                          A2(&elm_html_Html_fieldset,
                              List_fromArray(1,
                                  ((void* []){
                                      A1(&elm_html_Html_Attributes_class,
                                          &literal_string_form_2dgroup),
                                  })),
                              List_fromArray(1,
                                  ((void* []){
                                      A2(&elm_html_Html_input,
                                          List_fromArray(4,
                                              ((void* []){
                                                  A1(&elm_html_Html_Attributes_class,
                                                      &literal_string_form_2dcontrol),
                                                  A1(&elm_html_Html_Attributes_placeholder,
                                                      &literal_string_URL_20of_20profile_20picture),
                                                  A1(&elm_html_Html_Attributes_value,
                                                      Utils_access_eval(((void* []){
                                                          (void*)FIELD_avatar,
                                                          x_form,
                                                      }))),
                                                  A1(&elm_html_Html_Events_onInput,
                                                      &author_project_Page_Settings_EnteredAvatar),
                                              })),
                                          &Nil),
                                  }))),
                          A2(&elm_html_Html_fieldset,
                              List_fromArray(1,
                                  ((void* []){
                                      A1(&elm_html_Html_Attributes_class,
                                          &literal_string_form_2dgroup),
                                  })),
                              List_fromArray(1,
                                  ((void* []){
                                      A2(&elm_html_Html_input,
                                          List_fromArray(4,
                                              ((void* []){
                                                  A1(&elm_html_Html_Attributes_class,
                                                      &literal_string_form_2dcontrol_20form_2dcontrol_2dlg),
                                                  A1(&elm_html_Html_Attributes_placeholder,
                                                      &literal_string_Username),
                                                  A1(&elm_html_Html_Attributes_value,
                                                      Utils_access_eval(((void* []){
                                                          (void*)FIELD_username,
                                                          x_form,
                                                      }))),
                                                  A1(&elm_html_Html_Events_onInput,
                                                      &author_project_Page_Settings_EnteredUsername),
                                              })),
                                          &Nil),
                                  }))),
                          A2(&elm_html_Html_fieldset,
                              List_fromArray(1,
                                  ((void* []){
                                      A1(&elm_html_Html_Attributes_class,
                                          &literal_string_form_2dgroup),
                                  })),
                              List_fromArray(1,
                                  ((void* []){
                                      A2(&elm_html_Html_textarea,
                                          List_fromArray(5,
                                              ((void* []){
                                                  A1(&elm_html_Html_Attributes_class,
                                                      &literal_string_form_2dcontrol_20form_2dcontrol_2dlg),
                                                  A1(&elm_html_Html_Attributes_placeholder,
                                                      &literal_string_Short_20bio_20about_20you),
                                                  A2(&elm_html_Html_Attributes_attribute,
                                                      &literal_string_rows,
                                                      &literal_string_8),
                                                  A1(&elm_html_Html_Attributes_value,
                                                      Utils_access_eval(((void* []){
                                                          (void*)FIELD_bio,
                                                          x_form,
                                                      }))),
                                                  A1(&elm_html_Html_Events_onInput,
                                                      &author_project_Page_Settings_EnteredBio),
                                              })),
                                          &Nil),
                                  }))),
                          A2(&elm_html_Html_fieldset,
                              List_fromArray(1,
                                  ((void* []){
                                      A1(&elm_html_Html_Attributes_class,
                                          &literal_string_form_2dgroup),
                                  })),
                              List_fromArray(1,
                                  ((void* []){
                                      A2(&elm_html_Html_input,
                                          List_fromArray(4,
                                              ((void* []){
                                                  A1(&elm_html_Html_Attributes_class,
                                                      &literal_string_form_2dcontrol_20form_2dcontrol_2dlg),
                                                  A1(&elm_html_Html_Attributes_placeholder,
                                                      &literal_string_Email),
                                                  A1(&elm_html_Html_Attributes_value,
                                                      Utils_access_eval(((void* []){
                                                          (void*)FIELD_email,
                                                          x_form,
                                                      }))),
                                                  A1(&elm_html_Html_Events_onInput,
                                                      &author_project_Page_Settings_EnteredEmail),
                                              })),
                                          &Nil),
                                  }))),
                          A2(&elm_html_Html_fieldset,
                              List_fromArray(1,
                                  ((void* []){
                                      A1(&elm_html_Html_Attributes_class,
                                          &literal_string_form_2dgroup),
                                  })),
                              List_fromArray(1,
                                  ((void* []){
                                      A2(&elm_html_Html_input,
                                          List_fromArray(5,
                                              ((void* []){
                                                  A1(&elm_html_Html_Attributes_class,
                                                      &literal_string_form_2dcontrol_20form_2dcontrol_2dlg),
                                                  A1(&elm_html_Html_Attributes_type_,
                                                      &literal_string_password),
                                                  A1(&elm_html_Html_Attributes_placeholder,
                                                      &literal_string_Password),
                                                  A1(&elm_html_Html_Attributes_value,
                                                      Utils_access_eval(((void* []){
                                                          (void*)FIELD_password,
                                                          x_form,
                                                      }))),
                                                  A1(&elm_html_Html_Events_onInput,
                                                      &author_project_Page_Settings_EnteredPassword),
                                              })),
                                          &Nil),
                                  }))),
                          A2(&elm_html_Html_button,
                              List_fromArray(1,
                                  ((void* []){
                                      A1(&elm_html_Html_Attributes_class,
                                          &literal_string_btn_20btn_2dlg_20btn_2dprimary_20pull_2dxs_2dright),
                                  })),
                              List_fromArray(1,
                                  ((void* []){
                                      A1(&elm_html_Html_text,
                                          &literal_string_Update_20Settings),
                                  }))),
                      }))),
          })));
}
Closure author_project_Page_Settings_viewForm = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Page_Settings_viewForm,
};

void* eval_author_project_Page_Settings_viewProblem(void* args[]) {
  void* x_problem = args[0];
  void* tmp0;
  void* tmp1 = ((Custom*)x_problem)->ctor;
  if (tmp1 == CTOR_InvalidEntry) {
    void* x_message = Utils_destruct_index(x_problem, 1);
    tmp0 = x_message;
  } else {
    void* x_message = Utils_destruct_index(x_problem, 0);
    tmp0 = x_message;
  };
  void* x_errorMessage = tmp0;
  return A2(&elm_html_Html_li,
      &Nil,
      List_fromArray(1,
          ((void* []){
              A1(&elm_html_Html_text, x_errorMessage),
          })));
}
Closure author_project_Page_Settings_viewProblem = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Settings_viewProblem,
};
void* eval_author_project_Page_Settings_view(void* args[]) {
  void* x_model = args[0];
  void* x__v0 = A1(&author_project_Session_cred,
      Utils_access_eval(((void* []){
          (void*)FIELD_session,
          x_model,
      })));
  void* tmp0;
  void* tmp1 = ((Custom*)x__v0)->ctor;
  if (tmp1 == CTOR_Just) {
    void* x_cred = Utils_destruct_index(x__v0, 0);
    void* x__v1 = Utils_access_eval(((void* []){
        (void*)FIELD_status,
        x_model,
    }));
    void* tmp2;
    void* tmp3 = ((Custom*)x__v1)->ctor;
    if (tmp3 == CTOR_Loaded) {
      {
        void* x_form = Utils_destruct_index(x__v1, 0);
        tmp2 = A2(&author_project_Page_Settings_viewForm, x_cred, x_form);
      };
    } else if (tmp3 == CTOR_Loading) {
      { tmp2 = A1(&elm_html_Html_text, &literal_string_); };
    } else if (tmp3 == CTOR_LoadingSlowly) {
      { tmp2 = &author_project_Loading_icon; };
    } else {
      tmp2 = A1(&elm_html_Html_text, &literal_string_Error_20loading_20page_2e);
    };
    tmp0 = A2(&elm_html_Html_div,
        List_fromArray(1,
            ((void* []){
                A1(&elm_html_Html_Attributes_class, &literal_string_settings_2dpage),
            })),
        List_fromArray(1,
            ((void* []){
                A2(&elm_html_Html_div,
                    List_fromArray(1,
                        ((void* []){
                            A1(&elm_html_Html_Attributes_class,
                                &literal_string_container_20page),
                        })),
                    List_fromArray(1,
                        ((void* []){
                            A2(&elm_html_Html_div,
                                List_fromArray(1,
                                    ((void* []){
                                        A1(&elm_html_Html_Attributes_class,
                                            &literal_string_row),
                                    })),
                                List_fromArray(1,
                                    ((void* []){
                                        A2(&elm_core_Basics_apL,
                                            A1(&elm_html_Html_div,
                                                List_fromArray(1,
                                                    ((void* []){
                                                        A1(&elm_html_Html_Attributes_class,
                                                            &literal_string_col_2dmd_2d6_20offset_2dmd_2d3_20col_2dxs_2d12),
                                                    }))),
                                            List_fromArray(3,
                                                ((void* []){
                                                    A2(&elm_html_Html_h1,
                                                        List_fromArray(1,
                                                            ((void* []){
                                                                A1(&elm_html_Html_Attributes_class,
                                                                    &literal_string_text_2dxs_2dcenter),
                                                            })),
                                                        List_fromArray(1,
                                                            ((void* []){
                                                                A1(&elm_html_Html_text,
                                                                    &literal_string_Your_20Settings),
                                                            }))),
                                                    A2(&elm_html_Html_ul,
                                                        List_fromArray(1,
                                                            ((void* []){
                                                                A1(&elm_html_Html_Attributes_class,
                                                                    &literal_string_error_2dmessages),
                                                            })),
                                                        A2(&elm_core_List_map,
                                                            &author_project_Page_Settings_viewProblem,
                                                            Utils_access_eval(((void* []){
                                                                (void*)FIELD_problems,
                                                                x_model,
                                                            })))),
                                                    tmp2,
                                                }))),
                                    }))),
                        }))),
            })));
  } else {
    tmp0 = A1(
        &elm_html_Html_text, &literal_string_Sign_20in_20to_20view_20your_20settings_2e);
  };
  return NEW_RECORD(&fg_content_title,
      2,
      ((void* []){
          tmp0,
          &literal_string_Settings,
      }));
}
Closure author_project_Page_Settings_view = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Page_Settings_view,
};
void* eval_author_project_Main_view_lambda0(void* args[]) {
  void* x_viewer = args[0];
  void* x_page = args[1];
  void* x_toMsg = args[2];
  void* x_config = args[3];
  void* x__v2 = A3(&author_project_Page_view, x_viewer, x_page, x_config);
  void* x_title = Utils_access_eval(((void* []){
      (void*)FIELD_title,
      x__v2,
  }));
  void* x_body = Utils_access_eval(((void* []){
      (void*)FIELD_body,
      x__v2,
  }));
  return NEW_RECORD(&fg_body_title,
      2,
      ((void* []){
          A2(&elm_core_List_map, A1(&elm_html_Html_map, x_toMsg), x_body),
          x_title,
      }));
}
void* eval_author_project_Main_view(void* args[]) {
  void* x_model = args[0];
  void* x_viewer =
      A1(&author_project_Session_viewer, A1(&author_project_Main_toSession, x_model));
  void* x_viewPage = NEW_CLOSURE(1,
      4,
      &eval_author_project_Main_view_lambda0,
      ((void* []){
          x_viewer,
      }));
  void* tmp1;
  void* tmp2 = ((Custom*)x_model)->ctor;
  void* tmp3 = ((Custom*)Utils_destruct_index(x_model, 0))->ctor;
  if (tmp2 == CTOR_Redirect) {
    {
      tmp1 = A3(&author_project_Page_view,
          x_viewer,
          &author_project_Page_Other,
          &author_project_Page_Blank_view);
    };
  } else if (tmp2 == CTOR_NotFound) {
    {
      tmp1 = A3(&author_project_Page_view,
          x_viewer,
          &author_project_Page_Other,
          &author_project_Page_NotFound_view);
    };
  } else if (tmp2 == CTOR_Settings) {
    {
      void* x_settings = Utils_destruct_index(x_model, 0);
      tmp1 = A3(x_viewPage,
          &author_project_Page_Other,
          &author_project_Main_GotSettingsMsg,
          A1(&author_project_Page_Settings_view, x_settings));
    };
  } else if (tmp2 == CTOR_Home) {
    {
      void* x_home = Utils_destruct_index(x_model, 0);
      tmp1 = A3(x_viewPage,
          &author_project_Page_Home,
          &author_project_Main_GotHomeMsg,
          A1(&author_project_Page_Home_view, x_home));
    };
  } else if (tmp2 == CTOR_Login) {
    {
      void* x_login = Utils_destruct_index(x_model, 0);
      tmp1 = A3(x_viewPage,
          &author_project_Page_Other,
          &author_project_Main_GotLoginMsg,
          A1(&author_project_Page_Login_view, x_login));
    };
  } else if (tmp2 == CTOR_Register) {
    {
      void* x_register = Utils_destruct_index(x_model, 0);
      tmp1 = A3(x_viewPage,
          &author_project_Page_Other,
          &author_project_Main_GotRegisterMsg,
          A1(&author_project_Page_Register_view, x_register));
    };
  } else if (tmp2 == CTOR_Profile) {
    {
      void* x_username = Utils_destruct_index(x_model, 0);
      void* x_profile = Utils_destruct_index(x_model, 1);
      tmp1 = A3(x_viewPage,
          A1(&author_project_Page_Profile, x_username),
          &author_project_Main_GotProfileMsg,
          A1(&author_project_Page_Profile_view, x_profile));
    };
  } else if (tmp2 == CTOR_Article) {
    {
      void* x_article = Utils_destruct_index(x_model, 0);
      tmp1 = A3(x_viewPage,
          &author_project_Page_Other,
          &author_project_Main_GotArticleMsg,
          A1(&author_project_Page_Article_view, x_article));
    };
  } else if (tmp3 == CTOR_Nothing) {
    void* x__v1 = Utils_destruct_index(x_model, 0);
    void* x_editor = Utils_destruct_index(x_model, 1);
    tmp1 = A3(x_viewPage,
        &author_project_Page_NewArticle,
        &author_project_Main_GotEditorMsg,
        A1(&author_project_Page_Article_Editor_view, x_editor));
  } else {
    void* x_editor = Utils_destruct_index(x_model, 1);
    tmp1 = A3(x_viewPage,
        &author_project_Page_Other,
        &author_project_Main_GotEditorMsg,
        A1(&author_project_Page_Article_Editor_view, x_editor));
  };
  return tmp1;
}
Closure author_project_Main_view = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Main_view,
};
#define author_project_Main_main (*ptr_author_project_Main_main)
ElmValue* ptr_author_project_Main_main;
void* init_author_project_Main_main() {
  return A2(&author_project_Api_application,
      &author_project_Viewer_decoder,
      NEW_RECORD(&fg_init_onUrlChange_onUrlRequest_subscriptions_update_view,
          6,
          ((void* []){
              &author_project_Main_init,
              &author_project_Main_ChangedUrl,
              &author_project_Main_ClickedLink,
              &author_project_Main_subscriptions,
              &author_project_Main_update,
              &author_project_Main_view,
          })));
}

void** mains[] = {
    &ptr_author_project_Main_main,
    NULL,
};

int EMSCRIPTEN_KEEPALIVE main() {
  int exit_code = GC_init();
  if (exit_code) return exit_code;
  Utils_initGlobal(
      &ptr_author_project_Username_decoder, &init_author_project_Username_decoder);
  Utils_initGlobal(&ptr_NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_custom,
      &init_NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_custom);
  Utils_initGlobal(
      &ptr_author_project_Api_credDecoder, &init_author_project_Api_credDecoder);
  Utils_initGlobal(
      &ptr_author_project_Avatar_decoder, &init_author_project_Avatar_decoder);
  Utils_initGlobal(
      &ptr_author_project_Viewer_decoder, &init_author_project_Viewer_decoder);
  Utils_initGlobal(&ptr_author_project_Article_Body_decoder,
      &init_author_project_Article_Body_decoder);
  Utils_initGlobal(&ptr_author_project_Article_Slug_decoder,
      &init_author_project_Article_Slug_decoder);
  Utils_initGlobal(
      &ptr_author_project_Profile_decoder, &init_author_project_Profile_decoder);
  Utils_initGlobal(&ptr_elm_parser_Parser_end, &init_elm_parser_Parser_end);
  Utils_initGlobal(&ptr_rtfeldman_elm_iso8601_date_strings_Iso8601_monthYearDayInMs,
      &init_rtfeldman_elm_iso8601_date_strings_Iso8601_monthYearDayInMs);
  Utils_initGlobal(&ptr_rtfeldman_elm_iso8601_date_strings_Iso8601_iso8601,
      &init_rtfeldman_elm_iso8601_date_strings_Iso8601_iso8601);
  Utils_initGlobal(&ptr_rtfeldman_elm_iso8601_date_strings_Iso8601_decoder,
      &init_rtfeldman_elm_iso8601_date_strings_Iso8601_decoder);
  Utils_initGlobal(&ptr_author_project_Article_metadataDecoder,
      &init_author_project_Article_metadataDecoder);
  Utils_initGlobal(&ptr_elm_time_Time_here, &init_elm_time_Time_here);
  Utils_initGlobal(
      &ptr_author_project_CommentId_decoder, &init_author_project_CommentId_decoder);
  Utils_initGlobal(&ptr_author_project_Loading_slowThreshold,
      &init_author_project_Loading_slowThreshold);
  Utils_initGlobal(&ptr_elm_time_Time_utc, &init_elm_time_Time_utc);
  Utils_initGlobal(&ptr_NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_hardcoded,
      &init_NoRedInk_elm_json_decode_pipeline_Json_Decode_Pipeline_hardcoded);
  Utils_initGlobal(
      &ptr_author_project_Article_Tag_decoder, &init_author_project_Article_Tag_decoder);
  Utils_initGlobal(
      &ptr_author_project_Api_Endpoint_tags, &init_author_project_Api_Endpoint_tags);
  Utils_initGlobal(
      &ptr_author_project_Article_Tag_list, &init_author_project_Article_Tag_list);
  Utils_initGlobal(&ptr_elm_core_Platform_Cmd_none, &init_elm_core_Platform_Cmd_none);
  Utils_initGlobal(&ptr_author_project_Page_Settings_formDecoder,
      &init_author_project_Page_Settings_formDecoder);
  Utils_initGlobal(
      &ptr_author_project_Api_Endpoint_user, &init_author_project_Api_Endpoint_user);
  Utils_initGlobal(
      &ptr_author_project_Api_storeCache, &init_author_project_Api_storeCache);
  Utils_initGlobal(&ptr_author_project_Api_logout, &init_author_project_Api_logout);
  Utils_initGlobal(&ptr_elm_url_Url_Parser_top, &init_elm_url_Url_Parser_top);
  Utils_initGlobal(&ptr_author_project_Article_Slug_urlParser,
      &init_author_project_Article_Slug_urlParser);
  Utils_initGlobal(
      &ptr_author_project_Username_urlParser, &init_author_project_Username_urlParser);
  Utils_initGlobal(&ptr_author_project_Route_parser, &init_author_project_Route_parser);
  Utils_initGlobal(
      &ptr_author_project_Api_onStoreChange, &init_author_project_Api_onStoreChange);
  Utils_initGlobal(&ptr_elm_core_Platform_Sub_none, &init_elm_core_Platform_Sub_none);
  Utils_initGlobal(&ptr_author_project_Page_Article_Editor_fieldsToValidate,
      &init_author_project_Page_Article_Editor_fieldsToValidate);
  Utils_initGlobal(&ptr_author_project_Page_Home_scrollToTop,
      &init_author_project_Page_Home_scrollToTop);
  Utils_initGlobal(
      &ptr_author_project_Api_errorsDecoder, &init_author_project_Api_errorsDecoder);
  Utils_initGlobal(
      &ptr_author_project_Api_Endpoint_login, &init_author_project_Api_Endpoint_login);
  Utils_initGlobal(&ptr_author_project_Page_Login_fieldsToValidate,
      &init_author_project_Page_Login_fieldsToValidate);
  Utils_initGlobal(
      &ptr_author_project_Api_Endpoint_users, &init_author_project_Api_Endpoint_users);
  Utils_initGlobal(&ptr_author_project_Page_Register_fieldsToValidate,
      &init_author_project_Page_Register_fieldsToValidate);
  Utils_initGlobal(&ptr_author_project_Page_Settings_fieldsToValidate,
      &init_author_project_Page_Settings_fieldsToValidate);
  Utils_initGlobal(&ptr_elm_html_Html_a, &init_elm_html_Html_a);
  Utils_initGlobal(
      &ptr_elm_html_Html_Attributes_class, &init_elm_html_Html_Attributes_class);
  Utils_initGlobal(&ptr_elm_html_Html_div, &init_elm_html_Html_div);
  Utils_initGlobal(&ptr_elm_html_Html_footer, &init_elm_html_Html_footer);
  Utils_initGlobal(&ptr_elm_html_Html_span, &init_elm_html_Html_span);
  Utils_initGlobal(
      &ptr_author_project_Page_viewFooter, &init_author_project_Page_viewFooter);
  Utils_initGlobal(&ptr_elm_html_Html_nav, &init_elm_html_Html_nav);
  Utils_initGlobal(&ptr_elm_html_Html_li, &init_elm_html_Html_li);
  Utils_initGlobal(&ptr_elm_html_Html_ul, &init_elm_html_Html_ul);
  Utils_initGlobal(&ptr_elm_html_Html_i, &init_elm_html_Html_i);
  Utils_initGlobal(&ptr_elm_html_Html_img, &init_elm_html_Html_img);
  Utils_initGlobal(
      &ptr_author_project_Asset_defaultAvatar, &init_author_project_Asset_defaultAvatar);
  Utils_initGlobal(&ptr_elm_html_Html_h1, &init_elm_html_Html_h1);
  Utils_initGlobal(&ptr_elm_html_Html_hr, &init_elm_html_Html_hr);
  Utils_initGlobal(&ptr_elm_html_Html_Attributes_alt, &init_elm_html_Html_Attributes_alt);
  Utils_initGlobal(&ptr_author_project_Asset_loading, &init_author_project_Asset_loading);
  Utils_initGlobal(&ptr_author_project_Loading_icon, &init_author_project_Loading_icon);
  Utils_initGlobal(&ptr_elm_explorations_markdown_Markdown_defaultOptions,
      &init_elm_explorations_markdown_Markdown_defaultOptions);
  Utils_initGlobal(&ptr_elm_explorations_markdown_Markdown_toHtml,
      &init_elm_explorations_markdown_Markdown_toHtml);
  Utils_initGlobal(&ptr_elm_html_Html_button, &init_elm_html_Html_button);
  Utils_initGlobal(
      &ptr_elm_html_Html_Attributes_disabled, &init_elm_html_Html_Attributes_disabled);
  Utils_initGlobal(&ptr_elm_html_Html_form, &init_elm_html_Html_form);
  Utils_initGlobal(
      &ptr_elm_html_Html_Events_targetValue, &init_elm_html_Html_Events_targetValue);
  Utils_initGlobal(&ptr_elm_html_Html_p, &init_elm_html_Html_p);
  Utils_initGlobal(&ptr_elm_html_Html_Attributes_placeholder,
      &init_elm_html_Html_Attributes_placeholder);
  Utils_initGlobal(&ptr_elm_html_Html_textarea, &init_elm_html_Html_textarea);
  Utils_initGlobal(
      &ptr_elm_html_Html_Attributes_value, &init_elm_html_Html_Attributes_value);
  Utils_initGlobal(&ptr_elm_html_Html_fieldset, &init_elm_html_Html_fieldset);
  Utils_initGlobal(&ptr_elm_html_Html_input, &init_elm_html_Html_input);
  Utils_initGlobal(
      &ptr_author_project_Page_Blank_view, &init_author_project_Page_Blank_view);
  Utils_initGlobal(&ptr_author_project_Page_Home_viewBanner,
      &init_author_project_Page_Home_viewBanner);
  Utils_initGlobal(&ptr_author_project_Page_Home_globalFeed,
      &init_author_project_Page_Home_globalFeed);
  Utils_initGlobal(
      &ptr_elm_html_Html_Attributes_type_, &init_elm_html_Html_Attributes_type_);
  Utils_initGlobal(&ptr_author_project_Asset_error, &init_author_project_Asset_error);
  Utils_initGlobal(&ptr_elm_html_Html_Attributes_id, &init_elm_html_Html_Attributes_id);
  Utils_initGlobal(&ptr_elm_html_Html_main_, &init_elm_html_Html_main_);
  Utils_initGlobal(
      &ptr_author_project_Page_NotFound_view, &init_author_project_Page_NotFound_view);
  Utils_initGlobal(&ptr_elm_html_Html_h4, &init_elm_html_Html_h4);
  Utils_initGlobal(&ptr_author_project_Page_Profile_favoritedArticles,
      &init_author_project_Page_Profile_favoritedArticles);
  Utils_initGlobal(&ptr_author_project_Page_Profile_myArticles,
      &init_author_project_Page_Profile_myArticles);
  Utils_initGlobal(&ptr_author_project_Main_main, &init_author_project_Main_main);
  Wrapper_registerFieldGroups(app_field_groups);
  Wrapper_registerMains(mains);
  return 0;
}
