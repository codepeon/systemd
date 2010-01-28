/*-*- Mode: C; c-basic-offset: 8 -*-*/

#include <dirent.h>
#include <errno.h>

#include "unit.h"
#include "load-dropin.h"
#include "log.h"

int unit_load_dropin(Unit *u) {
        Iterator i;
        int r;
        char *t;

        assert(u);

        /* Load dependencies from supplementary drop-in directories */

        SET_FOREACH(t, u->meta.names, i) {
                char *path;
                DIR *d;
                struct dirent *de;

                if (asprintf(&path, "%s/%s.wants", unit_path(), t) < 0)
                        return -ENOMEM;

                if (!(d = opendir(path))) {
                        r = -errno;
                        free(path);

                        if (r == -ENOENT)
                                continue;

                        return r;
                }

                free(path);

                while ((de = readdir(d))) {
                        if (de->d_name[0] == '.')
                                continue;

                        assert(de->d_name[0]);

                        if (de->d_name[strlen(de->d_name)-1] == '~')
                                continue;

                        if (asprintf(&path, "%s/%s.wants/%s", unit_path(), t, de->d_name) < 0) {
                                closedir(d);
                                return -ENOMEM;
                        }

                        if (!unit_name_is_valid(de->d_name)) {
                                log_info("Name of %s is not a valid unit name. Ignoring.", path);
                                free(path);
                                continue;
                        }

                        r = unit_add_dependency_by_name(u, UNIT_WANTS, path);
                        free(path);

                        if (r < 0) {
                                closedir(d);
                                return r;
                        }
                }

                closedir(d);
        }

        return 0;
}
