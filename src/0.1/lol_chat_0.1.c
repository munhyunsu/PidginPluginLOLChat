//#define _DEBUG // Untag this line, if you want debug this plugin.
/*
 * ====================================================================================
 *
 *       Filename:  lol_chat.c
 *
 *    Description:  Pidgin plugin for PVP.net (League of Legends Chat)
 *
 *        Version:  0.1
 *        Created:  04/14/2013 10:02:25 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Mun Hyun Su (LunaticHarmony), munhyunsu@gmail.com, http://www.facebook.com/munhyunsu
 *   Organization:  C. N. U. Computer Science
 *
 * =====================================================================================
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

/* including glib.h for gboolean type */
#include <glib.h>

/* preveting compiler errors */
#ifndef G_GNUC_NULL_TERMINATED
# if __GNUC__ >= 4
#  define G_GNUC_NULL_TERMINATED __attribute__((__sentinel__))
# else
#  define G_GNUC_NULL_TERMINATED
# endif
#endif

/* required definition of PURPLE_PLUGINS for pidgin plugin*/
#ifndef PURPLE_PLUGINS
# define PURPLE_PLUGINS
#endif

/* define User-Macro */
#define LC_PLUGIN_ID "core-luha-lol_chat"
#define LC_PLUGIN_NAME "League of Legends Chat"
#define LC_PLUGIN_VERSION "0.1"
#define LC_PLUGIN_SUMMARY "League of Legends Chat Plugin"
#define LC_PLUGIN_DESCRIPTION N_("League of Legends Chatting Plugin.\nThis plugin makes User infomation more comport.")
#define LC_PLUGIN_AUTHOR "LunaticHarmony <munhyunsu@gmail.com>"
#define LC_PLUGIN_HOMEPAGE "http://www.facebook.com/munhyunsu"

#ifdef _DEBUG
# define LC_PLUGIN_DEBUG_PREFIX "Debug : "
#endif

/* including needed header files */
#include "internal.h" // string type definition
#include "notify.h" // notify message form
#include "plugin.h" // required header file
#include "version.h" // version managing
#include "blist.h" // buddy list managing
#include "xmlnode.h" // xml node managing
#include "gtkblist.h" // gtk buddy list managing(tooltip draw)

/* debug mode */
#ifdef _DEBUG
# include "debug.h"
#endif

/* type definition */

/* Grobal Variable */
PurplePlugin *lc_plugin = NULL; // main function pointer

/* Signal Handler */
/*
static gboolean jabber_presence_received(PurpleConnection *pc, const char *type, const char *from, xmlnode *presence)
{
	const char *p_from, *p_to;
	xmlnode *cursor = presence;
	PurpleBuddy *buddy;
	PurpleAccount *my_account, *b_account;

	//data = xmlnode_get_data(presence);
	//presence-NULL
	//cursor = presence->child; //to-ID
	p_from = presence->child->data; // from
	//p_to = presence->child->next->data; // to
	//cursor = cursor->next; //from-ID
	//cursor = cursor->next; //show-NULL
	//cursor = cursor->child; //contents
	//cursor = cursor->next; //status-NULL
	//cursor = cursor->child; //
	//cursor = cursor->next; //pariority
	//data = xmlnode_get_prefix(cursor);
	//data = xmlnode_get_data_unescaped(cursor);

	my_account = purple_connection_get_account(pc); // my account
	buddy = purple_find_buddy(my_account, from); // presence buddy
	if(buddy)
	{
		//b_account = purple_buddy_get_account(buddy); // buddy account
		b_account = buddy->account;
	}
	
	return FALSE;
}
*/

