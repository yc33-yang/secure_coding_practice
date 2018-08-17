#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// s should be an array
#define ary_snprintf(s, ...) _snprintf(s, sizeof(s), __VA_ARGS__)
#define safe_fclose(fp) do { if (fp) { fclose(fp); fp = NULL; } } while(0)

int main(int argc, char *argv[])
{
	FILE *fsp = NULL, *dfp = NULL; // source and destination file descriptors 
	char dest_file[100] = "-";
	char open_file_command[100] = { 0, };
	pevent_t *event = nullptr;

	if (argc <= 1) {
		printf("usage: %s input [output]\n", 
                       "          default output file name : <input>.html\n",
		       argv[0]);
		return 1;
	}
	/* opnen the file */
	if (NULL == (fsp = fopen(argv[1], "r")))
	{
		printf("Error! File %s could not be opened\n", argv[1]);
		return 2;
	}
	/* Check for output file */
	if (argc > 2) 	{
		ary_snprintf(dest_file, "%s.html", argv[2]);
	}
	else	{
		ary_snprintf(dest_file, "%s.html", argv[1]);
	}
	//........
	/* write HTML starting Tags */
	html_begin(dfp, HTML_OPEN);
	//........
	/* Call start_or_end_conv function for ending the convertation */
	html_end(dfp, HTML_CLOSE);

	/* close file */
	safe_fclose(fsp);
	safe_fclose(dfp);

	printf("Output file %s generated\n", dest_file);

	ary_snprintf(open_file_command, "%s/firefox", PATH_TO_FIREFOX);
	char *command_args[] = { &open_file_command, dest_file, NULL };
	execve(open_file_command, command_args, NULL);
	return 0;
}
