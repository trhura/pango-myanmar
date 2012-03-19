/* pango-myanmar
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Author: Thura Hlaing <trhura@gmail.com>
 *
 */

#include <datrie/trie.h> 
#include <glib.h>
#include <wchar.h>
#include <string.h>

#include "myanmar.h"
#include "wbrk.h"
#include "myctype.h"
#include "map.h"

static Trie *trie;

void	wbrk_init ()
{
	if (trie == NULL)
		trie = trie_new_from_file ("/home/trh/Development/pango-myanmar/data/mybrk.tri");
}

void	wbrk_unload ()
{
	if (trie != NULL)
		trie_free (trie);
}

gboolean	wbrk_is_ready ()
{
	return trie != NULL;
}

gunichar*	wbrk_normalize_string (gunichar *string)
{
	g_return_val_if_fail (string != NULL, NULL);
	gint len = G_N_ELEMENTS (_map);
	gunichar* ret = wcsdup (string);

	int i = 0, j;
	gunichar *tmp[2];
	gunichar *pos = NULL;

	for (; i < len;i++) {
		tmp[0] = ((gunichar **)(_map[i]))[0];
		tmp[1] = ((gunichar **)(_map[i]))[1];

		//g_printf ("%ls %d, %ls\n", tmp[0], wcslen(tmp[0]), tmp[1]);
		pos = ret;
		while ((pos = wcsstr (pos, tmp[0])) != NULL) {
			for (j = 0; j < wcslen(tmp[0]); j++) {
				pos[j] = tmp[1][j];
			}
		}
	}
	return ret;
}

gint	wbrk_get_next_brkpos (gunichar *string)
{
	g_assert (trie != NULL);

	gint j, i = 0;
	gint ret = 0;
	TrieState *state = trie_root (trie);

	int length = wcslen (string);

	if (length == 0)
		return 0;

	j = 0;

	while (j < length) {

		if (string[j] != L'-' && !trie_state_walk (state, string[j]))
			break;

		g_printf ("\tcheckpoint 1: %lc\n", string[j]);
		j++;
		if (!trie_state_is_terminal (state))
			continue;

		//g_printf ("\tcheckpoint 1: %lc\n", string[j-1]);
		if (j < length && my_wcismydiac (string[j]))
			continue;

		g_printf ("\tcheckpoint 2: %lc\n", string[j]);
		if (my_wcismyconsonant (string[j])) {
			if (j+1 < length && string[j+1] == MYANMAR_SIGN_ASAT)
				continue;

			g_printf ("\tcheckpoint 3: %lc\n", string[j]);
			i = j + 1;
			/* gboolean FOLLOWED_BY_ASAT = FALSE; */
			/* while (i < length && (string[i] == L'-' || my_wcismydiac (string[i]))) */
			/* { */
			/* 	if (string[i] == MYANMAR_VOWEL_SIGN_AA) */
			/* 		break; */

			/* 	if (string[i+1] == MYANMAR_SIGN_ASAT) { */
			/* 		FOLLOWED_BY_ASAT = TRUE; */
			/* 		break; */
			/* 	} */
			/* 	i++; */
            /* } */

			/* if (FOLLOWED_BY_ASAT) */
			/* 	continue; */

			g_printf ("\tcheckpoint 4: %lc\n", string[j]);
			/* i = 1; */
			/* gboolean FOLLOWED_BY_ASAT = FALSE; */
			/* while ((j+i < length && string[j+i] != MYANMAR_VOWEL_SIGN_AA && */
			/* 		(string[j+i] == L'-' || my_wcismydiac (string[j+i])))) */
			/* { */
			/* 	if (j+i+1 < length && string[j+i+1] == MYANMAR_SIGN_ASAT) { */
			/* 		/\* Followed by ASAT *\/ */
			/* 		FOLLOWED_BY_ASAT = TRUE; */
			/* 		break; */
			/* 	} */
			/* 	i++; */
			/* } */

			/* if (FOLLOWED_BY_ASAT) */
			/* 	continue; */
			
			g_printf ("\tcheckpoint 4\n");
			if ((j-1 > 0 && string[j-1] == MYANMAR_SIGN_VIRAMA) ||
				(j+1 > length && string[j+1] == MYANMAR_SIGN_VIRAMA))
				/*  Stacked Consonants Vs Kinzi */
				continue;
		}
		/* Otherwise */
		ret = j; 
	}

	if (ret == 0) {
		while (j < length &&
			   (!my_wcismyconsonant(string[j])))
			j++;
		ret = j;
	}

	return ret;
}
