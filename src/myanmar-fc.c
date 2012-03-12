/* Pango
 * myanmar-fc.c: Shaper for Myanmar script
 *
 * Copyright (C) 2005 Tin Myo Htet <tmhtet@gmail.com>
 * Author: Tin Myo Htet <tmhtet@gmail.com>
 * Font : Lookups used in Masterpiece Uni Sans font distributed together are based
 *    on lookups written by Ngwe Tun <ngwestar@gmail.com>
 *
 * Based on Khmer shaper
 * Copyright (C) 2004 Open Forum of Cambodia (www.forum.org.kh / www.khmeros.info)
 * Authors: Jens Herden <jens@khmeros.info> and Javier Sola <javier@khmeros.info>
 *
 * Based on code from other shapers
 * Copyright (C) 1999-2004 Red Hat Software
 * Author: Owen Taylor <otaylor@redhat.com>

 * Partially based on Indic shaper
 * Copyright (C) 2001, 2002 IBM Corporation
 * Author: Eric Mader <mader@jtcsv.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <string.h>
#include <stdio.h>
#include <glib.h>
#include <pango/pango-engine.h>
#include <pango/pango-ot.h>
#include <pango/pango-modules.h>
#include <pango/pangofc-font.h>


typedef PangoEngineShape		MyanmarEngineFc;
typedef PangoEngineShapeClass   MyanmarEngineFcClass;

#define SCRIPT_ENGINE_NAME "MyanmarScriptEngineFc"
#define RENDER_TYPE PANGO_RENDER_TYPE_FC

static PangoEngineScriptInfo myanmar_scripts[] = {
	{ PANGO_SCRIPT_MYANMAR, "*" }
};

static PangoEngineInfo script_engines[] = {
	{
		SCRIPT_ENGINE_NAME,
		PANGO_ENGINE_TYPE_SHAPE,
		RENDER_TYPE,
		myanmar_scripts, G_N_ELEMENTS(myanmar_scripts)
	}
};


static PangoGlyph
get_index (PangoFcFont *fc_font, gunichar wc)
{
	PangoGlyph index = pango_fc_font_get_glyph (fc_font, wc);
	if (!index)
		index = pango_fc_font_get_unknown_glyph (fc_font, wc);
	return index;
}

static gchar **blacklist_fonts;

static void
myanmar_engine_shape (PangoEngineShape *engine,
					  PangoFont		   *font,
					  const char	   *text,
					  int				length,
					  PangoAnalysis	   *analysis,
					  PangoGlyphString *glyphs)
{
	guint engine_type_id = g_quark_from_static_string (PANGO_ENGINE_TYPE_SHAPE);
	guint render_type_id = g_quark_from_static_string (PANGO_RENDER_TYPE_FC);
	PangoEngineShape *n_engine = (PangoEngineShape*)
		pango_map_get_engine (pango_find_map (pango_language_get_default (),
											  engine_type_id,
											  render_type_id),
							  PANGO_SCRIPT_COMMON);
	PANGO_ENGINE_SHAPE_GET_CLASS (n_engine)->script_shape (engine, font,
														   text,length,
														   analysis, glyphs);
	return;
}

static void
myanmar_engine_fc_class_init (PangoEngineShapeClass *class)
{
	class->script_shape = myanmar_engine_shape;
}

PANGO_ENGINE_SHAPE_DEFINE_TYPE (MyanmarEngineFc, myanmar_engine_fc,
								myanmar_engine_fc_class_init, NULL);

void
PANGO_MODULE_ENTRY(init) (GTypeModule *module)
{
	myanmar_engine_fc_register_type (module);
}

void
PANGO_MODULE_ENTRY(exit) (void)
{
}

void
PANGO_MODULE_ENTRY(list) (PangoEngineInfo **engines,
						  int          *n_engines)
{
	*engines = script_engines;
	*n_engines = G_N_ELEMENTS (script_engines);
}

PangoEngine *
PANGO_MODULE_ENTRY(create) (const char *id)
{
	if (!strcmp (id, SCRIPT_ENGINE_NAME))
		return g_object_new (myanmar_engine_fc_type, NULL);
	else
		return NULL;
}
