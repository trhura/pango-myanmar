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
#include "word-break.h"
#include "myanmar.h" 

static Trie *trie;

void	wbrk_load_wordlist ()
{
	g_return_if_fail (trie == NULL);
	trie = trie_new_from_file ("/home/trh/Development/pango-myanmar/data/mywords.tri");
}

void	wbrk_free_wordlist ()
{
	g_return_if_fail (trie!= NULL);
	trie_free (trie);
}

gboolean	wbrk_wordlist_is_loaded ()
{
	return trie != NULL;
}

gint	wbrk_get_next_brkpos (wchar_t *string)
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
			 my_wcismydigit (string[j])))
		j++;
	
	while (j < length) {
		
		if (trie_state_walk (state, string[j])) {
			j++;

			if (trie_state_is_terminal (state)) {
				ret = j;
			}
		} else {
			break;
		}
	}

	return ret; 
}
