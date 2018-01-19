#ifndef _VERSION_H
#define _VERSION_H

#include <string>

#ifndef GIT_BRANCH
#define GIT_BRANCH "unspecified"
#endif

#ifndef GIT_LATEST_COMMIT
#define GIT_LATEST_COMMIT "unspecified"
#endif

#ifndef GIT_COMMIT_TIME
#define GIT_COMMIT_TIME "unspecified"
#endif

std::string Hadron_PDFs_compilation();

#endif // _VERSION_H