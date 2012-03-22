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
static gint next_cluster_position (gunichar *string);

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

gint	wbrk_get_next_brkpos (gunichar *nstring, gunichar *string)
{
	g_assert (trie != NULL);

	gint j, i = 0;
	gint ret = 0;
	TrieState *state = trie_root (trie);

	int length = wcslen (nstring);

	if (length == 0)
		return 0;

	j = 0;

	while (j < length) {

		if (nstring[j] != L'-' && !trie_state_walk (state, nstring[j]))
			break;

		//g_printf ("\tcheckpoint 1: %lc\n", nstring[j]);
		j++;

		if (!trie_state_is_terminal (state))
			continue;

		//g_printf ("\tcheckpoint 2: %lc\n", nstring[j-1]);
		if (j < length && my_wcismydiac (nstring[j]))
			continue;

		//g_printf ("\tcheckpoint 3: %lc\n", nstring[j]);
		if (my_wcismyconsonant (nstring[j])) {

			if ((j-1 > 0 && nstring[j-1] == MYANMAR_SIGN_VIRAMA) ||
				(j+1 > length && nstring[j+1] == MYANMAR_SIGN_VIRAMA))
				/*  Stacked Consonants Vs Kinzi */
				continue;

			int i = 1;
			while (j + i < length) {
				/* Check whether followed by ASAT or not */
				//g_printf ("\tcheckpoint 4: %lc\n", nstring[j+i]);
				if (nstring[j+i-1] != MYANMAR_VOWEL_SIGN_AA &&
					nstring[j+i] == MYANMAR_SIGN_ASAT)
					goto contin;

				if (my_wcismydiac (nstring[j+i]) || nstring[j+i] == L'-') {
					i++;
					continue;
				}

				break;
			}
			/* Otherwise */
			ret = j;

		contin:
			continue;
		}
	}

	if (ret ==  0)
		ret = next_cluster_position (string);

	return ret;
}

static gint next_cluster_position (gunichar *string)
{
	if (string == NULL)
		/* if the string is empty. */
		goto notfound;

	int i = 0, j = 0;
	int length = wcslen (string);

	if (length == 0)
		goto notfound;

	if (!my_wcismyanmar  (string[i]))
	{
		while (!my_wcismyanmar (string[j]) && j <length)
			j++;
		goto finish;
	}

	if (!my_wcismyletter (string[i]) && i < length) {
		/* Find the first consonant */
		if (my_wcismydigit(string[i]) ||
			my_wcismypunct(string[i]) ||
			my_wcismyindependsymbol (string[i])) {
			j = i + 1;
			goto finish;
		}

		while (!my_wcismyletter (string[j]) && j <length)
			j++;

		goto finish;
	}

	j = i + 1;
	int devowelized;

	do {
		devowelized = 0;

		while (!my_wcismyletter (string[j]) && j < length) {
			/* find the next consonant */
			if (!my_wcismyanmar (string[j]) ||
				my_wcismydigit(string[j]) ||
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
	return j-i;

 notfound:
	g_warn_if_reached ();
	return -1;
}