static void gtk_buddy_tooltip(PurpleBlistNode *node, GString *text, gboolean full)
{


	gchar *temp = g_strdup(text->str);
	gchar *t_head, *t_tail;
	gchar *b_account = NULL, *b_status = NULL, *b_profileicon = NULL, *b_level = NULL, *b_wins = NULL, *b_leaves = NULL, *b_odinwins = NULL, *b_odinleaves = NULL, *b_queuetype = NULL, *b_rankedwins = NULL, *b_rankedlosses = NULL, *b_rankedrating = NULL, *b_tier = NULL, *b_statusmsg = NULL, *b_skinname = NULL, *b_gamequeuetype = NULL, *b_isobservable = NULL, *b_gamestatus = NULL, *b_timestamp = NULL, *b_subscription = NULL;

	//time_t raw_time;

	if(PURPLE_BLIST_NODE_IS_BUDDY(node))
	{
		PurpleBuddy *b = (PurpleBuddy *)node;
		
		if(strstr(b->name, "pvp.net"))
		{

			//Accont: xxx@pvp.net
			//b_account = g_strndup(b->name, strlen(b->name));
			b_account = g_strdup(purple_account_get_username(purple_buddy_get_account(b)));
			if(b_account)
			{
				g_string_assign(text, "<b>Account</b>: ");
				g_string_append_printf(text, "%s\n", b_account);
			}

			//Status: xxx
//			strtok(temp, " "); strtok(NULL, " "); strtok(NULL, " ");
			b_status = g_strstr_len(temp, -1, ": ") + strlen(": ");
			t_head = b_status = g_strstr_len(b_status, -1, ": ") + strlen(": ");
			t_tail = g_strstr_len(b_status, -1, ": ");
			if(t_head && t_tail)
			{
				b_status = g_strndup(t_head, t_tail - t_head);
				if(b_status)
				{
					g_string_append_printf(text, "<b>Status</b>: ");
					g_string_append_printf(text, "%s\n", b_status);
				}
			}

			//Profile Icon: 000
			t_head = g_strstr_len(temp, -1, "&lt;profileIcon&gt;") + strlen("&lt;profileIcon&gt;");
			t_tail = g_strstr_len(temp, -1, "&lt;/profileIcon&gt;");
			if(t_head && t_tail)
			{
				b_profileicon = g_strndup(t_head, t_tail - t_head);
				if(b_profileicon)
				{
					g_string_append_printf(text, "<b>Profile Icon</b>: ");
					g_string_append_printf(text, "%s\n", b_profileicon);
				}
			}

			//Level: 00
			t_head = g_strstr_len(temp, -1, "&lt;level&gt;") + strlen("&lt;level&gt;");
			t_tail = g_strstr_len(temp, -1, "&lt;/level&gt;");
			if(t_head && t_tail)
			{
				b_level = g_strndup(t_head, t_tail - t_head);
				if(b_level)
				{
					g_string_append_printf(text, "<b>Level</b>: ");
					g_string_append_printf(text, "%s\n", b_level);
				}			
			}

			//Wins: 0000
			t_head = g_strstr_len(temp, -1, "&lt;wins&gt;") + strlen("&lt;wins&gt;");
			t_tail = g_strstr_len(temp, -1, "&lt;/wins&gt;");
			if(t_head && t_tail)
			{
				b_wins = g_strndup(t_head, t_tail - t_head);
				if(b_wins)
				{
					g_string_append_printf(text, "<b>Wins</b>: ");
					g_string_append_printf(text, "%s\n", b_wins);
				}
			}

			//Leaves: 0000
			t_head = g_strstr_len(temp, -1, "&lt;leaves&gt;") + strlen("&lt;leaves&gt;");
			t_tail = g_strstr_len(temp, -1, "&lt;/leaves&gt;");
			if(t_head && t_tail)
			{
				b_leaves = g_strndup(t_head, t_tail - t_head);
				if(b_leaves)
				{
					g_string_append_printf(text, "<b>Leaves</b>: ");
					g_string_append_printf(text, "%s\n", b_leaves);
				}
			}

			//Odin Wins: 0000
			t_head = g_strstr_len(temp, -1, "&lt;odinWins&gt;") + strlen("&lt;odinWins&gt;");
			t_tail = g_strstr_len(temp, -1, "&lt;/odinWins&gt;");
			if(t_head && t_tail)
			{
				b_odinwins = g_strndup(t_head, t_tail - t_head);
				if(b_odinwins)
				{
					g_string_append_printf(text, "<b>Odin Wins</b>: ");
					g_string_append_printf(text, "%s\n", b_odinwins);
				}
			}

			//Odin Leaves: 0000
			t_head = g_strstr_len(temp, -1, "&lt;odinLeaves&gt;") + strlen("&lt;odinLeaves&gt;");
			t_tail = g_strstr_len(temp, -1, "&lt;/odinLeaves&gt;");
			if(t_head && t_tail)
			{
				b_odinleaves = g_strndup(t_head, t_tail - t_head);
				if(b_odinleaves)
				{
					g_string_append_printf(text, "<b>Odin Leaves</b>: ");
					g_string_append_printf(text, "%s\n", b_odinleaves);
				}
			}

			//Queue Type: xxx
			t_head = g_strstr_len(temp, -1, "&lt;queueType&gt;") + strlen("&lt;queueType&gt;");
			t_tail = g_strstr_len(temp, -1, "&lt;/queueType&gt;");
			if(t_head && t_tail)
			{
				b_queuetype = g_strndup(t_head, t_tail - t_head);
				if(b_queuetype)
				{
					g_string_append_printf(text, "<b>Queue Type</b>: ");
					g_string_append_printf(text, "%s\n", b_queuetype);
				}
			}

			//Ranked Wins: 0000
			t_head = g_strstr_len(temp, -1, "&lt;rankedWins&gt;") + strlen("&lt;rankedWins&gt;");
			t_tail = g_strstr_len(temp, -1, "&lt;/rankedWins&gt;");
			if(t_head && t_tail)
			{
				b_rankedwins = g_strndup(t_head, t_tail - t_head);
				if(b_rankedwins)
				{
					g_string_append_printf(text, "<b>Ranked Wins</b>: ");
					g_string_append_printf(text, "%s\n", b_rankedwins);
				}
			}

			//Ranked Losses: 0000
			t_head = g_strstr_len(temp, -1, "&lt;rankedLosses&gt;") + strlen("&lt;rankedLosses&gt;");
			t_tail = g_strstr_len(temp, -1, "&lt;/rankedLosses&gt;");
			if(t_head && t_tail)
			{
				b_rankedlosses = g_strndup(t_head, t_tail - t_head);
				if(b_rankedlosses)
				{
					g_string_append_printf(text, "<b>Ranked Losses</b>: ");
					g_string_append_printf(text, "%s\n", b_rankedlosses);
				}
			}

			//Ranked Rating: 0000
			t_head = g_strstr_len(temp, -1, "&lt;rankedRating&gt;") + strlen("&lt;rankedRating&gt;");
			t_tail = g_strstr_len(temp, -1, "&lt;/rankedRating&gt;");
			if(t_head && t_tail)
			{
				b_rankedrating = g_strndup(t_head, t_tail - t_head);
				if(b_rankedrating)
				{
					g_string_append_printf(text, "<b>Ranked Rating</b>: ");
					g_string_append_printf(text, "%s\n", b_rankedrating);
				}
			}

			//Tier: xxx
			t_head = g_strstr_len(temp, -1, "&lt;tier&gt;") + strlen("&lt;tier&gt;");
			t_tail = g_strstr_len(temp, -1, "&lt;/tier&gt;");
			if(t_head && t_tail)
			{
				b_tier = g_strndup(t_head, t_tail - t_head);
				if(b_tier)
				{
					g_string_append_printf(text, "<b>Tier</b>: ");
					g_string_append_printf(text, "%s\n", b_tier);
				}
			}

			//Status Message: xxx
			t_head = g_strstr_len(temp, -1, "&lt;statusMsg&gt;") + strlen("&lt;statusMsg&gt;");
			t_tail = g_strstr_len(temp, -1, "&lt;/statusMsg&gt;");
			if(t_head && t_tail)
			{
				b_statusmsg = g_strndup(t_head, t_tail - t_head);
				if(b_statusmsg)
				{
					g_string_append_printf(text, "<b>Status Message</b>: ");
					g_string_append_printf(text, "%s\n", b_statusmsg);
				}
			}

			//Skin Name: xxx
			t_head = g_strstr_len(temp, -1, "&lt;skinname&gt;") + strlen("&lt;skinname&gt;");
			t_tail = g_strstr_len(temp, -1, "&lt;/skinname&gt;");
			if(t_head && t_tail)
			{
				b_skinname = g_strndup(t_head, t_tail - t_head);
				if(b_skinname)
				{
					g_string_append_printf(text, "<b>Skin Name</b>: ");
					g_string_append_printf(text, "%s\n", b_skinname);
				}
			}


			//Game Queue Type: xxx
			t_head = g_strstr_len(temp, -1, "&lt;gameQueueType&gt;") + strlen("&lt;gameQueueType&gt;");
			t_tail = g_strstr_len(temp, -1, "&lt;/gameQueueType&gt;");
			if(t_head && t_tail)
			{
				b_gamequeuetype = g_strndup(t_head, t_tail - t_head);
				if(b_gamequeuetype)
				{
					g_string_append_printf(text, "<b>Game Queue Type</b>: ");
					g_string_append_printf(text, "%s\n", b_gamequeuetype);
				}
			}

			//Is Observable: xxx
			t_head = g_strstr_len(temp, -1, "&lt;isObservable&gt;") + strlen("&lt;isObservable&gt;");
			t_tail = g_strstr_len(temp, -1, "&lt;/isObsevable&gt;");
			if(t_head && t_tail)
			{
				b_isobservable = g_strndup(t_head, t_tail - t_head);
				if(b_isobservable)
				{
					g_string_append_printf(text, "<b>Is Observable</b>: ");
					g_string_append_printf(text, "%s\n", b_isobservable);
				}
			}

			//Game Status: xxx
			t_head = g_strstr_len(temp, -1, "&lt;gamestatus&gt;") + strlen("&lt;gamestatus&gt;");
			t_tail = g_strstr_len(temp, -1, "&lt;/gamestatus&gt;");
			if(t_head && t_tail)
			{
				b_gamestatus = g_strndup(t_head, t_tail - t_head);
				if(b_gamestatus)
				{
					g_string_append_printf(text, "<b>Game Status</b>: ");
					g_string_append_printf(text, "%s\n", b_gamequeuetype);
				}
			}

			//Time Stamp: 0000
			t_head = g_strstr_len(temp, -1, "&lt;timeStamp&gt;") + strlen("&lt;timeStamp&gt;");
			t_tail = g_strstr_len(temp, -1, "&lt;/timeStamp&gt;");
			if(t_head && t_tail)
			{
				b_timestamp = g_strndup(t_head, t_tail - t_head);
				//b_timestamp[strlen(b_timestamp)-3] = '\0';
				//raw_time = atoi(b_timestamp);
				if(b_timestamp)
				{
					g_string_append_printf(text, "<b>Time Stamp</b>: ");
					g_string_append_printf(text, "%s\n", b_timestamp);
					//g_string_append_printf(text, "%s", ctime(&raw_time));
				}
			}

			//Subscription: xxx
			t_head = g_strstr_len(temp, -1, "<b>Subscription</b>: ") + strlen("<b>Subscription</b>: ");
			b_subscription = g_strdup(t_head);
			if(b_subscription)
			{
				g_string_append_printf(text, "<b>Subscription</b>: ");
				g_string_append_printf(text, "%s\n", b_subscription);
			}
		}

		#ifdef _DEBUG
			purple_debug_info(LC_PLUGIN_ID, "Debug : PBNT(%d)\n", node->type);
			purple_debug_info(LC_PLUGIN_ID, "Debug : draw tooltip(%s)\n", text->str);
			purple_debug_info(LC_PLUGIN_ID, "Debug : b_status(%s)\n", b_status);
			purple_debug_info(LC_PLUGIN_ID, "Debug : b_profileicon(%s)\n", b_profileicon);
			purple_debug_info(LC_PLUGIN_ID, "Debug : b_level(%s)\n", b_level);
			purple_debug_info(LC_PLUGIN_ID, "Debug : b_wins(%s)\n", b_wins);
			purple_debug_info(LC_PLUGIN_ID, "Debug : b_leaves(%s)\n", b_leaves);
			purple_debug_info(LC_PLUGIN_ID, "Debug : b_odinwins(%s)\n", b_odinwins);
			purple_debug_info(LC_PLUGIN_ID, "Debug : b_odinleaves(%s)\n", b_odinleaves);
			purple_debug_info(LC_PLUGIN_ID, "Debug : b_queuetype(%s)\n", b_queuetype);
			purple_debug_info(LC_PLUGIN_ID, "Debug : b_rankedwins(%s)\n", b_rankedwins);
			purple_debug_info(LC_PLUGIN_ID, "Debug : b_rankedlosses(%s)\n", b_rankedlosses);
			purple_debug_info(LC_PLUGIN_ID, "Debug : b_rankedrating(%s)\n", b_rankedrating);
			purple_debug_info(LC_PLUGIN_ID, "Debug : b_tier(%s)\n", b_tier);
			purple_debug_info(LC_PLUGIN_ID, "Debug : b_statusmsg(%s)\n", b_statusmsg);
			purple_debug_info(LC_PLUGIN_ID, "Debug : b_skinname(%s)\n", b_skinname);
			purple_debug_info(LC_PLUGIN_ID, "Debug : b_gamequeuetype(%s)\n", b_gamequeuetype);
			purple_debug_info(LC_PLUGIN_ID, "Debug : b_isobservable(%s)\n", b_isobservable);
			purple_debug_info(LC_PLUGIN_ID, "Debug : b_gamestatus(%s)\n", b_gamestatus);
			purple_debug_info(LC_PLUGIN_ID, "Debug : b_timestamp(%s)\n", b_timestamp);
			#endif
	}
}
	
	
//	#ifdef _DEBUG
//		if(PURPLE_BLIST_NODE_IS_CHAT(node))
//		{
//			purple_debug_info(LC_PLUGIN_ID, "Debug : is chat\n");
//		}
//		if(PURPLE_BLIST_NODE_IS_CONTACT(node))
//		{
//			purple_debug_info(LC_PLUGIN_ID, "Debug : is contact\n");
//		}
//		if(PURPLE_BLIST_NODE_IS_BUDDY(node))
//		{
//			purple_debug_info(LC_PLUGIN_ID, "Debug : is buddy\n");
//		}
//		if(PURPLE_BLIST_NODE_IS_GROUP(node))
//		{
//			purple_debug_info(LC_PLUGIN_ID, "Debug : is group\n");
//		}
//	#endif

