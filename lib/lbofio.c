#include "lbolib.h"

struct file file_new(bool throw, size_t allosize) {
    struct file f = {};
    
    if (allosize) f.data = malloc(allosize);
    f.size = f.msize = allosize;
    f.throw = throw;
    
    return f;
}

void file_realloc(struct file* this, size_t sz) {
    assert(this->data = realloc(this->data, sz));
    this->msize = sz;
}

bool file_load(struct file* this, const char* file, char type) {
    FILE* f = NULL;
    size_t s;
    
    log("load file: %s", file);
    switch (type) {
        case 's':
            f = fopen(file, "r");
            break;
        case 'b':
            f = fopen(file, "rb");
            break;
        default:
            errr("file_load: unk type '%c'", type);
    }
    
    if (!f && this->throw) errr("file_load: %s", file);
    if (!f) return EXIT_FAILURE;
    
    fseek(f, 0, SEEK_END);
    this->size = s = ftell(f);
    rewind(f);
    
    if (this->msize < s) file_realloc(this, s * 2);
    if (s - fread(this->data, 1, s, f))
        log("%d bytes failed to fread from %s", s, file);
    fclose(f);
    
    fswap(this->name, strdup(file));
    
    return EXIT_SUCCESS;
}

void file_free(struct file* this) {
    free(this->data, this->name);
    this->size = this->msize = 0;
}
