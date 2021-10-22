#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#define MAX_FILE_PATH_LENGTH 30
#define FILE_NUMBER 13

int getTime(const char* file_path, time_t* time)
{
	struct stat file_stat;
	if (stat(file_path, &file_stat))
		return -1;

	*time = file_stat.st_mtime;
	return 0;
}

int recompile(const char* file, const char* command)
{
	printf("Recompile file '%s'...\n", file);

	if (!system(NULL))
	{
		printf("Making error: terminal access is denied\n");
		return -3;
	}

	return system(command);
}

int update(const char project_files[FILE_NUMBER][MAX_FILE_PATH_LENGTH], size_t nfiles, const char* exe_file, const char* compile_command)
{
	time_t exe_mtime;

	if (getTime(exe_file, &exe_mtime))
		exe_mtime = 0;

	int i;
	for (i = 0; i < nfiles; ++i)
	{
		time_t mtime;

		if (getTime(project_files[i], &mtime))
		{
			printf("Making error: can't get date of file '%s'\n", project_files[i]);
			return -1;
		}

		if (mtime > exe_mtime)
			return recompile(exe_file, compile_command);
	}

	return 0;
}

int main(int arc, char* argv[])
{

	const char STACK_TEST_FILES[FILE_NUMBER][MAX_FILE_PATH_LENGTH] =
	{
		"../src/stack_errors.h",
		"../src/prot/stack_verify.h",
		"../src/prot/stack_canary.h",
		"../src/prot/stack_hash.h",
		"../src/stack_utils.h",
		"../src/stack.h",

		"../src/stack_errors.c",
		"../src/prot/stack_verify.c",
		"../src/prot/stack_canary.c",
		"../src/prot/stack_hash.c",
		"../src/stack_utils.c",
		"../src/stack.c",
		"../src/test/stack_test.c"
	};

	if (update(STACK_TEST_FILES, FILE_NUMBER, "../bin/stack_test",
		"g++ ../src/stack_errors.c ../src/prot/stack_verify.c ../src/prot/stack_canary.c  ../src/prot/stack_hash.c ../src/stack_utils.c ../src/stack.c ../src/test/stack_test.c -o ../bin/stack_test"))
	{
		printf("Make aborted!\n");
		return 0;
	}

	system("../bin/stack_test");

	return 0;
}
