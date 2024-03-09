

char *pathJoin(char *path1, char *path2)
{
	const char path[999];
	snprintf(path, sizeof(path), "%s\\%s", path1, path2);
	return path;
}
