#include <GLFW/glfw3.h>
#include <libavformat/avformat.h>
#include <stdio.h>

int setup_window(GLFWwindow **window) {
    if (!glfwInit()) {
        fprintf(stderr, "Unable to initialize glfw.");
        return -2;
    }

    *window = glfwCreateWindow(640, 480, "CVideoPlayer", nullptr, nullptr);
    if (!*window) {
        glfwTerminate();
        fprintf(stderr, "Unable to open window.");
        return -2;
    }

    glfwMakeContextCurrent(*window);

    glClearColor(0.0f, 0.2f, 0.3f, 1.0f);
    return 0;
}

void cleanup(AVFormatContext *pFmtCtx) {
    if (pFmtCtx) avformat_free_context(pFmtCtx);
}

int main(const int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: cvideoplayer <file>");
        return -1;
    }

    const char *video_path = argv[1];

    GLFWwindow *pWin = nullptr;
    AVFormatContext *pFmtCtx = nullptr;

    const int value = setup_window(&pWin);
    if (value) return value;

    pFmtCtx = avformat_alloc_context();

    if (avformat_open_input(&pFmtCtx, video_path, nullptr, nullptr)) {
        glfwTerminate();
        fprintf(stderr, "Unable to open video: %s", video_path);
        return -3;
    }

    if (avformat_find_stream_info(pFmtCtx, nullptr)) {
        glfwTerminate();
        fprintf(stderr, "Unable to open video streams: %s", video_path);
        cleanup(pFmtCtx);
        return -4;
    }

    av_dump_format(pFmtCtx, 0, video_path, false);

    int vStreamIdx = -1, aStreamIdx = -1;
    for (int i = 0; i < pFmtCtx->nb_streams; i++) {
        if (vStreamIdx == -1 && pFmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            vStreamIdx = i;
        } else if (aStreamIdx == -1 && pFmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            aStreamIdx = i;
        }
    }

    if (vStreamIdx == -1 && aStreamIdx == -1) {
        glfwTerminate();
        fprintf(stderr, "Video has no video or audio streams: %s", video_path);
        cleanup(pFmtCtx);
        return -5;
    }

    while (!glfwWindowShouldClose(pWin)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(pWin);

        glfwPollEvents();
    }

    glfwTerminate();

    cleanup(pFmtCtx);
    return 0;
}