/* function implemetation */
/*
static void plugin_action_test_cb(PurplePluginAction *action)
{
	//PurpleBlistNode *root = purple_blist_get_root();
	//PidginBuddyList *root = pidgin_blist_get_default_gtk_blist(void);

	#ifdef _DEBUG
		purple_debug_info(LC_PLUGIN_ID, "Debug : action\n");
	#endif

	purple_notify_message(lc_plugin, PURPLE_NOTIFY_MSG_INFO, "Plugin Actions Test", "If debug mode, Show debug window", NULL, NULL, NULL);
}

static GList *plugin_actions(PurplePlugin *plugin, gpointer context)
{
	GList *list = NULL;
	PurplePluginAction *action = NULL;

	action = purple_plugin_action_new("Plugin Action Test", plugin_action_test_cb);

	list = g_list_append(list, action);

	return list;
}
*/
/** Protocol Functions **/

/** Pidgin Functions **/
/* calling when plugin load */
static gboolean plugin_load(PurplePlugin *plugin)
{
	//void *jabber_handle = purple_plugins_find_with_id("prpl-jabber");
	void *gtk_blist_handle = pidgin_blist_get_handle();
	//void *buddy_handle = purple_blist_get_handle();
	
	//pidgin_blist_init();

	//purple_signal_connect(jabber_handle, "jabber-receiving-presence", plugin, PURPLE_CALLBACK(jabber_presence_received), NULL);

	purple_signal_connect(gtk_blist_handle, "drawing-tooltip", plugin, PURPLE_CALLBACK(gtk_buddy_tooltip), NULL);

	return TRUE;
}

