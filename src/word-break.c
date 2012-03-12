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

gunichar* next_cluster (gunichar *string, gunichar **cluster)
{
	if (string == NULL)
		/* if the string is empty. */
		goto notfound;

	gint i = 0, j = 0;
	gunichar* ret_cluster;
	gint length = wcslen (string);
	gint cluster_length = 0;

	while (!my_wcismyletter (string[i]) && i < length) {
		/* Find the first consonant */
		if (my_wcismydigit(string[i]) ||
			my_wcismypunct(string[i]) ||
			my_wcismyindependsymbol (string[i])) {
			j = i + 1;
			goto finish;
		}
		i++;
	}

	if (i == length)
		/* if no consonant is found */
		goto notfound;

	j = i + 1;
	int devowelized;

	do {
		devowelized = 0;

		while (!my_wcismyletter (string[j]) && j < length) {
			/* find the next consonant */
			if (my_wcismydigit(string[j]) ||
				my_wcismypunct(string[j]) ||
				my_wcismyindependsymbol (string[j])) {
				goto finish;
			}
			j++;
		}

		if (j == length)
			goto finish;

		if (string[j-1] == MYANMAR_SIGN_VIRAMA) {
			/* preceded by virama */
			j += 1;
			devowelized = 1;
		}

		if ((j+1 < length) && string[j+1] == MYANMAR_SIGN_VIRAMA ) {
			/* followed by virama */
			j += 3;
			devowelized = 1;
		}

		/* Checks whether the consonant is followed by asat */
		int k = j;
		while (k + 1 < length && my_wcismydiac (string[k+1])) {
			if (string[k + 1] == MYANMAR_VOWEL_SIGN_AA)
				break;

			if (string[k + 1] == MYANMAR_SIGN_ASAT) {
				devowelized = 1;
				j = k + 1;
				break;
			}
			k++;
		}

	} while (devowelized != 0);

finish:
	if (cluster) {
		cluster_length = j - i;

		ret_cluster = (gunichar*) g_malloc (sizeof (gunichar) * (cluster_length + 1));
		wcsncpy (ret_cluster, string+i, cluster_length);
		ret_cluster[cluster_length] = L'\0';

		*cluster = ret_cluster;
	}
	return string+j;

notfound:
	if (cluster)
		*cluster = NULL;
	return NULL;
}

gunichar* next_n_cluster (gunichar* string, gunichar** cluster, gint n)
{
	gint i;
	gunichar *string_ptr  = string;
	gunichar *cluster_ptr = NULL;
	gunichar *ret_cluster = (gunichar*) g_malloc (sizeof(gunichar) * 512);

	/* TODO: dynamic allocation */
	for (i = 0; i < n; i++) {
		if (string_ptr = next_cluster (string_ptr, &cluster_ptr)) {
			wcscat (ret_cluster, cluster_ptr);
			g_free (cluster_ptr);
		} else{
			g_free (ret_cluster);
			ret_cluster = NULL;
		}
	}

	*cluster = ret_cluster;
	return string_ptr;
}

gboolean cluster_is_word (gunichar* cluster)
{
	g_return_val_if_fail (trie != NULL, FALSE);

	TrieData data;
	if (trie_retrieve  (trie, cluster, &data))
		return TRUE;
	else
		return FALSE;
}

gint get_max_break_position (GPtrArray *clusters)
{
	g_return_val_if_fail (trie != NULL, FALSE);
	
	int i = 0, j = 0;
	gint ret = 0;
	TrieState *state = trie_state_root (trie);
	wchar_t* cluster;
	
	while (i < clusters->len) {
		cluster = (wchar_t*) g_ptr_array_index (clusters, i);

		j = 0;
		while (j < wcslen (cluster)) {
			if (trie_state_walk (cluster[j]))
				j++;
			else
				goto break;
		}
		ret += j;
		i++; 
	}

	break:
		if (i) {
			return ret;
		}
		else
			return wcslen (i);
}

void load_wordlist ()
{
	g_return_if_fail (trie == NULL);
	trie = trie_new_from_file ("/home/trh/Development/pango-myanmar/data/mywords.tri");
}

void free_wordlist ()
{
	g_return_if_fail (trie!= NULL);
	trie_free (trie);
}

