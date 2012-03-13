/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * libburmese - Burmese Language Support Library
 * Copyright (C) 2011  Thura Hlaing <trhura@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

/*
 * mystr.h - various Burmese string related functions
 * Created: 2011-01-08
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "myanmar.h"
#include "myctype.h"
#include "mystr.h"
#include "mystr-priv.h"

/**
 * @brief       Save current cluster and skip to the start of next cluster.
 *
 * This routine assumes the str contains only Burmese unicode chracters in a
 * valid unicode 5.2  storage order.
 *
 * @param       str: pointer to the start of a valid Burmese unicode string
 * @param       cluster: location to store the current cluster, must be free manually.
 * @returns     pointer to the start of next cluster
 */

wchar_t* mystr_next_cluster (wchar_t *string, wchar_t **cluster)
{
    if (string == NULL)
        /* if the string is empty. */
        goto notfound;

    int i = 0, j = 0;
    wchar_t* ret_cluster;
    int length = wcslen (string);
    int cluster_length = 0;

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

        ret_cluster = (wchar_t*) malloc (sizeof (wchar_t) * (cluster_length + 1));
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

ClusterIter*    mystr_cluster_iter_new  (const wchar_t* string)
{
    if (string == NULL)
        return NULL;

    ClusterIter *iter = (ClusterIter*) malloc (sizeof (ClusterIter));
    iter->string    = wcsdup (string);
    iter->position  = iter->string;
    iter->length    = wcslen (string);

    return iter;
}

wchar_t*    mystr_cluster_iter_next (ClusterIter* iter)
{
    assert (iter != NULL);

    wchar_t* cluster;

    iter->position = mystr_next_cluster (iter->position, &cluster);
    return cluster;
}

void    mystr_cluster_iter_free (ClusterIter* iter)
{
    assert (iter != NULL);

    free (iter->string);
    free (iter);
}

/*
vi:ts=4:ai:expandtab
*/
