/* Gobby - GTK-based collaborative text editor
 * Copyright (C) 2008-2014 Armin Burgmeier <armin@arbur.net>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _GOBBY_TEXTSESSIONVIEW_HPP_
#define _GOBBY_TEXTSESSIONVIEW_HPP_

#include "core/sessionview.hpp"
#include "core/textundogrouping.hpp"
#include "core/preferences.hpp"

#include <gtkmm/tooltip.h>

#include <gtksourceview/gtksourceview.h>
#include <gtksourceview/gtksourcelanguagemanager.h>
#include <gtksourceview/gtksourcelanguage.h>

#include <libinftextgtk/inf-text-gtk-view.h>
#include <libinftextgtk/inf-text-gtk-viewport.h>
#include <libinftext/inf-text-session.h>
#include <libinftext/inf-text-user.h>

namespace Gobby
{

class TextSessionView: public SessionView
{
public:
	typedef sigc::signal<void, GtkSourceLanguage*> SignalLanguageChanged;

	TextSessionView(InfTextSession* session, const Glib::ustring& title,
	                const Glib::ustring& path,
	                const Glib::ustring& hostname,
	                const std::string& info_storage_key,
	                Preferences& preferences,
	                GtkSourceLanguageManager* manager);
	~TextSessionView();

	InfTextSession* get_session() { return INF_TEXT_SESSION(m_session); }

	const std::string& get_info_storage_key() const
	{
		return m_info_storage_key;
	}

	void get_cursor_position(unsigned int& row, unsigned int& col) const;
	void set_selection(const GtkTextIter* begin,
	                   const GtkTextIter* end);
	Glib::ustring get_selected_text() const;
	void scroll_to_cursor_position(double within_margin);

	GtkSourceLanguage* get_language() const;
	void set_language(GtkSourceLanguage* language);

	// cannot overwrite with covariant return type InfTextUser because
	// C++ does not know GObject inheritance:
	virtual InfUser* get_active_user() const;
	void set_active_user(InfTextUser* user);

	// requires active user to be set:
	TextUndoGrouping& get_undo_grouping() { return *m_undo_grouping; }

	GtkSourceView* get_text_view() { return m_view; }
	GtkSourceBuffer* get_text_buffer() { return m_buffer; }

	SignalLanguageChanged signal_language_changed() const
	{
		return m_signal_language_changed;
	}

protected:
	void on_user_color_changed();
	void on_alpha_changed();

	void on_show_remote_cursors_changed();
	void on_show_remote_selections_changed();
	void on_show_remote_current_lines_changed();
	void on_show_remote_cursor_positions_changed();

	void on_tab_width_changed();
	void on_tab_spaces_changed();
	void on_auto_indent_changed();
	void on_homeend_smart_changed();

	void on_wrap_mode_changed();
	void on_linenum_display_changed();
	void on_curline_highlight_changed();
	void on_margin_display_changed();
	void on_margin_pos_changed();
	void on_bracket_highlight_changed();
	void on_whitespace_display_changed();

	void on_font_changed();
	void on_scheme_changed();

	void on_view_style_updated();

	bool on_query_tooltip(int x, int y, bool keyboard_mode,
	                      const Glib::RefPtr<Gtk::Tooltip>& tooltip);

	static gboolean on_query_tooltip_static(GtkWidget* widget, int x,
	                                        int y, gboolean keyboard_mode,
	                                        GtkTooltip* tooltip,
	                                        gpointer user_data)
	{
		return static_cast<TextSessionView*>(user_data)->
			on_query_tooltip(x, y, keyboard_mode,
			                 Glib::wrap(tooltip, true));
	}

	static void on_view_style_updated_static(GtkWidget* view,
	                                         gpointer user_data)
	{
		static_cast<TextSessionView*>(user_data)->
			on_view_style_updated();
	}

	std::string m_info_storage_key;
	Preferences& m_preferences;

	GtkSourceView* m_view;
	GtkSourceBuffer* m_buffer;
	std::auto_ptr<TextUndoGrouping> m_undo_grouping;
	InfTextGtkView* m_infview;
	InfTextGtkViewport* m_infviewport;

	SignalLanguageChanged m_signal_language_changed;
};

}

#endif // _GOBBY_TEXTSESSIONVIEW_HPP_
