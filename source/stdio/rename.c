#include <stdio.h>
#include <ksys.h>

int rename(const char *name, const char *new_name) {
	return _ksys_file_rename((char*)name, (char*)new_name);
}
