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

#include <locale.h>
#include <wchar.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "myanmar.h"
#include "wbrk.h"
#include "myctype.h"
#include "map.h"

#define TEST_FILE "wbrk-test.txt"
#define OUT_FILE "wbrk-test.out"

int main(int argc, char *argv[])
{
	setlocale(LC_CTYPE, "");
	wbrk_init ();
	
	FILE *ifile = fopen (TEST_FILE, "r");
	FILE *ofile = fopen (OUT_FILE, "w");

	if (ifile == NULL || ofile == NULL) {
		printf ("error opening files.");
		return -1;
	}

	wchar_t *line = (wchar_t*)malloc (sizeof (wchar_t) * 1024);

//	int i = 0;
	while ((line = fgetws (line, 1024,  ifile))) {
		line[wcslen(line)-1] = L'\0';

		wchar_t *tmp =  wbrk_normalize_string (line);
		wchar_t buffer[128] = { L'\0' };
		int length = wcslen (tmp);
		int current_pos = 0;
		int word_start = 0;

		while (current_pos < length) {
			//g_printf ("---%ls---\n", nwcs+cword_end);
			int position =  wbrk_get_next_brkpos (tmp+current_pos,
												  line+current_pos);

			if (position == 0) {
				break;
			}

			word_start = current_pos;
			current_pos  += position;

			wcsncpy (buffer, line+word_start, position);
			buffer[position] = L'\0';
//			fprintf (ofile, "buffer =%ls\n", buffer);
			fprintf (ofile, "%ls + ", buffer);
		}

		g_free (tmp);
		fseek (ofile, -3, SEEK_CUR);
		fprintf (ofile, "\n");
	}

	fclose (ifile);
	fclose (ofile);
	wbrk_unload ();

	 return 0;
}
