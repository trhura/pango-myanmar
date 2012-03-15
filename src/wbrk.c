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

#include "myanmar.h"
#include "wbrk.h"
#include "myctype.h"

static Trie *trie;

void	wbrk_init ()
{
	if (trie == NULL)
		trie = trie_new_from_file ("/home/trhura/Development/pango-myanmar/data/mybrk.tri");
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

	while (j < length &&
		   (!my_wcismyanmar (string[j]) ||
			my_wcismypunct (string[j])	||
			my_wcismydigit (string[j])))
		j++;

	while (j < length) {
		if (trie_state_walk (state, string[j])) {
			j++;

			if (!trie_state_is_terminal (state))
				continue;

			//g_printf ("\tcheckpoint 1\n");
			if (j < length && my_wcismydiac (string[j]))
				continue;

			//g_printf ("\tcheckpoint 2: %lc\n", string[j]);
			if (my_wcismyconsonant (string[j])) {
				if (j+1 < length && string[j+1] == MYANMAR_SIGN_ASAT)
					continue;

				if ((j+1 < length && my_wcismydiac (string[j+1])) &&
					(j+2 < length && string[j+2] == MYANMAR_SIGN_ASAT))
					/* Followed by ASAT */
					continue;

				//g_printf ("\tcheckpoint 3\n");
				if ((j-1 > 0 && string[j-1] == MYANMAR_SIGN_VIRAMA) ||
					(j+1 > length && string[j+1] == MYANMAR_SIGN_VIRAMA))
					/*  Stacked Consonants Vs Kinzi */
					continue;
			}
			/* Otherwise */
			ret = j;
		} else {
			break;
		}
	}

	return ret;
}
