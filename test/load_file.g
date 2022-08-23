#include <stdio> as io
#include <stdlib> as lib

#define MEMFILE_FOPEN_ERROR    1
#define MEMFILE_ALLOC_ERROR    2
#define MEMFILE_SIZE_WARN      3

typedef signed char            s8;
typedef unsigned char          u8;
typedef signed short           s16;
typedef unsigned short         u16;
typedef signed int             s32;
typedef unsigned int           u32;
typedef signed long long int   s64;
typedef unsigned long long int u64;
typedef float                  f32;
typedef double                 f64;

typedef union CastUnion
	u8* u8;
	u16* u16;
	u32* u32;
	u64* u64;
	s8* s8;
	s16* s16;
	s32* s32;
	s64* s64;

typedef struct Memfile
	union
		void* ptr;
		char* str;
		CastUnion cast;
	// size instead of size_t
	size size;
	const char* filename;
	
	void* alloc(size size)
		return this.ptr = malloc(size);
	
	// Arguments can be optional
	bool loadFile(const char* filename, bool isStr = false )
		File& file = io.fopen(filename, isStr ? "r" : "rb");
		int return = 0; // We should allow having 'return' variable name
		size sz;
		
		strcpy(this.filename, filename);
		
		if (!file)
			return MEMFILE_FOPEN_ERROR;
		
		this.size = file.getSize();
		if (!this.alloc(this.size))
			return MEMFILE_ALLOC_ERROR;
		
		sz = file.read(this.data, 1, this.size);
		
		if (sz != this.size)
			this.size = sz;
			return = MEMFILE_SIZE_WARN;
		
		io.fclose(file);
		
		return return;
	
	void free()
		free(this.data);
		free(this.filename);
		memset(this, 0, sizeof(*this));

int main(int n, char** arg)
	Memfile mem = new();
	
	if (n != 2)
		lib.fprintf(lib.stderr, "Please provide an input file! %s <file>", arg[0]);
		return 1;
	
	switch (mem.loadFile("README.md", true))
		case MEMFILE_FOPEN_ERROR:
			lib.fprintf(lib.stdout, "Failed to fopen file [%s]", mem.filename);
			break;
		case MEMFILE_ALLOC_ERROR:
			lib.fprintf(lib.stdout, "Failed allocate Memfile");
			break;
		case MEMFILE_SIZE_ERROR:
			lib.fprintf(lib.stdout, "fread didn't match the expected size while loading file [%s]", mem.filename);
			break;
	
	mem.free();
	delete(mem);
	
	return 0;