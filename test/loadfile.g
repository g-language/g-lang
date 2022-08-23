#include "loadfile.gh"

void* Memfile.alloc(size size)
	return .ptr = malloc(size);

bool Memfile.loadFile(const char* filename, bool isStr)
	File& file = io.fopen(filename, isStr ? "r" : "rb");
	int return = 0; // We should allow having 'return' variable name
	size sz;
	
	strcpy(.filename, filename);
	
	if (!file)
		return MEMFILE_FOPEN_ERROR;
	
	.size = file.getSize();
	if (!.alloc(.size))
		return MEMFILE_ALLOC_ERROR;
	
	sz = file.read(.data, 1, .size);
	
	if (sz != .size)
		.size = sz;
		return = MEMFILE_SIZE_WARN;
	
	io.fclose(file);
	
	return return;

void Memfile.free()
	free(.data);
	free(.filename);
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