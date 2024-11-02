#if defined(PLATFORM_WEB)
#include "main.h"
#include "raylib.h"
#include <emscripten/emscripten.h>
#include <stdio.h>

int main(void) {
// NOTE: Trying out FS
#pragma GCC diagnostic ignored "-Wpedantic"
  EM_ASM_ARGS({
    // Make a directory other than '/'
    FS.mkdir('/disk');
    // Then mount with IDBFS type
    console.log(FS.filesystems);
    FS.mount(FS.filesystems.IDBFS, {autoPersist : true}, '/disk');

    // Then sync
    FS.syncfs(true, function(err){});
  });

  FILE *fp = fopen("/disk/test.txt", "w");
  if (fp) {
    fprintf(fp, "test\n");
    fclose(fp);
  }

  InitWindow(1200, 800, "raylib game template");
  init();
  emscripten_set_main_loop(main_loop, 0, 1);
}
#else
void empty_pedantic() {}
#endif
