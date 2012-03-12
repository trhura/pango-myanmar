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
#include <pango/pango-engine.h>
#include <pango/pango-break.h>

#include "myanmar.h"
#include "mystr.h"

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
		GUnicodeType type = g_unichar_type (wcs[i]);

		if (type	== G_UNICODE_OTHER_LETTER ||
			wcs[i]	== MYANMAR_SYMBOL_AFOREMENTIONED) {
			/* All letters (consonants) are character,line breaks.
			 */
			attrs[i].is_char_break = TRUE;
			attrs[i].is_line_break = TRUE;
			attrs[i].is_cursor_position = TRUE;

			gint next = 1;
			while ((i+next) < n_chars && /* Skip marks (medials, vowel) */
				   g_unichar_type (wcs[i+next]) == G_UNICODE_NON_SPACING_MARK)
			{
				if (wcs[i+next] == MYANMAR_SIGN_ASAT)
				{
					/* If it is followed ASAT, don't break . */
					attrs[i].is_line_break = FALSE;
					attrs[i].is_char_break = FALSE;
					attrs[i].is_cursor_position = FALSE;

					/* Howver, if it is infront of kinzi (followed by virama),
					 * cursor position break should be allowed .
					 */
					if ((i+next+1) < n_chars &&
						wcs[i+next+1] == MYANMAR_SIGN_VIRAMA)
						attrs[i].is_cursor_position = TRUE;
					break;
				}

				if (wcs[i+next] == MYANMAR_SIGN_VIRAMA) {
					attrs[i].is_char_break = FALSE;
					attrs[i].is_line_break = FALSE;
				}
				next++;

			}

			if (wcs[i] == MYANMAR_LETTER_GREAT_SA) {
				attrs[i].is_char_break = FALSE;
				attrs[i].is_line_break = FALSE;
			}

			/* For stacked consonants & (after) kinzi, doesn't allow break */
			if (i > 0 && wcs[i-1] == MYANMAR_SIGN_VIRAMA) {
				attrs[i].is_line_break = FALSE;
				attrs[i].is_char_break = FALSE;
				attrs[i].is_cursor_position = FALSE;
			}
		}
	}

	if (wcslen (wcs) < 12)
		return;

	/*	Determine Word Boundaries */
	/* gunichar* cluster; */
	/* gunichar* tmp = wcs; */
	/* gunichar* syllable[5]; */
	/* gint c_word_start = 0; */
	/* gint c_pos = 0; */

	/* while (c_pos < wcslen (wcs)) { */
	/*  for (i = 0; tmp != NULL && i < 5; i++) { */
	/*      tmp = next_cluster (tmp, &(syllable[i])); */
	/*  } */

	/*  gunichar test[256] =  { L'\0' }; */
	/*  for (i -= 1; i > 0; i--) { */
	/*      test[0] = L'\0'; */
	/*      //printf ("%d ", i); */
	/*      for (j = 0; j < i; j++) { */
	/*          wcscat (test, syllable[j]); */
	/*      }; */
	/*      //g_printf ("sylalble: %ls", test); */
	/*      if (cluster_is_word (test)) { */
	/*          //printf ("word found.\n"); */
	/*          break; */
	/*      } */
	/*      //g_free (syllable[i]);  */
	/*  } */

	/*  attrs[c_word_start].is_word_start = TRUE; */
	/*  c_word_start = c_pos; */
	/*  c_pos += wcslen (test); */
	/*  attrs[c_pos].is_word_end  = TRUE; */
	/*  attrs[c_pos].is_word_start = TRUE; */
	/*  tmp = wcs + c_pos; */
	/* } */

	i = 0;
	GPtrArray *clusters = mystr_get_clusters (wcs); 

	while (
	get_max_break_position (clusters);
	g_ptr_array_free (clusters, TRUE);

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
	load_wordlist ();
}

void
PANGO_MODULE_ENTRY(exit) (void)
{
	free_wordlist ();
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
