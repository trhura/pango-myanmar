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
#include "mystr.h"

static Trie *trie;
static GPtrArray *cluster_array;

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

gboolean	wbrk_prepare (const wchar_t *string)
{
	if (string == NULL || trie == NULL)
		return FALSE;

	if (cluster_array != NULL)
	{
		g_ptr_array_free (cluster_array, TRUE);
	}

	int i = 0;
	wchar_t* cluster;
	cluster_array = g_ptr_array_new_with_free_func (g_free);
	//	g_debug ("string to break word = %ls\n", string);
	ClusterIter *iter	= mystr_cluster_iter_new (string);

	while ((cluster = mystr_cluster_iter_next (iter)) != NULL) {
		//		g_debug ("\tcluster = %ls\n",cluster);
		g_ptr_array_add (cluster_array, cluster);
		i++;
	}

	return TRUE;
}

gint	wbrk_get_next_brkpos ()
{
	g_assert (trie != NULL);
	g_assert (cluster_array != NULL);

	// g_printf ("length = %d\n", cluster_array->len);

	if (cluster_array->len < 0)
		return 0;

	gint j, i = 0;
	gint ret = 0;
	TrieState *state = trie_root (trie);
	wchar_t* cluster;

	while (i < cluster_array->len) {
		cluster = (wchar_t*) g_ptr_array_index (cluster_array, i);
		// g_printf ("\tchecking for cluster %d: %ls\n", i, cluster);

		j = 0;
		while (j < wcslen (cluster)) {
			// g_printf ("\t\tchecking character: %lc\n", cluster[j]);
			if (trie_state_walk (state, cluster[j])) {
				// g_printf ("\t\t\twalking...\n");
				j++;
			} else {
				// g_printf ("\t\tbreaking ret=%d...\n", ret);

				if (ret)
					/* return is not zero */
					return ret;

				ret += wcslen (cluster);
				// g_printf ("\t\t %ls not found\n", cluster);
				g_ptr_array_remove_index (cluster_array, i);
				break;
			}
		}

		if (trie_state_is_terminal (state)) {
			ret += j;
			// g_printf ("\t\adding ret += %d\n", j);
			g_ptr_array_remove_index (cluster_array, i);
		}
	}

	return ret;
}
