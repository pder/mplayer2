/*
 * This file is part of mplayer2.
 *
 * mplayer2 is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * mplayer2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with mplayer2.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <quvi/quvi.h>

#include "talloc.h"
#include "mp_msg.h"
#include "options.h"
#include "stream.h"

struct mp_resolve_result *mp_resolve_quvi(const char *url, struct MPOpts *opts)
{
    QUVIcode rc;

    quvi_t q;
    rc = quvi_init(&q);
    if (rc != QUVI_OK)
        return NULL;

    // Don't try to use quvi on an URL that's not directly supported, since
    // quvi will do a network access anyway in order to check for HTTP
    // redirections etc.
    // The documentation says this will fail on "shortened" URLs.
    if (quvi_supported(q, (char *)url) != QUVI_OK) {
        quvi_close(&q);
        return NULL;
    }

    mp_msg(MSGT_OPEN, MSGL_INFO, "[quvi] Checking URL...\n");

    // Can use quvi_query_formats() to get a list of formats like this:
    // "fmt05_240p|fmt18_360p|fmt34_360p|fmt35_480p|fmt43_360p|fmt44_480p"
    // (This example is youtube specific.)
    // That call requires an extra net access. quvi_next_media_url() doesn't
    // seem to do anything useful. So we can't really do anything useful
    // except pass through the user's format setting.
    quvi_setopt(q, QUVIOPT_FORMAT, opts->quvi_format);

    quvi_media_t m;
    rc = quvi_parse(q, (char *)url, &m);
    if (rc != QUVI_OK) {
        mp_msg(MSGT_OPEN, MSGL_ERR, "[quvi] %s\n", quvi_strerror(q, rc));
        quvi_close(&q);
        return NULL;
    }

    struct mp_resolve_result *result =
        talloc_zero(NULL, struct mp_resolve_result);

    char *val;

    if (quvi_getprop(m, QUVIPROP_MEDIAURL, &val) == QUVI_OK)
        result->url = talloc_strdup(result, val);

    if (quvi_getprop(m, QUVIPROP_PAGETITLE, &val) == QUVI_OK)
        result->title = talloc_strdup(result, val);

    quvi_parse_close(&m);
    quvi_close(&q);

    if (!result->url) {
        talloc_free(result);
        result = NULL;
    }

    return result;
}
