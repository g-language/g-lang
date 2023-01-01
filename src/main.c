#include "lbolib.h"
#include "token.h"

int main(int n, const char** arg) {
    struct file i = file_new(true, 0);
    
    file_load(&i, "test/hello_world.gg", 's');
    info("%08X", i.size);
    token_init(i.data, i.size);
    
    const char* tkn;
    
    while ((tkn = token_next()))
        info("%s", tkn);
    
    file_free(&i);
}
