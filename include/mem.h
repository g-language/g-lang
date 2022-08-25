#ifndef MEM_H
#define MEM_H

#include "types.h"

typedef struct memfile {
	char*  data;
	size_t size;
	size_t memsize;
	char*  filename;
} memfile;

// postfree add to queue
void*que(const void* data);
// postfree free queue
void quefree(void);

static inline void*zalloc(size_t size) {
	return memset(malloc(size), 0, size);
}

static inline void*memdup(const void* data, size_t size) {
	return memcpy(malloc(size), data, size);
}

static inline char*line(const char* str, s32 line) {
	const char* ln = str;
	
	if (!str)
		return NULL;
	
	while (line--) {
		ln = strpbrk(ln, "\n");
		
		if (!ln++)
			return NULL;
	}
	
	return (char*)ln;
}

static inline char*word(const char* str, s32 word) {
	if (!str)
		return NULL;
	
	while (!isgraph(*str))
		str++;
	while (word--) {
		if (!(str = strpbrk(str, " \t\n\r")))
			return NULL;
		while (!isgraph(*str))
			str++;
	}
	
	return (char*)str;
}

static inline size_t linecount(const char* str) {
	size_t count = 1;
	
	if (!str)
		return 0;
	
	while (str) {
		str = line(str, 1);
		count++;
	}
	
	return count;
}

static inline char* wordcpy(const char* str) {
	return strndup(str, strcspn(str, " \t"));
}

static inline char* linecpy(const char* str) {
	return strndup(str, strcspn(str, "\n"));
}

static inline char* fmt(const char* fmt, ...) {
	char* s;
	va_list va;
	
	va_start(va, fmt);
	vasprintf(&s, fmt, va);
	va_end(va);
	
	return s;
}

static inline bool chracpt(char c, const char* accept) {
	u32 count = strlen(accept);
	
	for (u32 i = 0; i < count; i++)
		if (c == accept[i])
			return true;
	
	return false;
}

static inline char* linehead(const char* origin, const char* point) {
	while (point > origin && !chracpt(point[-1], "\n\r"))
		point--;
	
	return (char*)point;
}

static inline u32 linenum(const char* origin, const char* point) {
	u32 ln = 1;
	
	point = linehead(origin, point);
	
	while (point > origin) {
		origin = line(origin, 1);
		ln++;
	}
	
	return ln;
}

static inline u32 charnum(const char* origin, const char* point) {
	origin = linehead(origin, point);
	
	return point - origin;
}

static inline s32 memfile_open(memfile* this, const char* file, const char* mode) {
	FILE* f;
	s32 r = 0;
	
	f = fopen(file, mode);
	if (!f)
		return 1;
	
	if (strchr(mode, 'r')) {
		fseek(f, 0, SEEK_END);
		this->size = ftell(f);
		rewind(f);
		
		this->memsize = max(this->size, this->memsize);
		this->data = realloc(this->data, this->memsize);
		
		if (fread(this->data, 1, this->size, f) != this->size)
			r = -1;
	} else if (strchr(mode, 'w')) {
		if (fwrite(this->data, 1, this->size, f) != this->size)
			r = -1;
	}
	
	fclose(f);
	
	return r;
}

static inline void memfile_free(memfile* this) {
	free(this->data);
	free(this->filename);
	this->memsize = this->size = 0;
}

static inline void memfile_unload(memfile* this) {
	this->size = 0;
}

#endif