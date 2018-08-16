#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "macros.h"

// s should be an array
#define ary_snprintf(s, ...) _snprintf(s, sizeof(s), __VA_ARGS__)
#define safe_fclose(fp) do { if (fp) { fclose(fp); fp = NULL; } } while(0)

#define __STR(s) #s
#define _STR(s) __STR(s)
#define STR(s) _STR(s) // looks aguly but safe for evalution (e.g. 1024+500 --> "1524")

#define BUFF_SIZE 1024+100


static size_t strlcpy(char * dst, const char * src, size_t dsize);
static size_t	strlcat(char *dst, const char *src, size_t siz);

void _sendFile(int clientSocket) {
	char buffer[BUFF_SIZE+1] = { '\0' };
	bzero(buffer, sizeof(buffer)); // enforce null-termination

	int readStatus = read(clientSocket, buffer, BUFF_SIZE);
	if (readStatus < 0) {
		fprintf(stderr, "Error: Reading from socket.\n");
		return;
	}

	// Temporary GET handling.
	char fileName[BUFF_SIZE + 1] = { '\0' };
	// int sscanf ( const char * s, const char * format, ...);
	// Reads data from s and stores them according to parameter format into the locations given by the additional argument
	sscanf(buffer, "GET /%" STR(BUFF_SIZE) "s", fileName);

	// Handle file request.
	FILE* fp = fopen(fileName, "rb");
	if (!fp) {
		fprintf(stderr, "Error: File opening.\n");
		return;
	}

	fprintf(stdout, "Sending: %s\n", fileName);

	int fileChar = 0;
	int i = 0;
	bzero(buffer, BUFF_SIZE);
	while (((fileChar = fgetc(fp)) != EOF) && (i < BUF_SIZE)) {
		buffer[i++] = fileChar;
	}
	safe_fclose(fp);

	// Temporary header handling - only text/HTML at the moment.
	char header[BUFF_SIZE]		  = "HTTP/1.1 200 OK\r\n";
	char contentType[BUFF_SIZE]   = "Content-Type: text/html; charset=UTF-8\r\n";
	char contentLength[BUFF_SIZE] = { '\0' };
	ary_snprintf(contentLength, "Content-Length: %zu\r\n", strlen(buffer) + 1);

	char response[BUFF_SIZE] = { '\0' };
	ary_snprintf(response, "%s%s%s%s"
						, header
						, contentType
						, contentLength
						, "\r\n\r\n");

	// There goes the header.
	if (write(clientSocket, response, strlen(response)) < 0) {
		fprintf(stderr, "Error: Sending header.\n");
		return;
	};

	// Now the file.
	if (write(clientSocket, buffer, strlen(buffer) + 1) < 0) {
		fprintf(stderr, "Error: Sending file.\n");
		return;
	}

	fprintf(stdout, "Sent: %s\n\n", fileName);
	fprintf(stdout, "Waiting..\n\n");
}




// Reference of strlcpy, strlcat:
// https://github.com/freebsd/freebsd/tree/386ddae58459341ec567604707805814a2128a57/crypto/openssh/openbsd-compat

/*
* Copy string src to buffer dst of size dsize.  At most dsize-1
* chars will be copied.  Always NUL terminates (unless dsize == 0).
* Returns strlen(src); if retval >= dsize, truncation occurred.
*/
static size_t strlcpy(char * dst, const char * src, size_t dsize)
{
	const char *osrc = src;
	size_t nleft = dsize;

	/* Copy as many bytes as will fit. */
	if (nleft != 0) {
		while (--nleft != 0) {
			if ((*dst++ = *src++) == '\0')
				break;
		}
	}

	/* Not enough room in dst, add NUL and traverse rest of src. */
	if (nleft == 0) {
		if (dsize != 0)
			*dst = '\0';		/* NUL-terminate dst */
		while (*src++)
			;
	}
	return(src - osrc - 1);	/* count does not include NUL */
}

static size_t	strlcat(char *dst, const char *src, size_t siz)
{
	char *d = dst;
	const char *s = src;
	size_t n = siz;
	size_t dlen;

	/* Find the end of dst and adjust bytes left but don't go past end */
	while (n-- != 0 && *d != '\0')
		d++;
	dlen = d - dst;
	n = siz - dlen;

	if (n == 0)
		return(dlen + strlen(s));
	while (*s != '\0') {
		if (n != 1) {
			*d++ = *s;
			n--;
		}
		s++;
	}
	*d = '\0';

	return(dlen + (s - src));	/* count does not include NUL */
}
