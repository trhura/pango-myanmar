/* Pango
 * myanmar-lang.c: Language Engine for Myanmar script
 *
 * Copyright (C) 2005 Tin Myo Htet <tmhtet@gmail.com>
 *
 * Based on Language Engine from pango-libthai
 * Copyright (C) 2003 Theppitak Karoonboonyanan <thep@linux.myanmar.net>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */


#include <glib.h>
#include <wchar.h>
#include <string.h>
#include <pango/pango-engine.h>
#include <pango/pango-break.h>

#include "myanmar.h"

/* No extra fields needed */
typedef PangoEngineLang		 MyanmarEngineLang;
typedef PangoEngineLangClass MyanmarEngineLangClass;

#define SCRIPT_ENGINE_NAME "MyanmarScriptEngineLang"

static PangoEngineScriptInfo myanmar_scripts[] = {
	{ PANGO_SCRIPT_MYANMAR, "*" }
};


static PangoEngineInfo script_engines[] = {
	{
		SCRIPT_ENGINE_NAME,
		PANGO_ENGINE_TYPE_LANG,
		PANGO_RENDER_TYPE_NONE,
		myanmar_scripts, G_N_ELEMENTS(myanmar_scripts)
	}
};

static void
myanmar_engine_break (PangoEngineLang *engine,
					  const char	  *text,
					  int			   length,
					  PangoAnalysis	  *analysis,
					  PangoLogAttr	  *attrs,
					  int			   attrs_len)
{

	gint i, j;
	glong n_chars;
	gunichar *wcs = g_utf8_to_ucs4_fast (text, length, &n_chars);

	/* Determine Character Boundar */
	for (i = 0; i < n_chars; i++)
	{
		attrs[i].is_char_break = FALSE;
		//attrs[i].is_line_break = TRUE;
		attrs[i].is_cursor_position = FALSE;

		if (g_unichar_type (wcs[i]) == G_UNICODE_OTHER_LETTER ||
			wcs[i] == MYANMAR_SYMBOL_AFOREMENTIONED)
		{
			attrs[i].is_char_break = TRUE;
			attrs[i].is_line_break = TRUE;
			attrs[i].is_cursor_position = TRUE;

			// (C|CKC|CA|CAKC) skip kinzi & final consonants
			while (i + 1 < n_chars &&
				   (wcs[i+1] == MYANMAR_SIGN_ASAT ||
					wcs[i+1] == MYANMAR_SIGN_VIRAMA))
			{
				i++;

				if (i + 1 < n_chars &&
					g_unichar_type (wcs[i+1]) == G_UNICODE_OTHER_LETTER)
					i++;
			}

			//[M][V] Medials/Vowels
			while (i + 1 < n_chars &&
				   g_unichar_type (wcs[i+1]) == G_UNICODE_NON_SPACING_MARK)
				i++;

			//[F<CA>] Finals
			if (i + 1 < n_chars &&
				g_unichar_type  (wcs[i+1]) == G_UNICODE_OTHER_LETTER)
			{
				if (i + 2 < n_chars &&
					wcs[i+2] == MYANMAR_SIGN_ASAT)
					i += 2;
			}

			//[D] Tones
			while (i + 1 < n_chars &&
				   g_unichar_type (wcs[i+1]) == G_UNICODE_NON_SPACING_MARK)
				i++;
		}
	}
}

static void
myanmar_engine_lang_class_init (PangoEngineLangClass *class)
{
	class->script_break = myanmar_engine_break;
}

PANGO_ENGINE_LANG_DEFINE_TYPE (MyanmarEngineLang, myanmar_engine_lang,
							   myanmar_engine_lang_class_init, NULL);

void
PANGO_MODULE_ENTRY(init) (GTypeModule *module)
{
	myanmar_engine_lang_register_type (module);
	//wbrk_init ();
}

void
PANGO_MODULE_ENTRY(exit) (void)
{
	//wbrk_unload ();
}

void
PANGO_MODULE_ENTRY(list) (PangoEngineInfo **engines, gint *n_engines)
{
	*engines = script_engines;
	*n_engines = G_N_ELEMENTS (script_engines);
}

/* Load a particular engine given the ID for the engine */
PangoEngine *
PANGO_MODULE_ENTRY(create) (const char *id)
{
	if (!strcmp (id, SCRIPT_ENGINE_NAME))
		return g_object_new (myanmar_engine_lang_type, NULL);
	else
		return NULL;
}