/* calling when plugin unload */
static gboolean plugin_unload(PurplePlugin *plugin)
{
	return TRUE;
}

/* MUST IMPLEMENTE plugin_initilize */
static void init_plugin(PurplePlugin *plugin)
{
}

/* Required plugin infomation */
static PurplePluginInfo info = {
	PURPLE_PLUGIN_MAGIC,				// magic
	PURPLE_MAJOR_VERSION,				// major_version
	PURPLE_MINOR_VERSION,				// minor_version
	PURPLE_PLUGIN_STANDARD,				// type
	NULL,								// ui_requirement
	0,									// flags
	NULL,								// dependencies
	PURPLE_PRIORITY_DEFAULT,			// priority
	LC_PLUGIN_ID,						// id
	LC_PLUGIN_NAME,					// name
	LC_PLUGIN_VERSION,					// version
	LC_PLUGIN_SUMMARY,					// summary
	LC_PLUGIN_DESCRIPTION,				// description
	LC_PLUGIN_AUTHOR,					// author
	LC_PLUGIN_HOMEPAGE,				// homepage
	plugin_load,						// load
	plugin_unload,						// unload
	NULL,								// destroy
	NULL,								// ui_info
	NULL,							// extra_info
	NULL,								// prefs_info
	//plugin_actions,						// actions
	NULL,
	NULL,								// padding...
	NULL,
	NULL,
	NULL,
};

/* MUST Required macro */
PURPLE_INIT_PLUGIN(lol_chat, init_plugin, info)
