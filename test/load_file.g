#include <stdio> as io
#include <stdlib> as NONE

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

typedef union cast_val
	u8* u8;
	u16* u16;
	u32* u32;
	u64* u64;
	s8* s8;
	s16* s16;
	s32* s32;
	s64* s64;

typedef enum memfreturn
	MEMFILE_FOPEN_ERROR = 1,
	MEMFILE_ALLOC_ERROR,
	MEMFILE_SIZE_ERROR,

typedef struct memfile
	union
		void* ptr;
		char* str;
		castval cast;
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
			return 1;
		
		this.size = file.getSize();
		if (!this.alloc(this.size))
			return 2;
		
		sz = file.read(this.data, 1, this.size);
		
		if (sz != this.size)
			this.size = sz;
			return = 3;
		
		io.fclose(file);
		
		return return;
	
	void free()
		free(this.data);
		free(this.filename);
		memset(this, 0, sizeof(*this));

int main(int n, char** arg)
	memfile mem = {};
	
	switch (mem.loadFile("README.md", true))
		case MEMFILE_FOPEN_ERROR:
			printf("Failed to fopen file [%s]", mem.filename);
			break;
		case MEMFILE_ALLOC_ERROR:
			printf("Failed allocate memfile");
			break;
		case MEMFILE_SIZE_ERROR:
			printf("fread didn't match the expected size while loading file [%s]", mem.filename);
			break;
	
	mem.free();
	
	return 0;