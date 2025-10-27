#include <GLFW/glfw3.h>
#include <libavformat/avformat.h>
#include <stdio.h>

int setup_window(GLFWwindow **window) {
    if (!glfwInit()) {
        fprintf(stderr, "Unable to initialize glfw.");
        return -2;
    }

    *window = glfwCreateWindow(640, 480, "CVideoPlayer", NULL, NULL);
    if (!*window) {
        glfwTerminate();
        fprintf(stderr, "Unable to open window.");
        return -2;
    }

    glfwMakeContextCurrent(*window);

    glClearColor(0.0, 0.2, 0.3, 1.0);
    return 0;
}

int main(const int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: cvideoplayer <file>");
        return -1;
    }

    const char *video_path = argv[1];

    GLFWwindow *window;

    const int value = setup_window(&window);
    if (value) return value;

    AVFormatContext *fmtCtx = avformat_alloc_context();

    if (avformat_open_input(&fmtCtx, video_path, nullptr, nullptr)) {
        glfwTerminate();
        fprintf(stderr, "Unable to open video: %s", video_path);
        return -3;
    }

    printf("AVFormatContext SIZEOF: %lu, ALIGNOF: %lu, IFORMAT: %lu, DURATION: %lu\n", sizeof(AVFormatContext), alignof(AVFormatContext), offsetof(AVFormatContext, iformat), offsetof(AVFormatContext, duration));

    printf("Video format: %s, duration: %ld us\n", fmtCtx->iformat->long_name, fmtCtx->duration);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();

    if (fmtCtx) avformat_free_context(fmtCtx);
    return 0;
}
